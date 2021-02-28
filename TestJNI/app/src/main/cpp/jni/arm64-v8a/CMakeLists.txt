message("CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")

set(OPENCV_DIR ${CMAKE_SOURCE_DIR}/jni/arm64-v8a)

message("OPENCV_DIR=${OPENCV_DIR}")

add_library( opencv_java4 SHARED IMPORTED )
set_target_properties(opencv_java4 PROPERTIES IMPORTED_LOCATION
        ${OPENCV_DIR}/libopencv_java4.so)