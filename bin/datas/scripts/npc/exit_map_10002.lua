function main()
	if getMonsterCount(24) > 0 then
        say("你还没有消灭[魔犯]，请继续加油吧！"
        .. "<我知道了/close/icon3>")
    else
        say("恭喜你消灭了[魔犯]，完成了任务！"
        .. "<完成任务/likai/icon3>")
    end
end

function likai()
	completeCopies()
	leaveCopies()
end
