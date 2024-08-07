cmake_minimum_required(VERSION 3.5)

project(PreDataBase VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Concurrent)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        database.h
        database.cpp
        dbdata.h
        dbdata.cpp
        dbdata.ui
)

qt_add_executable(PreDataBase
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/PG_Libs)
target_link_libraries(PreDataBase PRIVATE Qt6::Widgets
                                          Qt6::Sql
                                          Qt6::Concurrent
                                          libcrypto-3-x64
                                          libiconv-2
                                          libintl-9
                                          libpq
                                          libssl-3-x64)

qt_finalize_executable(PreDataBase)
