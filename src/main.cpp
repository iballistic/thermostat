#include<iostream>
#include<cstdio>
#include<string>
#include<string_view>
#include<format>
#include<algorithm>


#include"sqlhelper/sqlhelper.h"
#include"venstar/thermostat.h"
#include"weather/darksky.h"
#include"schema.h"


//int main();
//int main(int argc, char* argv[]);

int main(int argc, char* argv[]) {

	int result = 0;
	Weather::DarkSky weather = Weather::DarkSky(L"694130becabc99d29f1e55aac65d5cf6");
	weather.setUnit(Weather::MeasurementUnit::METRIC);
	auto weatherData = weather.getWeather(51.213654, -114.288212);


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
	//must connect to the database first
	sqlHelper->connect(databaseName);

	//create all required tables
	sqlHelper->run_query(Database::TableCreate::runtimes);
	sqlHelper->run_query(Database::TableCreate::runtimes_staging);
	sqlHelper->run_query(Database::TableCreate::weather);
	sqlHelper->run_query(Database::TableCreate::weather_staging);



	//update database with weather data
	sqlHelper->run_query(Database::Transaction::begin);
	sqlHelper->run_query(std::format(
		Database::TableDataInsert::weatherStaging,
		weatherData.dateTime,
		weatherData.temperatureCurrent,
		weatherData.temperatureLow,
		weatherData.temperatureHigh,
		weatherData.humidity,
		weatherData.pressure,
		weatherData.windSpeed,
		weatherData.timestampToString(),
	    char(weatherData.unit)
		)
	);
	sqlHelper->run_query(Database::TableDataMerge::weather);
	sqlHelper->run_query(Database::TableDataDelete::weatherStaging);
	result = sqlHelper->run_query(Database::Transaction::commit);
	std::cout << "Weather data commit result: " << result << std::endl;


	//Fetch thermostat data and update database
	Thermostat thermo = Thermostat(L"https://192.168.0.237/query/runtimes", L"THERMO", L"THERMO", false);
	auto runtimes = thermo.getRuntimes();
	sqlHelper->run_query(Database::Transaction::begin);
	for (auto runtime : runtimes) {

		sqlHelper->run_query(std::format(
			Database::TableDataInsert::runtimesStaging,
			runtime.timestamp,
			runtime.heat1,
			runtime.heat2,
			runtime.timestampToString())
		);
	}

	//merge the result from staging to the actual table
	sqlHelper->run_query(Database::TableDataMerge::runtimes);
	//delete from staging
	sqlHelper->run_query(Database::TableDataDelete::runtimesStaging);
	result = sqlHelper->run_query(Database::Transaction::commit);
	std::cout << "Terhmostat data commit result: " << result << std::endl;



	return 0;
}