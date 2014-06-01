function main()
    say("你敢和我猜拳吗？如果你赢了,我会送你大量金币，输了你也要给我金币。"
		.. "<我押 1000 金币/add_money(1000)/icon1>"
		.. "<我押  1万 金币/add_money(10000)/icon1>"
		.. "<我押 10万 金币/add_money(100000)/icon1>"
		)
end

-- 押注
local final_money = 0
function add_money(money)
	final_money = money
	beginFinger()
end


function beginFinger()
	if getMoney() < final_money then
		say("对不起，你的金币好像不够啊。。。"
		.. "<我去拿钱/close>"
		)
		return
	end

	say("你可以选择了，要想清楚啦。"
		.. "<石头/finger(1)>"
		.. "<剪刀/finger(2)>"
		.. "<布/finger(3)>"
		.. "<重新押注/main>"
		)
end

-- 检测结果,0-输了，1-赢了, 2-平局
function checkWiner(val1, val2)
	if val1 == val2 then
		return 2
	end

	-- 石头对剪刀
	if val1 == 1 and val2 == 2 then
		return 1
	end

	-- 剪刀对布
	if val1 == 2 and val2 == 3 then
		return 1
	end

	-- 布对石头
	if val1 == 3 and val2 == 1 then
		return 1
	end

	return 0
end

function getFingerName(value)
	if value == 1 then
		return "石头"
	elseif value == 2 then
		return "剪刀"
	elseif value == 3 then
		return "布"
	end
end

function finger(value)
	npcVal = math.random(3)
	playerVal = value
	result = checkWiner(playerVal, npcVal)
	if result == 2 then
		say("我也出["..  getFingerName(npcVal) .."]，平局"
		.. "<再来/beginFinger>"
		)
	elseif result == 0 then
		say("我出["..  getFingerName(npcVal) .."]你输了,请继续努力！"
		.. "<再来/beginFinger>"
		)
		subMoney(final_money)
		sysChat("你被扣除金币 " .. final_money)
	elseif result == 1 then
		say("恭喜你，你赢了！！"
		.. "<再来/beginFinger>"
		)
		addMoney(final_money)
		sysChat("你赢得金币 " .. final_money)
	end
end