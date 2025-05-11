<div align="center">
  <img src="icon.svg" alt="UniAPI Icon" width="200" height="200"> <!-- 插入图标 -->
</div>

<div align="center">
  <h3>万能な LLM API 統合ツールキット</h3>
  <p><i>"一つの標準で全部を呼び出す"</i></p>
  <a href="https://opensource.org/licenses/MIT"><img src="https://img.shields.io/badge/License-MIT-yellow.svg" alt="License: MIT"></a>
  <a href="https://en.cppreference.com/w/cpp/compiler_support"><img src="https://img.shields.io/badge/C++-20%2B-00599C?logo=c%2B%2B" alt="C++20"></a>
  <a href="https://developer.nvidia.com/cuda-toolkit"><img src="https://img.shields.io/badge/CUDA-Optional-76B900?logo=nvidia" alt="CUDA"></a>
  <a href="https://hub.docker.com/r/ryoshi541/uniapi"><img src="https://img.shields.io/badge/Docker-Available-2496ED?logo=docker" alt="Docker"></a>
</div>

---
## Star History

[![Star History Chart](https://api.star-history.com/svg?repos=NGLSG/UniAPI&type=Date)](https://www.star-history.com/#NGLSG/UniAPI&Date)

<a id="how-to-support-this-project"></a>
## 🌟 このプロジェクトをサポートする方法

私たちは UniAPI の継続的な改善と新機能の追加に努めています。皆様のサポートはこのプロジェクトの継続的な発展にとって非常に重要です。以下のような方法でご支援いただけます：

- **スターをつけてください**  
  UniAPI が役立つと思ったら、[GitHub](https://github.com/NGLSG/UniAPI) でスターをつけてください。これにより、私たちの可視性が高まり、より多くのコントリビューターを引き付けることができます。

- **プロジェクトに貢献する**  
  コミュニティからの貢献を歓迎します！バグの修正、新機能の追加、またはドキュメントの改善など、どんな貢献も貴重です。始めは簡単です。リポジトリをフォークしてプルリクエストを送信するだけです。

- **このプロジェクトを共有する**  
  UniAPI を友達、同僚、またはソーシャルメディアで共有してください。このプロジェクトを知っている人が多いほど良いです！

- **フィードバックを提供する**  
  あなたのフィードバックは私たちにとって重要です。提案、アイデア、または問題が発生した場合は、遠慮せずに [GitHub](https://github.com/NGLSG/UniAPI/issues) で issue を開いてください。

皆様のサポートをありがとうございます！皆様と一緒に UniAPI をさらに向上させましょう！

---

## 📋 目次

- [言語選択](#language-selection)
- [特徴](#features)
- [TODO](#todo)
- [ローカルモデルのサポート](#local-model-support)
- [設定ファイル編集ツール](#config-file-editor)
- [インストール方法](#installation-methods)
- [カスタム設定ルールの説明](#custom-configuration-rule-explanation)
- [設定例](#example-configurations)
- [ライセンス](#license)

---

<a id="language-selection"></a>

## 📖 言語選択

- [中文版](ReadME.md)
- [English Version](ReadME_EN.md)

---

<a id="features"></a>

## ✨ 特徴

- **認証不要の高速デプロイ**：デフォルトでは認証が不要で、プロバイダーの動的ロードが可能です。
- **高性能な推論**：CUDA と Vulkan をサポートし、CPU 推論も可能です。
- **ローカルモデルのサポート**：ローカルの量子化された GGML モデルを直接推論できます。
- **クロスプラットフォーム対応**：Windows、Linux、macOS に対応しています。
- **柔軟な設定**：`Config.yaml` ファイルを使用して API を柔軟に設定できます。
- **高い拡張性**：几乎所有のインターフェースをルールで適応可能です。

---

<a id="todo"></a>

## 🔨 TODO

- [ ] 認証システム
- [ ] ユーザーシステム
- [ ] 料金システム

---

<a id="local-model-support"></a>

## 📦 ローカルモデルのサポート

UniAPI は以下の推論方式をサポートしています：

- **CUDA**：NVIDIA GPU を使用して高速推論を行います。
- **Vulkan**：NVIDIA 以外の GPU でも使用できるクロスプラットフォームのグラフィックスとコンピューティング API。
- **CPU**：GPU が使用できない環境では、CPU を使用して直接推論を行います。

---

<a id="config-file-editor"></a>

## 🛠️ 設定ファイル編集ツール

設定ファイルをテキストエディターで手動で編集したくない場合は、[ChatBot GUI 設定ツール](https://github.com/NGLSG/ChatBot)
を使用してください。このツールで設定を編集後、少し変更することで、このプロジェクトで直接使用できます。

---

<a id="installation-methods"></a>

## 🚀 インストール方法

### 方法 1：Docker イメージを使用（最も簡単）

```bash
# 事前構築された Docker イメージを取得
docker pull ryoshi541/uniapi:latest

# デフォルト設定でコンテナを実行
docker run -d -p 8080:8080 ryoshi541/uniapi:latest

# カスタム設定で実行
docker run -d -p 8080:8080 \
  -v /path/to/Config.yaml:/app/Config.yaml \
  ryoshi541/uniapi:latest
```

### 方法 2：独自の Docker イメージをビルド

```bash
# リポジトリをクローン
git clone --recursive https://github.com/NGLSG/UniAPI.git     
cd UniAPI

# Docker イメージをビルド
docker build -t uniapi:latest .

# コンテナを実行
docker run -it --rm -p 8080:8080 uniapi:latest
```

### 方法 3：ローカルでのコンパイル

#### 1. リポジトリとサブモジュールのクローン

```bash
git clone --recursive https://github.com/NGLSG/UniAPI.git     
cd UniAPI
# サブモジュールが取得されていない場合：
git submodule update --init --recursive
```

#### 2. 依存関係のインストール

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
```

#### 3. プロジェクトのコンパイル

```bash
mkdir build && cd build
cmake ..
make -j4
```

#### 4. サーバーの実行

```bash
./bin/UniAPI 8080  # ポートは変更可能
```

---

<a id="custom-configuration-rule-explanation"></a>

## 📚 カスタム設定ルールの説明

UniAPI は、ルールベースの設定を使用して異なる API インターフェースに適応します。以下は設定パラメータの詳細説明です：

| パラメータ               | 説明                             |
|---------------------|--------------------------------|
| `enable`            | このプロバイダーを有効にするかどうか             |
| `supportSystemRole` | システムロールをサポートするかどうか             |
| `name`              | プロバイダー名（識別用）                   |
| `model`             | デフォルトのモデル名                     |
| `apiPath`           | API エンドポイント URL（変数プレースホルダーを含む） |
| `apiKeyRole`        | API キーの設定                      |
| `headers`           | カスタム HTTP ヘッダー                 |
| `roles`             | ロールマッピング（OpenAI 形式とプロバイダー形式の間） |
| `extraMust`         | 追加の必須パラメータ                     |
| `promptRole`        | プロンプト構造のマッピング設定                |
| `params`            | 送信する追加パラメータ                    |
| `responseRole`      | レスポンス抽出設定                      |
| `supportModels`     | サポートされているモデルのリスト               |

---

<a id="example-configurations"></a>

## 📋 設定例

### Gemini API 設定例

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
  description: Gemini カスタム実装
```

---

<a id="license"></a>

## 📜 ライセンス

MIT - 商用および個人利用は無料です。詳細は [LICENSE](LICENSE) をご確認ください。