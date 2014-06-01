function main()
    say("急速副本: 欢迎参加活动,在指定时间内通过副本，就会获得额外奖励。"
        .. "<进入初级急速副本/fuben_zudui/icon1>"
		.. "<进入高级急速副本/fuben_zudui/icon1>"
		)
end


-- 组队副本
function fuben_zudui()
	if not joinActivity() then
		sysChat("加入活动失败！！")	
		return
	end


    enterCopiesByAuto(10000, 4, 32)

    --[[
    if getTeamMemberCount() == 0 then
        enterCopies(10000, 4, 32)
    else
        requestTeamCopiesVote(10000, 4, 32)
    end
    ]]
    
    --[[
    if getTeamMemberCount() == 0 then
        --sendSysChat("你需要组建一支队伍才能进")
    else
        enterCopiesByTeam(10000, 4, 32)
    end
    ]]
end

--战场说明
function helpzc()
    say("鲜血之环：鲜血与荣誉之战！你必须达到30级才能加入战斗。率先累计击杀20次对手的一方获胜。"
        .. "<返回/zhanchang>")
end
