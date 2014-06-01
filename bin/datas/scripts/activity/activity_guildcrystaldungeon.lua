function main()
	enter_fuben()

	--[[
    say("公会晶体副本: 欢迎参加活动,在指定时间内通过副本，就会获得额外奖励。"
        .. "<进入初级急速副本/fuben_zudui/icon1>"
		.. "<进入高级急速副本/fuben_zudui/icon1>"
		)
	]]
end


-- 进入副本
function enter_fuben()
	if isInCopies() then
		sysChat("您正在副本中，无法参加其他副本活动")
		return
	end

	if not joinActivity() then
		sysChat("加入活动失败！！")	
		return
	end

    enterCopiesByAuto(10003, 30, 72)
end

