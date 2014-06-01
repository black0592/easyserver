local teleportInfo = nil

function teleportMain(info)
	teleportInfo = info

	-- 生成传送列表
	local content = ""
	local count = table.getn(info)
	for i=0,count do
		local tmpInfo = teleportInfo[i]
		content = content .. string.format("<%s(LV%d)%s%d金币/teleportCheck(%d)>",
		tmpInfo["name"], tmpInfo["level"], tmpInfo["space"], tmpInfo["money"], i)
	end

	-- 弹出对话框
    say("想好要去哪了吗？那就请您选择吧！" .. content)
end

-- 传送检查
function teleportCheck(index)
	local tmpInfo = teleportInfo[index]

	if getMoney() < tmpInfo["money"] then
		sysChat("您的金币不够，无法传送")
		return
	end

	 say("您确定花【" .. tmpInfo["money"] .. "】金币, 传送到【" .. tmpInfo["name"]  .. "】吗？"
		.. "<我很确定/teleport(".. index ..")>"
		.. "<我再想想/main>"
	 )
end

-- 传送处理
function teleport(index)
	local tmpInfo = teleportInfo[index]
	subMoney( tmpInfo["money"] )
	local mapId = tmpInfo["map"]

	-- 加入国家的处理
	local country = getMapCountry()
	if country > 0 and mapId < 1000 then
		mapId = mapId + (country - 1) * 1000
	end

	gotoMap(mapId, tmpInfo["x"], tmpInfo["y"] )
end

