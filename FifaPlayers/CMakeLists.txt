cmake_minimum_required(VERSION 3.5)

project(FifaPlayers VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_MACOSX_BUNDLE ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        DialogWindow/dialog.h
        DialogWindow/dialog.cpp
        DialogWindow/dialog.ui
        TableModel/tablemodel.h
        TableModel/tablemodel.cpp
        Application/application.h
        Application/application.cpp
        Application/application.ui
        CreateRowDialog/adddialog.h
        CreateRowDialog/adddialog.cpp
        CreateRowDialog/adddialog.ui
        ../data/Top_650_FIFA.csv
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FifaPlayers
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FifaPlayers APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(FifaPlayers SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FifaPlayers
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(FifaPlayers PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


set(CMAKE_MACOSX_BUNDLE ON)
set(CMAKE_MACOSX_RPATH ON)
install(TARGETS ${target} BUNDLE DESTINATION .
                          RUNTIME DESTINATION .)
set_source_files_properties(PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

set_target_properties(FifaPlayers PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_NAME}"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

set(CPACK_BUNDLE_PLIST ${CMAKE_BINARY_DIR}/Info.plist)


