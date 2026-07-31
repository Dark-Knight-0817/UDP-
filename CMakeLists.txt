cmake_minimum_required(VERSION 3.10)
project(ReliableUDP LANGUAGES CXX)

# C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译选项:开警告，方便发现问题
add_compile_options(-Wall -Wextra)

# 可执行文件输出到 build/bin 下，保持整洁
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 两个目标：sender 和 receiver
add_executable(receiver src/receiver.cpp)
add_executable(sender   src/sender.cpp)

# 如果后面用到多线程（std::thread），需要链接 pthread
find_package(Threads REQUIRED)
target_link_libraries(receiver PRIVATE Threads::Threads)
target_link_libraries(sender   PRIVATE Threads::Threads)
