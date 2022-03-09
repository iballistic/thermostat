#include"darksky.h"


namespace Weather {

    WeatherStruct DarkSky::getWeather(double latitude, double longitude) {

        WeatherStruct weather;

        auto base_uri = U("https://api.darksky.net");
        uri_builder uri = uri_builder(U("forecast"));
        uri.append_path(utility::conversions::to_string_t(this->getApiKey()), false);
        uri.append_path(utility::conversions::to_string_t(std::format("{},{}", latitude, longitude)), true);
        if (this->getUnit() == MeasurementUnit::METRIC) {

            //units = [units] optional
            //    Return weather conditions in the requested units.[units] should be one of the following :

            //auto : automatically select units based on geographic location
            //    ca : same as si, except that windSpeedand windGust are in kilometers per hour
            //    uk2 : same as si, except that nearestStormDistanceand visibility are in miles, and windSpeedand windGust in miles per hour
            //    us : Imperial units(the default)
            //    si : SI units
            //    SI units are as follows :
            //    summary: Any summaries containing temperature or snow accumulation units will have their values in degrees Celsius or in centimeters(respectively).
            //    nearestStormDistance : Kilometers.
            //    precipIntensity : Millimeters per hour.
            //    precipIntensityMax : Millimeters per hour.
            //    precipAccumulation : Centimeters.
            //    temperature : Degrees Celsius.
            //    temperatureMin : Degrees Celsius.
            //    temperatureMax : Degrees Celsius.
            //    apparentTemperature : Degrees Celsius.
            //    dewPoint : Degrees Celsius.
            //    windSpeed : Meters per second.
            //    windGust : Meters per second.
            //    pressure : Hectopascals.
            //    visibility : Kilometers.

            uri.append_query(U("units"), "si");
        }
        else{
            //imperial units
            uri.append_query(U("units"), "us");
        }



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
                    auto temperature = currently.at(L"temperature").as_number();
                   

                    time_t tsCurrent = std::time(nullptr);
                    weather.temperatureCurrent = temperature.to_double();
                    weather.unit = this->getUnit();

                    //we need to get daily low and high from the daily json data
                    auto dailyData = jsonObject.at(L"daily").as_object().at(L"data").as_array();
                    for (auto daily : dailyData) {

                        time_t dailyStamp = daily.at(L"time").as_number().to_int64();
                        auto diffResult = std::difftime(dailyStamp, tsCurrent);

                        //auto timeinfo = localtime(&dailyStamp);
                        //std::cout << "Time stamp: " << dailyStamp << ": " << std::asctime(timeinfo) << std::endl;
                        //std::cout << std::asctime(timeinfo);
                        //std::cout << "diff result " << diffResult << std::endl;

                        //less that 24 hours
                        if (diffResult <= 86400) {
                            auto tempLow = daily.at(L"temperatureLow").as_number();
                            auto tempHigh = daily.at(L"temperatureHigh").as_number();
                            auto windSpeed = currently.at(L"windSpeed").as_number();

                            weather.temperatureLow = tempLow.to_double();
                            weather.temperatureHigh = tempHigh.to_double();
                            weather.windSpeed = windSpeed.to_double();
                            weather.dateTime = dailyStamp;
                            break;
                                                      
                        }
                        
                    }

                   



            });


            request.wait();
            return weather;
    }
};