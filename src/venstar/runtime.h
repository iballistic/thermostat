#pragma once

#include <ctime>
#include <iostream>
#include <format>
#include <string>
struct Runtime
{
	long long int timestamp = { 0 };
	int heat1 = { 0 };
	int heat2 = { 0 };
	
	std::string timestamp_tostrig() {
		auto timeinfo = localtime(&timestamp);
		return std::asctime(timeinfo);
	}


};