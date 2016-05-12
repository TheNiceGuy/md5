@echo off
chcp 1252


cls

:: On demande à l'utilisateur de rentrer un string.
set /p MESSAGE="Veuillez entrer le message à hacher : "

:: On execute le programme en mode string.
bin\md5.exe --string "%MESSAGE%"

:: On écrit une ligne blanche.
echo.

:: On attend l'intervention de l'utilisateur pour quitter.
pause
