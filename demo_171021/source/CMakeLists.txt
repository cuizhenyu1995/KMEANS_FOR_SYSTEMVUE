cmake_minimum_required(VERSION 3.1.1)

if( NOT UNIX )
	#Path to SystemVue installer directory on Windows.
	set( SystemVueInstallDirectory "C:/Program Files/SystemVue2016.08/")
else()
	#Path to SystemVue code generation libraries Linux.
	set( SystemVueInstallDirectory "$ENV{HOME}/SystemVue/2016.08")
endif()

include ( ${SystemVueInstallDirectory}/ModelBuilder/CMake/CodeGeneration.cmake )

project( demo_171021 )

add_subdirectory( SystemVue )
