@echo off



cls


set /p MESSAGE="Veuillez entrer le message � hacher : "

:: On execute le programme en mode string.
bin\md5.exe --string "%MESSAGE%"

:: On �crit une ligne blanche.
echo.

:: On attend l'intervention de l'utilisateur pour quitter.
pause