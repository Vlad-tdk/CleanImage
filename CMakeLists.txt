cmake_minimum_required(VERSION 3.14)
project(CleanImage)

set(CMAKE_CXX_STANDARD 20)

# Указание пути к OpenCV, установленному через vcpkg
set(OpenCV_DIR "/Users/vlad/vcpkg/installed/arm64-osx/share/opencv4")
set(INIREADER_PATH "/Users/vlad/inih")

# Проверка наличия файлов INIReader и ini.c
if(NOT EXISTS "${INIREADER_PATH}/cpp/INIReader.cpp" OR NOT EXISTS "${INIREADER_PATH}/ini.c")
    message(FATAL_ERROR "INIReader.cpp or ini.c not found in ${INIREADER_PATH}")
endif()

# Сообщение о компиляции INIReader
message(STATUS "Compiling INIReader with ${INIREADER_PATH}/cpp/INIReader.cpp and ${INIREADER_PATH}/ini.c")

# Поиск OpenCV и других библиотек
find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
find_package(libzip CONFIG REQUIRED)

# Указать пути к заголовочным файлам
include_directories(${OpenCV_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include ${INIREADER_PATH})

# Явное указание пути к libexiv2.a
set(EXIV2_LIB "/Users/vlad/vcpkg/installed/arm64-osx/lib/libexiv2.a")

# Создание библиотеки INIReader с ini.c
add_library(INIReader ${INIREADER_PATH}/cpp/INIReader.cpp ${INIREADER_PATH}/ini.c)

# Добавить исполняемый файл
add_executable(CleanImage
        src/main.cpp
        src/image_processing.cpp
        src/exif_operations.cpp
        src/zip_operations.cpp
)

# Линковка с библиотеками
target_link_libraries(CleanImage PRIVATE INIReader ${OpenCV_LIBS} ${EXIV2_LIB} libzip::zip iconv)
