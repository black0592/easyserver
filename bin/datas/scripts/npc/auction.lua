function main()
    say("如果你有不用的道具，那请来这里寄售吧！"
     .. "<物品寄售/openItemAuction/icon1>"
     .. "<金币寄售/openMoneyAuction/icon1>")
end

-- 打开物品寄售
function openItemAuction()
    openGUI("itemauction")
end

-- 打开金币寄售
function openMoneyAuction()
    openGUI("moneyauction")
end

