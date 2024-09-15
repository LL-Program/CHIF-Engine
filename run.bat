@echo off

echo This script is for the dev version of the engine.
:: Check if Python is installed
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Python is not installed. Please install Python first.
    pause
    exit /b
)

:: Check if pip is installed
pip --version >nul 2>&1
if %errorlevel% neq 0 (
    echo pip is not installed. Installing pip...
    python -m ensurepip --upgrade
)

:: Ensure pip is updated - takes longer 
::echo Updating pip...
::python -m pip install --upgrade pip

:: Install required packages
echo Installing required packages: moderngl, numpy, pygame, PyGLM, pywavefront, customtkinter(maybe tkinter), matplotlib...
echo This could take a while if you run it for the first time...
pip install moderngl numpy pygame PyGLM pywavefront customtkinter matplotlib

:: Check if installation was successful
if %errorlevel% neq 0 (
    echo Failed to install some packages. Exiting.
    pause
    exit /b
)

:: Run the main Python script
echo Running src/main.py...
python src/main.py

:: Pause to see output if needed
pause
