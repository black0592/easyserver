@echo off
REM protoc --lua_out=./out person.proto

rem �л���.protoЭ�����ڵ�Ŀ¼  
rem ����ǰ�ļ����е�����Э���ļ�ת��Ϊlua�ļ�  
for %%i in (*.proto) do (    
echo %%i  
"protoc.exe" --plugin=protoc-gen-lua="C:\workspace\proto_lua\protoc-gen-lua\plugin\protoc-gen-lua.bat" --lua_out=./out %%i  
  
)  
REM echo end 

pause