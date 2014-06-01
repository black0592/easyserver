function main()
    say("VIP充值测试: 您当前累计充值【" .. getTotalGold() .. "】"
		.. "<公会测试/guildTest/icon1>"
		.. "<充1000钻石/addgold3/icon1>"
		.. "<我要升级/addexp/icon1>"
		.. "<我要金币/addmoney/icon1>"
		.. "<清除数据/cleargold/icon1>"
		)
end

-- 随机任务
function randommission()
	openGUI("randommission")
end

-- 公会测试
function guildTest()
	credits = 100
	res1 = 100
	res2 = 100
	testGuild(credits, res1, res2)
end

-- 充1
function addgold1()
	addGold(1)
    addTotalGold(1)
	main()
end

-- 充100
function addgold2()
	addGold(100)
    addTotalGold(100)
	main()
end

-- 充1000
function addgold3()
	addGold(1000)
    addTotalGold(1000)
	main()
end

function addexp()
    addExp(getLevel() * 1000)
end

function addmoney()
    addMoney(getLevel() * 1000)
end


function cleargold()
	clearTotalGold()
	clearVipAward()
	main()
end