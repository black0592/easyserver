function main()
    say(
		"你的装备损坏了吗？让我来帮你修理吧！"
		.. "<修理身上/repair1/icon1>"
		.. "<修理全部/repair2/icon1>"
		)
end

-- 修理身上的所有装备
function repair1()
    repairEquip()
end

-- 修理身上和背包里的所有装备
function repair2()
   repairAllEquip()
end

