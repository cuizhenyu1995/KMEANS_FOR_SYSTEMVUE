@ECHO OFF
set PATH=C:/Program Files/SystemVue2016.08/Tools/CMake/bin;%PATH%
@ECHO ON
mkdir build-win64-vs2013
cd build-win64-vs2013
cmake -DCMAKE_INSTALL_PREFIX=../output-vs2013 -G "Visual Studio 12 Win64" ../source
cd ..
pause
