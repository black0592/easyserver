require 'includes'
	
--game engine project
project 'libbase'
	targetdir './lib/libbase'
	location( builddir )
    kind 'StaticLib'
	includedirs { libbase_includedirs }
	files { 'libbase/**.*', }
	vpaths { ['./*'] = {'libbase/**.*'}, }
	configuration 'Debug'
        targetsuffix '_d'


--game base project
project 'libgame'
	targetdir './lib/libgame'
	location( builddir )
    kind 'StaticLib'
	includedirs { libgame_includedirs }
	files { 'libgame/**.*', }
	vpaths { ['./*'] = {'libgame/**.*'}, }
	configuration 'Debug'
        targetsuffix '_d'


