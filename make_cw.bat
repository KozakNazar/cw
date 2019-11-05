rem ###########################
rem  N.Kozak // Lviv'2018
rem ############################

@echo off
cls
setlocal
..\..\portable_masm32\masm32p\masm32\bin\ML /c /coff %1.asm
if errorlevel 1 goto terminate
rem link /SUBSYSTEM:CONSOLE /LIBPATH:..\..\portable_masm32\masm32p\masm32\lib %1.obj
..\..\portable_masm32\masm32p\masm32\bin\link /SUBSYSTEM:CONSOLE ..\..\portable_masm32\masm32p\masm32\lib\msvcrt.lib %1.obj
if errorLevel 1 goto terminate
echo OK
:terminate
endlocal
EXIT %errorlevel% 