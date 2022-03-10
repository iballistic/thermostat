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


void showUsage(const char* programName) {
	std::cerr << std::format("Usage: {}", programName) << std::endl;
	std::cerr << " -key <Weather API Key> e.g  -key 123456789abcdef" << std::endl;
	std::cerr << " -tu <Thermostat Base Url> e.g -tu https ://192.168.0.237" << std::endl;
}

int wmain(int argc, wchar_t* argv[]) {

	char programName[256];
	//https://www.cplusplus.com/reference/cstdlib/wcstombs/
	//https://en.cppreference.com/w/cpp/string/multibyte/mbsrtowcs
	//https://docs.microsoft.com/en-us/cpp/dotnet/how-to-convert-system-string-to-wchar-t-star-or-char-star?view=msvc-170
	std::wcstombs(programName, argv[0], sizeof(programName));

	if (argc <= 3) {
		showUsage(programName);
		return 1;
	}

	std::wstring apiKey{};
	std::wstring baseURL{};
	


	for (size_t i = 1; i < argc; i++) {
		if (std::wstring(argv[i]) == L"-h") {
			showUsage(programName);
			return 1;
		}else if (std::wstring(argv[i]) == L"-key"){
			apiKey = std::wstring(argv[++i]);
		}
		else if (std::wstring(argv[i]) == L"-tu") {
			baseURL = std::wstring(argv[++i]);
		}
	}

	int result = 0;
	Weather::DarkSky weather = Weather::DarkSky(apiKey);
	//Weather::DarkSky weather = Weather::DarkSky(L"694130becabc99d29f1e55aac65d5cf6");
	weather.setUnit(Weather::MeasurementUnit::METRIC);
	auto weatherData = weather.getWeather(51.213654, -114.288212);


	//https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2013/d45bbxx4(v=vs.120)
	// 
	//char buf[16];
	//strcpy_s(buf, 16, "Start");
	//strcat_s(buf, 16, " End");               // Correct
	//strcat_s(buf, 16 – strlen(buf), " End"); // Incorrect
	// 
	//const wstring programName = std::wstring(argv[0]);

	char databaseName[256] = { 0 };
	strcat_s(databaseName, sizeof(databaseName), programName);
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
	baseURL.append(L"/query/runtimes");
	Thermostat thermo = Thermostat(baseURL, L"THERMO", L"THERMO", false);
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