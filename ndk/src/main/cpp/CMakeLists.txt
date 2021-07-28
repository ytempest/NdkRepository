# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

# cmake 最低版本需求
cmake_minimum_required(VERSION 3.4.1)

# 添加-fPIC编译选项
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
#
# Enable C++11编译
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


# 查找已有的so
# 参数1：给查找到的so命名
# 参数2：待查找的so名
find_library(
        log-lib
        log
)

# --------- ndk_tools ---------
message(STATUS "start build ndk_tools.so")

# 编译生成新的so
# 参数1：so名称
# 参数2：so库类型
# 参数3：so编译的源文件
add_library(
        ndk_tools
        SHARED
        ndk_tools.cpp
)

# 为编译的so添加其他so依赖
# 参数1：编译的so名
# 参数2：依赖的so名
target_link_libraries(
        ndk_tools
        ${log-lib}
)

# --------- file_encrypt ---------
message(STATUS "start build file_encrypt.so")

add_library(
        file_encrypt
        SHARED
        file_encrypt.cpp
)

target_link_libraries(
        file_encrypt
        ${log-lib}
)

# --------- image compress ---------
message(STATUS "start build compress-image.so")

# 引入指定目录下的源文件
include_directories(${PROJECT_SOURCE_DIR}/image)
# 加载指定目录下所有so，若有一个libmath.so，那么在target_link_libraries方法中，直接使用math名称即可代表为libmath.so
link_directories(${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})

add_library(
        compress-img
        SHARED
        ${PROJECT_SOURCE_DIR}/image/compress_img.cpp
)

target_link_libraries(
        compress-img

        turbojpeg
        jpeg

        jnigraphics
        ${log-lib}
)