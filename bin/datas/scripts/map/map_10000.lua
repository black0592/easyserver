function main()
	
end

local daojishi = 7
local tickStep = 2
local first = true
local moncount = 1

function autoCreateMonster(moncnt, num)
	if moncnt == 1 then
		createMonster(1001, 20, 20, 100, num)
	elseif moncnt == 2 then
		createMonster(1002, 20, 20, 100, num)
	elseif moncnt == 3 then
		createMonster(1003, 20, 20, 100, num)
	elseif moncnt == 4 then
		createMonster(1004, 20, 20, 100, num)
	elseif moncnt == 5 then
		createMonster(1005, 20, 20, 100, num)
	elseif moncnt == 6 then
		createMonster(1006, 20, 20, 100, num)
	elseif moncnt == 7 then
		createMonster(1007, 20, 20, 100, num)
	elseif moncnt == 8 then
		createMonster(1008, 20, 20, 100, num)
	elseif moncnt == 9 then
		createMonster(1009, 20, 20, 100, num)
	elseif moncnt == 10 then
		createMonster(1010, 20, 20, 100, num)
	elseif moncnt == 11 then
		createMonster(1011, 20, 20, 100, num)
	elseif moncnt == 12 then
		createMonster(1012, 20, 20, 100, num)
	elseif moncnt == 13 then
		createMonster(1023, 20, 20, 100, num)
	end
end

function onTimer(second)
	if second % tickStep == 0 and getMonsterCount() == 0 then

		if moncount > 3 then
			return
		end

		if moncount == 3 then
			sysChat("恭喜你完成了<无尽的进攻>！！")
			moncount = moncount + 1
			createNpc(205, 20, 20, "猜拳")
			return
		end


		if first then
			-- 如果提一次，就提示给玩家触发了隐藏活动
			sysChat("恭喜你歼灭了怪物，<无尽的进攻>开启了!")
			first = false
		end



		if daojishi == 0 then
			-- 创建蓝色幼龙(怪物id,x,y,掉落,概率)
			sysChat("怪物出现了，当心了！！")
			autoCreateMonster(moncount, 5)
			moncount = moncount + 1
			-- 初始化倒计时
			daojishi = 7
		else
			if daojishi <=5 then
				sysChat("下一波怪物倒计时: " .. daojishi)
			end
			daojishi = daojishi - 1
		end

	end
end