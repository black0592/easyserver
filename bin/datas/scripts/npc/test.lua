function main()
    say("技术测试期间可以每天领取一定数量的金币和钻石"
		.. "<领取 1000 金币/takemoney/icon1>"
		.. "<领取 500  钻石/takegold/icon1>"
		.. "<升级去了/exit/icon1>"
		)
end

function takemoney()
	if counterDayGet(100) > 0 then
		sysChat("今日的金币奖励已经领过了，请明天再来！")
		return
	end

	addMoney(1000)
	counterDaySet(100)
	main()
end

function takegold()
	if counterDayGet(200) > 0 then
		sysChat("今日的钻石奖励已经领过了，请明天再来！")
		return
	end

	addGold(500)
	counterDaySet(200)
	main()
end