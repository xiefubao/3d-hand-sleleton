

cmake_minimum_required ( VERSION 3.0)       # 支持最低的CMake版本
project                ( hand )              # 我的工程名叫red

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )      # 生成 compile_commands.json供vim c++插件语义补全使用


set(SRC_LIST )

add_executable(${PROJECT_NAME} hand.cpp ModelPlus.cpp ParseJson.cpp)

INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
FIND_LIBRARY(COCOA_LIBRARY Cocoa)
FIND_LIBRARY(GLUT_LIBRARY GLUT )
FIND_LIBRARY(OpenGL_LIBRARY OpenGL )
MARK_AS_ADVANCED (COCOA_LIBRARY GLUT_LIBRARY OpenGL_LIBRARY)
SET(EXTRA_LIBS ${COCOA_LIBRARY} ${GLUT_LIBRARY} ${OpenGL_LIBRARY})
target_link_libraries(hand ${EXTRA_LIBS})






find_package( OpenCV REQUIRED )
target_link_libraries( hand ${OpenCV_LIBS} )


