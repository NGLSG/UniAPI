# 使用官方Ubuntu基础镜像
FROM ubuntu:22.04

# 设置非交互式前端以避免安装过程中出现提示
ENV DEBIAN_FRONTEND=noninteractive

# 更新包列表并安装基本工具
RUN apt-get update && apt-get install -y \
    git \
    cmake \
    build-essential \
    curl \
    && rm -rf /var/lib/apt/lists/*

# 安装主要依赖项（分步进行）
RUN apt-get update && apt-get install -y \
    libjsoncpp-dev \
    uuid-dev \
    libssl-dev \
    zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

# 安装数据库相关依赖
RUN apt-get update && apt-get install -y \
    libmysqlclient-dev \
    libsqlite3-dev \
    libhiredis-dev \
    && rm -rf /var/lib/apt/lists/*

# 安装其他依赖
RUN apt-get update && apt-get install -y \
    libvulkan-dev \
    libcurl4-openssl-dev \
    && rm -rf /var/lib/apt/lists/*

# 设置工作目录
WORKDIR /app

# 复制项目文件
COPY . .

# 初始化并更新git子模块
RUN git submodule init && git submodule update

# 创建构建目录并构建项目
RUN mkdir build && \
    cd build && \
    cmake -DCMAKE_CXX_FLAGS="-w" -DCMAKE_C_FLAGS="-w" .. && \
    make -j$(nproc)

# 设置默认端口环境变量
ENV PORT=8080

# 设置容器启动命令
ENTRYPOINT ["sh", "-c", "./build/bin/UniAPI ${PORT}"]