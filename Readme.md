# Building on Windows 
## Using packages
### Configure vcpkg
```
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

### Install required packages
```
.\vcpkg install sqlite3:x64-windows
.\vcpkg install curl[tool]:x64-windows
```

### Inlcude packages in the CMakeLists.txt

```
#sqlite packages
 find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(thermostat PRIVATE unofficial::sqlite3::sqlite3)
```
```
#curl packages
find_package(CURL CONFIG REQUIRED)
target_link_libraries(thermostat PRIVATE CURL::libcurl)
```

mkdir "out"
cd "out"
mdkir "build"
cmake -B build -S .. -DCMAKE_TOOLCHAIN_FILE=d:\gitrepos\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build "build"