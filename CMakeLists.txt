cmake_minimum_required(VERSION 3.5)

add_compile_definitions (GTEST_INCLUDE_DIR=${GTEST_INCLUDE_DIR})
add_compile_definitions (GTEST_LIBRARY_DIR="./lib")

set (GTEST_LIBRARY ${GTEST_LIBRARY_DIR}/libgtest.a)
set (GTEST_MAIN_LIBRARY ${GTEST_LIBRARY_DIR}/libgtest_main.a)

# 指定项目名
project(Lexer)

# 生成 compile_commands.json
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 添加可执行文件
add_executable(${PROJECT_NAME}
               src/main.cpp # 添加源文件，建议在此逐个列出而不是使用变量
               src/lexer.cpp
)

# 搜索第三方包
enable_testing()
find_package(GTest)

if (GTest_FOUND)
    message(STATUS "Build GTest Project")
    message(STATUS "GTEST_INCLUDE_DIR: ${GTEST_INCLUDE_DIR}")
    message(STATUS "GTEST_LIBRARY_DIR: ${GTEST_LIBRARY_DIR}")
    # 添加可执行文件
    add_executable(${PROJECT_NAME}_test
               src/gtest.cpp # 添加源文件，建议在此逐个列出而不是使用变量
               src/lexer.cpp
    )
    # 添加头文件目录
    target_include_directories(
        ${PROJECT_NAME}_test
        PUBLIC
        ${GTEST_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/include 
    )
    # 添加链接文件
    target_link_libraries(
        ${PROJECT_NAME}_test
        PRIVATE
        ${GTEST_LIBRARIES}
        ${GTEST_MAIN_LIBRARIES}
    )
else()
    message(STATUS "Build Ordinary Test Project")
    # 添加可执行文件
    add_executable(${PROJECT_NAME}_test 
                src/test.cpp # 添加源文件，建议在此逐个列出而不是使用变量
                src/lexer.cpp
    )
    # 添加头文件目录
    target_include_directories(
        ${PROJECT_NAME}_test
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include 
    )
endif()

# 添加头文件目录
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include 
)

# 指定安装地址
install (TARGETS ${PROJECT_NAME}
         DESTINATION ${PROJECT_SOURCE_DIR}/bin)
         
# 指定安装地址
install (TARGETS ${PROJECT_NAME}_test
         DESTINATION ${PROJECT_SOURCE_DIR}/bin)