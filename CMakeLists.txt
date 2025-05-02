cmake_minimum_required(VERSION 3.22)
project(UniAPI)

set(CMAKE_CXX_STANDARD 20)

# 设置输出目录为统一路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# GPU 加速配置（CUDA > Vulkan > CPU）
option(GGML_CUDA_USE_CUBLASLT "使用 cuBLASLt (需要 CUDA 12+)" ON)
option(GGML_CUDA_FORCE_MMQ "强制使用 CUDA MMQ (如可用)" ON)

# 收集源文件和头文件
file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES include/*.h include/*.hpp)

# 添加外部依赖库
add_subdirectory(External)

# 检测CUDA
include(CheckLanguage)
check_language(CUDA)
if (CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    # CUDA版本检测
    if (CMAKE_CUDA_COMPILER_VERSION VERSION_GREATER_EQUAL 11.0)
        message(STATUS "【GPU加速】检测到 CUDA (${CMAKE_CUDA_COMPILER_VERSION})")
        set(GGML_CUDA ON CACHE BOOL "启用 CUDA 支持" FORCE)
        set(GGML_CUDA_MMQ_SUPPORT ON CACHE BOOL "启用 CUDA MMQ 支持" FORCE)

        # 获取CUDA路径信息
        get_filename_component(CUDA_BIN_DIR ${CMAKE_CUDA_COMPILER} DIRECTORY)
        get_filename_component(CUDA_ROOT_DIR ${CUDA_BIN_DIR} DIRECTORY)
        set(CUDA_DLL_DIR "${CUDA_ROOT_DIR}/bin")
    endif ()
else ()
    # 使用Vulkan
    find_package(Vulkan REQUIRED)
    if (Vulkan_FOUND)
        message(STATUS "【GPU加速】使用 Vulkan")
        set(GGML_VULKAN ON CACHE BOOL "启用 Vulkan 支持" FORCE)
    else ()
        message(STATUS "【GPU加速】将使用 CPU 模式")
    endif ()
endif ()

# 创建可执行文件目标
add_executable(UniAPI main.cpp ${SRC_FILES} ${HEADER_FILES})
target_include_directories(UniAPI PUBLIC include)


target_link_libraries(UniAPI PRIVATE uniLib)
if (MSVC)
    target_compile_options(UniAPI PRIVATE /bigobj)
endif ()

# 设置所有目标的输出路径
get_property(TARGETS_LIST DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
foreach (TARGET_NAME ${TARGETS_LIST})
    if (TARGET ${TARGET_NAME})
        get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)
        if (NOT "${TARGET_TYPE}" STREQUAL "INTERFACE_LIBRARY")
            set_target_properties(${TARGET_NAME} PROPERTIES
                    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
                    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            )
        endif ()
    endif ()
endforeach ()

# 对于Windows平台，复制相关DLL到输出目录
if (WIN32)
    # 添加构建后执行的命令
    add_custom_command(TARGET UniAPI POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "正在复制依赖库到输出目录..."
    )

    # 复制CUDA DLL（如果有CUDA）
    if (DEFINED CUDA_DLL_DIR AND EXISTS "${CUDA_DLL_DIR}")
        file(GLOB CUDA_DLLS
                "${CUDA_DLL_DIR}/cudart64_*.dll"
                "${CUDA_DLL_DIR}/cublas64_*.dll"
                "${CUDA_DLL_DIR}/cublasLt64_*.dll")
        foreach (DLL_FILE ${CUDA_DLLS})
            add_custom_command(TARGET UniAPI POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${DLL_FILE}" "${CMAKE_BINARY_DIR}/bin")
        endforeach ()
    endif ()
endif ()

# 输出构建信息
message(STATUS "【输出目录信息】所有库文件和可执行文件将输出到: ${CMAKE_BINARY_DIR}/bin")