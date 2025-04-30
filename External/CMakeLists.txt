cmake_minimum_required(VERSION 3.18)

project(uniLibProject)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)

# GPU 加速配置（CUDA > Vulkan > CPU）
option(GGML_CUDA_USE_CUBLASLT "使用 cuBLASLt (需要 CUDA 12+)" ON)
option(GGML_CUDA_FORCE_MMQ "强制使用 CUDA MMQ (如可用)" ON)


# 添加Boost asio组件
find_package(Boost REQUIRED COMPONENTS asio)

# 添加子目录
add_subdirectory(json)
add_subdirectory(llama-cpp)
add_subdirectory(yaml-cpp)
add_subdirectory(spdlog)
add_subdirectory(crow)
find_package(CURL REQUIRED)
find_package(Vulkan REQUIRED)

# 检测 CUDA
include(CheckLanguage)
check_language(CUDA)
if (CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    # 简化的CUDA版本检测
    if (CMAKE_CUDA_COMPILER_VERSION VERSION_GREATER_EQUAL 11.0)
        message(STATUS "【GPU加速】检测到 CUDA (${CMAKE_CUDA_COMPILER_VERSION})")
        set(GGML_CUDA ON CACHE BOOL "启用 CUDA 支持" FORCE)
        set(GGML_CUDA_MMQ_SUPPORT ON CACHE BOOL "启用 CUDA MMQ 支持" FORCE)
    endif ()
else ()
    # 使用Vulkan
    if (Vulkan_FOUND)
        message(STATUS "【GPU加速】使用 Vulkan")
        set(GGML_VULKAN ON CACHE BOOL "启用 Vulkan 支持" FORCE)
    else ()
        message(STATUS "【GPU加速】将使用 CPU 模式")
    endif ()
endif ()

# 创建接口库
add_library(uniLib INTERFACE)

# 链接所需库
target_link_libraries(uniLib INTERFACE
        nlohmann_json::nlohmann_json  # json库
        yaml-cpp::yaml-cpp            # yaml-cpp库
        llama                         # llama.cpp库
        CURL::libcurl                 # curl库
        ${Vulkan_LIBRARIES}           # Vulkan库
        spdlog::spdlog                # 日志库
        Boost::asio                   # Boost asio库
        Crow                          # Crow框架库
)
target_include_directories(uniLib INTERFACE
        ${Vulkan_INCLUDE_DIRS})

# 根据GPU加速情况添加相应的链接库和编译定义
if (GGML_CUDA)
    # 如果有CUDA可用，链接CUDA相关库
    target_compile_definitions(uniLib INTERFACE HAVE_CUDA=1)
    target_link_libraries(uniLib INTERFACE ggml-cuda)
    if (CMAKE_CUDA_COMPILER_VERSION VERSION_GREATER_EQUAL 12.0)
        target_compile_definitions(uniLib INTERFACE HAVE_CUDA12=1)
    endif ()
elseif (GGML_VULKAN)
    # 如果使用Vulkan
    target_compile_definitions(uniLib INTERFACE HAVE_VULKAN=1)
    target_link_libraries(uniLib INTERFACE ggml-vulkan)
else ()
    # 仅CPU模式
    target_compile_definitions(uniLib INTERFACE CPU_ONLY=1)
    target_link_libraries(uniLib INTERFACE ggml-cpu)
endif ()

# 添加包含目录
target_include_directories(uniLib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/json/include
        ${CMAKE_CURRENT_SOURCE_DIR}/llama-cpp/include
        ${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp/include
)