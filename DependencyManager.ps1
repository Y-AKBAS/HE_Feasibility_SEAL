
# For windows

cd "C:/"
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
#./bootstrap-vcpkg.sh # for unix
.\bootstrap-vcpkg.bat # for windows
./vcpkg integrate install

# Dependencies
./vcpkg install seal
./vcpkg install grpc
./vcpkg install doctest
./vcpkg install log4cplus
./vcpkg install boost

cd $PSScriptRoot
