set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "-g -Wall")

# Install settings
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

include_directories(../src/frame/window)
include_directories(../src/frame/navigation)
include_directories(../src/frame/window/search)

aux_source_directory(../src/frame/window WINDOW)
aux_source_directory(../src/frame/navigation NAVIGATION)
aux_source_directory(../src/frame/window/search SEARCH)

file(GLOB SRC_PATH
    ${WINDOW}
    ${NAVIGATION}
    ${SEARCH}
    )

add_subdirectory("mouse")
