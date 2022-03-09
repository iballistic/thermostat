# CMakeList.txt : CMake project for thermostat, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)

project (THERMOSTAT VERSION 1.0)

# https://cmake.org/cmake/help/latest/prop_tgt/CXX_STANDARD.html
# https://docs.microsoft.com/en-us/cpp/build/reference/std-specify-language-standard-version?view=msvc-170
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_subdirectory(src/utils)
add_subdirectory(src/venstar)
add_subdirectory(src/weather)
add_subdirectory(src/sqlhelper)

# Add source to this project's executable.
add_executable (Thermostat "src/main.cpp" "src/schema.h")

target_link_libraries(Thermostat PUBLIC Utils)
target_link_libraries(Thermostat PUBLIC VenstarRestClient)
target_link_libraries(Thermostat PUBLIC WeatherRestClient)
target_link_libraries(Thermostat PUBLIC SQLHelper)
