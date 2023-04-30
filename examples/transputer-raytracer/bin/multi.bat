@echo off
set SHLINK=
if "%1"=="" goto :noargs

set N=%1
if "%1"=="/sl" goto :setshlink

:checkfiles
set mapfile=raytrace%N%.map
set btlfile=raytrace%N%.btl

if not exist %mapfile% goto :nomap
if not exist %btlfile% goto :nobtl

set /a nworkers=%N%-1
echo "Workers = %nworkers%"

set SPYNET=%mapfile%
for /L %%i in (1,1,%nworkers%) do (
echo "Starting worker%%i"
start /min t4 -s8 %SHLINK% -sn %%i -si
timeout 1 >NUL
)

echo "Starting HOST"
t4 -s8 %SHLINK% -sn 0 -sb %btlfile%

rem Kill workers
taskkill /f /im t4.exe 1>NUL 2>NUL

goto :exit

:setshlink
set SHLINK="/sl"
set N=%2
goto :checkfiles

:noargs
echo "usage: multi.bat [/sl] <#processors>"
goto :exit

:nomap
echo "%mapfile% does not exist!"
goto :exit

:nobtl
echo "%btlfile% does not exist!"

:exit
