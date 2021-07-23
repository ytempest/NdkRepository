# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

# cmake 最低版本需求
cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

# 编译生成新的so
# 参数1：so名称
# 参数2：so库类型
# 参数3：so编译的源文件
add_library( # Sets the name of the library.
        ndk_tools

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ndk_tools.cpp)

add_library(
        file_encrypt
        SHARED
        file_encrypt.cpp
)

target_link_libraries( # Specifies the target library.
        file_encrypt

        # 生成so需要依赖的其他so库名
        ${log-lib})

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

# 查找已有的so
find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        ndk_tools

        # 生成so需要依赖的其他so库名
        ${log-lib})