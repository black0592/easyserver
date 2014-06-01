function main()
	if getMonsterCount(16) > 0 then
        say("你还没有消灭[布艾拉德之王]，请继续加油吧！"
        .. "<我知道了/close/icon3>")
    else
        say("恭喜你消灭了[布艾拉德之王]，完成了任务！"
        .. "<完成任务/likai/icon3>")
    end

	-- 16 布艾拉德之王
    --say("副本中的怪物很厉害，但是奖励也很丰富！加油了"
    --    .. "<我要离开/complete/icon3>"
	--	)
end

function complete()
    if getMonsterCount(16) > 0 then
         say("[布艾拉德之王]还没有被消灭，请继续加油吧！"
        .. "<我知道了/close/icon3>")
    else
        likai()
    end
end

function likai()
	completeCopies()
	leaveCopies()
end
