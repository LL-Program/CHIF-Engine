@echo off
SET version=%~1%
if "%~1"=="" SET version="vs2022"

set devenv_cmd=devenv.exe
where /q devenv.exe
set devenv_cmd="C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe"

start "" %devenv_cmd% "BUILD/CHIFEngine.sln"
echo "Please build the following project, before starting the exe!"
PAUSE
