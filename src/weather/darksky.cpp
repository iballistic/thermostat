#include"darksky.h"


namespace Weather {

    WeatherStruct DarkSky::getWeather(double latitude, double longitude) {

        WeatherStruct weather;


        auto base_uri = U("https://api.darksky.net");
        uri_builder uri = uri_builder(U("forecast"));
        uri.append_path(utility::conversions::to_string_t(this->getApiKey()), false);
        uri.append_path(utility::conversions::to_string_t(std::format("{},{}", latitude, longitude)), true);
        uri.append_query(U("units"), "us");


        http_client client(base_uri);
        //std::cout << utility::conversions::to_utf8string(uri.to_string());
        auto request = client.request(methods::GET, uri.to_string())

            .then([](http_response response) {

            std::cout << "Status response: " << response.status_code() << std::endl;
            if (response.status_code() == 200) {
                auto headers = response.headers();
                auto content_type = headers.content_type();

                return response.extract_json(true);
            }
                })

            .then([&](JsonValue jsonObject) {

                    auto currently = jsonObject.at(L"currently").as_object();
                    auto current_time = currently.at(L"time").as_number();
                    auto temperature = currently.at(L"temperature").as_number();
                    auto windSpeed = currently.at(L"windSpeed").as_number();

                    time_t ts_current = std::time(nullptr);
                    weather.temperatureCurrent = temperature.to_double();
                    weather.windSpeed = windSpeed.to_double();
                    weather.localTime = ts_current;

            });


            request.wait();
            return weather;
    }
};