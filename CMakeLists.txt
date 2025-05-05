cmake_minimum_required(VERSION 3.12)
project(ParallelProject LANGUAGES CXX)

# 设置编译选项
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加编译选项
add_compile_options(-O3 -Wall -Wextra -Wno-free-nonheap-object)

# 查找 OpenMP
find_package(OpenMP REQUIRED)

# 设置包含目录
include_directories(include)

# 收集源文件
file(GLOB SOURCES "src/*.cpp")

# 创建构建目录
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build
)

# 链接 OpenMP
target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)

# 添加自定义目标
add_custom_target(run
    COMMAND ./${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/build
    DEPENDS ${PROJECT_NAME}
)

# 清理命令
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/build
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
)