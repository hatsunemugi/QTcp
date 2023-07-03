cmake_minimum_required(VERSION 3.16)

project(telegram VERSION 0.1 LANGUAGES CXX)

file(GLOB_RECURSE CPP_FILES "source/*.cpp" "header/*.h")

file(GLOB QML_PATHS "qml/*.qml")

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Svg Widgets QuickControls2 Xml Core)

qt_standard_project_setup()

qt_add_executable(apptelegram
    ${CPP_FILES}
    qml.qrc
)

set_property(TARGET apptelegram APPEND PROPERTY
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)

list(APPEND QML_HEADER_LIST
    "header/backend.h"
    "header/chat.h"
    "header/peer.h"
)

list(APPEND QML_SOURCE_LIST
    "source/backend.cpp"
    "source/chat.cpp"
    "source/peer.cpp"
)

foreach(filepath ${QML_PATHS})
    message(${filepath} "\\t" ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath}) #.+/(.+)\\\\..*" "\\\\1
list(APPEND QML_FILES ${filename})
endforeach(filepath)
list(REMOVE_DUPLICATES QML_FILES) #表示删除非第一个重名值

file(GLOB_RECURSE IMG_PATHS "qml/img/*.png" "qml/img/*.jpg" "qml/img/*.svg"
    )

foreach(filepath ${IMG_PATHS})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath}) #.+/(.+)\\\\..*" "\\\\1
    list(APPEND IMG_FILES ${filename})

endforeach(filepath)
list(REMOVE_DUPLICATES IMG_FILES)

qt_add_qml_module(apptelegram
    URI telegram
    VERSION 1.0
    QML_FILES #[[qml/Main.qml]]
        ${QML_FILES}
    RESOURCES
        ${IMG_FILES}
        ${QML_HEADER_LIST}
        ${QML_SOURCE_LIST}
    )


set_target_properties(apptelegram PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(ANDROID)
    target_compile_definitions(apptelegram PRIVATE
        Android_Platform
    )
endif()

target_link_libraries(apptelegram
    PRIVATE Qt6::Quick Qt6::Svg Qt6::Widgets Qt6::QuickControls2 Qt6::Xml Qt6::CorePrivate
)

target_include_directories(apptelegram
    PUBLIC header

)

install(TARGETS apptelegram
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
