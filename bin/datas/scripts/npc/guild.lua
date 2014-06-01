function main()
     say("如果你有能力就自己组建个公会吧！"
        .. "<公会管理/guildManager/icon1>"
        .. "<公会争霸/guildFight/icon2>"
		.. "<公会商店/guildStore/icon2>"
		)
end


-------------------- 公会管理 开始 -------------------------------
-- 公会管理
function guildManager()
    if hasGuild() then
        say("让你的公会强大起来吧！"
        .. "<公会列表/guildList>"
        .. "<离开公会/leaveGuild>"
		.. "<返回/main>"
		)
    else
        say("建立自己的公会，招募志同道合的小伙伴吧"
        .. "<加入公会/joinGuild>"
        .. "<创建公会/createGuild>"
		.. "<返回/main>"
		)
    end
end


-- 公会列表
function guildList()
    openGUI("guildlist")
end

-- 离开公会
function leaveGuild()
   openGUI("leaveguild") 
   close();
end

-- 加入公会
function joinGuild()
	 if getLevel() < getJoinGuildLevel() then
        say(getJoinGuildLevel() .. "级以后才能加入公会！"
			.. "<我知道了/guildManager>")
    else
        openGUI("guildlist")
        close();
    end
end

-- 创建公会
function createGuild()
    if getLevel() < getCreateGuildLevel() then
        say(getCreateGuildLevel() .. "级以后才能建立自己的公会！"
			.. "<我知道了/guildManager>")
    else
        openGUI("createguild")
        close();
    end
end
-------------------- 公会管理 结束 -------------------------------




-------------------- 公会战争 开始 -------------------------------
function guildFight()
	sysChat("此功能还未开放")
end
-------------------- 公会战争 结束 -------------------------------





-------------------- 公会商店 开始 -------------------------------
function guildStore()
	if not hasGuild() then
		sysChat("您还没有加入公会，不能使用公会商店")
		return
	end

	if getGuildLevel() < 3 then
		sysChat("公会商店需要公会等级到达3级，才能开启，请继续努力！")
		return
	end

	-- 打开公会商店
	openGuildStore()
end
-------------------- 公会商店 结束 -------------------------------