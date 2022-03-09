#pragma once

#include<cstdio>
#include<iostream>
#include<string>
#include<ctime>


namespace Utils {

	class TimeUtil {

    public:
      static std::tuple<errno_t, std::string> datetimeToString(time_t dateTime=0) {
            auto timeinfo = localtime(&dateTime);
            const size_t bufferSize = 32;
            char buffer[bufferSize];
            errno_t error;

            //https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/asctime-s-wasctime-s?view=msvc-170
            error = asctime_s(buffer, bufferSize, timeinfo);
            if (error) {
               
                return std::make_tuple(error, "");
            }

            return std::make_tuple(error, buffer);

        }
	};
};
