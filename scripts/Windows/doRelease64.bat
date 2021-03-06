@echo off

call env-win64.bat

SET BUILD_DIR=SevenZipJBinding.Release64

rmdir /S /Q %BUILD_DIR%
mkdir %BUILD_DIR%
cd %BUILD_DIR%

cmake ../SevenZipJBinding %CMAKE_TOOLCHAIN% -DCMAKE_BUILD_TYPE=Release "-DJAVA_JDK=%JAVA_JDK%" 
%CMD_MAKE%
ctest -D Experimental -I
%CMD_MAKE% package
