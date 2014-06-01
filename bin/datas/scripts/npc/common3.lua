
require 'teleporter_comm'

-- 初始化传送信息
local teleportInfo = {
	[0] = {map=2,		x=21, y=22, money=200, name="主城", space="    ", level=11},
}


function main()
    say("黑暗力量，永远藏在深处，并一直注视着你。"
		.. "<好可怕！/close/icon1>"
		.. "<回到主城/go_teleport/icon1>"
		)
end


function go_teleport()
	teleportMain(teleportInfo)
end

