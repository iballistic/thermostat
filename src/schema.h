#ifndef SCHEMA_H
#define SCHEMA_H
#include<cstdio>
#include<string>


//for reporting
//SELECT strftime('%Y-%m-%d', datetime(timestamp, 'unixepoch')) as date FROM weather_staging;
//SELECT strftime('%Y-%m-%d', datetime(timestamp, 'unixepoch')) as date FROM runtimes;
//https://www.sqlite.org/lang_datefunc.html
namespace Database {
	namespace Transaction {

		const std::string begin = {R"(BEGIN TRANSACTION;)"};
		const std::string commit = { R"(COMMIT;)"};

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
			[timestamp_string]	TEXT,
			[unit]	TEXT
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
			[timestamp_string]	TEXT,
			[unit]	TEXT
			);)" };
	};

	namespace TableDataInsert {

		const std::string runtimesStaging = {
			R"(insert into [runtimes_staging] (
			[timestamp],
			[heat1],
			[heat2],
			[timestamp_string]
			) VALUES ('{}','{}','{}','{}');)"
		};

		const std::string weatherStaging = {
		R"(insert into [weather_staging] (
		[timestamp],
		[current],
		[low],
		[high],
		[humidity],
		[pressure],
		[windspeed],
		[timestamp_string],
		[unit]) VALUES ('{}','{}','{}','{}','{}','{}','{}','{}', '{}');)" };
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
					where t2.[timestamp] is null)"};

		const std::string weather = { R"(insert into weather (
				[timestamp],
				[current],
				[low],
				[high],
				[humidity],
				[pressure],
				[windspeed],
				[timestamp_string],
				[unit]
				)
				select distinct
				t1.[timestamp],
				t1.[current],
				t1.[low],
				t1.[high],
				t1.[humidity],
				t1.[pressure],
				t1.[windspeed],
				t1.[timestamp_string],
				t1.[unit]
				from weather_staging  as t1 
				left join [weather]  as t2 on t1.[timestamp] = t2.[timestamp]
				where t2.[timestamp] is null)"};
	};

	namespace TableDataDelete {

		const std::string runtimesStaging = { R"(delete from runtimes_staging)" };
		const std::string weatherStaging = { R"(delete from weather_staging)" };
	};
};

#endif // SCHEMA_H
