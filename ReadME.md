# -*- coding: utf-8 -*-
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

UniAPI 是一个**高性能 C++ 库**，它将 50 多个 LLM API 统一到单个 **OpenAI 兼容的 REST 服务器**中。该库设计用于快速部署，默认情况下**不需要身份验证**，并支持动态提供者加载。

```bash
# 通过 OpenAI 标准端点访问任何 LLM
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
sudo apt-get install -y git cmake build-essential libboost-all-dev libvulkan-dev curl libasio-dev libcurl4-openssl-dev

# CentOS/RHEL 系统
sudo yum install -y git cmake gcc-c++ boost-devel vulkan-devel curl asio-devel libcurl-devel
```

##### 3. 编译项目

```bash
mkdir build && cd build
cmake .. \
  -DCROW_USE_BOOST=1 \          # 为 Crow HTTP 启用 Boost
  -DCMAKE_CUDA_COMPILER=[CUDA_DIR]/bin/nvcc \  # 可选 CUDA 编译器
  -DCMAKE_TOOLCHAIN_FILE=[vcpkg_dir]/scripts/buildsystems/vcpkg.cmake  # 如果使用 vcpkg
make -j4
```

##### 4. 运行服务器

```bash
./bin/UniAPI 8080  # 端口可以更改
```

➡️ **立即测试**访问 `http://<your_ip>:8080/v1/chat/completions`

### ⚙️ 主要特性

| 特性 | 描述 |
|------|------|
| **🔓 零认证** | 默认情况下不需要 API 密钥 |
| **⚡ 快速推理** | 可选 CUDA 支持，用于 GPU 加速 |
| **📡 REST 标准** | 100% OpenAI 兼容端点（即使对于非 OpenAI 模型）|
| **🔄 配置** | 编辑 'Config.yaml' 来配置你的 API |

### 📦 支持的后端

| 提供商 | 描述 |
|--------|------|
| **OpenAI** | OpenAI 的 GPT-3.5 和 GPT-4 模型 |
| **Claude** | Anthropic 的 Claude-1、2 和 3 模型 |
| **OpenAI-Interface** | 用于自定义模型的 OpenAI API 接口 |
| **Gemini** | 来自 Google 的 Gemini 1.5 模型 |
| **其他** | 使用规则适配几乎任何接口 |

### 🔧 配置

编辑 `Config.yaml` 文件来设置你的提供商。如果你想使用 GUI 进行配置，可以使用 [ChatBot](https://github.com/NGLSG/ChatBot) 项目轻松配置 `Config.yaml` 文件（配置完成后，你可以在 ChatBot 可执行文件的同一目录下找到 config.yaml，适当修改后可应用于本项目）。

你也可以使用 [ChatBot](https://github.com/NGLSG/ChatBot) 项目，它非常方便地与 LLM 聊天。

### 📜 许可证

MIT - 免费用于商业和个人用途。详见 [LICENSE](LICENSE)。

---

<a id="日本語版"></a>
## 日本語版

### 🔍 概要

UniAPIは、50以上のLLM APIを単一の**OpenAI互換RESTサーバー**に統合する**高性能C++ライブラリ**です。迅速な展開のために設計され、デフォルトでは**認証不要**で、動的プロバイダーのロードをサポートしています。

```bash
# OpenAI標準エンドポイントを介して任意のLLMにアクセス
curl -X POST http://localhost:8080/v1/chat/completions \
  -H "Content-Type: application/json" \
  -d '{"model":"claude-3","messages":[{"role":"user","content":"こんにちは！"}]}'
```

### 🚀 クイックスタート（認証なし）

#### 方法1：Dockerを使用（推奨）

##### 1. Dockerイメージの構築

```bash
# リポジトリのクローン
git clone --recursive https://github.com/NGLSG/UniAPI.git
cd UniAPI

# Dockerイメージの構築
docker build -t uniapi:latest .
```

##### 2. Dockerコンテナの実行

```bash
# デフォルトポート（8080）で実行
docker run -it --rm -p 8080:8080 uniapi:latest

# カスタムポートで実行
docker run -it --rm -e PORT=9090 -p 9090:9090 uniapi:latest

# カスタム設定ファイルをマウント
docker run -it --rm -p 8080:8080 -v $(pwd)/Config.yaml:/app/Config.yaml uniapi:latest
```

#### 方法2：ローカルでコンパイル

##### 1. リポジトリとサブモジュールをクローン

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
sudo apt-get install -y git cmake build-essential libboost-all-dev libvulkan-dev curl libasio-dev libcurl4-openssl-dev

# CentOS/RHEL
sudo yum install -y git cmake gcc-c++ boost-devel vulkan-devel curl asio-devel libcurl-devel
```

##### 3. プロジェクトのコンパイル

```bash
mkdir build && cd build
cmake .. \
  -DCROW_USE_BOOST=1 \          # Crow HTTPのためのBoost有効化
  -DCMAKE_CUDA_COMPILER=[CUDA_DIR]/bin/nvcc \  # オプションのCUDAコンパイラ
  -DCMAKE_TOOLCHAIN_FILE=[vcpkg_dir]/scripts/buildsystems/vcpkg.cmake  # vcpkgを使用する場合
make -j4
```

##### 4. サーバーの実行

```bash
./bin/UniAPI 8080  # ポートは変更可能
```

➡️ **すぐにテスト**：`http://<your_ip>:8080/v1/chat/completions`にアクセス

### ⚙️ 主な機能

| 機能 | 説明 |
|------|------|
| **🔓 認証不要** | デフォルトでAPIキー不要 |
| **⚡ 高速推論** | GPUアクセラレーションのためのオプションCUDAサポート |
| **📡 REST標準** | 100% OpenAI互換エンドポイント（非OpenAIモデルでも） |
| **🔄 設定** | 'Config.yaml'を編集してAPIを設定 |

### 📦 サポートされるバックエンド

| プロバイダー | 説明 |
|--------------|------|
| **OpenAI** | OpenAIのGPT-3.5およびGPT-4モデル |
| **Claude** | AnthropicのClaude-1、2、3モデル |
| **OpenAI-Interface** | カスタムモデル用のOpenAI APIインターフェース |
| **Gemini** | GoogleのGemini 1.5モデル |
| **その他** | ルールを使用してほぼどんなインターフェースにも適応 |

### 🔧 設定

`Config.yaml`ファイルを編集してプロバイダーを設定します。GUIで設定したい場合は、[ChatBot](https://github.com/NGLSG/ChatBot)プロジェクトを使用して`Config.yaml`ファイルを簡単に設定できます（設定完了後、ChatBot実行ファイルと同じディレクトリにconfig.yamlが見つかり、適切な修正を施してこのプロジェクトに適用できます）。

また、[ChatBot](https://github.com/NGLSG/ChatBot)プロジェクトを使用すると、LLMとのチャットが非常に便利になります。

### 📜 ライセンス

MIT - 商用および個人利用は無料。詳細は[LICENSE](LICENSE)をご覧ください。

---

<a id="english-version"></a>
## English Version

### 🔍 Overview

UniAPI is a **high-performance C++ library** that unifies 50+ LLM APIs into a single **OpenAI-compatible REST server**. Designed for rapid deployment, it requires **no authentication** by default and supports dynamic provider loading.

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
sudo apt-get install -y git cmake build-essential libboost-all-dev libvulkan-dev curl libasio-dev libcurl4-openssl-dev

# CentOS/RHEL
sudo yum install -y git cmake gcc-c++ boost-devel vulkan-devel curl asio-devel libcurl-devel
```

##### 3. Compile Project

```bash
mkdir build && cd build
cmake .. \
  -DCROW_USE_BOOST=1 \          # Enable Boost for Crow HTTP
  -DCMAKE_CUDA_COMPILER=[CUDA_DIR]/bin/nvcc \  # Optional CUDA compiler
  -DCMAKE_TOOLCHAIN_FILE=[vcpkg_dir]/scripts/buildsystems/vcpkg.cmake  # If using vcpkg
make -j4
```

##### 4. Run Server

```bash
./bin/UniAPI 8080  # Port can be changed
```

➡️ **Immediately test** at `http://<your_ip>:8080/v1/chat/completions`

### ⚙️ Key Features

| Feature | Description |
|---------|-------------|
| **🔓 Zero Auth** | No API key required by default |
| **⚡ Fast Inference** | Optional CUDA support for GPU-accelerated providers |
| **📡 REST Standard** | 100% OpenAI-compatible endpoints (even for non-OpenAI models) |
| **🔄 Configure** | Edit the 'Config.yaml' to configure your API |

### 📦 Supported Backends

| Provider | Description |
|----------|-------------|
| **OpenAI** | OpenAI's GPT-3.5 and GPT-4 models |
| **Claude** | Anthropic's Claude-1, 2, and 3 models |
| **OpenAI-Interface** | OpenAI's API interface for custom models |
| **Gemini** | Gemini 1.5 models from Google |
| **Other** | Use rules to adapt to almost any interface |

### 🔧 Configuration

Edit the `Config.yaml` file to set up your providers. If you want to use GUI to configure, you can use the [ChatBot](https://github.com/NGLSG/ChatBot) project to easily configure the `Config.yaml` file (After the configuration is completed, you can find config.yaml in the same directory as the ChatBot executable file, and the appropriate modification can be applied to this project).

You can also use the [ChatBot](https://github.com/NGLSG/ChatBot) project, which is very convenient for chatting with LLMs.

### 📜 License

MIT - Free for commercial and personal use. See [LICENSE](LICENSE).