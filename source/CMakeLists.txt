cmake_minimum_required(VERSION 3.20)
project(stewEngine)
cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH MYPROJECT_DIR)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MYPROJECT_DIR}/cmake)
find_package(Vulkan REQUIRED FATAL_ERROR)
find_package(GLFW3 REQUIRED FATAL_ERROR)
find_package(TinyOBJLoader REQUIRED FATAL_ERROR)
find_package(STB REQUIRED FATAL_ERROR)
find_package(GLM REQUIRED FATAL_ERROR)

file(GLOB SOURCE_FILES "*.cpp" "*.h")


# add the executable
add_executable(StewEngine ${SOURCE_FILES})

set_property(TARGET StewEngine PROPERTY CXX_STANDARD 17)

target_include_directories(StewEngine PUBLIC
${GLM_INCLUDE_DIR}
${STB_INCLUDE_DIR}
${TinyOBJLoader_INCLUDE_DIR}
${GLFW3_INCLUDE_DIR}
${Vulkan_INCLUDE_DIR})

message("INCLUDE DIRECTORIES  
${GLM_INCLUDE_DIR}
${STB_INCLUDE_DIR}
${TinyOBJLoader_INCLUDE_DIR}
${GLFW3_INCLUDE_DIR}
${Vulkan_INCLUDE_DIR}")

target_link_libraries(StewEngine
	${GLFW3_LIBRARY}
	${Vulkan_LIBRARY}
)