@echo off

:: On cherche tous les fichiers avec les extensions *.c, et *.h ou qui commencent avec makefile.
for /f %%A in ('dir *.c *.h makefile* /b/s') do (
    :: On écrit le nom du fichier.
    echo %%A

    :: On execute le programme en mode fichier.
    bin\md5.exe --file %%A

    :: On execute le programme en mode stdin en recevant le message par une redirection (pipe).
    type %%A | bin\md5.exe --stdin

    :: On compare avec une implémentation indépendante de MD5.
    bin\md5deep.exe %%A 2> nul

    :: On écrit une ligne blanche.
    echo.
)

:: On attend l'intervention de l'utilisateur pour quitter.
pause
