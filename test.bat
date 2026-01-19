:: Test, that is compile and run a c++11 program
:: *********************************************
:: *            _                              *
:: *           | | __ _ _ __   ___  ___        *
:: *        _  | |/ _` | '_ \ / _ \/ __|       *
:: *       | |_| | {_) | | | | ( ) \__ \       *
:: *        \___/ \__,_|_| |_|\___/|___/       *
:: *                                           *
:: *********************************************
:: For g++ on Windows you can use e.g. the MinGW
:: distribution maitained by
:: STL (Stephan T. Lavavej) at https://nuwen.net
::
@IF "%1"==""       GOTO Usage
@IF "%1"=="/?"     GOTO Usage
@IF "%1"=="-h"     GOTO Usage
@IF "%1"=="--help" GOTO Usage
@IF "%1"=="--Help" GOTO Usage
@IF "%1"=="--HELP" GOTO Usage
@if not exist bin mkdir bin
@SET fd=%~d1
@SET fp=%~p1
@SET fn=%~n1
@SET fx=%~x1
@if "%fx%" == ""     SET fx=.cpp
@if "%fx%" == ".cpp" GOTO cpp
@if "%fx%" == ".cxx" GOTO cxx
GOTO Extension
:cxx
:cpp
@SET fqn=%fd%%fp%%fn%
@if "%CD%\"=="%~dp1" SET fqn=%fn%
@if not exist "%fqn%%fx%" GOTO Missing
@SET cv=c++23
@SET optargs=
@SET optlibs=
@SET appargs=
@SET separator= 
@SET opt=-O2
@SET dln=
@SET deb=
:Loop
@SHIFT
@IF "%1"==""            GOTO Compleeted
@IF not "%appargs%"=="" GOTO CollectAppArgs
@IF "%1"=="-args"       GOTO InitAppArgs
@IF "%1"=="-std"        GOTO Loop
@IF "%1"=="EQUAL"       GOTO Equal
@IF "%1"=="Equal"       GOTO Equal
@IF "%1"=="equal"       GOTO Equal
@IF "%1"=="gdb"         GOTO Debug2
@IF "%1"=="-g3"         GOTO Debug
@IF "%1"=="-g2"         GOTO Debug
@IF "%1"=="-g1"         GOTO Debug
@IF "%1"=="-g0"         GOTO Debug
@IF "%1"=="-g"          GOTO Debug
@IF "%1"=="-O3"         GOTO Optimize
@IF "%1"=="-O2"         GOTO Optimize
@IF "%1"=="-O1"         GOTO Optimize
@IF "%1"=="-O0"         GOTO Optimize
@IF "%1"=="c++11"       GOTO Std
@IF "%1"=="c++14"       GOTO Std
@IF "%1"=="c++17"       GOTO Std
@IF "%1"=="c++20"       GOTO Std
@IF "%1"=="c++23"       GOTO Std
@IF "%1"=="-l"          GOTO AddLib
@IF "%1"=="-L"          GOTO AddLibDir
@SET optargs=%optargs%%separator%%1
@SET separator= 
@GOTO Loop
:InitAppArgs
@SET appargs= 
@GOTO Loop
:CollectAppArgs
@SET appargs=%appargs% %1
@GOTO Loop
:Debug2
@SET opt=-O0
@SET dln=-g2
@SET deb=gdb
@GOTO Loop
:AddLib
:AddLibDir
@SET optlibs=%optlibs% %1 %2
@SHIFT
@GOTO Loop
:Debug
@SET opt=
@SET dln=%1
@SET deb=gdb
@GOTO Loop
:Optimize
@SET opt=%1
@GOTO Loop
:Equal
@SET optargs=%optargs%=
@SET separator=
@GOTO Loop
:Std
@SET cv=%1
@GOTO Loop
:Compleeted
g++ -std=%cv% %opt% %dln% -Wall -Wextra -Wpedantic -Werror -time %optargs% %fqn%%fx% -o bin\%fn% && %deb% .\bin\%fn%%appargs%
@GOTO :EOF
:Usage
@ECHO Usage:
@ECHO test cpp_filename_without_the_cpp_extension [ compiler options ]
@ECHO test [ --help ] for this help
@ECHO The [ ] square brackets denote the optionality of the enclosed part.
@ECHO For compiler options substitute the "=" (equal sign) with the word "equal".
@ECHO For example instead of the "-D VERBOSE=11" write "-D VERBOSE equal 11".
@ECHO You can specify the language version by typing [-std=]c++11, c++14, c++17, c++20 or c++23.
@ECHO You can specify the optimization level by typing -O0, -O1, -O2, or -O3.
@ECHO You can specify the debug level by typing -g -g0 -g1, -g2, -g3 or gdb.
@ECHO You can specify any artuments for your application after typing -args.
@GOTO :EOF
:Extension
@echo "File extension must be .cpp or .cxx, the default is .cpp!"
@echo "This script is not prepared for handling %fx% as source file extension!"
@GOTO :EOF
:Missing
@echo "%fqn%%fx% file doesn't exist!"
