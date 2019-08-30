@echo off

call expected_test.exe > temp.txt
SET ec=%ERRORLEVEL%
SET /p output=<temp.txt

IF NOT '%ec%'=='0' (
    ECHO Program returned %ec%
    EXIT %ec%
)

ECHO %output% | FINDSTR /I "All tests passed" > nul
SET ec=%ERRORLEVEL%

IF NOT '%ERRORLEVEL%'=='0' (
    ECHO Program output incorrect: %output%
    EXIT %ERRORLEVEL%
)

ECHO All tests passed
ECHO %output%
EXIT 0
