# CMakeList.txt : CMake project for thermostat, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("thermostat")

# Add source to this project's executable.
add_executable (thermostat "src/thermostat.cpp" "src/thermostat.h" "src/sqlhelper/sqlhelper.cpp" "src/sqlhelper/sqlhelper.h" "src/main.cpp")

# TODO: Add tests and install targets if needed.
#sqlite packages
 find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(thermostat PRIVATE unofficial::sqlite3::sqlite3)

#curl packages

find_package(CURL CONFIG REQUIRED)
target_link_libraries(thermostat PRIVATE CURL::libcurl)

find_package(cpprestsdk CONFIG REQUIRED)
target_link_libraries(thermostat PRIVATE cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_brotli_internal)
