@echo off
REM protoc --lua_out=./out person.proto

rem 切换到.proto协议所在的目录  
rem 将当前文件夹中的所有协议文件转换为lua文件  
for %%i in (*.proto) do (    
echo %%i  
"protoc.exe" --plugin=protoc-gen-lua="C:\workspace\proto_lua\protoc-gen-lua\plugin\protoc-gen-lua.bat" --lua_out=./out %%i  
  
)  
REM echo end 

pause