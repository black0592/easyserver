require "login_handler"

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

