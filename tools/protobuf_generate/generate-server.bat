@echo off

rem ���������Э��
cd .\ProtoServer\
mkdir .\out
..\protoc *.proto --cpp_out=./out
xcopy /y .\out\*.* ..\..\..\libgame\ProtoServer\
rmdir /Q/S .\out\
cd ..

pause