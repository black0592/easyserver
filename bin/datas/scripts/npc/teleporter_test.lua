--搜索lua配置模块
require 'teleporter_comm'

-- 初始化传送信息
local teleportInfo = {
	[0] = {map=50,		x=17, y=15, money=1, name="奥.贫瘠驿站", space="    ", level=11},
	[1] = {map=22,		x=44, y=80, money=200, name="水晶南岛", space="    ", level=11},
	[2] = {map=23,		x=10, y=25, money=300, name="水晶北岛", space="    ", level=21},
	[3] = {map=10002,	x=11, y=50, money=400, name="矿石矿场", space="    ", level=31},
	[4] = {map=10003,	x=30, y=30, money=500, name="水晶矿场", space="    ", level=41},
}

function main()
	teleportMain(teleportInfo)
end
