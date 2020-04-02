cmake_minimum_required(VERSION 3.5)

# 指定项目名
project(Compiler)

# 设置 "make VERBOSE=1"，获取 make 过程的详细输出
set (CMAKE_VERBOSE_MAKEFILE ON)

# 生成 compile_commands.json
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 添加可执行文件
add_executable(Parser
               src/main.cpp # 添加源文件，建议在此逐个列出而不是使用变量
               src/dictionary.cpp
               src/error.cpp
               src/token.cpp               
               src/scanner.cpp
               src/ast.cpp
               src/parser.cpp
               src/jsonformatter.cpp
)

# 添加头文件目录
target_include_directories(
    Parser
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include 
)

target_compile_options(Parser PRIVATE -DPARSER)

# 指定安装地址
install (TARGETS Parser
         DESTINATION ${PROJECT_SOURCE_DIR}/bin)

# 添加可执行文件
add_executable(Lexer
               src/main.cpp # 添加源文件，建议在此逐个列出而不是使用变量
               src/dictionary.cpp
               src/error.cpp
               src/token.cpp               
               src/scanner.cpp
)

# 添加头文件目录
target_include_directories(
    Lexer
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include 
)

target_compile_options(Lexer PRIVATE -DLEXER)


# 指定安装地址
install (TARGETS Lexer
         DESTINATION ${PROJECT_SOURCE_DIR}/bin)