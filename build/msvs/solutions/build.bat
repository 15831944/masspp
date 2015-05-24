@echo off

@rem Usage:  
@rem   build [CONFIG] [ARCH]
@rem   ( CONFIG: Release, Debug or all(default),  ARCH: x86(default) or x64 )
@rem 
@rem Caution: This batch file is just tested on Windows 7
@rem 
if "%2"=="" (set ARCH=x86) else (set ARCH=%2)
if "%1"=="" (set Configuration=all) else (set Configuration=%1)
if %Configuration%==all (set CONFIG=Release Debug) else (set CONFIG=%Configuration%)
@rem PLTFRM: Win32 or  x64 
if "%2"=="x64" (set PLTFRM=x64) else (set PLTFRM=Win32)

@echo on
@echo Builiding Mass++ for %ARCH% at %time% on %date%. 
@echo off

@rem remove old backup log and backup latest log
for %%c in (%CONFIG%) do (
  if exist build-%ARCH%-%%c.log.bak del build-%ARCH%-%%c.log.bak
  if exist build-%ARCH%-%%c.log ren build-%ARCH%-%%c.log build-%ARCH%-%%c.log.bak
  echo Start of building Mass++ for %ARCH% at %time% on %date%. >>  build-%ARCH%-%%c.log  2>&1
  echo; >>  build-%ARCH%-%%c.log  2>&1
)

@rem  Build Plug-ins written in Native Code

for %%s in (
 common-lib
 common-exe
 common-plugin-analyze
 common-plugin-io
 shimadzu-input-plugin
 common-plugin-operation
 common-plugin-view
 eisai-common-plugin
) do (
 for %%c in (%CONFIG%) do (
  call :build-savelog %%s %%c %PLTFRM% %ARCH%
))


@rem  Build Plug-ins written in Managed Code
for %%s in (
 shimadzu-win-plugin
) do (
 for %%c in (%CONFIG%) do (
  call :build-savelog %%s %%c %ARCH%  %ARCH%
))


echo Finished building Mass++ for %ARCH% at %time% on %date%.

goto :eof


@rem
@rem SUBROUTINE
@rem

:build-savelog
@rem %1:solution ,%2:Debug or Release, %3: Win32, x86, x64, etc  %4: x86 or x64
call :addcomment %1 %4 %2 %3 Start
call :build %1 %2 %3 %4
call :addcomment %1 %4 %2 %3 End
goto :eof


:build
@rem %1:solution name, %2:Debug or Release, %3:Win32, x86, x64, etc. %4: %ARCH%
@echo on
msbuild %1/%1.sln /p:Platform=%3 /p:Configuration=%2 >> build-%4-%2.log 2>&1
@echo off
goto :eof

:addcomment
@rem %1 solution %2:ARCH %3:Config %4:Win32, x86, x64, etc. %5 End or Start
@echo; >>  build-%2-%3.log  2>&1
@echo %5 of building %1 (%3) for %4 at %time% on %date%. >>  build-%2-%3.log  2>&1
@echo; >>  build-%2-%3.log  2>&1
@echo off
goto :eof
