@echo off
if "%1"=="" goto :noargs

set mapfile=raytrace%1.map
set btlfile=raytrace%1.btl

if not exist %mapfile% goto :nomap
if not exist %btlfile% goto :nobtl

set /a nworkers=%1-1
echo "Workers = %nworkers%"

set SPYNET=%mapfile%
for /L %%i in (1,1,%nworkers%) do (
echo "Starting worker%%i"
start /min t4 -s8 -sn %%i -si
timeout 1 >NUL
)

echo "Starting HOST"
t4 -s8 -sn 0 -sb %btlfile%

rem Kill workers
taskkill /f /im t4.exe 1>NUL 2>NUL

goto :exit

:noargs
echo "usage: multi.bat <#processors>"
goto :exit

:nomap
echo "%mapfile% does not exist!"
goto :exit

:nobtl
echo "%btlfile% does not exist!"

:exit
