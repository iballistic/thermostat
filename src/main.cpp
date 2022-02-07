#include<iostream>
#include<cstdio>
#include<string>
#include<string_view>
#include<format>
#include<algorithm>


#include"sqlhelper/sqlhelper.h"
#include"venstar/thermostat.h"

//int main();
//int main(int argc, char* argv[]);

int main(int argc, char* argv[]) {

	const std::string queryCreateTable = { R"(
	CREATE TABLE IF NOT EXISTS [{}] (
		[id] INTEGER PRIMARY KEY AUTOINCREMENT,
		[timestamp]	INTEGER,
		[heat1]	REAL,
		[heat2]	REAL,
		[timestamp_string] TEXT
		);" )"};

	const std::string insertQuery = "insert into [runtimes_staging] ([timestamp],[heat1],[heat2],[timestamp_string])\
     VALUES ('{}','{}','{}','{}');\n";

	//A raw string literal is a string in which the escape 
	//characters like \n, \t or \”  of C++ are not processed.Hence, 
	//this was introduced in C++11, a raw string literal which starts with R”(and ends in)”.
	const std::string mergeQuery = { R"(insert into runtimes (
					[timestamp],
					[heat1],
					[heat2],
					[timestamp_string]
					)
					select t1.[timestamp],
					t1.[heat1],
					t1.[heat2],
					t1.[timestamp_string] from runtimes_staging  as t1 
					left join [runtimes]  as t2 on t1.[timestamp] = t2.[timestamp]
					where t2.[timestamp] is null)"
	};

	const std::string deleteQuery = {R"(delete from runtimes_staging)" };

	auto tables = { "runtimes", "runtimes_staging" };

	//https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2013/d45bbxx4(v=vs.120)
	// 
	//char buf[16];
	//strcpy_s(buf, 16, "Start");
	//strcat_s(buf, 16, " End");               // Correct
	//strcat_s(buf, 16 – strlen(buf), " End"); // Incorrect

	char databaseName[256] = { 0 };
	strcat_s(databaseName, sizeof(databaseName), argv[0]);
	strcat_s(databaseName, sizeof(databaseName), ".db");
	SQLHelper* sqlHelper = new SQLHelper();
	sqlHelper->connect(databaseName);

	for (auto table : tables) {
#ifdef DEBUG
		std::cout << std::format(queryCreateTable, table);
#endif // DEBUG

		 
		sqlHelper->run_query(std::format(queryCreateTable, table));

	}
	

	
	

	Thermostat thermo = Thermostat(L"https://192.168.0.111/query/runtimes",L"THERMO", L"THERMO", false);
	auto runtimes = thermo.getRuntimes();

	sqlHelper->run_query("BEGIN TRANSACTION;");
	for (auto runtime : runtimes) {

		sqlHelper->run_query(std::format(insertQuery, runtime.timestamp, runtime.heat1, runtime.heat2, runtime.timestamp_tostrig()));
		//std::cout << runtime.heat1 << std::endl;
		//std::cout << runtime.heat2 << std::endl;
		//std::cout << runtime.heat1 << std::endl;
		//std::cout << runtime.timestamp_tostrig() << std::endl;
	}

	//merge the result from staging to the actual table
	sqlHelper->run_query(mergeQuery);
	
	//delete from staging
	sqlHelper->run_query(deleteQuery);

	int result = sqlHelper->run_query("COMMIT;");
	std::cout << "Commit result: " << result << std::endl;


	return 0;
}