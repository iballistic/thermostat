#include"thermostat.h"




//http://www.atakansarioglu.com/easy-quick-start-cplusplus-rest-client-example-cpprest-tutorial/
//https://github.com/microsoft/cpprestsdk/wiki/Getting-Started-Tutorial
//https://en.cppreference.com/w/cpp/chrono/c/time

Thermostat::Thermostat(const wstring uri, const wstring username, const  wstring password, const bool requireValidCert = true)
{
    this->uri = uri;
    this->username = username;
    this->password = password;
    this->requireValidCert = requireValidCert;
};

std::vector<Runtime> Thermostat::getRuntimes() {

    if (this->uri == L"") {
        throw "Invalid uri";
    }

    std::vector<Runtime> return_data = std::vector<Runtime>();
 
    http_client client(this->uri, prepareClientConfig());
    auto request = client.request(methods::GET)

        .then([](http_response response) {

        std::cout << "Status response: " << response.status_code() << std::endl;
        if (response.status_code() == 200) {
            auto headers = response.headers();
            auto content_type = headers.content_type();

            return response.extract_json(true);
        }
       })



        // & means caputering 
        //an enclosing - function local variable cannot be referenced in a lambda body unless if it is in capture list
        .then([&](JsonValue jsonObject) {

#ifdef DEBUG
                string_t json_data = jsonObject.to_string();
                std::cout << utility::conversions::to_utf8string(json_data) << std::endl;
#endif // DEBUG

                // Character literals
                // /https://docs.microsoft.com/en-us/cpp/cpp/string-and-character-literals-cpp?view=msvc-170
                //auto c0 = 'A'; // char
                //auto c1 = u8'A'; // char
                //auto c2 = L'A'; // wchar_t
                //auto c3 = u'A'; // char16_t
                //auto c4 = U'A'; // char32_t
                // 
                //auto runtimes = jsonObject.at(utility::conversions::to_string_t("runtimes")).as_array();
                auto runtimes = jsonObject.at(L"runtimes").as_array();
                for (auto runtime : runtimes) {
                    auto heat1 = runtime.at(L"heat1").as_number();
                    auto heat2 = runtime.at(L"heat2").as_number();
                    auto ts = runtime.at(L"ts").as_number();

                    time_t ts_input = ts.to_int64();
                    time_t ts_current = std::time(nullptr);
                    auto diff_result = std::difftime(ts_current, ts_input);
#ifdef DEBUG
                    auto timeinfo = localtime(&ts_input);
                    std::cout << "Time stamp: " << ts_input << ": " << std::asctime(timeinfo) << std::endl;
                    std::cout << std::asctime(timeinfo);
                    std::cout << "diff result " << diff_result << std::endl;
#endif // DEBUG  
                    //less that 24 hours
                    if (diff_result <= 86400) {
                        std::cout << "ignoring today's date as it is not complete data" << std::endl;
                        continue;
                    }
                    else {

                        Runtime runtime;
                        runtime.timestamp = ts.to_int64();
                        runtime.heat1 = heat1.to_int64();
                        runtime.heat2 = heat2.to_int64();
                        return_data.push_back(runtime);
                       
                       
                    }

                }
            });



            request.wait();

            return return_data;
}

HttpClientConfig Thermostat::prepareClientConfig()
{
    HttpClientConfig client_config = HttpClientConfig();
    if (this->username != L"" && this->password != L"") {
        credentials creds(username, password);
        client_config.set_credentials(creds);
    }

    client_config.set_validate_certificates(this->requireValidCert);

    return client_config;
}


Thermostat::~Thermostat()
{
}