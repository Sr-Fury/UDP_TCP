cmake_minimum_required(VERSION 3.16)
project(ChatClient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)
# find_package(Qt5 REQUIRED COMPONENTS Core Network Widgets)

add_executable(ChatClient 
    main.cpp 
    mainwindow.cpp 
    client.cpp
)

target_link_libraries(ChatClient PRIVATE Qt6::Core Qt6::Network Qt6::Widgets)
# target_link_libraries(ChatClient PRIVATE Qt5::Core Qt5::Network Qt5::Widgets)