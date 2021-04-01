cmake_minimum_required(VERSION 3.7)

set(VERSION 4.0)

set(BIN_NAME dde-control-center)

# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "-g -Wall -Wl,--as-need -fPIE -rdynamic")

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "mips64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ftree-vectorize -march=loongson3a -mhard-float -mno-micromips -mno-mips16 -flax-vector-conversions -mloongson-ext2 -mloongson-mmi")
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie")

# disable qt keywords, cause gio have signals member;
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

if (DEFINED ENABLE_MIEEE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mieee")
endif()

# Install settings
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

set(DEFINED_LIST
DISABLE_OPACITY_ANIMATION
DISABLE_CLOUD_SYNC
DISABLE_SYS_UPDATE
DISABLE_ACCOUNT
DISABLE_DISPLAY
DISABLE_DEFAULT_APPLICATIONS
DISABLE_PERSONALIZATION
DISABLE_NETWORK
DISABLE_BLUETOOTH
DISABLE_SOUND
DISABLE_DATETIME
DISABLE_POWER
DISABLE_MOUSE
DISABLE_KAYBOARD
DISABLE_WACOM
DISABLE_SYSINFO
DISABLE_NETWORK_PPPOE
DISABLE_NETWORK_VPN
DISABLE_NETWORK_PROXY
DISABLE_SYS_UPDATE_SOURCE_CHECK
DISABLE_SYS_UPDATE_MIRRORS
DISABLE_MAIN_PAGE
DISABLE_RECOVERY
DCC_ENABLE_ADDOMAIN
DCC_DISABLE_KBLAYOUT
DCC_DISABLE_LANGUAGE
DCC_DISABLE_POWERSAVE
DCC_DISABLE_FEEDBACK
DCC_DISABLE_ROTATE
DCC_DISABLE_MIRACAST
DCC_DISABLE_TIMEZONE
DCC_DISABLE_GRUB
DCC_KEEP_SETTINGS_LIVE
DCC_COMMUNITY_LICENSE
)

foreach(LETTER ${DEFINED_LIST})
    if(DEFINED ${LETTER})
        add_definitions(-D${LETTER})
    endif()
endforeach()

# load files
set(FRAME_FILES
                main.cpp
                dbuscontrolcenterservice.cpp
)

set(WINDOW_FILES
    window/mainwindow.cpp
    window/utils.h
    window/gsettingwatcher.cpp
    window/gsettingwatcher.h
    window/accessibleinterface.h
    window/accessible.h
    window/protocolfile.cpp
    window/insertplugin.cpp
    window/insertplugin.h
    window/search/searchwidget.cpp
)

# load interfaces
set(INTERFACES_FILES
                interfaces/frameproxyinterface.h
                interfaces/moduleinterface.h
                interfaces/plugininterface.h

                ../../include/interface/moduleinterface.h
                ../../include/interface/frameproxyinterface.h
)

# load widgets
set(WIDGETS_FILES
                widgets/backbutton.cpp
                widgets/basiclistdelegate.cpp
                ../../include/widgets/basiclistdelegate.h
                widgets/buttontuple.cpp
                widgets/dccslider.cpp
                ../../include/widgets/dccslider.h
                widgets/dccsliderannotated.cpp
                ../../include/widgets/dccsliderannotated.h
                ../../include/widgets/labels/largelabel.h
                ../../include/widgets/labels/normallabel.h
                ../../include/widgets/labels/smalllabel.h
                ../../include/widgets/labels/tipslabel.h
                widgets/lineeditwidget.cpp
                widgets/loadingindicator.cpp
                ../../include/widgets/loadingindicator.h
                widgets/loadingnextpagewidget.cpp
                widgets/nextbutton.cpp
                widgets/optionlistpage.cpp
                widgets/searchinput.cpp
                ../../include/widgets/searchinput.h
                widgets/separator.cpp
                ../../include/widgets/separator.h
                widgets/settingsheaderitem.cpp
                ../../include/widgets/settingsheaderitem.h
                widgets/settingsitem.cpp
                ../../include/widgets/settingsitem.h
                widgets/spinboxwidget.cpp
                ../../include/widgets/spinboxwidget.h
                widgets/buttoneditwidget.cpp
                ../../include/widgets/buttoneditwidget.h
                widgets/tipsitem.cpp
                ../../include/widgets/tipsitem.h
                widgets/titlelabel.cpp
                ../../include/widgets/titlelabel.h
                widgets/titlevalueitem.cpp
                ../../include/widgets/titlevalueitem.h
                widgets/translucentframe.cpp
                ../../include/widgets/translucentframe.h
                widgets/settingsgroup.cpp
                widgets/switchwidget.cpp
                widgets/comboxwidget.cpp
                ../../include/widgets/comboxwidget.h
                widgets/themeitempic.cpp
                ../../include/widgets/themeitempic.h
                ../../include/widgets/utils.h
                ../../include/widgets/backbutton.h
                widgets/basiclistmodel.cpp
                ../../include/widgets/basiclistmodel.h
                widgets/basiclistview.cpp
                ../../include/widgets/basiclistview.h
                ../../include/widgets/buttontuple.h
                widgets/comboboxwidget.cpp
                ../../include/widgets/comboboxwidget.h
                widgets/contentwidget.cpp
                ../../include/widgets/contentwidget.h
                widgets/editablenextpagewidget.cpp
                ../../include/widgets/editablenextpagewidget.h
                widgets/filechoosewidget.cpp
                ../../include/widgets/filechoosewidget.h
                ../../include/widgets/lineeditwidget.h
                ../../include/widgets/loadingnextpagewidget.h
                ../../include/widgets/nextbutton.h
                widgets/nextpagewidget.cpp
                ../../include/widgets/nextpagewidget.h
                widgets/optionitem.cpp
                ../../include/widgets/optionitem.h
                ../../include/widgets/optionlistpage.h
                ../../include/widgets/settingsgroup.h
                widgets/settingshead.cpp
                ../../include/widgets/settingshead.h
                ../../include/widgets/switchwidget.h
                widgets/themeitem.cpp
                ../../include/widgets/themeitem.h
                widgets/titledslideritem.cpp
                ../../include/widgets/titledslideritem.h
                widgets/timeoutdialog.cpp
                ../../include/widgets/timeoutdialog.h
                widgets/multiselectlistview.cpp
                ../../include/widgets/multiselectlistview.h
                widgets/powerdisplaywidget.cpp
                ../../include/widgets/powerdisplaywidget.h
                widgets/widgets.qrc
)

set(SRCS
        ${FRAME_FILES}
        ${WINDOW_FILES}
        ${INTERFACES_FILES}
)

set(QRC
        notify/images.qrc
        frame.qrc
        window/search/translator/languages.qrc
        window/icons/dde-control-center-icons.qrc
)

# Find the library
find_package(PkgConfig REQUIRED)
find_package(DtkWidget REQUIRED)
find_package(DtkGui REQUIRED)
find_package(KF5NetworkManagerQt REQUIRED)
find_package(DtkCMake REQUIRED)
find_package(PolkitQt5-1)
find_package(Qt5 COMPONENTS
    Core
    Widgets
    Concurrent
    X11Extras
    DBus
    Network
    Multimedia
    Svg
REQUIRED)

pkg_check_modules(XCB_EWMH REQUIRED xcb-ewmh x11 xext)
pkg_check_modules(DFrameworkDBus REQUIRED dframeworkdbus)
pkg_check_modules(QGSettings REQUIRED gsettings-qt)
pkg_check_modules(DDE-Network-Utils REQUIRED dde-network-utils)
pkg_check_modules(Gio-2.0 REQUIRED gio-2.0)
pkg_check_modules(LibNM REQUIRED libnm)

if (NOT DEFINED DISABLE_RECOVERY)
    pkg_search_module(UDisk2 REQUIRED udisks2-qt5)
    include_directories(AFTER ${UDisk2_INCLUDE_DIRS})
endif()

set(Qt_LIBS
    Qt5::Core
    Qt5::Gui
    Qt5::DBus
    Qt5::Widgets
    Qt5::X11Extras
    Qt5::Network
    Qt5::Multimedia
)

# KF5 network manager
get_target_property(KF5_QT_INCLUDE_DIRS KF5::NetworkManagerQt INTERFACE_INCLUDE_DIRECTORIES)

set(TRY_KF5_LIBRARIES
IMPORTED_LOCATION_DEBIAN
IMPORTED_LOCATION_NOCONFIG
)

foreach(LETTER ${TRY_KF5_LIBRARIES})
    if (NOT KF5_QT_LIBRARIES)
        get_target_property(KF5_QT_LIBRARIES KF5::NetworkManagerQt ${LETTER})
    endif()
endforeach()

if (NOT KF5_QT_LIBRARIES)
    find_library(KF5_QT_LIBRARIES libKF5NetworkManagerQt.so)
endif()
# end KF5 network manager

set(DEEPIN_PW_CHECK libdeepin_pw_check.so)

function(add_build_macro macro)
    string(TOUPPER ${macro} macro)
    add_definitions(-D${macro})
endfunction()

set(BUILD_MACRO)

if (OS_DEEPIN_PROFESSIONAL)
    if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(BUILD_MACRO
            ${BUILD_MACRO}
            DCC_ENABLE_ADDOMAIN
            DCC_DISABLE_MIRACAST
            DISABLE_CLOUD_SYNC
            DCC_COMMUNITY_LICENSE
        )
    endif()

    set(BUILD_MACRO
        ${BUILD_MACRO}
    )
endif()

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "sw_64")
    set(BUILD_MACRO
        ${BUILD_MACRO}
        DISABLE_SYS_UPDATE_SOURCE_CHECK
        DISABLE_SYS_UPDATE_MIRRORS
        DCC_DISABLE_MIRACAST
        DCC_ENABLE_AUTOSTART
        DCC_DISABLE_POWERSAVE
        DCC_KEEP_SETTINGS_LIVE
        DCC_DISABLE_OPACITY_ANIMATION
        DISABLE_SYS_UPDATE_MIRRORS
        DCC_DISABLE_FEEDBACK
        DISABLE_CLOUD_SYNC
        DISABLE_SCREEN_RECORDING
    )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mieee")
endif()

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "mips64")
    set(BUILD_MACRO
        ${BUILD_MACRO}
        DISABLE_SYS_UPDATE_SOURCE_CHECK
        DCC_DISABLE_MIRACAST
        DCC_ENABLE_AUTOSTART
        DCC_DISABLE_OPACITY_ANIMATION
        DCC_KEEP_SETTINGS_LIVE
        DCC_DISABLE_GRUB_THEME
        DISABLE_CLOUD_SYNC
        DISABLE_SCREEN_RECORDING
    )
endif()

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
    set(BUILD_MACRO
        ${BUILD_MACRO}
        DISABLE_POWER
        DISABLE_CLOUD_SYNC
        DCC_DISABLE_GRUB
    )
endif()

foreach(LETTER ${BUILD_MACRO})
    add_build_macro(${LETTER})
endforeach()

set(LIBS)
set(INCLUDES)

if (NOT DEFINED DISABLE_RECOVERY)
    set(INCLUDE
        ${INCLUDES}
        ${UDisk2_INCLUDE_DIRS}
        )
    set(LIBS
        ${LIBS}
        ${UDisk2_LIBRARIES}
        )
endif()

add_library(dccwidgets SHARED
    ${WIDGETS_FILES}
)
target_include_directories(dccwidgets PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${DtkWidget_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
)
target_link_libraries(dccwidgets PRIVATE
    ${DtkWidget_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
)

add_executable(${BIN_NAME} ${SRCS} ${QRC})
target_include_directories(${BIN_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${DtkWidget_INCLUDE_DIRS}
    ${XCB_EWMH_INCLUDE_DIRS}
    ${DFrameworkDBus_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${PROJECT_BINARY_DIR}
    ${QGSettings_INCLUDE_DIRS}
    ${DDE-Network-Utils_INCLUDE_DIRS}
    ${Gio-2.0_INCLUDE_DIRS}
    ${LibNM_INCLUDE_DIRS}
    ${Qt5X11Extras_INCLUDE_DIRS}
    ${KF5_QT_INCLUDE_DIRS}
    ${INCLUDES}
)
target_link_libraries(${BIN_NAME} PRIVATE
    dccwidgets
    ${XCB_EWMH_LIBRARIES}
    ${DFrameworkDBus_LIBRARIES}
    ${DtkWidget_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Concurrent_LIBRARIES}
    ${Qt5X11Extras_LIBRARIES}
    ${Qt5DBus_LIBRARIES}
    ${Qt5Network_LIBRARIES}
    ${Qt5Multimedia_LIBRARIES}
    ${QGSettings_LIBRARIES}
    ${DDE-Network-Utils_LIBRARIES}
    ${Gio-2.0_LIBRARIES}
    ${LibNM_LIBRARIES}
    ${KF5_QT_LIBRARIES}
    ${Qt5Svg_LIBRARIES}
    ${DEEPIN_PW_CHECK}
    crypt
    ${LIBS}
    PolkitQt5-1::Agent
)

# bin
install(TARGETS ${BIN_NAME} DESTINATION bin)
install(TARGETS dccwidgets LIBRARY DESTINATION lib)

find_package(Qt5 COMPONENTS
Test
REQUIRED)

set(Qt_LIBS
${Qt_LIBS}
Qt5::Test
)
