@echo off

robocopy %1 %2 %~3 /NP

if %ERRORLEVEL% geq 8 goto end

:reseterror
set errorlevel=0

:end
exit /b
