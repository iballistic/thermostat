//// thermostat.cpp : Defines the entry point for the application.
////
//#include<stdio.h>
//#include<string.h>
//
//#include<sqlite3.h>
//#include<curl/curl.h>
//
//
//#include "thermostat.h"
//
//using namespace std;
//
//
//int main()
//{
//	CURL* curl = { nullptr };
//	CURLcode result = CURLE_OK;
//	char* url = { nullptr };
//
//	curl = curl_easy_init();
//	url = (char*)calloc(256, sizeof(char));
//
//	strcpy(url, "https://192.168.0.111/query/runtimes");
//
//
//	if (curl) {
//		curl_easy_setopt(curl, CURLOPT_URL, url);
//		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
//		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_DIGEST);
//		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
//		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//		curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
//		//creds
//		//curl_easy_setopt(curl, CURLOPT_USERNAME, "THERMO");
//		//curl_easy_setopt(curl, CURLOPT_PASSWORD, "THERMO");
//
//		curl_easy_setopt(curl, CURLOPT_USERPWD, "THERMO:THERMO");
//		//write headers
//		struct curl_slist* headers = NULL;
//		headers = curl_slist_append(headers, "Accept: text/plain");
//		headers = curl_slist_append(headers, "Transfer-Encoding: chunked");
//		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
//		result = curl_easy_perform(curl);
//		printf("Curl result %s\n", curl_easy_strerror(result));
//
//	}
//	curl_easy_cleanup(curl);
//	printf("%s\n", sqlite3_libversion());
//	return 0;
//}

/***
 * Copyright (C) Microsoft. All rights reserved.
 * Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
 *
 * =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *
 * bingrequest.cpp - Simple cmd line application that makes an HTTP GET request to bing searching and outputting
 *       the resulting HTML response body into a file.
 *
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 ****/
#include <iostream>
#include<format>
#include <string>
#include <string_view>
#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>

typedef web::json::value JsonValue;
typedef web::json::value::value_type JsonValueType;

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;


/* Can pass proxy information via environment variable http_proxy.
   Example:
   Linux:   export http_proxy=http://192.1.8.1:8080
 */
web::http::client::http_client_config prepare_client_config(bool requireValidCert = true)
{
    web::http::client::http_client_config client_config;
    credentials creds(L"THERMO", L"THERMO");
    client_config.set_credentials(creds);
    client_config.set_validate_certificates(requireValidCert);

    return client_config;
    }

#ifdef _WIN32
int wmain(int argc, wchar_t* args[])
#else
int main(int argc, char* args[])
#endif
{
    const string_t  outputFileName = L"c:\\temp\\data.txt";
    // Create an HTTP request.
    // Encode the URI query since it could contain special characters like spaces.
   http_client client(U("https://192.168.0.111/query/runtimes"), prepare_client_config(false));
   //http_client client(U("http://192.168.0.1/login.htm"), client_config_for_creads());
    pplx::task<http_response> task_response = client.request(methods::GET);

    auto response = task_response.get();

    std::cout << "Status response: " << response.status_code() << std::endl;
    if (response.status_code() == 200) {
        auto headers = response.headers();
        auto content_type = headers.content_type();

        auto task_json = response.extract_json(true);

        JsonValue json_value = task_json.get();
        string_t json_data = json_value.to_string();
        std::cout << utility::conversions::to_utf8string(json_data);

        auto temp = json_value.at(utility::conversions::to_string_t("runtimes"));
        auto runtimes = temp.as_array();

        for (auto runtime : runtimes) {
           auto heat1 = runtime.at(utility::conversions::to_string_t("heat1")).as_number();
           auto ts = runtime.at(utility::conversions::to_string_t("ts")).as_number();
        }


   


    }



    return 0;

}

//#ifdef _WIN32
//int wmain(int argc, wchar_t* args[])
//#else
//int main(int argc, char* args[])
//#endif
//{
//    
//    const string_t outputFileName = L"C:\\temp\\data.txt";
//
//    // Open a stream to the file to write the HTTP response body into.
//    auto fileBuffer = std::make_shared<streambuf<uint8_t>>();
//    file_buffer<uint8_t>::open(outputFileName, std::ios::out)
//        .then([=](streambuf<uint8_t> outFile) -> pplx::task<http_response> {
//        *fileBuffer = outFile;
//
//        // Create an HTTP request.
//        // Encode the URI query since it could contain special characters like spaces.
//        http_client client(U("https://192.168.0.111/query/runtimes"), prepare_client_config(false));
//        return client.request(methods::GET);
//            })
//
//        // Write the response body into the file buffer.
//                .then([=](http_response response) -> pplx::task<size_t> {
//                printf("Response status code %u returned.\n", response.status_code());
//
//                return response.body().read_to_end(*fileBuffer);
//                    })
//
//                // Close the file buffer.
//                        .then([=](size_t) { return fileBuffer->close(); })
//
//                        // Wait for the entire response body to be written into the file.
//                        .wait();
//
//                    return 0;
//}


