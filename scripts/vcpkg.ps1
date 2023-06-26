
# For windows
# I find it better to install vcpkg in a global place in pc
# so that we can use it for different projects at the same time instead
# of fetching and building all the libraries again and again

cd "C:/"
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
#./bootstrap-vcpkg.sh # for unix
./bootstrap-vcpkg.bat # for windows
./vcpkg integrate install
