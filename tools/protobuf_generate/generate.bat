@echo off

rem ����ͨ��Э��
cd .\ProtoCommon\
mkdir .\out
..\protoc *.proto --cpp_out=./out
xcopy /y .\out\*.* ..\..\..\libgame\ProtoCommon\ /EXCLUDE:exclude.txt
rmdir /Q/S .\out\
cd ..

pause