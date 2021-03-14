# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("testjni")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#判断编译器类型，如果是gcc编译器，则在编译选项中加入C++11支持
if(CMAKE_COMPILER_IS_GUNCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GUNCXX)
#需要引入头文件，以这个配置的目录为基准
#if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
#    add_subdirectory(./jni/arm64-v8a)
#else("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
#    add_subdirectory(./jni/armeabi-v7a)
#endif("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")

#message("CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
    set(OPENCV_DIR ${CMAKE_SOURCE_DIR}/jni/arm64-v8a)
else("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
    set(OPENCV_DIR ${CMAKE_SOURCE_DIR}/jni/armeabi-v7a)
endif("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
add_library( opencv_java4 SHARED IMPORTED )
set_target_properties(opencv_java4 PROPERTIES IMPORTED_LOCATION
        ${OPENCV_DIR}/libopencv_java4.so)

include_directories(${CMAKE_SOURCE_DIR}/jni/include)

#message($(CMAKE_SOURCE_DIR))

#set(distribution_DIR ${CMAKE_SOUR}/../jni)
#add_library(
#        opencv_java4
#        SHARED
#        IMPORTED)
#set_target_properties(
#        opencv_java4
#        PROPERTIES IMPORTED_LOCACTION
#        ../jni/arm64-v8a/libopencv_java4.so)
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

FILE(GLOB SRC_LIST_CPP "${CMAKE_SOURCE_DIR}/*.cpp")
FILE(GLOB SRC_LIST_HPP "${CMAKE_SOURCE_DIR}/*.hpp")
add_library(native-lib SHARED ${SRC_LIST_CPP} ${SRC_LIST_HPP})

#add_library( # Sets the name of the library.
#             native-lib
#
#             # Sets the library as a shared library.
#             SHARED
#
#             # Provides a relative path to your source file(s).
#             native-lib.cpp md5.cpp Student.cpp Teacher.cpp ArrayList.cpp ArrayList.hpp BitmapMatUtil.cpp cardocr.cpp
#             LinkedList.hpp SortUtils.cpp RandomUtil.cpp ArrayUtil.cpp ArrayStack.hpp LinkStack.hpp ArrayStack.hpp
#            TreeNode.hpp PriorityQueue.hpp BSTree.hpp AVLTree.hpp map2.hpp HuffmanTree.cpp pQueue.cpp LeeCode.cpp
#        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library(log-lib log )
find_library(jnigraphics-lib jnigraphics)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib ${jnigraphics-lib} opencv_java4
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )