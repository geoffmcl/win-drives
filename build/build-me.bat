@setlocal
@set TMPPRJ=drives
@set TMPSRC=..
@set TMPLOG=bldlog-1.txt

@if NOT EXIST %TMPSRC%\CMakeLists.txt goto NOCM

@call chkmsvc %TMPPRJ%

@echo Begin %TMPPRJ% %DATE% %TIME% > %TMPLOG%

@echo Doing: 'cmake %TMPSRC%`
@echo Doing: 'cmake %TMPSRC%` >> %TMPLOG%
@cmake %TMPSRC% >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR1

@echo Doing: 'cmake --build . --config Debug'
@echo Doing: 'cmake --build . --config Debug' >> %TMPLOG%
@cmake --build . --config Debug >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR2

@echo Doing: 'cmake --build . --config Release'
@echo Doing: 'cmake --build . --config Release' >> %TMPLOG%
@cmake --build . --config Release >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR3

@echo.
@echo Appears a successful build...
@echo No install at this time... 
@echo updexe.bat may be useful to copy release exe to C:\MDOS\, if that is suitable...
@echo.
@goto END

:NOCM
@echo Error: Can NOT locate %TMPSRC%\CMakeLists.txt! *** FIX ME ***
@goto ISERR

:ERR1
@echo cmake config, generation error
@goto ISERR

:ERR2
@echo cmake debug build error
@goto ISERR

:ERR3
@echo cmake release build error
@goto ISERR

:ISERR
@endlocal
@exit /b 1

:END
@endlocal
@exit /b 0

@REM eof
