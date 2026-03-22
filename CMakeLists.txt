cmake_minimum_required(VERSION 3.16)

project(Chess_Paring_system VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS PrintSupport)

set(Qt6Gui_PRIVATE_INCLUDE_DIRS "")
add_subdirectory(QXlsx)
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Chess_Paring_system
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        player.h player.cpp
        game.h game.cpp
        playerlist.h playerlist.cpp
        treeviewer.h treeviewer.cpp
        scoreselector.h scoreselector.cpp
        paring.h paring.cpp
        tournament.h tournament.cpp
        inputs.h inputs.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Chess_Paring_system APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Chess_Paring_system SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Chess_Paring_system
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Chess_Paring_system PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Chess_Paring_system
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        QXlsx::QXlsx
)
target_link_libraries(Chess_Paring_system PRIVATE Qt6::PrintSupport)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Chess_Paring_system)
endif()
set_target_properties(Chess_Paring_system PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Chess_Paring_system
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Chess_Paring_system)
endif()
