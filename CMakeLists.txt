cmake_minimum_required(VERSION 3.14)

if (WIN32)
    project(HorarisFIB LANGUAGES CXX)
elseif(UNIX)
    project(HorarisFIB)
endif()

set(CMAKE_CONFIGURATION_TYPES "Release;Debug" CACHE STRING "" FORCE)

#=============== Qt ==============#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC on)
set(Qt5_DIR "/lib/qt5/bin")
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(OpenSSL REQUIRED)

# Inclusions
set(FORMS_DIR "${CMAKE_SOURCE_DIR}/ui")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

include_directories(${FORMS_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})

file(GLOB_RECURSE SOURCES
    "${FORMS_DIR}/*.ui"
    "${FORMS_DIR}/*.qrc"
    "${INCLUDE_DIR}/*.h*"
    "${SOURCE_DIR}/*.c*"
)

# Setup executable
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
    $<$<CONFIG:DEBUG>:QT_MESSAGELOGCONTEXT>
)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

qt5_wrap_cpp(horaris_SRC ${INCLUDE_DIR}/MyComboBox.hh ${INCLUDE_DIR}/MyForm.h ${INCLUDE_DIR}/App.hh)
qt5_wrap_ui(horaris_UI ${FORMS_DIR}/MyFormWindow.ui)

# Add exec
if (WIN32)
    add_executable(HorarisFIB WIN32 ${SOURCES} ${HEADERS} ${horaris_UI} ${horaris_SRC}) #removes the command line
elseif(UNIX)
    add_executable(HorarisFIB ${SOURCES} ${HEADERS} ${horaris_UI} ${horaris_SRC})
endif()

# Target includes
target_include_directories(HorarisFIB PRIVATE ${FORMS_DIR})
target_include_directories(HorarisFIB PRIVATE ${INCLUDE_DIR})
target_include_directories(HorarisFIB PRIVATE ${SOURCE_DIR})

target_link_libraries(HorarisFIB Qt5::Core Qt5::Gui Qt5::Widgets OpenSSL::SSL OpenSSL::Crypto)