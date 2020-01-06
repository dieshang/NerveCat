rem @echo off
dir /b /s /AD debug>>files.txt
dir /b /s /AD release>>files.txt
dir /b /s /AD ipch >>files.txt
for /f "usebackq delims==" %%i in (files.txt) do rd /s /q "%%i"
del /q files.txt

dir /b /s *.ncb>>files.txt
dir /b /s *.suo>>files.txt
dir /b /s *.aps>>files.txt
dir /b /s *cache.dat>>files.txt
dir /b /s *.ilk>>files.txt
dir /b /s *.pdb>>files.txt
dir /b /s *.res>>files.txt
dir /b /s *.pch>>files.txt
dir /b /s *.idb>>files.txt
dir /b /s *.sdf>>files.txt
dir /b /s BuildLog.htm>>files.txt
dir /b /s mt.dep>>files.txt
for /f "usebackq delims==" %%i in (files.txt) do del /s /q "%%i"
del /q files.txt
rem @echo on