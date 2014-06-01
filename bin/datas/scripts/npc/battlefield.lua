function main()
    say("战场入口：参加战场和竞技场的勇士可以用荣誉奖牌兑换物品"
        .. "<战场/zhanchang/icon1>"
        .. "<荣誉商店/rongyu/icon2>"
--        .. "<当前荣誉/showRongYu/icon2>"
--        .. "<增加荣誉/addRongYU/icon2>"
--        .. "<减少荣誉/subRongYU/icon2>"
        .. "<竞技场/jingji/icon3>")
end

--战场
function zhanchang()
    say("选择战场进入："
        .. "<死亡大漠(LV40)/xianxue>"
        .. "<组队副本/xianxue_zudui>"
        .. "<战场说明/helpzc>"
        .. "<返回/main>")
end

function rongyu()

end

--显示荣誉
function showRongYu()
    local varRY = getIntVar("rongyu")
    sysChat("荣誉值：" .. varRY)
end

-- 增加荣誉值
function addRongYU()
    local varRY = getIntVar("rongyu")
    varRY = varRY + 1
    setIntVar("rongyu", varRY)
    showRongYu()
end

-- 减少荣誉值
function subRongYU()
    local varRY = getIntVar("rongyu")
    if varRY > 0 then
        varRY = varRY - 1
        setIntVar("rongyu", varRY)
        showRongYu()
    end
end

--竞技场
function jingji()

end

--鲜血之环
function xianxue()
    --gotoMap(3, 50, 70)
    if getLevel() < 40 then
        sysChat("等级不够，无法进入副本")
    else
        enterDuplicate(10000, 4, 32)
    end
end

-- 组队副本
function xianxue_zudui()
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
