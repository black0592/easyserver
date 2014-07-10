function ServerTask_OnConnect()
	print("Call ServerTask_OnConnect");
end

function ServerTask_OnDisconnect()
	print("Call ServerTask_OnDisconnect");
end

function ServerTask_handleProtoMsg(args)
	print("Call ServerTask_handleProtoMsg");
end