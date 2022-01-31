

#include <ctime>
#include <iostream>
#include <format>
#include <string>
#include <string_view>
#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>

#include "sqlhelper/sqlhelper.h"

typedef web::json::value JsonValue;
typedef web::json::value::value_type JsonValueType;
typedef web::http::client::http_client_config HttpClientConfig;

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;


/* Can pass proxy information via environment variable http_proxy.
   Example:
   Linux:   export http_proxy=http://192.1.8.1:8080
 */
HttpClientConfig prepare_client_config(bool requireValidCert = true)
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
    // Create an HTTP request.
    // Encode the URI query since it could contain special characters like spaces.
    http_client client(U("https://192.168.0.111/query/runtimes"), prepare_client_config(false));
    auto request = client.request(methods::GET)

        .then([](http_response response) {

        std::cout <<  "Status response: " << response.status_code() << std::endl;
        if (response.status_code() == 200) {
            auto headers = response.headers();
            auto content_type = headers.content_type();

            return response.extract_json(true);
        }
            })




        .then([](JsonValue jsonObject) {

#ifdef DEBUG
                string_t json_data = jsonObject.to_string();
                std::cout << utility::conversions::to_utf8string(json_data) << std::endl;
#endif // DEBUG
                auto runtimes = jsonObject.at(utility::conversions::to_string_t("runtimes")).as_array();
                for (auto runtime : runtimes) {
                    auto heat1 = runtime.at(utility::conversions::to_string_t("heat1")).as_number();

         
                    std::cout << "Heat: " << heat1.to_int64() << std::endl;

                    auto ts = runtime.at(utility::conversions::to_string_t("ts")).as_number();
                    time_t ts_t = ts.to_int64();
                    //https://en.cppreference.com/w/cpp/chrono/c/time
                    auto timeinfo = localtime(&ts_t);
                    //std::cout << std::asctime(timeinfo);

                    std::cout << "Time stamp: " << ts.to_int64() << ": " << std::asctime(timeinfo) << std::endl;
                    auto ts_current = std::time(nullptr);
                    auto diff_result = std::difftime(ts_current, ts_t);
                    std::cout << "diff result " << diff_result << std::endl;
                    //less that 24 hours
                    if (diff_result <= 86400) {
                        std::cout << "ignoring today's date as it is not complete data";

                    }

                }
            });

            

            request.wait();


            return 0;

}



