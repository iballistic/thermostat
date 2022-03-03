#ifndef SCHEMA_H
#define SCHEMA_H
#include<cstdio>
#include<string>

namespace Database {
	namespace Transaction {

		const std::string begin = "BEGIN TRANSACTION;";
		const std::string commit = "COMMIT;";

	}
	namespace TableCreate {
		const std::string runtimes = { R"(
		CREATE TABLE IF NOT EXISTS [runtimes] (
		[id] INTEGER PRIMARY KEY AUTOINCREMENT,
		[timestamp]	INTEGER,
		[heat1]	REAL,
		[heat2]	REAL,
		[timestamp_string] TEXT
		);" )" };

		const std::string runtimes_staging = { R"(
		CREATE TABLE IF NOT EXISTS [runtimes_staging] (
		[id] INTEGER PRIMARY KEY AUTOINCREMENT,
		[timestamp]	INTEGER,
		[heat1]	REAL,
		[heat2]	REAL,
		[timestamp_string] TEXT
		);" )" };


		const std::string weather = { R"(CREATE TABLE IF NOT EXISTS [weather] (
			[id]	INTEGER PRIMARY KEY AUTOINCREMENT,
			[timestamp]	INTEGER,
			[current]	REAL,
			[low]	REAL,
			[high]	REAL,
			[humidity] REAL,
			[pressure] REAL,
			[windspeed] REAL,
			[timestamp_string]	TEXT
			);)" };


		const std::string weather_staging = { R"(CREATE TABLE IF NOT EXISTS [weather_staging] (
			[id]	INTEGER PRIMARY KEY AUTOINCREMENT,
			[timestamp]	INTEGER,
			[current]	REAL,
			[low]	REAL,
			[high]	REAL,
			[humidity] REAL,
			[pressure] REAL,
			[windspeed] REAL,
			[timestamp_string]	TEXT
			);)" };
	};

	namespace TableDataInsert {

		const std::string runtimesStaging = "insert into [runtimes_staging] ([timestamp],[heat1],[heat2],[timestamp_string])\
     VALUES ('{}','{}','{}','{}');\n";
	};

	namespace TableDataMerge {
	//A raw string literal is a string in which the escape 
	//characters like \n, \t or \”  of C++ are not processed.Hence, 
	//this was introduced in C++11, a raw string literal which starts with R”(and ends in)”.
		const std::string runtimes = { R"(insert into runtimes (
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
	};

	namespace TableDataDelete {

		const std::string runtimesStaging = { R"(delete from runtimes_staging)" };
	};
};

#endif // SCHEMA_H
