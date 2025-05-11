<div align="center">
  <img src="icon.svg" alt="UniAPI Icon" width="200" height="200"> <!-- 插入图标 -->
</div>


<div align="center">
  <h3>通用 LLM API 集成工具包</h3>
  <p><i>"一个标准调用所有"</i></p>
  <a href="https://opensource.org/licenses/MIT"><img src="https://img.shields.io/badge/License-MIT-yellow.svg" alt="License: MIT"></a>
  <a href="https://en.cppreference.com/w/cpp/compiler_support"><img src="https://img.shields.io/badge/C++-20%2B-00599C?logo=c%2B%2B" alt="C++20"></a>
  <a href="https://developer.nvidia.com/cuda-toolkit"><img src="https://img.shields.io/badge/CUDA-Optional-76B900?logo=nvidia" alt="CUDA"></a>
  <a href="https://hub.docker.com/r/ryoshi541/uniapi"><img src="https://img.shields.io/badge/Docker-Available-2496ED?logo=docker" alt="Docker"></a>
</div>

---

## Star History

[![Star History Chart](https://api.star-history.com/svg?repos=NGLSG/UniAPI&type=Date)](https://www.star-history.com/#NGLSG/UniAPI&Date)

<a id="how-to-support-this-project"></a>

## 🌟 如何支持本项目

我们致力于持续改进 UniAPI 并添加新功能。您的支持对我们项目的持续发展至关重要。以下是您可以帮助的方式：

- **给我们点个星**  
  如果您觉得 UniAPI 有用，请在 [GitHub](https://github.com/NGLSG/UniAPI) 上给我们点个星。这能帮助我们获得更多关注并吸引更多贡献者。

- **参与项目贡献**  
  我们欢迎来自社区的贡献！无论是修复漏洞、添加新功能还是改进文档，每一份贡献都弥足珍贵。开始贡献很简单，只需要 fork 仓库并提交一个
  pull request 即可。

- **分享本项目**  
  把 UniAPI 分享给您的朋友、同事，或者在社交媒体上宣传。知道这个项目的人越多越好！

- **提供反馈**  
  您的反馈对我们很重要。如果您有任何建议、想法，或者遇到任何问题，请随时在 [GitHub](https://github.com/NGLSG/UniAPI/issues)
  上提交 issue。

感谢您的支持！让我们携手让 UniAPI 更上一层楼！

---

## 📋 目录

- [语言选择](#language-selection)
- [特性](#features)
- [待办事项](#todo)
- [本地模型支持](#local-model-support)
- [配置文件编辑工具](#config-file-editor)
- [安装方法](#installation-methods)
- [自定义配置规则说明](#custom-configuration-rule-explanation)
- [示例配置](#example-configurations)
- [许可协议](#license)

---

<a id="language-selection"></a>

## 📖 语言选择

- [日本語版](ReadME_JP.md)
- [English Version](ReadME_EN.md)

---

<a id="features"></a>

## ✨ 特性

- **解锁快速部署**：默认无需身份验证，支持动态加载提供者。
- **高性能推理**：支持 GPU 加速（CUDA 和 Vulkan），并兼容 CPU 推理。
- **本地模型支持**：直接推理本地量化 GGML 模型，无需额外配置。
- **跨平台兼容**：支持 Windows、Linux 和 macOS。
- **灵活配置**：通过 `Config.yaml` 文件灵活配置 API。
- **高度扩展性**：使用规则适配几乎任何接口。

---

<a id="todo"></a>

## 🔨 待办事项

- [ ] 鉴权系统
- [ ] 用户系统
- [ ] 计费系统

---

<a id="local-model-support"></a>

## 📦 本地模型支持

UniAPI 支持以下推理方式：

- **CUDA**：使用 NVIDIA GPU 进行加速推理。
- **Vulkan**：跨平台的图形和计算 API，适用于非 NVIDIA GPU。
- **CPU**：在没有 GPU 的环境中直接使用 CPU 进行推理。

---

<a id="config-file-editor"></a>

## 🛠️ 配置文件编辑工具

如果你不想使用文本编辑器手动修改配置文件，可以使用配套的 [ChatBot GUI 配置工具](https://github.com/NGLSG/ChatBot)
。通过该工具编辑配置后，稍作修改即可直接用于本项目。

---

<a id="installation-methods"></a>

## 🚀 安装方法

### 方法一：使用 Docker 镜像（最简单）

```bash
# 拉取预构建的 Docker 镜像
docker pull ryoshi541/uniapi:latest

# 使用默认配置运行容器
docker run -d -p 8080:8080 ryoshi541/uniapi:latest

# 使用自定义配置运行
docker run -d -p 8080:8080 \
  -v /path/to/Config.yaml:/app/Config.yaml \
  ryoshi541/uniapi:latest
```

### 方法二：构建自己的 Docker 镜像

```bash
# 克隆仓库
git clone --recursive https://github.com/NGLSG/UniAPI.git     
cd UniAPI

# 构建 Docker 镜像
docker build -t uniapi:latest .

# 运行容器
docker run -it --rm -p 8080:8080 uniapi:latest
```

### 方法三：本地编译安装

#### 1. 克隆仓库及子模块

```bash
git clone --recursive https://github.com/NGLSG/UniAPI.git     
cd UniAPI
# 如果子模块未拉取：
git submodule update --init --recursive
```

#### 2. 安装依赖项

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
```

#### 3. 编译项目

```bash
mkdir build && cd build
cmake ..
make -j4
```

#### 4. 运行服务器

```bash
./bin/UniAPI 8080  # 端口可以更改
```

---

<a id="custom-configuration-rule-explanation"></a>

## 📚 自定义配置规则说明

UniAPI 使用基于规则的配置来适配不同的 API 接口。以下是配置参数的详细说明：

| 参数                  | 说明                      |
|---------------------|-------------------------|
| `enable`            | 是否启用此提供者                |
| `supportSystemRole` | 是否支持系统角色                |
| `name`              | 提供者名称（用于标识）             |
| `model`             | 默认模型名称                  |
| `apiPath`           | API 端点 URL（可包含变量占位符）    |
| `apiKeyRole`        | API 密钥配置                |
| `headers`           | 自定义 HTTP 头              |
| `roles`             | 角色映射（OpenAI 格式与提供者格式之间） |
| `extraMust`         | 额外的必需参数                 |
| `promptRole`        | 提示结构映射配置                |
| `params`            | 要发送的附加参数                |
| `responseRole`      | 响应提取配置                  |
| `supportModels`     | 支持的模型列表                 |

---

<a id="example-configurations"></a>

## 📋 示例配置

### Gemini API 配置示例

```yaml
- enable: true
  supportSystemRole: false
  name: mGemini
  model: gemini-2.5-flash-preview-04-17
  apiPath: https://generativelanguage.googleapis.com/v1beta/models/${MODEL}:streamGenerateContent?key=${API_KEY}
  apiKeyRole:
    key: ~
    role: URL
    header: "Authorization: Bearer "
  headers: ~
  vars: ~
  roles:
    system: system
    user: user
    assistant: model
  extraMust:
    - suffix: topK
      path: generationConfig
      content: ${TOPK}
      isStr: false
    - suffix: topP
      path: generationConfig
      content: ${TOPP}
      isStr: false
    - suffix: temperature
      path: generationConfig
      content: ${TEMP}
      isStr: false
    - suffix: presencePenalty
      path: generationConfig
      content: ${PRES}
      isStr: false
    - suffix: frequencyPenalty
      path: generationConfig
      content: ${FREQ}
      isStr: false
  promptRole:
    role:
      suffix: contents
      path: role
      content: content
      isStr: false
    prompt:
      suffix: contents
      path: parts/0/text
      content: content
      isStr: false
  supportModels:
    - gemini-2.5-flash-preview-04-17
  params: [ ]
  responseRole:
    suffix: ""
    content: candidates/content/parts/text
    callback: RESPONSE
    stopFlag: ""
  author: Ryoshi
  version: 1.0
  description: Gemini 自定义实现
```

---

<a id="license"></a>

## 📜 许可协议

MIT - 免费用于商业和个人用途。详见 [LICENSE](LICENSE) 文件。