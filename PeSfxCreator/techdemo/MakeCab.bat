@echo off

SET BASE_DIR=app
SET OUT_DIR=%CD%
SET EXE_FILE=app.cmd
SET INTERMEDIATE_DIR=%%TMP%%

cd %BASE_DIR%
MAKECAB.EXE /f ..\CAB.DDF
pause
