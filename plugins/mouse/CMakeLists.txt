set(PLUGIN_NAME "mouse")

project(${PLUGIN_NAME})

# Find the library
find_package(PkgConfig REQUIRED)
find_package(DtkWidget REQUIRED)
find_package(DtkGui REQUIRED)
find_package(DtkCMake REQUIRED)

find_package(Qt5 COMPONENTS
        Core
        Widgets
        DBus
        LinguistTools
        Concurrent
        REQUIRED)

pkg_check_modules(DFrameworkDBus REQUIRED dframeworkdbus)

include_directories(controller controller/widget viewer ../../include)

aux_source_directory(controller CONTROLLER)
aux_source_directory(controller/widget WIDGET)
aux_source_directory(viewer VIEWER)
aux_source_directory(../../include INCLUDE)

file(GLOB_RECURSE SRCS ${CONTROLLER} ${WIDGET} ${VIEWER} ${INCLUDE}
                       "../../src/frame/window/mainwindow.h" "../../src/frame/window/mainwindow.cpp"
                       "../../src/frame/window/search/searchwidget.h" "../../src/frame/window/search/searchwidget.cpp"
                       "../../src/frame/window/gsettingwatcher.h" "../../src/frame/window/gsettingwatcher.cpp"
                       "../../src/frame/window/insertplugin.h" "../../src/frame/window/insertplugin.cpp")

add_definitions("${QT_DEFINITIONS} -DQT_PLUGIN")
add_library(${PLUGIN_NAME} SHARED ${SRCS} controller/mouse.qrc)
set_target_properties(${PLUGIN_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../)
target_include_directories(${PLUGIN_NAME} PUBLIC
        ${DtkWidget_INCLUDE_DIRS}
        ${DFrameworkDBus_INCLUDE_DIRS}
        ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
        ${Qt5Concurrent_INCLUDE_DIRS}
        ${QGSettings_INCLUDE_DIRS}
        )

target_link_libraries(${PLUGIN_NAME} PRIVATE
        ${DFrameworkDBus_LIBRARIES}
        ${DtkWidget_LIBRARIES}
        ${Qt5Widgets_LIBRARIES}
        ${Qt5DBus_LIBRARIES}
        ${Qt5Concurrent_LIBRARIES}
        ${QGSettings_LIBRARIES}
        )

# 设置执行 make install 时哪个目标应该被 install 到哪个位置
install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION /lib/dde-control-center/system-modules)
