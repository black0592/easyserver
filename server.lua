builddir = 'build/' .. _ACTION
require 'includes'

local common_includedirs = 
{
    'deps/mysql/include',
--	'deps/VLD/include',
	libbase_includedirs,
	libgame_includedirs,
}

local server_libdirs_x64 = {
            'lib/x64',
            'deps/lib/window/x64',
            'deps/lib/linux',
--			'deps/VLD/lib/Win64/',
        }

local window_links = 
{
    'libmysql'
}

local linux_links = 
{
    "-Wl,-Bstatic -lmysqlclient",
    "-Wl,-Bdynamic -lrt -ldl",
}


-- 项目解决方案
solution 'SiliconServer'
--	location( builddir )
    targetdir 'bin'
	debugdir '$(TargetDir)'
    configurations { 'Debug', 'Release' }
    platforms { "x64" }
    kind 'ConsoleApp'
    language 'C++'
--	flags {'WinMain', 'StaticRuntime'}
	flags {'StaticRuntime', 'Symbols'}
--linux版本不支持startproject      startproject 'GameServer'
	includedirs { common_includedirs }
	libdirs { server_libdirs_x64 }

	-- 平台差异
	if os.is("windows") then
		-- windows下的配置
		defines { 'LANCE_WIN32', 'WIN32', '_CRT_SECURE_NO_WARNINGS', 'WIN32_LEAN_AND_MEAN'}
		links { window_links }
		buildoptions {'/MP4'}
	elseif os.is("linux") then
		-- linux下的配置
		defines { 'LANCE_LINUX' }
		buildoptions { "-std=c++11" }
		linkoptions { linux_links }
	end    

	-- Debug和Release差异
    configuration 'Debug'
		targetsuffix '_d'
	 configuration 'Release'
		flags {'Optimize'}
      


	-- 编译子模块
	dofile 'modules.lua'

    -- 编译服务器集群
    dofile 'servergroup.lua'

		
																		 