add_library(VenstarRestClient thermostat.h runtime.h thermostat.cpp)
target_include_directories(VenstarRestClient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(VenstarRestClient PUBLIC Utils)