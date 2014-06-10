local game_includedirs = 
{
    'server_group/game_server',
    
    'server_group/game_server/Defines',
    'server_group/game_server/Config',
    'server_group/game_server/Database',
    'server_group/game_server/Tables',
    'server_group/game_server/Datas',
    'server_group/game_server/Timer',
    'server_group/game_server/MsgHandler/HandlerBase',
    'server_group/game_server/MsgHandler/HandlerHelp',
    'server_group/game_server/MsgHandler',

    'server_group/game_server/Component',

    'server_group/game_server/Script',
 
    'server_group/game_server/Modules',
    'server_group/game_server/Modules/Package',
    'server_group/game_server/Modules/Fight',
    'server_group/game_server/Modules/Guild',
	'server_group/game_server/Modules/DropItem',
    'server_group/game_server/Modules/Auction',
    'server_group/game_server/Modules/Mail',
    'server_group/game_server/Modules/Skill',
    'server_group/game_server/Modules/Page',
    'server_group/game_server/Modules/Mission',
    'server_group/game_server/Modules/Relations',
    'server_group/game_server/Modules/SelfStore',
    'server_group/game_server/Modules/SiegeWar',
    'server_group/game_server/Modules/Team',
    'server_group/game_server/Modules/Trader',
    'server_group/game_server/Modules/Activity',
	'server_group/game_server/Modules/Event',
    'server_group/game_server/Modules/CrackFile',
    'server_group/game_server/Modules/MapPath',
    'server_group/game_server/Modules/AOI',
    'server_group/game_server/Modules/Logger',
	'server_group/game_server/Modules/Chatting',
	'server_group/game_server/Modules/Counter',
	'server_group/game_server/Modules/Buyback',

    'server_group/game_server/Objects',
    'server_group/game_server/Objects/Base',
    'server_group/game_server/Objects/User',
    'server_group/game_server/Objects/Item',
    'server_group/game_server/Objects/MapDropItem',
    'server_group/game_server/Objects/Map',
    'server_group/game_server/Objects/Monster',
    'server_group/game_server/Objects/NPC',
    'server_group/game_server/Objects/JumpPoint',
    'server_group/game_server/Objects/ShortcutKey',
}


local gamedb_includedirs = 
{
     'server_group/gamedb_server',

     'server_group/gamedb_server/MsgHandler/HandlerBase',
     'server_group/gamedb_server/MsgHandler',
}



-- ����������(ȫ��Ψһ)
project 'super_server'
        location( builddir )
        links { 'libbase', 'libgame' }
        files {'server_group/super_server/**.*'}
		if os.is("linux") then
			files {'libgame/**.*'}
		end

--[[
-- ����������(ÿ����Ψһ)
project 'zone_server'
        location( builddir )
        links { 'libbase', 'libgame' }
        files {'server_group/zone_server/**.*'}
		if os.is("linux") then
			files {'libgame/**.*'}
		end

-- ����ͼ�������
project 'guildmark_server'
        location( builddir )
        links { 'libbase', 'libgame' }
        files {'server_group/guildmark_server/**.*'}
		if os.is("linux") then
			files {'libgame/**.*'}
		end

-- ��½������
project 'login_server'
        location( builddir )
        links { 'libbase', 'libgame' }
        files {'server_group/login_server/**.*'}
		if os.is("linux") then
			files {'libgame/**.*'}
		end


-- ���ط�����
project 'gateway_server'
        location( builddir )
        links { 'libbase', 'libgame' }
        files {'server_group/gateway_server/**.*'}
		if os.is("linux") then
			files {'libgame/**.*'}
		end

-- ��Ϸ������
project 'game_server'
        location( builddir )
        links { 'libbase', 'libgame' }
        includedirs { game_includedirs }
        files {'server_group/game_server/**.*'}
		if os.is("linux") then
			files {'libgame/**.*'}
		end

-- GameDB������
project 'gamedb_server'
        location( builddir )
        links { 'libbase', 'libgame' }
	    includedirs { gamedb_includedirs }
        files {'server_group/gamedb_server/**.*'}
		if os.is("linux") then
			files {'libgame/**.*'}
		end
]]


-- ���Է�����(��������ײ�)
project 'test_server'
        location( builddir )
        links { 'libbase' }
        files {'server_group/test_server/**.*'}


local client_includedirs = 
{
    'server_group/test_client',
    'server_group/test_client/Tables',
}

-- ���Կͻ���
project 'test_client'
        location( builddir )
        links { 'libbase', 'libgame' }
	    includedirs { client_includedirs }
        files {'server_group/test_client/**.*'}
		if os.is("linux") then
			files {'libgame/**.*'}
		end

