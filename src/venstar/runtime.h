#pragma once

#include <ctime>
#include <iostream>
#include <format>
#include <string>

#include"utils.h"

struct Runtime
{
	long long int timestamp = { 0 };
	int heat1 = { 0 };
	int heat2 = { 0 };
	
    std::string timestampToString() {
        auto result = Utils::TimeUtil::datetimeToString(timestamp);
        auto errorCode = std::get<0>(result);
        auto timeinfo = std::get<1>(result);
        return timeinfo;
    }

};