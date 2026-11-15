cmake_minimum_required(VERSION 3.16)

project(Inmemorydatabase)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(redis
    src/main.cpp
    src/parser/CommandParser.cpp

    src/commands/SetCommand.cpp
    src/commands/GetCommand.cpp

    src/executor/CommandExecutor.cpp
)

target_include_directories(redis PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
