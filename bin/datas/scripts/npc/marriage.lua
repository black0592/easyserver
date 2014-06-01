--print("测试开始");

function main()
	--print("调用主函数");
    say("祝你们永远幸福" ..
        "<婚姻说明/help/icon1>" ..
        "<结婚申请/marry/icon2>" ..
        "<我要离婚/unmarry/icon3>")
end

function help()
    openGUI("marrydesc")
end

function marry()
    if isMarried() then
        sysChat("您已经结婚了！不能再结了")
        return
    end

    if checkCanMarry() then
        openGUI("marry") 
    end
end

function unmarry()
    if not isMarried() then
        sysChat("您还没结婚，不能离婚！")
        return
    end

    openGUI("unmarry")
end


