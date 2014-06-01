--搜索lua配置模块
package.path = 'datas/scripts/item/?.lua;'

-- 增加金币
function itemAddMoney(money)
	addMoney(money)
	sysChat("获得金币 " .. money)
end

-- 改变生命值
function itemChangeHP(hp)
	--上下浮动5%
	local realHp = hp + hp*((5 - math.random(10))/100)
	changeHP(realHp)
	sysChat("增加生命值 " .. realHp)
end

-- 改变魔法值
function itemChangeMP(mp)
	--上下浮动5%
	local realMp = mp + mp*((5 - math.random(10))/100)
	changeMP(realMp)
	sysChat("增加魔法值 " .. realMp)
end

-- 增加PK保护buff
function itemAddPKSafe(second)
	remainTime = getBuffTime(BuffType_PKSafe)
	remainTime = remainTime + second
	
	if remainTime > 20 * 3600 then
		sysChat("最多只能叠加到20个小时")
		return 1
	end

	setBuff(BuffType_PKSafe, remainTime)
	sysChat("增加PK保护1小时")
end

-- 添加多倍经验BUFF
function itemAddMultExp(rate, second)
	if rate == 2 then
		remainTime = getBuffTime(BuffType_MultExp2)
		remainTime = remainTime + second
		
		if remainTime > 24 * 3600 then
			sysChat("最多只能叠加到24个小时")
			return 1
		end

		setBuff(BuffType_MultExp2, remainTime)
	else
		sysChat("无效的经验倍数")
		return 1
	end

	sysChat("增加2倍打怪经验1小时")
end


-- 洗点书(重置角色属性点)
function itemClearAttribPoint()
	if not clearAttribPoint() then
		sysChat("您好像不需要洗点")
		return 1
	end

	sysChat("洗点成功")
end

-- 攻击力buff
function itemAttackBuff(second, param1)
	setBuff(BuffType_AttackPercent, second, param1)
	sysChat("攻击力提升了")
end

-- 防御力buff
function itemDefenceBuff(second, param1)
	setBuff(BuffType_PhyDefPercent, second, param1)
	setBuff(BuffType_MagDefPercent, second, param1)
	sysChat("防御力提升了")
end

-- 打开礼包
function itemOpenGift()
	local itemId = getItemId()
	if not openGift( itemId ) then
		return 1
	end
end
