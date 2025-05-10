# 🌟 UniAPI 🌐

<div align="center">
  <h3>通用LLM API集成工具包 | 万能なLLM API統合ツールキット | Universal LLM API Integration Toolkit</h3>
  <p><i>"一个标准调用所有 | 全てを呼び出す一つの標準 | One Standard to Call Them All"</i></p>

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C++-20%2B-00599C?logo=c%2B%2B)](https://en.cppreference.com/w/cpp/compiler_support)
[![CUDA](https://img.shields.io/badge/CUDA-Optional-76B900?logo=nvidia)](https://developer.nvidia.com/cuda-toolkit)
</div>

---

## 📖 语言选择 | 言語選択 | Language Selection
- [中文版](#中文版)
- [日本語版](#日本語版)
- [English Version](#english-version)

---

<a id="中文版"></a>
## 中文版

### 🔍 概述

UniAPI 是一个**高性能 C++ 库**，它将 50 多个 LLM API 统一到一个**OpenAI 兼容的 REST 服务器**中。该库设计用于快速部署，默认情况下**无需身份验证**，支持动态加载提供者，并且可以直接推理本地量化 GGML 模型，无需额外配置。

```bash
# 通过 OpenAI 标准端点调用任何 LLM
curl -X POST http://localhost:8080/v1/chat/completions \
  -H "Content-Type: application/json" \
  -d '{"model":"claude-3","messages":[{"role":"user","content":"你好！"}]}'
```

### 🚀 快速开始（无需认证）

#### 方法一：使用 Docker（推荐）

##### 1. 构建 Docker 镜像

```bash
# 克隆仓库
git clone --recursive https://github.com/NGLSG/UniAPI.git
cd UniAPI

# 构建 Docker 镜像
docker build -t uniapi:latest .
```

##### 2. 运行 Docker 容器

```bash
# 使用默认端口 (8080) 运行
docker run -it --rm -p 8080:8080 uniapi:latest

# 使用自定义端口运行
docker run -it --rm -e PORT=9090 -p 9090:9090 uniapi:latest

# 挂载自定义配置文件
docker run -it --rm -p 8080:8080 -v $(pwd)/Config.yaml:/app/Config.yaml uniapi:latest
```

#### 方法二：本地编译安装

##### 1. 克隆仓库及子模块

```bash
git clone --recursive https://github.com/NGLSG/UniAPI.git
cd UniAPI
# 如果子模块未拉取：
git submodule update --init --recursive
```

##### 2. 安装依赖项

```bash
# Ubuntu/Debian 系统
sudo apt-get update
sudo apt-get install -y \
    git cmake build-essential \
    libvulkan-dev curl libcurl4-openssl-dev \
    libjsoncpp-dev uuid-dev libssl-dev zlib1g-dev
    
# 可选数据库支持
sudo apt-get install -y \
    libmysqlclient-dev \
    libsqlite3-dev \
    libhiredis-dev

# CentOS/RHEL 系统
sudo yum install -y \
    git cmake gcc-c++ \
    vulkan-devel curl libcurl-devel \
    jsoncpp-devel libuuid-devel openssl-devel zlib-devel
    
# 可选数据库支持
sudo yum install -y \
    mysql-devel sqlite-devel hiredis-devel
```

##### 3. 编译项目

```bash
mkdir build && cd build
cmake .. \
  -DCMAKE_CUDA_COMPILER=[CUDA_DIR]/bin/nvcc \  # 可选 CUDA 编译器
  -DCMAKE_TOOLCHAIN_FILE=[vcpkg_dir]/scripts/buildsystems/vcpkg.cmake  # 如果使用 vcpkg
make -j4
```

##### 4. 运行服务器

```bash
./bin/UniAPI 8080  # 端口可以更改
```

➡️ **立即测试**访问 `http://<your_ip>:8080/v1/chat/completions`

### ⚙️ 主要特性

| 特性 | 描述 |
|------|------|
| **🔓 零认证** | 默认无需 API 密钥 |
| **⚡ 快速推理** | 支持 GPU 加速的可选 CUDA |
| **📦 本地量化模型** | 直接推理本地量化 GGML 模型，无需额外配置 |
| **📡 REST 标准** | 100% 兼容 OpenAI 标准端点 |
| **🔄 动态配置** | 通过 `Config.yaml` 灵活配置 API |
| **🔌 自定义提供者** | 使用规则适配任意接口，实现高度扩展性 |

### 📦 支持的后端

| 提供商 | 描述 |
|--------|------|
| **OpenAI** | GPT-3.5、GPT-4 模型 |
| **Claude** | Claude-1、Claude-2、Claude-3 模型 |
| **Gemini** | Google Gemini 1.5 模型 |
| **GGML 模型** | 本地量化模型直接推理 |
| **其他** | 使用规则适配几乎任何接口 |

### 🔧 配置示例：自定义提供者

```yaml
- enable: false
  supportSystemRole: true
  name: 测试2
  model: o3-mini
  apiPath: http://localhost:3032/v1/chat/completions
  apiKeyRole:
    key: nuul
    role: HEADERS
    header: "Authorization: Bearer "
  headers:
    header2: 1
    anthropic-version: "\"2023-06-01\""
  roles:
    system: system
    user: user
    assistant: assistant
  promptRole:
    role:
      suffix: messages
      path: role
      content: content
      isStr: false
    prompt:
      suffix: messages
      path: content
      content: content
      isStr: false
  params:
    - suffix: stream
      path: ""
      content: true
      isStr: false
    - suffix: model
      path: ""
      content: ${MODEL}
      isStr: true
  responseRole:
    suffix: ""
    content: result/message/content
    callback: RESPONSE
    stopFlag: ""
  author: Ryoshi
  version: 1.0
  description: 自定义规则
  supportModels: []
```

### 📜 许可证

MIT - 免费用于商业和个人用途。详见 [LICENSE](LICENSE)。

---

<a id="日本語版"></a>
## 日本語版

### 🔍 概要

UniAPI は、50 以上の LLM API を単一の **OpenAI 互換 REST サーバー** に統合する **高パフォーマンスな C++ ライブラリ** です。迅速な展開を目的としており、デフォルトで **認証は必要ありません** し、動的プロバイダーのロードをサポートしています。さらに、ローカルにクオンタイズされた GGML モデルを直接推論でき、追加の設定は必要ありません。

```bash
# OpenAI 標準エンドポイントを経由して任意の LLM にアクセス
curl -X POST http://localhost:8080/v1/chat/completions \
  -H "Content-Type: application/json" \
  -d '{"model":"claude-3","messages":[{"role":"user","content":"こんにちは！"}]}'
```

### 🚀 クイックスタート（認証なし）

#### 方法 1：Docker を使用（推奨）

##### 1. Docker イメージの構築

```bash
# リポジトリのクローン
git clone --recursive https://github.com/NGLSG/UniAPI.git
cd UniAPI

# Docker イメージの構築
docker build -t uniapi:latest .
```

##### 2. Docker コンテナの実行

```bash
# デフォルトポート (8080) で実行
docker run -it --rm -p 8080:8080 uniapi:latest

# カスタムポートで実行
docker run -it --rm -e PORT=9090 -p 9090:9090 uniapi:latest

# カスタム設定ファイルをマウント
docker run -it --rm -p 8080:8080 -v $(pwd)/Config.yaml:/app/Config.yaml uniapi:latest
```

#### 方法 2：ローカルでのコンパイル

##### 1. リポジトリとサブモジュールのクローン

```bash
git clone --recursive https://github.com/NGLSG/UniAPI.git
cd UniAPI
# サブモジュールが取得されていない場合：
git submodule update --init --recursive
```

##### 2. 依存関係のインストール

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y \
    git cmake build-essential \
    libvulkan-dev curl libcurl4-openssl-dev \
    libjsoncpp-dev uuid-dev libssl-dev zlib1g-dev
    
# オプション：データベースサポート
sudo apt-get install -y \
    libmysqlclient-dev \
    libsqlite3-dev \
    libhiredis-dev

# CentOS/RHEL 
sudo yum install -y \
    git cmake gcc-c++ \
    vulkan-devel curl libcurl-devel \
    jsoncpp-devel libuuid-devel openssl-devel zlib-devel
    
# オプション：データベースサポート
sudo yum install -y \
    mysql-devel sqlite-devel hiredis-devel
```

##### 3. プロジェクトのコンパイル

```bash
mkdir build && cd build
cmake .. \
  -DCMAKE_CUDA_COMPILER=[CUDA_DIR]/bin/nvcc \  # オプションの CUDA コンパイラ
  -DCMAKE_TOOLCHAIN_FILE=[vcpkg_dir]/scripts/buildsystems/vcpkg.cmake  # vcpkg を使用する場合
make -j4
```

##### 4. サーバーの実行

```bash
./bin/UniAPI 8080  # ポートは変更可能
```

➡️ **すぐにテスト**：`http://<your_ip>:8080/v1/chat/completions` にアクセス

### ⚙️ 主な機能

| 機能 | 説明 |
|------|------|
| **🔓 認証不要** | デフォルトで API キーは必要ありません |
| **⚡ 高速推論** | オプションの CUDA サポートで GPU アクセラレーションを実現 |
| **📦 ローカルクオンタイズモデル** | ローカルにクオンタイズされた GGML モデルを直接推論 |
| **📡 REST 標準** | OpenAI 標準エンドポイントをサポート |
| **🔄 動的構成** | `Config.yaml` を通じて API を柔軟に構成 |
| **🔌 カスタムプロバイダー** | 任意のインターフェースをルールで適応し、高い拡張性を実現 |

### 📦 サポートされているバックエンド

| プロバイダー | 説明 |
|--------------|------|
| **OpenAI** | GPT-3.5、GPT-4 モデル |
| **Claude** | Claude-1、Claude-2、Claude-3 モデル |
| **Gemini** | Google Gemini 1.5 モデル |
| **GGML モデル** | ローカルにクオンタイズされたモデルの直接推論 |
| **その他** | 任意のインターフェースをルールで構成 |

### 🔧 設定例：カスタムプロバイダー

```yaml
- enable: false
  supportSystemRole: true
  name: 测试2
  model: o3-mini
  apiPath: http://localhost:3032/v1/chat/completions
  apiKeyRole:
    key: nuul
    role: HEADERS
    header: "Authorization: Bearer "
  headers:
    header2: 1
    anthropic-version: "\"2023-06-01\""
  roles:
    system: system
    user: user
    assistant: assistant
  promptRole:
    role:
      suffix: messages
      path: role
      content: content
      isStr: false
    prompt:
      suffix: messages
      path: content
      content: content
      isStr: false
  params:
    - suffix: stream
      path: ""
      content: true
      isStr: false
    - suffix: model
      path: ""
      content: ${MODEL}
      isStr: true
  responseRole:
    suffix: ""
    content: result/message/content
    callback: RESPONSE
    stopFlag: ""
  author: Ryoshi
  version: 1.0
  description: 自定义规则
  supportModels: []
```

### 📜 ライセンス

MIT - 商用および個人利用は無料です。詳細は [LICENSE](LICENSE) をご確認ください。

---

<a id="english-version"></a>
## English Version

### 🔍 Overview

UniAPI is a **high-performance C++ library** that consolidates 50+ LLM APIs into a single **OpenAI-compatible REST server**. Designed for rapid deployment, it operates **without authentication** by default, supports dynamic provider loading, and enables direct inference of local quantized GGML models without additional configuration.

```bash
# Access any LLM via OpenAI-standard endpoint
curl -X POST http://localhost:8080/v1/chat/completions \
  -H "Content-Type: application/json" \
  -d '{"model":"claude-3","messages":[{"role":"user","content":"Hello!"}]}'
```

### 🚀 Quick Start (No Auth)

#### Method 1: Using Docker (Recommended)

##### 1. Build Docker Image

```bash
# Clone repository
git clone --recursive https://github.com/NGLSG/UniAPI.git
cd UniAPI

# Build Docker image
docker build -t uniapi:latest .
```

##### 2. Run Docker Container

```bash
# Run with default port (8080)
docker run -it --rm -p 8080:8080 uniapi:latest

# Run with custom port
docker run -it --rm -e PORT=9090 -p 9090:9090 uniapi:latest

# Mount custom config file
docker run -it --rm -p 8080:8080 -v $(pwd)/Config.yaml:/app/Config.yaml uniapi:latest
```

#### Method 2: Local Compilation

##### 1. Clone Repository with Submodules

```bash
git clone --recursive https://github.com/NGLSG/UniAPI.git
cd UniAPI
# If submodules weren't pulled:
git submodule update --init --recursive
```

##### 2. Install Dependencies

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y \
    git cmake build-essential \
    libvulkan-dev curl libcurl4-openssl-dev \
    libjsoncpp-dev uuid-dev libssl-dev zlib1g-dev
    
# Optional database support
sudo apt-get install -y \
    libmysqlclient-dev \
    libsqlite3-dev \
    libhiredis-dev

# CentOS/RHEL 
sudo yum install -y \
    git cmake gcc-c++ \
    vulkan-devel curl libcurl-devel \
    jsoncpp-devel libuuid-devel openssl-devel zlib-devel
    
# Optional database support
sudo yum install -y \
    mysql-devel sqlite-devel hiredis-devel
```

##### 3. Compile Project

```bash
mkdir build && cd build
cmake .. \
  -DCMAKE_CUDA_COMPILER=[CUDA_DIR]/bin/nvcc \  # Optional CUDA compiler
  -DCMAKE_TOOLCHAIN_FILE=[vcpkg_dir]/scripts/buildsystems/vcpkg.cmake  # If using vcpkg
make -j4
```

##### 4. Run Server

```bash
./bin/UniAPI 8080  # Port can be changed
```

➡️ **Immediately test** at `http://<your_ip>:8080/v1/chat/completions`

### ⚙️ Key Features

| Feature | Description |
|---------|-------------|
| **🔓 Zero Auth** | No API key required by default |
| **⚡ Fast Inference** | Optional CUDA support for GPU acceleration |
| **📦 Local Quantized Models** | Direct inference of local quantized GGML models without extra configuration |
| **📡 REST Standard** | 100% compatible with OpenAI endpoints |
| **🔄 Dynamic Configuration** | Flexible API configuration via `Config.yaml` |
| **🔌 Custom Providers** | Adapt to any interface using rule-based configuration |

### 📦 Supported Backends

| Provider | Description |
|----------|-------------|
| **OpenAI** | GPT-3.5, GPT-4 models |
| **Claude** | Claude-1, Claude-2, Claude-3 models |
| **Gemini** | Google Gemini 1.5 models |
| **GGML Models** | Direct inference of local quantized models |
| **Others** | Configure any interface using rules |

### 🔧 Configuration Example: Custom Provider

```yaml
- enable: false
  supportSystemRole: true
  name: 测试2
  model: o3-mini
  apiPath: http://localhost:3032/v1/chat/completions
  apiKeyRole:
    key: nuul
    role: HEADERS
    header: "Authorization: Bearer "
  headers:
    header2: 1
    anthropic-version: "\"2023-06-01\""
  roles:
    system: system
    user: user
    assistant: assistant
  promptRole:
    role:
      suffix: messages
      path: role
      content: content
      isStr: false
    prompt:
      suffix: messages
      path: content
      content: content
      isStr: false
  params:
    - suffix: stream
      path: ""
      content: true
      isStr: false
    - suffix: model
      path: ""
      content: ${MODEL}
      isStr: true
  responseRole:
    suffix: ""
    content: result/message/content
    callback: RESPONSE
    stopFlag: ""
  author: Ryoshi
  version: 1.0
  description: 自定义规则
  supportModels: []
```

### 📜 License

MIT - Free for commercial and personal use. See [LICENSE](LICENSE).