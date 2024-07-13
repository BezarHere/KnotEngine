@echo off

SET EXE="./x64/Debug/tests.exe"

objdump -d -C -M intel -M notes -S --source-comment="; " --visualize-jumps -M reg-names-raw %EXE% > "./release.s"
rem objdump -d -C -M amd64 %EXE% > "./release.asm"

rem py py/comment_call_asm.py
