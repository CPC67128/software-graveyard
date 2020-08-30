@ECHO OFF
set TMPPATH=%PATH%
rem ***************************************************
rem * Configuration des chemins d'accés pour          * 
rem * Java Advanced Imaging                           *
rem ***************************************************
rem * Il faut que vous configuriez manuellement       *
rem * les chemins d'accés aux fichiers de JAI pour    *
rem * que l'ouverture des fichiers se passe bien      *
rem * Il faut modifier la variable 'PATH' et aussi    *
rem * mettre en place la variable 'JARPATH'           *
rem * il faut que cette derniere contiene :           *
rem * jai_core.jar, jai_codec.jar, mlibwrapper_jai.jar*
rem * Exemples :                                      *
rem ***************************************************
rem * JAI installé sur I:\P03A :
rem * set PATH=I:\P03A;%PATH%
rem * set JARPATH=I:\P03A\jai_core.jar;I:\P03A\jai_codec.jar;I:\P03A\mlibwrapper_jai.jar
rem ***************************************************
rem * JAI installé sur E:\programm\java\jre\lib\ext :
rem * set PATH=E:\programm\java\jre\lib\ext;%PATH%
rem * set JARPATH=E:\programm\java\jre\lib\ext\jai_core.jar;E:\programm\java\jre\lib\ext\jai_codec.jar;E:\programm\java\jre\lib\ext\mlibwrapper_jai.jar
rem ***************************************************
set PATH=%PATH%;I:\P03A
set JARPATH=I:\P03A\jai_core.jar;I:\P03A\jai_codec.jar;I:\P03A\mlibwrapper_jai.jar

if "%1"=="" GOTO normal
if "%1"=="debug" GOTO debug
if "%1"=="clean" GOTO clean
GOTO erreur_1

:debug
ECHO Debug mode
ECHO Creation des répertoires
mkdir .\debug
mkdir .\debug\class
mkdir .\debug\aide
mkdir .\debug\aide\html
if "%OS%"=="Windows_NT" GOTO winnt
Echo Compilation
javac -d .\debug\class -sourcepath ".\source" -classpath %CLASSPATH%;%JARPATH%;.\source source/photoshed/*.java >.\makeout.txt
GOTO jar

:winnt
ECHO Compilation version Windows NT
javac -d .\debug\class -sourcepath ".\source" -classpath %CLASSPATH%;%JARPATH%;.\source source/photoshed/*.java >.\makeout.txt 2>.\makeerr.txt

:jar
ECHO Creation du fichier jar dans le repertoire .\debug
jar cvfm debug\Photoshed.jar manifest.txt -C .\debug\class . >>.\makeout.txt
ECHO Copie des resources
copy source\photoshed\images debug\ >>.\makeout.txt
copy *.wlg .\debug >>.\makeout.txt
copy .\source\photoshed\aide\html\*.* .\debug\aide\html >>.\makeout.txt
GOTO end

:normal
ECHO Creation des répertoires
mkdir .\release >.\makeout.txt
mkdir .\release\class >>.\makeout.txt
mkdir .\release\aide >>.\makeout.txt
mkdir .\release\aide\html >>.\makeout.txt
ECHO Compilation
javac -d .\release\class -sourcepath ".\source" -O -g:none -classpath %CLASSPATH%;%JARPATH%;.\source source/photoshed/*.java >>.\makeout.txt
ECHO Creation du fichier jar dans le repertoire .\release
jar cvfm release\Photoshed.jar manifest.txt -C .\release\class . >>.\makeout.txt
ECHO Copie des resources
copy source\photoshed\images .\release >>.\makeout.txt
copy *.wlg .\release >>.\makeout.txt
copy .\source\photoshed\aide\html\*.* .\release\aide\html >>.\makeout.txt
ECHO Suppression des fichiers temporaires
rmdir /S /Q .\release\class >>.\makeout.txt
del /Q .\makeout.txt
GOTO end

:clean
del makeerr.txt
del makeout.txt
rmdir /S /Q .\release
rmdir /S /Q .\debug
GOTO end

:erreur_1
ECHO Option %1 incorrecte
ECHO Usage : make [-debug]
ECHO    -debug            Met en place les options de debug

:end
set PATH=%TMPPATH%
set TMPPATH=
set JARPATH=