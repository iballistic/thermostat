#pragma once

#include<ctime>
#include<iostream>
#include<format>
#include<string>
#include<xstring>
#include<string_view>
#include<vector>
#include<cpprest/filestream.h>
#include<cpprest/http_client.h>
#include<cpprest/uri.h>
#include<cpprest/json.h>


typedef web::json::value JsonValue;
typedef web::json::value::value_type JsonValueType;
typedef web::http::client::http_client_config HttpClientConfig;

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;
using std::wstring;

#include"weatherprovider.h"

namespace Weather {

	class DarkSky : public ProviderAPI {

	public:
		DarkSky(const std::wstring apiKey) : ProviderAPI(apiKey) {};
		WeatherStruct getWeather(double latitude, double longitude);

	};

};
