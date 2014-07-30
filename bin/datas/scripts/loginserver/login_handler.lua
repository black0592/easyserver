require "ProtoSvrLogin_pb"

-- Âß¼­´¦Àí
function handleRequestRegisterGameServer(task, pb_data)
	local msg = ProtoSvrLogin_pb.RequestRegisterGameServer()
	msg:ParseFromString(pb_data)
	print( string.format("parser2: id=%d name=%s port=%d", msg.id, msg.name, msg.port) )
end