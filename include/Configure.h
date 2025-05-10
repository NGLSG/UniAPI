#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <yaml-cpp/yaml.h>
#include <string>
#include <iostream>

struct LLamaCreateInfo
{
    std::string model = "";
    int contextSize = 32000; //32k
    int maxTokens = 4096;
};

struct ClaudeAPICreateInfo
{
    bool enable = false;
    std::string apiKey;
    std::string model = "claude-3.5";
    std::string apiVersion = "2023-06-01";
    std::string _endPoint = "https://api.anthropic.com/v1/complete";
    std::vector<std::string> supportedModels;
};

struct OpenAIBotCreateInfo
{
    bool enable = true;
    bool useWebProxy = false;
    std::string api_key = "";
    std::string model = "gpt-4o";
    std::string proxy = "";
    std::string _endPoint = "";
    std::vector<std::string> supportedModels;
};

struct GPTLikeCreateInfo
{
    bool enable;
    bool useLocalModel = false;
    std::string api_key;
    std::string model = ""; //Must be set
    std::string apiHost = "";
    std::string apiPath = "";
    LLamaCreateInfo llamaData;
    std::vector<std::string> supportedModels;

    GPTLikeCreateInfo()
    {
        enable = false;
    }


    GPTLikeCreateInfo& operator=(const GPTLikeCreateInfo& data)
    {
        this->enable = data.enable;
        this->api_key = data.api_key;
        this->model = data.model;
        this->apiHost = data.apiHost;
        this->apiPath = data.apiPath;
        this->llamaData = data.llamaData;
        this->useLocalModel = data.useLocalModel;

        return *this;
    }
};

struct ClaudeBotCreateInfo
{
    bool enable = false;
    std::string channelID;
    std::string slackToken;
    std::string userName;
    std::string cookies;
    std::vector<std::string> supportedModels;
};

struct GeminiBotCreateInfo
{
    bool enable = false;
    std::string _apiKey;
    std::string _endPoint;
    std::string model = "gemini-2.0-flash";
    std::vector<std::string> supportedModels;
};


struct ResponseRole
{
    std::string suffix = "";
    std::string content = "";
    std::string callback = "";
    std::string stopFlag = "[DONE]";
};

struct APIKeyRole
{
    std::string key = "";
    std::string role = "HEADERS";
    std::string header = "Authorization: Bearer ";
};

struct ParamsRole
{
    std::string suffix = "messages"; //json的key
    std::string path = "content"; //json的路径
    std::string content = "content"; //
    bool isStr = false;
};

struct PromptRole
{
    ParamsRole role;
    ParamsRole prompt;
};

struct CustomRule
{
    bool enable = false;
    bool supportSystemRole = false;
    std::string author = "Ryoshi";
    std::string version = "1.0";
    std::string description = "自定义规则";
    std::string name = "";
    std::string model = "";
    std::string apiPath =
        "https://generativelanguage.googleapis.com/v1beta/models/${MODEL}:streamGenerateContent?key=${API_KEY}";

    APIKeyRole apiKeyRole;
    //SYSTEM,USER,ASSISTANT
    PromptRole promptRole;
    std::vector<ParamsRole> params;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> roles{{"system", ""}, {"user", ""}, {"assistant", ""}};
    ResponseRole responseRole{"data: ", "choices/delta/content", "RESPONSE", "[DONE"};
    std::vector<std::string> supportedModels;
};

struct Configure
{
    OpenAIBotCreateInfo openAi;
    ClaudeBotCreateInfo claude;
    GeminiBotCreateInfo gemini;
    GPTLikeCreateInfo grok;
    GPTLikeCreateInfo mistral;
    GPTLikeCreateInfo qianwen;
    GPTLikeCreateInfo sparkdesk;
    GPTLikeCreateInfo chatglm;
    GPTLikeCreateInfo hunyuan;
    GPTLikeCreateInfo baichuan;
    GPTLikeCreateInfo huoshan;
    ClaudeAPICreateInfo claudeAPI;
    std::unordered_map<std::string, GPTLikeCreateInfo> customGPTs;
    std::vector<CustomRule> customRules;
};

namespace YAML
{
    // ResponseRole 结构体的转换模板
    template <>
    struct convert<ResponseRole>
    {
        // 从 YAML 节点转换为 ResponseRole 结构体
        static bool decode(const Node& node, ResponseRole& rhs)
        {
            // 检查节点是否为映射类型
            if (!node.IsMap())
            {
                return false;
            }

            // 解析各个字段，如果字段不存在则保留默认值
            if (node["suffix"])
                rhs.suffix = node["suffix"].as<std::string>();
            if (node["content"])
                rhs.content = node["content"].as<std::string>();
            if (node["callback"])
                rhs.callback = node["callback"].as<std::string>();
            if (node["stopFlag"])
                rhs.stopFlag = node["stopFlag"].as<std::string>();

            return true;
        }

        // 从 ResponseRole 结构体转换为 YAML 节点
        static Node encode(const ResponseRole& rhs)
        {
            Node node;
            // 将结构体各个字段写入 YAML 节点
            node["suffix"] = rhs.suffix;
            node["content"] = rhs.content;
            node["callback"] = rhs.callback;
            node["stopFlag"] = rhs.stopFlag;
            return node;
        }
    };

    // APIKeyRole 结构体的转换模板
    template <>
    struct convert<APIKeyRole>
    {
        static bool decode(const Node& node, APIKeyRole& rhs)
        {
            // 检查节点是否为映射类型
            if (!node.IsMap())
            {
                return false;
            }

            // 解析各个字段
            if (node["key"])
                rhs.key = node["key"].as<std::string>();
            if (node["role"])
                rhs.role = node["role"].as<std::string>();
            if (node["header"])
                rhs.header = node["header"].as<std::string>();

            return true;
        }

        static Node encode(const APIKeyRole& rhs)
        {
            Node node;
            node["key"] = rhs.key;
            node["role"] = rhs.role;
            node["header"] = rhs.header;
            return node;
        }
    };

    // ParamsRole 结构体的转换模板
    template <>
    struct convert<ParamsRole>
    {
        static bool decode(const Node& node, ParamsRole& rhs)
        {
            // 检查节点是否为映射类型
            if (!node.IsMap())
            {
                return false;
            }

            // 解析各个字段
            if (node["suffix"])
                rhs.suffix = node["suffix"].as<std::string>();
            if (node["path"])
                rhs.path = node["path"].as<std::string>();
            if (node["content"])
                rhs.content = node["content"].as<std::string>();
            if (node["isStr"])
                rhs.isStr = node["isStr"].as<bool>();

            return true;
        }

        static Node encode(const ParamsRole& rhs)
        {
            Node node;
            node["suffix"] = rhs.suffix;
            node["path"] = rhs.path;
            node["content"] = rhs.content;
            node["isStr"] = rhs.isStr;
            return node;
        }
    };

    // PromptRole 结构体的转换模板
    template <>
    struct convert<PromptRole>
    {
        static bool decode(const Node& node, PromptRole& rhs)
        {
            // 检查节点是否为映射类型
            if (!node.IsMap())
            {
                return false;
            }

            // 解析各个字段
            if (node["role"])
                rhs.role = node["role"].as<ParamsRole>();
            if (node["prompt"])
                rhs.prompt = node["prompt"].as<ParamsRole>();

            return true;
        }

        static Node encode(const PromptRole& rhs)
        {
            Node node;
            node["role"] = rhs.role;
            node["prompt"] = rhs.prompt;
            return node;
        }
    };

    // CustomRule 结构体的转换模板
    template <>
    struct convert<CustomRule>
    {
        static bool decode(const Node& node, CustomRule& rhs)
        {
            // 检查节点是否为映射类型
            if (!node.IsMap())
            {
                return false;
            }

            // 解析各个字段
            if (node["enable"])
                rhs.enable = node["enable"].as<bool>();
            if (node["supportSystemRole"])
                rhs.supportSystemRole = node["supportSystemRole"].as<bool>();
            if (node["name"])
                rhs.name = node["name"].as<std::string>();
            if (node["model"])
                rhs.model = node["model"].as<std::string>();
            if (node["apiPath"])
                rhs.apiPath = node["apiPath"].as<std::string>();

            // 解析嵌套结构
            if (node["apiKeyRole"])
                rhs.apiKeyRole = node["apiKeyRole"].as<APIKeyRole>();

            // 解析 headers 映射
            if (node["headers"] && node["headers"].IsMap())
            {
                for (const auto& header : node["headers"])
                {
                    rhs.headers[header.first.as<std::string>()] = header.second.as<std::string>();
                }
            }

            // 解析 roles 映射
            if (node["roles"] && node["roles"].IsMap())
            {
                for (const auto& role : node["roles"])
                {
                    rhs.roles[role.first.as<std::string>()] = role.second.as<std::string>();
                }
            }
            if (node["supportModels"] && node["supportModels"].IsSequence())
            {
                rhs.supportedModels.clear();
                for (const auto& model : node["supportModels"])
                {
                    rhs.supportedModels.push_back(model.as<std::string>());
                }
            }

            // 解析 promptRole
            if (node["promptRole"])
                rhs.promptRole = node["promptRole"].as<PromptRole>();

            // 解析 params 数组
            if (node["params"] && node["params"].IsSequence())
            {
                rhs.params.clear();
                for (const auto& param : node["params"])
                {
                    rhs.params.push_back(param.as<ParamsRole>());
                }
            }


            // 解析 responseRole
            if (node["responseRole"])
                rhs.responseRole = node["responseRole"].as<ResponseRole>();
            if (node["author"])
                rhs.author = node["author"].as<std::string>();
            if (node["version"])
                rhs.version = node["version"].as<std::string>();
            if (node["description"])
                rhs.description = node["description"].as<std::string>();

            return true;
        }

        static Node encode(const CustomRule& rhs)
        {
            Node node;
            // 基本字段
            node["enable"] = rhs.enable;
            node["supportSystemRole"] = rhs.supportSystemRole;
            node["name"] = rhs.name;
            node["model"] = rhs.model;
            node["apiPath"] = rhs.apiPath;

            // 嵌套结构
            node["apiKeyRole"] = rhs.apiKeyRole;

            // headers 映射
            Node headersNode;
            for (const auto& header : rhs.headers)
            {
                headersNode[header.first] = header.second;
            }
            node["headers"] = headersNode;

            // roles 映射
            Node rolesNode;
            for (const auto& role : rhs.roles)
            {
                rolesNode[role.first] = role.second;
            }
            node["roles"] = rolesNode;

            // promptRole
            node["promptRole"] = rhs.promptRole;
            node["supportModels"] = rhs.supportedModels;

            // params 数组
            Node paramsNode(NodeType::Sequence);
            for (const auto& param : rhs.params)
            {
                paramsNode.push_back(param);
            }
            node["params"] = paramsNode;

            // responseRole
            node["responseRole"] = rhs.responseRole;
            node["author"] = rhs.author;
            node["version"] = rhs.version;
            node["description"] = rhs.description;

            return node;
        }
    };

    template <>
    struct convert<ClaudeAPICreateInfo>
    {
        static Node encode(const ClaudeAPICreateInfo& data)
        {
            Node node;
            node["enable"] = data.enable;
            node["apiKey"] = data.apiKey;
            node["model"] = data.model;
            node["apiVersion"] = data.apiVersion;
            node["endPoint"] = data._endPoint;
            node["supportModels"] = data.supportedModels;
            return node;
        }

        static bool decode(const Node& node, ClaudeAPICreateInfo& data)
        {
            data.enable = node["enable"].as<bool>();
            data.apiKey = node["apiKey"].as<std::string>();
            data.model = node["model"].as<std::string>();
            data.apiVersion = node["apiVersion"].as<std::string>();
            data._endPoint = node["endPoint"].as<std::string>();
            if (node["supportModels"] && node["supportModels"].IsSequence())
            {
                data.supportedModels.clear();
                for (const auto& model : node["supportModels"])
                {
                    data.supportedModels.push_back(model.as<std::string>());
                }
            }
            else
            {
                data.supportedModels = {"claude-3.5", "claude-3", "claude-2"};
            }
            return true;
        }
    };

    template <>
    struct convert<LLamaCreateInfo>
    {
        static Node encode(const LLamaCreateInfo& data)
        {
            Node node;
            node["model"] = data.model;
            node["contextSize"] = data.contextSize;
            node["maxTokens"] = data.maxTokens;
            return node;
        }

        static bool decode(const Node& node, LLamaCreateInfo& data)
        {
            data.model = node["model"].as<std::string>();
            data.contextSize = node["contextSize"].as<int>();
            data.maxTokens = node["maxTokens"].as<int>();
            return true;
        }
    };

    template <>
    struct convert<GPTLikeCreateInfo>
    {
        static Node encode(const GPTLikeCreateInfo& data)
        {
            Node node;
            node["enable"] = data.enable;
            node["api_key"] = data.api_key;
            node["model"] = data.model;
            node["apiHost"] = data.apiHost;
            node["apiPath"] = data.apiPath;
            node["useLocalModel"] = data.useLocalModel;
            node["llamaData"] = data.llamaData;
            node["supportModels"] = data.supportedModels;
            return node;
        }

        static bool decode(const Node& node, GPTLikeCreateInfo& data)
        {
            data.enable = node["enable"].as<bool>();
            data.api_key = node["api_key"].as<std::string>();
            data.model = node["model"].as<std::string>();
            if (node["apiHost"])
                data.apiHost = node["apiHost"].as<std::string>();
            if (node["apiPath"])
                data.apiPath = node["apiPath"].as<std::string>();
            if (node["useLocalModel"])
                data.useLocalModel = node["useLocalModel"].as<bool>();
            if (node["llamaData"])
                data.llamaData = node["llamaData"].as<LLamaCreateInfo>();
            if (node["supportModels"] && node["supportModels"].IsSequence())
            {
                data.supportedModels.clear();
                for (const auto& model : node["supportModels"])
                {
                    data.supportedModels.push_back(model.as<std::string>());
                }
            }
            else
            {
                data.supportedModels = {"grok-1.0", "mistral-7b", "mistral-7b-chat", "mistral-7b-instruct-v1"};
            }
            return true;
        }
    };

    template <>
    struct convert<GeminiBotCreateInfo>
    {
        static Node encode(const GeminiBotCreateInfo& data)
        {
            Node node;
            node["enable"] = data.enable;
            node["api_Key"] = data._apiKey;
            node["endPoint"] = data._endPoint;
            node["model"] = data.model;
            node["supportModels"] = data.supportedModels;
            return node;
        }

        static bool decode(const Node& node, GeminiBotCreateInfo& data)
        {
            data._apiKey = node["api_Key"].as<std::string>();
            data.enable = node["enable"].as<bool>();
            data._endPoint = node["endPoint"].as<std::string>();
            if (node["model"])
            {
                data.model = node["model"].as<std::string>();
            }
            else
            {
                data.model = "gemini-2.0-flash";
            }
            if (node["supportModels"] && node["supportModels"].IsSequence())
            {
                data.supportedModels.clear();
                for (const auto& model : node["supportModels"])
                {
                    data.supportedModels.push_back(model.as<std::string>());
                }
            }
            else
            {
                data.supportedModels = {"gemini-2.0-flash", "gemini-1.5"};
            }
            return true;
        }
    };

    template <>
    struct convert<ClaudeBotCreateInfo>
    {
        static Node encode(const ClaudeBotCreateInfo& data)
        {
            Node node;
            node["enable"] = data.enable;
            node["channelID"] = data.channelID;
            node["userName"] = data.userName;
            node["cookies"] = data.cookies;
            node["slackToken"] = data.slackToken;
            node["supportModels"] = data.supportedModels;
            return node;
        }

        static bool decode(const Node& node, ClaudeBotCreateInfo& data)
        {
            if (!node["channelID"])
            {
                return false;
            }
            data.cookies = node["cookies"].as<std::string>();
            data.userName = node["userName"].as<std::string>();
            data.enable = node["enable"].as<bool>();
            data.channelID = node["channelID"].as<std::string>();
            data.slackToken = node["slackToken"].as<std::string>();
            if (node["supportModels"] && node["supportModels"].IsSequence())
            {
                data.supportedModels.clear();
                for (const auto& model : node["supportModels"])
                {
                    data.supportedModels.push_back(model.as<std::string>());
                }
            }
            else
            {
                data.supportedModels = {"claude-3.5", "claude-3", "claude-2"};
            }
            return true;
        }
    };

    template <>
    struct convert<OpenAIBotCreateInfo>
    {
        static Node encode(const OpenAIBotCreateInfo& data)
        {
            Node node;
            node["enable"] = data.enable;
            node["api_key"] = data.api_key;
            node["model"] = data.model;
            node["proxy"] = data.proxy;
            node["useWebProxy"] = data.useWebProxy;
            node["endPoint"] = data._endPoint;
            node["supportModels"] = data.supportedModels;

            return node;
        }

        static bool decode(const Node& node, OpenAIBotCreateInfo& data)
        {
            data.enable = node["enable"].as<bool>();
            data.api_key = node["api_key"].as<std::string>();
            data.useWebProxy = node["useWebProxy"].as<bool>();
            if (node["model"])
            {
                data.model = node["model"].as<std::string>();
            }
            data.proxy = node["proxy"].as<std::string>();
            data._endPoint = node["endPoint"].as<std::string>();
            if (node["supportModels"] && node["supportModels"].IsSequence())
            {
                data.supportedModels.clear();
                for (const auto& model : node["supportModels"])
                {
                    data.supportedModels.push_back(model.as<std::string>());
                }
            }
            else
            {
                data.supportedModels = {"gpt-4o", "gpt-4", "gpt-3.5-turbo"};
            }
            return true;
        }
    };

    template <>
    struct convert<Configure>
    {
        static Node encode(const Configure& config)
        {
            Node node;
            node["openAi"] = config.openAi;
            node["claude"] = config.claude;
            node["gemini"] = config.gemini;
            node["grok"] = config.grok;
            node["mistral"] = config.mistral;
            node["qwen"] = config.qianwen;
            node["chatglm"] = config.chatglm;
            node["hunyuan"] = config.hunyuan;
            node["baichuan"] = config.baichuan;
            node["sparkdesk"] = config.sparkdesk;
            node["huoshan"] = config.huoshan;
            node["claudeAPI"] = config.claudeAPI;
            node["customGPTs"] = config.customGPTs;
            node["customRules"] = config.customRules;
            return node;
        }

        static bool decode(const Node& node, Configure& config)
        {
            if (node["claudeAPI"])
            {
                config.claudeAPI = node["claudeAPI"].as<ClaudeAPICreateInfo>();
            }
            if (node["mistral"])
            {
                config.mistral = node["mistral"].as<GPTLikeCreateInfo>();
            }
            if (node["qwen"])
            {
                config.qianwen = node["qwen"].as<GPTLikeCreateInfo>();
            }
            if (node["sparkdesk"])
            {
                config.sparkdesk = node["sparkdesk"].as<GPTLikeCreateInfo>();
            }
            if (node["chatglm"])
            {
                config.chatglm = node["chatglm"].as<GPTLikeCreateInfo>();
            }
            if (node["hunyuan"])
            {
                config.hunyuan = node["hunyuan"].as<GPTLikeCreateInfo>();
            }
            if (node["baichuan"])
            {
                config.baichuan = node["baichuan"].as<GPTLikeCreateInfo>();
            }
            if (node["huoshan"])
            {
                config.huoshan = node["huoshan"].as<GPTLikeCreateInfo>();
            }
            if (node["openAi"])
                config.openAi = node["openAi"].as<OpenAIBotCreateInfo>();
            if (node["gemini"])
                config.gemini = node["gemini"].as<GeminiBotCreateInfo>();
            if (node["claude"])
                config.claude = node["claude"].as<ClaudeBotCreateInfo>();
            if (node["grok"])
                config.grok = node["grok"].as<GPTLikeCreateInfo>();
            if (node["customGPTs"])
                config.customGPTs = node["customGPTs"].as<std::unordered_map<std::string, GPTLikeCreateInfo>>();
            if (node["customRules"])
                config.customRules = node["customRules"].as<std::vector<CustomRule>>();
            return true;
        }
    };
}

#endif
