# CMakeList.txt : CMake project for thermostat, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)

project ("thermostat" VERSION 1.0)

# https://cmake.org/cmake/help/latest/prop_tgt/CXX_STANDARD.html
# https://docs.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Add source to this project's executable.
add_executable (Thermostat "src/main.cpp")

#Venstart REST Client
add_library(VenstarRestClient "src/venstar/thermostat.cpp")
target_link_libraries(Thermostat PUBLIC VenstarRestClient)
target_include_directories(Thermostat PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/venstar")


# SQLite Helper Library
add_library(SQLHelper "src/sqlhelper/sqlhelper.cpp")
target_link_libraries(Thermostat PUBLIC SQLHelper)
target_include_directories(Thermostat PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/sqlhelper")

# TODO: Add tests and install targets if needed.
#sqlite packages
find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(Thermostat PRIVATE unofficial::sqlite3::sqlite3)

#curl packages

find_package(CURL CONFIG REQUIRED)
target_link_libraries(Thermostat PRIVATE CURL::libcurl)

find_package(cpprestsdk CONFIG REQUIRED)
target_link_libraries(Thermostat PRIVATE cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_brotli_internal)
