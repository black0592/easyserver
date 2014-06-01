function main()
    say("野外怪物都很厉害,一定要备足补给品,快来买点吧！"
        .. "<购买药水/openYaoShui/icon1>"
		.. "<购买武器/openZhuangBei/icon1>"
		)
end

-- 打开npc默认商店
function openNpcShop()
    openShop()
end

-- 打开指定id的商店(药水)
function openYaoShui()
    openShop(2)
end

-- 打开指定id的商店(装备)
function openZhuangBei()
    openShop(6)
end
