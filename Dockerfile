FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# 安装基础工具链
RUN apt-get update && apt-get install -y \
    git \
    cmake \
    build-essential \
    curl \
    && rm -rf /var/lib/apt/lists/*

# 安装主要依赖项
RUN apt-get update && apt-get install -y \
    libjsoncpp-dev \
    uuid-dev \
    libssl-dev \
    zlib1g-dev \
    libpq-dev \
    libbrotli-dev \
    libmysqlclient-dev \
    libsqlite3-dev \
    libhiredis-dev \
    libvulkan-dev \
    libcurl4-openssl-dev \
    doxygen \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

# 递归初始化所有子模块
RUN git submodule update --init --recursive

# 构建配置（添加详细日志）
RUN mkdir build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc) VERBOSE=1

ENV PORT=8080
ENTRYPOINT ["sh", "-c", "./build/bin/UniAPI ${PORT}"]