set(OPENCV_DIR ${CMAKE_SOURCE_DIR}/jni/armeabi-v7a)

add_library( opencv_java4 SHARED IMPORTED )
set_target_properties( opencv_java4 PROPERTIES IMPORTED_LOCATION
        ${OPENCV_DIR}/libopencv_java4.so )