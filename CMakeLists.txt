cmake_minimum_required(VERSION 3.5)

# 指定项目名
project(Lexer)

# 添加可执行文件
add_executable(${PROJECT_NAME}
               src/main.cpp # 添加源文件，建议在此逐个列出而不是使用变量
               src/lexer.cpp
)

# 添加可执行文件
add_executable(${PROJECT_NAME}_test 
               src/test.cpp # 添加源文件，建议在此逐个列出而不是使用变量
               src/lexer.cpp
)

# 添加头文件目录
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include 
)

# 添加头文件目录
target_include_directories(
    ${PROJECT_NAME}_test
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include 
)

# 指定安装地址
install (TARGETS ${PROJECT_NAME}
         DESTINATION ${PROJECT_SOURCE_DIR}/bin)
         
# 指定安装地址
install (TARGETS ${PROJECT_NAME}_test
         DESTINATION ${PROJECT_SOURCE_DIR}/bin)