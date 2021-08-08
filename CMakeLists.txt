cmake_minimum_required(VERSION 3.14)

project(ShipPathCalculations VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package(...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}")
set(CRYENGINE_DIR "${PROJECT_SOURCE_DIR}/CryEngine")
include_directories("${CRYENGINE_DIR}/CryCommon")

if(MSVC)
    add_definitions(-DCRY_COMPILER_MSVC)
endif()

# include("${CRYENGINE_DIR}/CryCommonConfig.cmake")
include("${CRYENGINE_DIR}/CommonMacros.cmake")
add_subdirectory("${CRYENGINE_DIR}/CryCommon")
file(GLOB_RECURSE CryEngine_SRC
     "CryEngine/*.inl"
)

set(common_SRC
    common/CryCommon.h
    common/NavigationTypes.h
    common/NavigationTypes.cpp
)

set(Calculations_SRC
    mainwindow.h
    mainwindow.cpp

    Calculations/MovementCalculator.h
    Calculations/MovementCalculator.cpp

    CalculationsTest/CalculationsTest.h
    CalculationsTest/CalculationsTest.cpp
)

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
        ${CryEngine_SRC}
        ${common_SRC}
        ${Calculations_SRC}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    #qt_add_executable(${CMAKE_PROJECT_NAME}
    #    MANUAL_FINALIZATION
    #    ${PROJECT_SOURCES}
    #)
add_executable(${CMAKE_PROJECT_NAME}
  ${PROJECT_SOURCES}
)
else()
    if(ANDROID)
        add_library(${CMAKE_PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(${CMAKE_PROJECT_NAME}
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE NOT_USE_CRY_MEMORY_MANAGER=1)
add_definitions(-DCRY_SAFE_RUNTIME)
add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)
add_definitions(-D_SCL_SECURE_NO_WARNINGS)

target_compile_definitions(${CMAKE_PROJECT_NAME}
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${CMAKE_PROJECT_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick ntdll INTERFACE CryCommon)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

target_include_directories(${CMAKE_PROJECT_NAME}
PRIVATE
        "${CRYENGINE_DIR}/CryCommon"
        "${CRYENGINE_DIR}/CrySchematyc/Core/Interface"
        "${CRYENGINE_DIR}/CrySchematyc/STDEnv/Interface"
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(${CMAKE_PROJECT_NAME})
    qt_finalize_executable(${CMAKE_PROJECT_NAME})
endif()
