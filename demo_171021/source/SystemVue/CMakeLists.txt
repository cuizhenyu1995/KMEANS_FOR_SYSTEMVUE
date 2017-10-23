cmake_minimum_required(VERSION 3.1.1)

# SystemVueInstallDirectory defined in ../CMakeLists.txt
include ( ${SystemVueInstallDirectory}/ModelBuilder/CMake/ModelBuilder.cmake )

# SystemVue code generation maintains this variable, do not add files to this variable
set ( SVU_GENERATED_FILES 
	LibraryProperties.cpp
)

set ( KMEANS_FILES
        kmeans_in.cpp
        kmeans_in.h
        kmeans_run.cpp
        kmeans_run.h
)

# Change content below as necessary. For example, add more files, etc.

# Add a new model easily from a template by using the SVU_CREATE_MODEL macro as commented
# out below.  See the documentation for more details.
# You will need to add the generated source as shown in the commented out files below inside
# the SVU_ADD_MODELBUILDER_LIBRARY area.
# SVU_CREATE_MODEL ( MyCustomModel )

# Uncomment the lines below to customize the version information for your DLL.
# On the PC the info will show either by mousing-over your DLL or right
# clicking on the DLL and inspecting Properties -> Details.  
#
# The version numbers below, can be up to 4 integers separated by periods.  For
# example, "1.20.22.3".   For more details, refer to the SystemVue documentation.
SVU_CREATE_VERSION_FILE(
	VERSION_FILE
	# FILE_VERSION 1.0
	# COMPANY_NAME "My Company Name"
	# COPYRIGHT "(c) My Company Name, "
	# PRODUCT_NAME "My Product Name"
	# PRODUCT_VERSION 1.0
	TARGET_TYPE "Model Library"
)

#Option "CODE_GEN_SUPPORT" will create a stitic library, the static archives as well as the header
#  files in the current source directory will be placed to the output directory for code generation.
#
#  If models in the SystemVue model library is used in code generation, append the static library to
#  list "CUSTOM_LIB_SEARCH_DIRS" by expression "list(APPEND CUSTOM_LIB_SEARCH_DIRS customLibDir).
#
#  If the generated code is placed to the directory where the static library is placed, the static
#  library will be included by the generated code with default settings.
SVU_ADD_MODELBUILDER_LIBRARY(
	SystemVue-demo_171021
	${SVU_GENERATED_FILES}
	${VERSION_FILE}
#	CODE_GEN_SUPPORT
#	MyCustomModel.h
#	MyCustomModel.cpp
)

SVU_INSTALL_MODELBUILDER_LIBRARY( SystemVue-demo_171021 )
