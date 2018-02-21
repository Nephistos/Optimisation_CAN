 @echo off
cls

::QUESTION
:question
set /p choix= Choix de la messagerie C1A : 1 - C1AHS : 2. :
 
if /I "%choix%"=="1" (goto :A)
if /I "%choix%"=="2" (goto :B)
goto :question
 
 ::C1A
:A
echo.

echo MODE1 : Routage des trames et/ou evolution de la topologie
echo MODE2 (Beta) : Mise en trame des parametres
echo MODE3 : Ajout d'un nouveau calculateur/d'une nouvelle trame et routage des trames
echo.

set /p choix= MODE1 : 1 - MODE2 : 2 - MODE3 : 3. :
 
if /I "%choix%"=="1" (goto :C1AMODE1)
if /I "%choix%"=="2" (goto :C1AMODE2)
if /I "%choix%"=="3" (goto :C1AMODE3)
pause
goto :A
 
 ::C1AHS
:B
echo.

echo MODE1 : Routage des trames et/ou evolution de la topologie
echo MODE2 (Beta) : Mise en trame des parametres
echo MODE3 : Ajout d'un nouveau calculateur/d'une nouvelle trame et routage des trames
echo.

set /p choix= MODE1 : 1 - MODE2 : 2 - MODE3 : 3. :
 
if /I "%choix%"=="1" (goto :C1AHSMODE1)
if /I "%choix%"=="2" (goto :C1AHSMODE2)
if /I "%choix%"=="3" (goto :C1AHSMODE3)
pause
goto :B

::C1AMODE1
:C1AMODE1

copy CFG\C1A\MODE1\cfg.txt
pause

goto :eof

::C1AMODE2
:C1AMODE2

copy CFG\C1A\MODE2\cfg.txt
pause

goto :eof

::C1AMODE3
:C1AMODE3

copy CFG\C1A\MODE3\cfg.txt
pause

goto :eof

::C1AHSMODE1
:C1AHSMODE1

copy CFG\C1AHS\MODE1\cfg.txt
pause

goto :eof

::C1AHSMODE2
:C1AHSMODE2

copy CFG\C1AHS\MODE2\cfg.txt
pause

goto :eof

::C1AHSMODE3
:C1AHSMODE3

copy CFG\C1AHS\MODE3\cfg.txt
pause

goto :eof
 
:end
pause

 