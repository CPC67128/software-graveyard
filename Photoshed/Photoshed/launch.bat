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

if "%1"=="debug" GOTO debug
cd release
javaw -cp %CLASSPATH%;%JARPATH%;Photoshed.jar photoshed.Photoshed %1 %2 %3 %4 %5 %6 %7 %8 %9
GOTO end

:debug
cd debug
if "%OS%"=="Windows_NT" GOTO winnt
java -Xfuture -cp %CLASSPATH%;%JARPATH%;Photoshed.jar photoshed.Photoshed -debug %2 %3 %4 %5 %6 %7 %8 %9
GOTO end

:winnt
java -Xfuture -cp %CLASSPATH%;%JARPATH%;Photoshed.jar photoshed.Photoshed -debug %2 %3 %4 %5 %6 %7 %8 %9 >out.txt 2>err.txt
GOTO end

:end
set PATH=%TMPPATH%
set TMPPATH=
set JARPATH=
cd ..