add_library(SQLHelper sqlhelper.h sqlhelper.cpp)
target_include_directories(SQLHelper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#sqlite packages
find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(SQLHelper PRIVATE unofficial::sqlite3::sqlite3)