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


-- ��Ŀ�������
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
--linux�汾��֧��startproject      startproject 'GameServer'
	includedirs { common_includedirs }
	libdirs { server_libdirs_x64 }

	-- ƽ̨����
	if os.is("windows") then
		-- windows�µ�����
		defines { 'LANCE_WIN32', 'WIN32', '_CRT_SECURE_NO_WARNINGS', 'WIN32_LEAN_AND_MEAN'}
		links { window_links }
		buildoptions {'/MP4'}
	elseif os.is("linux") then
		-- linux�µ�����
		defines { 'LANCE_LINUX' }
		buildoptions { "-std=c++11" }
		linkoptions { linux_links }
	end    

	-- Debug��Release����
    configuration 'Debug'
		targetsuffix '_d'
	 configuration 'Release'
		flags {'Optimize'}
      


	-- ������ģ��
	dofile 'modules.lua'

    -- �����������Ⱥ
    dofile 'servergroup.lua'

		
																		 