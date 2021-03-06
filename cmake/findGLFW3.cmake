set( GLFW3_INCLUDE_DIR "${MYPROJECT_DIR}/3rdParty/glfw/include")
set( _glfw3_LIB_SEARCH_DIRS "${MYPROJECT_DIR}/3rdParty/glfw/lib-vc2019")

# Search for the library
FIND_LIBRARY(GLFW3_LIBRARY NAMES glfw3 glfw PATHS ${_glfw3_LIB_SEARCH_DIRS} )
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLFW3 DEFAULT_MSG GLFW3_LIBRARY GLFW3_INCLUDE_DIR)