#include <drogon/drogon.h>
#include <string>
#include "Impls/Bots.h"
#define NOMINMAX
#ifdef min
#undef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifdef max
#undef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif


long long GetTS()
{
    // 使用C++11的chrono库获取当前时间点
    auto 当前时间点 = std::chrono::system_clock::now();

    // 转换为自1970年1月1日以来的毫秒数
    auto 时间戳_毫秒 = std::chrono::duration_cast<std::chrono::milliseconds>(
        当前时间点.time_since_epoch()
    ).count();

    return 时间戳_毫秒;
}


struct SseCharStreamState
{
    std::string pendingSendData;
    bool botQueryFinished = false;
    bool doneMarkerSent = false;

    SseCharStreamState() :
        botQueryFinished(false),
        doneMarkerSent(false)
    {
    }
};

// 辅助函数：获取统一配置的 JSON 写入器
Json::StreamWriterBuilder getUTF8JsonWriter()
{
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    builder["emitUTF8"] = true; // 禁止 Unicode 转义
    builder["encoding"] = "UTF-8"; // 明确指定编码
    return builder;
}

int main(int argc, char* argv[])
{
    Logger::Init();
    if (!UDirectory::CreateDirIfNotExists("Conversations"))
    {
        LogError("创建对话目录失败");
        return -1;
    }
    if (!UFile::Exists("Config.yaml"))
    {
        LogError("配置文件不存在，请检查路径: Config.yaml");
        LogError("请配置文件后重试");
        Configure config;
        SaveYaml("Config.yaml", toYaml(config));
        return -1;
    }
    Configure config;
    try
    {
        config = LoadYaml<Configure>("Config.yaml").value();
    }
    catch (const std::exception& e)
    {
        LogError("加载配置文件失败: {}", e.what());
        return -1;
    }
    std::unordered_map<std::string, std::shared_ptr<ChatBot>> Bots;

    // 初始化日志系统
    for (auto& it : config.openAi.supportedModels)
    {
        config.openAi.model = it;
        Bots["OpenAI/" + it] = std::make_shared<ChatGPT>(config.openAi);
    }
    for (auto& it : config.claudeAPI.supportedModels)
    {
        config.claudeAPI.model = it;
        Bots["Claude/" + it] = std::make_shared<Claude>(config.claudeAPI);
    }
    for (auto& it : config.gemini.supportedModels)
    {
        config.gemini.model = it;
        Bots["Gemini/" + it] = std::make_shared<Gemini>(config.gemini, "");
    }
    for (auto& it : config.mistral.supportedModels)
    {
        config.mistral.model = it;
        Bots["Mistral/" + it] = std::make_shared<Mistral>(config.mistral);
    }
    for (auto& it : config.qianwen.supportedModels)
    {
        config.qianwen.model = it;
        Bots["Qianwen/" + it] = std::make_shared<TongyiQianwen>(config.qianwen);
    }
    for (auto& it : config.sparkdesk.supportedModels)
    {
        config.sparkdesk.model = it;
        Bots["SparkDesk/" + it] = std::make_shared<SparkDesk>(config.sparkdesk);
    }
    for (auto& it : config.chatglm.supportedModels)
    {
        config.chatglm.model = it;
        Bots["ChatGLM/" + it] = std::make_shared<ChatGLM>(config.chatglm);
    }
    for (auto& it : config.hunyuan.supportedModels)
    {
        config.hunyuan.model = it;
        Bots["Hunyuan/" + it] = std::make_shared<HunyuanAI>(config.hunyuan);
    }
    for (auto& it : config.baichuan.supportedModels)
    {
        config.baichuan.model = it;
        Bots["Baichuan/" + it] = std::make_shared<BaichuanAI>(config.baichuan);
    }
    for (auto& it : config.huoshan.supportedModels)
    {
        config.huoshan.model = it;
        Bots["Huoshan/" + it] = std::make_shared<HuoshanAI>(config.huoshan);
    }
    for (auto& it : config.customGPTs)
    {
        for (auto& it2 : it.second.supportedModels)
        {
            it.second.model = it2;
            Bots[it.first + "/" + it2] = std::make_shared<GPTLike>(it.second);
        }
    }
    for (auto& it : config.customRules)
    {
        for (auto& it2 : it.supportedModels)
        {
            it.model = it2;
            Bots[it.name + "/" + it2] = std::make_shared<CustomRule_Impl>(it);
        }
    }


    uint16_t port = 8080;
    if (argc > 1)
    {
        try
        {
            port = static_cast<uint16_t>(std::stoi(argv[1]));
            LogInfo("使用指定端口: {0}", port);
        }
        catch (const std::exception& e)
        {
            LogInfo("端口参数无效 '{0}', 使用默认端口: {1}. Error: {2}", argv[1], port, e.what());
        }
    }
    else
    {
        LogInfo("未指定端口，使用默认端口: {0}", port);
    }

    auto& app = drogon::app();

    app.registerHandler("/v1/models",
                        [Bots](const drogon::HttpRequestPtr& req,
                               std::function<void(const drogon::HttpResponsePtr&)>&& callback)
                        {
                            Json::Value response_json;
                            response_json["object"] = "list";
                            Json::Value data_array(Json::arrayValue);

                            for (const auto& bot_pair : Bots)
                            {
                                Json::Value model_data;
                                model_data["id"] = bot_pair.first;
                                model_data["object"] = "model";
                                model_data["owned_by"] = "artiverse";
                                model_data["created"] = Json::UInt64(std::time(nullptr));

                                Json::Value permissions_array(Json::arrayValue);
                                Json::Value perm_obj;
                                perm_obj["id"] = bot_pair.first;
                                perm_obj["object"] = "model_permission";
                                perm_obj["created"] = Json::UInt64(std::time(nullptr));
                                permissions_array.append(perm_obj);
                                model_data["permission"] = permissions_array;

                                data_array.append(model_data);
                            }
                            response_json["data"] = data_array;
                            auto resp = drogon::HttpResponse::newHttpJsonResponse(response_json);
                            callback(resp);
                        },
                        {drogon::Get});
    app.registerHandler("/v1/chat/completions",
                        [Bots](const drogon::HttpRequestPtr& req,
                               std::function<void(const drogon::HttpResponsePtr&)>&& callback)
                        {
                            Json::Value error_response_json;
                            auto json_body_ptr = req->getJsonObject();

                            if (!json_body_ptr)
                            {
                                error_response_json["error"]["message"] = "无效的JSON格式";
                                error_response_json["error"]["type"] = "invalid_request_error";
                                auto resp = drogon::HttpResponse::newHttpJsonResponse(error_response_json);
                                resp->setContentTypeCodeAndCustomString(drogon::CT_APPLICATION_JSON,
                                                                        "application/json; charset=utf-8");
                                resp->setStatusCode(drogon::k400BadRequest);
                                callback(resp);
                                return;
                            }
                            const Json::Value& 请求数据 = *json_body_ptr;
                            LogInfo("收到数据: {0}", 请求数据.toStyledString());

                            if (!请求数据.isMember("model") || !请求数据["model"].isString())
                            {
                                error_response_json["error"]["message"] = "缺少必要的model字段";
                                error_response_json["error"]["type"] = "invalid_request_error";
                                auto resp = drogon::HttpResponse::newHttpJsonResponse(error_response_json);
                                resp->setContentTypeCodeAndCustomString(drogon::CT_APPLICATION_JSON,
                                                                        "application/json; charset=utf-8");
                                resp->setStatusCode(drogon::k400BadRequest);
                                callback(resp);
                                return;
                            }
                            std::string 模型名称 = 请求数据["model"].asString();

                            bool 是否流式 = false;
                            if (请求数据.isMember("stream") && 请求数据["stream"].isBool())
                            {
                                是否流式 = 请求数据["stream"].asBool();
                            }

                            if (!请求数据.isMember("messages") || !请求数据["messages"].isArray() || 请求数据["messages"].empty())
                            {
                                error_response_json["error"]["message"] = "缺少必要的messages字段或格式不正确";
                                error_response_json["error"]["type"] = "invalid_request_error";
                                auto resp = drogon::HttpResponse::newHttpJsonResponse(error_response_json);
                                resp->setContentTypeCodeAndCustomString(drogon::CT_APPLICATION_JSON,
                                                                        "application/json; charset=utf-8");
                                resp->setStatusCode(drogon::k400BadRequest);
                                callback(resp);
                                return;
                            }

                            std::vector<std::pair<std::string, std::string>> 消息列表;
                            bool 需要添加系统消息 = true;
                            const Json::Value& messages_json_array = 请求数据["messages"];

                            if (messages_json_array.isValidIndex(0) &&
                                messages_json_array[Json::Value::ArrayIndex(0)].isMember("role") &&
                                messages_json_array[Json::Value::ArrayIndex(0)]["role"].asString() == "system")
                            {
                                需要添加系统消息 = false;
                                if (messages_json_array[Json::Value::ArrayIndex(0)].isMember("content") &&
                                    messages_json_array[Json::Value::ArrayIndex(0)]["content"].isString())
                                {
                                    消息列表.push_back({
                                        "system", messages_json_array[Json::Value::ArrayIndex(0)]["content"].asString()
                                    });
                                }
                                else
                                {
                                    消息列表.push_back({"system", "You're a helpful assistant."});
                                }
                            }

                            if (需要添加系统消息)
                            {
                                消息列表.push_back({"system", "You're a helpful assistant."});
                            }

                            std::string lastPrompt = "";
                            Json::Value::ArrayIndex startIndexInJson = 0;
                            if (!需要添加系统消息)
                            {
                                startIndexInJson = 1;
                            }

                            for (Json::Value::ArrayIndex i = startIndexInJson; i < messages_json_array.size(); ++i)
                            {
                                const Json::Value& 消息_json = messages_json_array[i];
                                if (!消息_json.isMember("role") || !消息_json.isMember("content") ||
                                    !消息_json["role"].isString() || !消息_json["content"].isString())
                                {
                                    error_response_json["error"]["message"] = "消息格式不正确，必须包含role和content字段";
                                    error_response_json["error"]["type"] = "invalid_request_error";
                                    auto resp = drogon::HttpResponse::newHttpJsonResponse(error_response_json);
                                    resp->setContentTypeCodeAndCustomString(drogon::CT_APPLICATION_JSON,
                                                                            "application/json; charset=utf-8");
                                    resp->setStatusCode(drogon::k400BadRequest);
                                    callback(resp);
                                    return;
                                }
                                std::string 角色 = 消息_json["role"].asString();
                                std::string 内容 = 消息_json["content"].asString();

                                if (i == messages_json_array.size() - 1)
                                {
                                    lastPrompt = 内容;
                                }
                                else
                                {
                                    消息列表.push_back({角色, 内容});
                                }
                            }
                            if (messages_json_array.size() == 1 && startIndexInJson == 0)
                            {
                                const Json::Value& 唯一消息 = messages_json_array[Json::Value::ArrayIndex(0)];
                                if (唯一消息.isMember("content") && 唯一消息["content"].isString())
                                {
                                    lastPrompt = 唯一消息["content"].asString();
                                }
                                else
                                {
                                    lastPrompt = "";
                                }
                            }

                            auto bot_iter = Bots.find(模型名称);
                            if (bot_iter == Bots.end())
                            {
                                error_response_json["error"]["message"] = "不支持的模型名称: " + 模型名称;
                                error_response_json["error"]["type"] = "invalid_request_error";
                                auto resp = drogon::HttpResponse::newHttpJsonResponse(error_response_json);
                                resp->setContentTypeCodeAndCustomString(drogon::CT_APPLICATION_JSON,
                                                                        "application/json; charset=utf-8");
                                resp->setStatusCode(drogon::k404NotFound);
                                callback(resp);
                                return;
                            }
                            auto bot = bot_iter->second;

                            bot->BuildHistory(消息列表);
                            long long timeStamp = GetTS();
                            float temp = 0.7;
                            float top_p = 0.9;
                            uint32_t top_k = 40;
                            float pres_pen = 0.0;
                            float freq_pen = 0.0;

                            if (请求数据.isMember("temperature") && !请求数据["temperature"].asString().empty())
                            {
                                temp = std::stof(请求数据["temperature"].asString());
                            }

                            if (请求数据.isMember("top_p") && !请求数据["top_p"].asString().empty())
                            {
                                top_p = std::stof(请求数据["top_p"].asString());
                            }

                            if (请求数据.isMember("top_k") && !请求数据["top_k"].asString().empty())
                            {
                                top_k = static_cast<uint32_t>(std::stoi(请求数据["top_k"].asString()));
                            }

                            if (请求数据.isMember("presence_penalty") && !请求数据["presence_penalty"].asString().empty())
                            {
                                pres_pen = std::stof(请求数据["presence_penalty"].asString());
                            }

                            if (请求数据.isMember("frequency_penalty") && !请求数据["frequency_penalty"].asString().empty())
                            {
                                freq_pen = std::stof(请求数据["frequency_penalty"].asString());
                            }
                            bot->SubmitAsync(lastPrompt, timeStamp, Role::User, "default", temp, top_p, top_k, pres_pen,
                                             freq_pen);

                            if (是否流式)
                            {
                                auto streamState = std::make_shared<SseCharStreamState>();
                                std::shared_ptr<std::string> 最终回复 = std::make_shared<std::string>();

                                auto streamCb_char_ptr =
                                    [bot, timeStamp, 模型名称, streamState, 最终回复](
                                    char* outputBuffer, std::size_t maxSize) -> std::size_t
                                {
                                    if (!streamState->pendingSendData.empty())
                                    {
                                        std::size_t bytesToCopy = min(
                                            streamState->pendingSendData.length(), maxSize);
                                        if (bytesToCopy > 0)
                                        {
                                            memcpy(outputBuffer, streamState->pendingSendData.data(), bytesToCopy);
                                            streamState->pendingSendData.erase(0, bytesToCopy);
                                        }
                                        return bytesToCopy;
                                    }

                                    if (streamState->doneMarkerSent)
                                    {
                                        return 0;
                                    }

                                    bool generatedNewDataThisCall = false;

                                    if (!streamState->botQueryFinished)
                                    {
                                        std::string 增量内容 = bot->GetResponse(timeStamp);
                                        *最终回复 += 增量内容;
                                        if (!增量内容.empty())
                                        {
                                            Json::Value 消息块;
                                            std::string 响应ID = "chatcmpl-" + std::to_string(timeStamp);
                                            Json::UInt64 创建时间 = static_cast<Json::UInt64>(timeStamp / 1000);
                                            消息块["id"] = 响应ID;
                                            消息块["object"] = "chat.completion.chunk";
                                            消息块["created"] = 创建时间;
                                            消息块["model"] = 模型名称;
                                            Json::Value choices_array(Json::arrayValue);
                                            Json::Value choice;
                                            Json::Value delta;

                                            delta["content"] = 增量内容;
                                            choice["delta"] = delta;
                                            choice["index"] = 0;
                                            choice["finish_reason"] = Json::nullValue;
                                            choices_array.append(choice);
                                            消息块["choices"] = choices_array;

                                            auto builder = getUTF8JsonWriter();
                                            std::string chunk_str = Json::writeString(builder, 消息块);
                                            streamState->pendingSendData.append("data: " + chunk_str + "\n\n");
                                            generatedNewDataThisCall = true;
                                        }

                                        if (bot->Finished(timeStamp))
                                        {
                                            streamState->botQueryFinished = true;
                                            Json::Value 完成消息;
                                            std::string 响应ID = "chatcmpl-" + std::to_string(timeStamp);
                                            Json::UInt64 创建时间 = static_cast<Json::UInt64>(timeStamp / 1000);
                                            完成消息["id"] = 响应ID;
                                            完成消息["object"] = "chat.completion.chunk";
                                            完成消息["created"] = 创建时间;
                                            完成消息["model"] = 模型名称;
                                            Json::Value choices_array(Json::arrayValue);
                                            Json::Value choice_val;
                                            Json::Value delta_val;
                                            if (!最终回复->empty())
                                                delta_val["content"] = "";
                                            else
                                            {
                                                delta_val["content"] = bot->GetLastRawResponse();
                                            }
                                            choice_val["delta"] = delta_val;
                                            choice_val["index"] = 0;
                                            choice_val["finish_reason"] = "stop";
                                            choices_array.append(choice_val);
                                            完成消息["choices"] = choices_array;

                                            auto builder = getUTF8JsonWriter();
                                            std::string final_chunk_str = Json::writeString(builder, 完成消息);
                                            streamState->pendingSendData.append("data: " + final_chunk_str + "\n\n");
                                            generatedNewDataThisCall = true;
                                            LogInfo("返回结果: {0}", *最终回复);
                                        }
                                    }

                                    if (streamState->botQueryFinished && !streamState->doneMarkerSent)
                                    {
                                        streamState->pendingSendData.append("data: [DONE]\n\n");
                                        streamState->doneMarkerSent = true;
                                        generatedNewDataThisCall = true;
                                    }

                                    if (!generatedNewDataThisCall && streamState->pendingSendData.empty() && !
                                        streamState->doneMarkerSent)
                                    {
                                        Json::Value heartbeat_chunk;
                                        long long current_ts_for_hb = GetTS();
                                        std::string heartbeat_id = "chatcmpl-hb-" + std::to_string(timeStamp) + "-" +
                                            std::to_string(current_ts_for_hb);
                                        Json::UInt64 heartbeat_created = static_cast<Json::UInt64>(current_ts_for_hb /
                                            1000);

                                        heartbeat_chunk["id"] = heartbeat_id;
                                        heartbeat_chunk["object"] = "chat.completion.chunk";
                                        heartbeat_chunk["created"] = heartbeat_created;
                                        heartbeat_chunk["model"] = 模型名称;
                                        Json::Value choices_array(Json::arrayValue);
                                        Json::Value choice;
                                        Json::Value delta;
                                        delta["content"] = "";
                                        choice["delta"] = delta;
                                        choice["index"] = 0;
                                        choice["finish_reason"] = Json::nullValue;
                                        choices_array.append(choice);
                                        heartbeat_chunk["choices"] = choices_array;

                                        auto builder = getUTF8JsonWriter();
                                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

                                        std::string chunk_str = Json::writeString(builder, heartbeat_chunk);
                                        streamState->pendingSendData.append("data: " + chunk_str + "\n\n");
                                    }

                                    if (!streamState->pendingSendData.empty())
                                    {
                                        std::size_t bytesToCopy = min(
                                            streamState->pendingSendData.length(), maxSize);
                                        if (bytesToCopy > 0)
                                        {
                                            memcpy(outputBuffer, streamState->pendingSendData.data(), bytesToCopy);
                                            streamState->pendingSendData.erase(0, bytesToCopy);
                                        }
                                        return bytesToCopy;
                                    }
                                    return 0;
                                };

                                auto resp = drogon::HttpResponse::newStreamResponse(
                                    streamCb_char_ptr,
                                    "",
                                    drogon::CT_CUSTOM,
                                    "text/event-stream; charset=utf-8",
                                    req
                                );

                                resp->setStatusCode(drogon::k200OK);
                                resp->addHeader("Cache-Control", "no-cache");
                                resp->addHeader("Connection", "keep-alive");
                                resp->addHeader("Access-Control-Allow-Origin", "*");

                                callback(resp);
                            }
                            else
                            {
                                auto captured_bot = bot;
                                auto captured_callback = callback;
                                std::string captured_lastPrompt = lastPrompt;
                                std::string captured_模型名称 = 模型名称;

                                std::thread(
                                    [captured_bot, timeStamp, captured_模型名称, captured_lastPrompt, captured_callback]()
                                    {
                                        std::string 最终回复 = "";
                                        while (!captured_bot->Finished(timeStamp))
                                        {
                                            std::string 增量内容 = captured_bot->GetResponse(timeStamp);
                                            最终回复 += 增量内容;
                                            std::this_thread::sleep_for(std::chrono::milliseconds(50));
                                        }
                                        std::string 最后增量内容 = captured_bot->GetResponse(timeStamp);
                                        if (!最后增量内容.empty())
                                        {
                                            最终回复 += 最后增量内容;
                                        }
                                        if (最终回复.empty())
                                        {
                                            最终回复 = captured_bot->GetLastRawResponse();
                                        }

                                        Json::Value response_json;
                                        std::string 响应ID = "chatcmpl-" + std::to_string(timeStamp);
                                        Json::UInt64 创建时间 = static_cast<Json::UInt64>(timeStamp / 1000);

                                        response_json["id"] = 响应ID;
                                        response_json["object"] = "chat.completion";
                                        response_json["created"] = 创建时间;
                                        response_json["model"] = captured_模型名称;

                                        Json::Value choices_array(Json::arrayValue);
                                        Json::Value choice_item;
                                        Json::Value message;
                                        message["role"] = "assistant";
                                        message["content"] = 最终回复;
                                        choice_item["message"] = message;
                                        choice_item["index"] = 0;
                                        choice_item["finish_reason"] = "stop";
                                        choices_array.append(choice_item);
                                        response_json["choices"] = choices_array;

                                        Json::Value usage;
                                        usage["prompt_tokens"] = static_cast<int>(captured_lastPrompt.length() / 4 +
                                            10);
                                        usage["completion_tokens"] = static_cast<int>(最终回复.length() / 4);
                                        usage["total_tokens"] = usage["prompt_tokens"].asInt() + usage[
                                            "completion_tokens"].asInt();
                                        response_json["usage"] = usage;

                                        auto builder = getUTF8JsonWriter();
                                        std::string json_str = Json::writeString(builder, response_json);
                                        auto resp = drogon::HttpResponse::newHttpResponse();
                                        resp->setBody(json_str);
                                        resp->setContentTypeCodeAndCustomString(drogon::CT_APPLICATION_JSON,
                                            "application/json; charset=utf-8");
                                        captured_callback(resp);
                                    }).detach();
                            }
                        },
                        {drogon::Post});

    app.registerHandler("/",
                        [](const drogon::HttpRequestPtr& req,
                           std::function<void(const drogon::HttpResponsePtr&)>&& callback)
                        {
                            auto resp = drogon::HttpResponse::newHttpResponse();
                            resp->setBody("Chat API Server is running with Drogon!");
                            resp->setContentTypeCode(drogon::CT_TEXT_PLAIN);
                            callback(resp);
                        },
                        {drogon::Get});


    LogInfo("Drogon server starting on http://localhost:{0}", port);
    app.setLogLevel(trantor::Logger::kWarn);
    app.addListener("0.0.0.0", port);
    app.run();

    return 0;
}
