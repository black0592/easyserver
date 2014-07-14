require "ProtoSvrLogin_pb"

function ServerTask_OnConnect(task)
	print("task:", task)
	print("Call ServerTask_OnConnect");
end

function ServerTask_OnDisconnect(task)
	print("task:", task)
	print("Call ServerTask_OnDisconnect");
end

function ServerTask_handleProtoMsg(task, msg)
	print("task:" .. task .. "msg:" .. msg);
	--print("parser:", msg.id, msg.name, msg.port)
	print("Call ServerTask_handleProtoMsg");
end