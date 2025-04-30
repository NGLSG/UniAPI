# UniAPI 🌐

**Universal LLM API Integration Toolkit for C++**  
*"One Standard to Call Them All"*

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C++-20%2B-00599C?logo=c%2B%2B)](https://en.cppreference.com/w/cpp/compiler_support)
[![CUDA](https://img.shields.io/badge/CUDA-Optional-76B900?logo=nvidia)](https://developer.nvidia.com/cuda-toolkit)

## 🔍 Overview

UniAPI is a **high-performance C++ library** that unifies 50+ LLM APIs into a single **OpenAI-compatible REST server**.
Designed for rapid deployment, it requires **no authentication** by default and supports dynamic provider loading.

```bash
# Access any LLM via OpenAI-standard endpoint
curl -X POST http://localhost:8080/v1/chat/completions \
  -H "Content-Type: application/json" \
  -d '{"model":"claude-3","messages":[{"role":"user","content":"Hello!"}]}'
```

## 🚀 **Quick Start (No Auth)**

### 1. Clone with Submodules

```bash
git clone --recursive https://github.com/NGLSG/UniAPI.git
cd UniAPI
# If submodules weren't pulled:
git submodule update --init --recursive
```

### 2. Compile with Options

```bash
mkdir build && cd build
cmake .. \
  -DCROW_USE_BOOST=1 \          # Enable Boost for Crow HTTP
  -DCMAKE_CUDA_COMPILER=[CUDA_DIR]/bin/nvcc \  # Optional CUDA compiler"
  -DCMAKE_TOOLCHAIN_FILE=[vcpkg_dir]/scripts/buildsystems/vcpkg.cmake  # If using vcpkg
make -j4
```

### 3. Run the Server

```bash
./UniAPI 8080  # Port can be changed
```

➡️ **Immediately test** at `http://<your_ip>:8080/v1/chat/completions`

## ⚙️ **Key Features**

| Feature              | Description                                                   |
|----------------------|---------------------------------------------------------------|
| **🔓 Zero Auth**     | No API key required by default                                |
| **⚡ Fast Inference** | Optional CUDA support for GPU-accelerated providers           |
| 📡 **REST Standard** | 100% OpenAI-compatible endpoints (even for non-OpenAI models) |
| 🔄 **Configure**     | Edit the 'Config.yaml' to configure your api                  |

## 📦 **Supported Backends**

| Provider             | Description                                |
|----------------------|--------------------------------------------|
| **OpenAI**           | OpenAI's GPT-3.5 and GPT-4 models          |
| **Claude**           | Anthropic's Claude-1, 2, and 3 models      |
| **OpenAI-Interface** | OpenAI's API interface for custom models   |
| **Gemini**           | Gemini 1.5 models from Google              |
| **Other**            | Use rules to adapt to almost any interface |

## 🔧 **Configuration**

Edit the `Config.yaml` file to set up your providers.
If you want use GUI to configure, you can use [ChatBot](https://github.com/NGLSG/ChatBot) project to easily configure
the `Config.yaml` file(After the configuration is completed, you can find config.yaml in the same directory as the
ChatBot executable file, and the appropriate modification can be applied to this project).
And also you can use [ChatBot](https://github.com/NGLSG/ChatBot) project it is very convenient to chat with the LLMs.

## 📜 **License**

MIT - Free for commercial and personal use. See [LICENSE](LICENSE).

---