add_library(WeatherRestClient weatherprovider.h darksky.h darksky.cpp)
target_include_directories(WeatherRestClient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#sqlite packages
find_package(cpprestsdk CONFIG REQUIRED)
target_link_libraries(WeatherRestClient PRIVATE cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_brotli_internal)
target_link_libraries(WeatherRestClient PUBLIC Utils)