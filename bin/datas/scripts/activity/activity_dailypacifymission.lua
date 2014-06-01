function main()
	accept()
   -- say("每日平定任务：每天一次！！！"
   --     .. "<接收任务/accept/icon1>")
end

function accept()
	if countPacifyDialy() > 0 then
		sysChat("同时只能接取一个同类型的任务")
	else
		openGUI("randommission")
		sendPacifyDialy()
	end
end

