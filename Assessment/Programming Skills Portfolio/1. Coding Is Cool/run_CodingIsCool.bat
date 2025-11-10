@echo off
cls
echo Compiling CodingIsCool...
g++ CodingIsCool.cpp -o CodingIsCool.exe
if %errorlevel% neq 0 (
	echo.
	echo Compilation failed!
	pause
	exit /b
)
echo Runing CodingIsCool...
CodingIsColl.exe
pause