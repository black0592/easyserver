--require "login_handler"
require "ProtoSvrLogin_pb"

-- 链接
function ServerTask_OnConnect(task)
	print("task:", task)
	print("Call ServerTask_OnConnect");
end

-- 断开链接
function ServerTask_OnDisconnect(task)
	print("task:", task)
	print("Call ServerTask_OnDisconnect");
end


-------------------------------------------------------------------------------
function objinfo(obj)

	local meta = getmetatable(obj)
	if meta ~= nil then
		metainfo(meta)
	else
		print("no object infomation !!")
	end
end

function metainfo(meta)

	if meta ~= nil then
		local name = meta["__name"]
		if name ~= nil then
			metainfo(meta["__parent"])
			print("<"..name..">")
			for key,value in pairs(meta) do 
				if not string.find(key, "__..") then 
					if type(value) == "function" then
						print("\t[f] "..name..":"..key.."()") 
					elseif type(value) == "userdata" then
						print("\t[v] "..name..":"..key)
					end
				end
			end
		end
	end
end
-------------------------------------------------------------------------------

function test_func()
	--print("objinfo(g_task)")
	--temp_task = ServerTaskSync()
	--temp_task:sendProtoMsg()

	--objinfo(g_task)
	--g_task:sendProtoMsg()

	print("111ggggggg")
	print(g_test:ret_int())
	print("222ggggg")

	print("111hhhhhh")

	print("fun-333")
	g_task:sendProtoMsg3()

	print("fun-111")

	local msg = ProtoSvrLogin_pb.RequestRegisterGameServer()
	msg.id = 567
	msg.name = "lua_test"
	msg.port = 8888
	sendProtoMsg(msg)
	--local pb_data = msg:SerializeToString()
	--g_task:sendProtoMsg(pb_data)
	--g_task:sendProtoMsg()

	--g_task:sendProtoMsg2()
	print("222hhhhhh")
end

function sendProtoMsg(msg)
	local pb_data = msg:SerializeToString()
	local pb_size = msg:ByteSize()
	print("--pb_data--")
	print(pb_data)
	print("--pb_data--")
	g_task:sendProtoMsg2(pb_data, pb_size)
end

--[[
function test_func(ttt)
	print("111tttttt")
	print(ttt:ret_int())
	ttt:sendProtoMsg()
	print("222tttttt")
end
]]

-- 消息回调
--[[
function ServerTask_handleProtoMsg(task, pb_data)
	print("pb_data: " .. pb_data)

	-- LoginCmd.RequestRegisterGameServer
	local msg = ProtoSvrLogin_pb.RequestRegisterGameServer()
	msg:ParseFromString(pb_data)
	--print(msg.name)
	print( string.format("parser: %d %s %d", msg.id, msg.name, msg.port) )
	print("Call ServerTask_handleProtoMsg");
	--handleRequestRegisterGameServer(msg)
	
	--package.loaded["login_handler"] = nil
	--require "login_handler"
end
]]

