set( _TinyOBJLoader_HEADER_SEARCH_DIRS "${MYPROJECT_DIR}/3rdParty/tinyobjloader")


# Search for the header
FIND_PATH(TinyOBJLoader_INCLUDE_DIR "tiny_obj_loader.h" PATHS ${_TinyOBJLoader_HEADER_SEARCH_DIRS} )

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TinyOBJLoader DEFAULT_MSG TinyOBJLoader_INCLUDE_DIR)