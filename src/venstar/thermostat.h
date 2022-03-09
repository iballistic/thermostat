// thermostat.h : Include file for standard system include files,
// or project specific include files.

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

#include"runtime.h"

typedef web::json::value JsonValue;
typedef web::json::value::value_type JsonValueType;
typedef web::http::client::http_client_config HttpClientConfig;

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;
using std::wstring;


class Thermostat
{
public:
    Thermostat(const wstring uri, const wstring username, const wstring password, const bool requireValidCert);
	~Thermostat();
	std::vector<Runtime> getRuntimes();


private:
    std::wstring username = L"";
    std::wstring password = L"";
    std::wstring uri = L"";
    bool requireValidCert = true;
    HttpClientConfig prepareClientConfig();

};
