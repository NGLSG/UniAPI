# 使用官方Ubuntu基础镜像
FROM ubuntu:22.04

# 设置非交互式前端以避免安装过程中出现提示
ENV DEBIAN_FRONTEND=noninteractive

# 安装基本工具和依赖
RUN apt-get update && apt-get install -y \
    git \
    cmake \
    build-essential \
    libboost-all-dev \
    libvulkan-dev \
    curl \
    libasio-dev \
    libcurl4-openssl-dev \
    && rm -rf /var/lib/apt/lists/*

# 设置工作目录
WORKDIR /app

# 复制项目文件
COPY . .

# 确保Config.yaml在容器中存在
COPY Config.yaml /app/Config.yaml

# 初始化并更新git子模块
RUN git submodule init && git submodule update

# 创建构建目录并构建项目（禁止显示警告，禁用Vulkan）
RUN mkdir build && \
    cd build && \
    cmake -DCMAKE_CXX_FLAGS="-w" -DCMAKE_C_FLAGS="-w" .. && \
    make -j16

# 设置默认端口环境变量
ENV PORT=8080

# 设置容器启动命令（修正了可执行文件路径）
ENTRYPOINT ["sh", "-c", "./build/bin/UniAPI ${PORT}"]