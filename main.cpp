#include "crow.h"
#include <iostream>
#include <string>
#include "Impls/Bots.h"

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


    // 创建应用实例
    crow::SimpleApp app;

    // 解析端口参数
    uint16_t port = 8080; // 默认端口
    if (argc > 1)
    {
        try
        {
            // 尝试将第一个参数转换为端口号
            port = static_cast<uint16_t>(std::stoi(argv[1]));
            std::cout << "使用指定端口: " << port << std::endl;
        }
        catch (const std::exception& e)
        {
            // 如果转换失败，使用默认端口
            std::cout << "端口参数无效，使用默认端口: " << port << std::endl;
        }
    }
    else
    {
        std::cout << "未指定端口，使用默认端口: " << port << std::endl;
    }

    CROW_ROUTE(app, "/v1/chat/completions").methods("POST"_method)
    ([Bots](const crow::request& req)
    {
        // 准备响应对象
        crow::json::wvalue 响应;

        try
        {
            // 解析请求体中的JSON数据
            auto 请求数据 = crow::json::load(req.body);

            // 检查JSON是否有效
            if (!请求数据)
            {
                响应["error"] = "无效的JSON格式";
                return crow::response(400, 响应);
            }

            // 检查并提取model字段
            if (!请求数据.has("model"))
            {
                响应["error"] = "缺少必要的model字段";
                return crow::response(400, 响应);
            }
            std::string 模型名称 = 请求数据["model"].s();

            // 检查stream参数，默认为false
            bool 是否流式 = false;
            if (请求数据.has("stream"))
            {
                是否流式 = 请求数据["stream"].b();
            }

            // 检查消息数组是否存在
            if (!请求数据.has("messages"))
            {
                响应["error"] = "缺少必要的messages字段或格式不正确";
                return crow::response(400, 响应);
            }

            // 创建处理后的消息列表
            std::vector<std::pair<std::string, std::string>> 消息列表;

            // 检查是否需要添加system角色消息
            bool 需要添加系统消息 = true;

            // 检查第一条消息是否是system角色
            if (请求数据["messages"].size() > 0)
            {
                const auto& 第一条消息 = 请求数据["messages"][0];
                if (第一条消息.has("role") && 第一条消息["role"].s() == "system")
                {
                    需要添加系统消息 = false;

                    // 将系统消息添加到消息列表
                    if (第一条消息.has("content"))
                    {
                        消息列表.push_back({"system", 第一条消息["content"].s()});
                    }
                    else
                    {
                        // 如果系统消息没有内容，添加默认内容
                        消息列表.push_back({"system", "You're a helpful assistant."});
                    }
                }
            }

            // 如果需要添加系统消息（即第一条不是系统消息）
            if (需要添加系统消息)
            {
                // 在消息列表开头添加默认的系统消息
                消息列表.push_back({"system", "You're a helpful assistant."});
            }

            // 处理除最后一条外的所有用户消息
            std::string lastPrompt = "";
            size_t 起始索引 = 需要添加系统消息 ? 0 : 1; // 如果已有系统消息，从1开始；否则从0开始

            for (size_t i = 起始索引; i < 请求数据["messages"].size() - 1; i++)
            {
                const auto& 消息 = 请求数据["messages"][i];

                // 检查消息格式是否正确
                if (!消息.has("role") || !消息.has("content"))
                {
                    响应["error"] = "消息格式不正确，必须包含role和content字段";
                    return crow::response(400, 响应);
                }

                // 提取角色和内容
                std::string 角色 = 消息["role"].s();
                std::string 内容 = 消息["content"].s();

                // 添加到消息列表
                消息列表.push_back({角色, 内容});
            }

            // 将最后一个消息的内容保存到lastPrompt
            if (请求数据["messages"].size() > 0)
            {
                // 获取最后一个消息
                const auto& 最后消息 = 请求数据["messages"][请求数据["messages"].size() - 1];

                // 检查最后一条消息是否包含content字段
                if (最后消息.has("content"))
                {
                    // 将最后一个消息的内容保存到lastPrompt变量
                    lastPrompt = 最后消息["content"].s();
                }
                else
                {
                    // 如果没有content字段，将lastPrompt设为空
                    lastPrompt = "";
                }
            }

            // 查找模型
            auto bot = Bots.find(模型名称);
            if (bot == Bots.end())
            {
                响应["error"] = "不支持的模型名称";
                return crow::response(400, 响应);
            }

            // 构建历史记录
            bot->second->BuildHistory(消息列表);
            long long timeStamp = GetTS();
            bot->second->SubmitAsync(lastPrompt, timeStamp);
            //TODO 流式有bug暂时禁用
            /*// 根据是否流式响应进行不同处理
            if (是否流式)
            {
                // 创建流式响应
                auto 流式响应 = std::make_shared<crow::response>(200);
                // 设置流式响应头
                流式响应->set_header("Content-Type", "text/event-stream");
                流式响应->set_header("Cache-Control", "no-cache");
                流式响应->set_header("Connection", "keep-alive");
                流式响应->set_header("Transfer-Encoding", "chunked");

                // 创建响应ID和时间戳
                std::string 响应ID = "chatcmpl-" + std::to_string(timeStamp);
                uint64_t 创建时间 = static_cast<uint64_t>(timeStamp / 1000); // 转换为秒

                // 分离线程处理流式响应
                std::thread([
                        流式响应,
                        botN = bot->second,
                        timeStamp,
                        响应ID,
                        创建时间,
                        模型名称
                    ]() mutable
                    {
                        // 初始消息标记
                        bool 是首条消息 = true;

                        // 循环获取响应内容
                        while (!botN->Finished(timeStamp))
                        {
                            // 获取增量内容 (直接就是增量)
                            std::string 增量内容 = botN->GetResponse(timeStamp);

                            // 如果有内容，则发送
                            if (!增量内容.empty())
                            {
                                // 构建OpenAI风格的流式响应
                                crow::json::wvalue 消息块;

                                // 如果是首条消息，添加更多元数据
                                if (是首条消息)
                                {
                                    消息块["id"] = 响应ID;
                                    消息块["object"] = "chat.completion.chunk";
                                    消息块["created"] = 创建时间;
                                    消息块["model"] = 模型名称;
                                    是首条消息 = false;
                                }

                                // 构建choices部分
                                crow::json::wvalue 选择;
                                crow::json::wvalue 增量;
                                增量["content"] = 增量内容;
                                // 使用std::move将左值转为右值引用
                                选择["delta"] = std::move(增量);
                                选择["index"] = 0;
                                // 使用null而不是字符串"null"
                                选择["finish_reason"] = "null";

                                crow::json::wvalue 选择列表 = crow::json::wvalue::list();
                                // 使用std::move将左值转为右值引用
                                选择列表[0] = std::move(选择);
                                // 使用std::move将左值转为右值引用
                                消息块["choices"] = std::move(选择列表);

                                // 发送数据
                                流式响应->write("data: " + 消息块.dump() + "\n\n");
                            }

                            // 短暂休眠以避免过度消耗CPU资源
                            std::this_thread::sleep_for(std::chrono::milliseconds(50));
                        }

                        // 处理最后一个增量内容（在Finished返回true后可能仍有最后一个增量）
                        std::string 最后增量内容 = botN->GetResponse(timeStamp);
                        if (!最后增量内容.empty())
                        {
                            // 构建最后一个增量的响应
                            crow::json::wvalue 最后消息块;
                            最后消息块["id"] = 响应ID;
                            最后消息块["object"] = "chat.completion.chunk";
                            最后消息块["created"] = 创建时间;
                            最后消息块["model"] = 模型名称;

                            // 构建choices部分
                            crow::json::wvalue 选择;
                            crow::json::wvalue 增量;
                            增量["content"] = 最后增量内容;
                            选择["delta"] = std::move(增量);
                            选择["index"] = 0;
                            选择["finish_reason"] = "null";

                            crow::json::wvalue 选择列表 = crow::json::wvalue::list();
                            选择列表[0] = std::move(选择);
                            最后消息块["choices"] = std::move(选择列表);

                            // 发送最后的增量数据
                            流式响应->write("data: " + 最后消息块.dump() + "\n\n");
                        }

                        // 发送完成消息
                        crow::json::wvalue 完成消息;
                        完成消息["id"] = 响应ID;
                        完成消息["object"] = "chat.completion.chunk";
                        完成消息["created"] = 创建时间;
                        完成消息["model"] = 模型名称;

                        crow::json::wvalue 选择列表 = crow::json::wvalue::list();
                        crow::json::wvalue 选择;
                        crow::json::wvalue 增量;

                        // 空的delta表示消息结束
                        // 使用std::move修复所有赋值
                        选择["delta"] = std::move(增量);
                        选择["index"] = 0;
                        选择["finish_reason"] = "stop";

                        选择列表[0] = std::move(选择);
                        完成消息["choices"] = std::move(选择列表);

                        // 发送最后的数据块和结束标记
                        流式响应->write("data: " + 完成消息.dump() + "\n\n");
                        流式响应->write("data: [DONE]\n\n");
                        流式响应->write("0\r\n\r\n");
                    }).detach();

                // 返回流式响应
                return std::move(*流式响应);
            }
            else*/
            {
                // 非流式响应，等待完成后一次性返回所有数据
                std::string 最终回复 = "";

                // 收集所有增量回复
                while (!bot->second->Finished(timeStamp))
                {
                    // 获取增量响应并累加
                    std::string 增量内容 = bot->second->GetResponse(timeStamp);
                    最终回复 += 增量内容;
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
                auto 最后增量内容 = bot->second->GetResponse(timeStamp);
                if (!最后增量内容.empty())
                {
                    最终回复 += 最后增量内容;
                }

                // 构建OpenAI风格的非流式响应
                std::string 响应ID = "chatcmpl-" + std::to_string(timeStamp);
                uint64_t 创建时间 = static_cast<uint64_t>(timeStamp / 1000); // 转换为秒

                响应["id"] = 响应ID;
                响应["object"] = "chat.completion";
                响应["created"] = 创建时间;
                响应["model"] = 模型名称;

                // 构建选择部分
                crow::json::wvalue 选择列表 = crow::json::wvalue::list();
                crow::json::wvalue 选择;

                // 构建消息
                crow::json::wvalue 消息;
                消息["role"] = "assistant";
                消息["content"] = 最终回复;

                选择["message"] = std::move(消息);
                选择["index"] = 0;
                选择["finish_reason"] = "stop";

                选择列表[0] = std::move(选择);
                响应["choices"] = std::move(选择列表);

                // 添加用量统计（模拟数据）
                crow::json::wvalue 用量;
                用量["prompt_tokens"] = lastPrompt.length() / 4 + 10; // 简单估算
                用量["completion_tokens"] = 最终回复.length() / 4;
                用量["total_tokens"] = lastPrompt.length() / 4 + 最终回复.length() / 4 + 10;

                响应["usage"] = std::move(用量);

                // 返回非流式响应
                return crow::response(响应);
            }
        }
        catch (const std::exception& e)
        {
            // 处理异常
            响应["error"] = std::string("处理请求时出错: ") + e.what();
            return crow::response(500, 响应);
        }
    });

    CROW_ROUTE(app, "/v1/models")
    ([Bots](const crow::request& req)
    {
        crow::json::wvalue model_list = crow::json::wvalue::list();
        int index = 0;
        for (const auto& bot : Bots)
        {
            // 在Crow中，对json数组的添加是通过索引进行的，而不是push_back
            crow::json::wvalue model;
            model = bot.first;

            // 使用索引添加到数组
            model_list[index++] = std::move(model);
        }

        return crow::response(model_list);
    });

    // 启动服务器，使用指定的端口
    std::cout << "服务器启动中，监听端口: " << port << std::endl;
    app.port(port).multithreaded().run();
    return 0;
}
