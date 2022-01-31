// thermostat.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <ctime>
#include <iostream>
#include <format>
#include <string>
#include <string_view>
#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>


typedef web::json::value JsonValue;
typedef web::json::value::value_type JsonValueType;
typedef web::http::client::http_client_config HttpClientConfig;

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;


public class Thermostat
{
public:
	Thermostat();
	~Thermostat();
	void getRuntimes();
	HttpClientConfig clientConfig(string proxyServer = nullptr)
	void 

private:

};

Thermostat::Thermostat()
{
}

Thermostat::getRuntimes() {

}

HttpClientConfig clientConfig(wchar_t username = nullptr, wchar_t password = nullptr, bool requireValidCert = true)
{
    HttpClientConfig client_config;
	if (username != nullptr && password != nullptr) {
		credentials creds(username, password);
		client_config.set_credentials(creds);
	}

    client_config.set_validate_certificates(requireValidCert);

    return client_config;
}


Thermostat::~Thermostat()
{
}