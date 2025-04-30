#ifndef UTILS_H
#define UTILS_H
#include <fstream>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

#include "Logger.h"

class UFile
{
public:
    static bool Exists(const std::string& filename);

    // 获取目录下所有文件
    static std::vector<std::string> GetFilesInDirectory(const std::string& folder);

    static std::string PlatformPath(std::string path);

    // 检查是否以某后缀结尾
    static bool EndsWith(const std::string& str, const std::string& suffix);

    static bool UCopyFile(const std::string& src, const std::string& dst);
};

class UDirectory
{
public:
    static bool Create(const std::string& dirname);

    static bool CreateDirIfNotExists(const std::string& dir);

    static bool Exists(const std::string& dirname);

    static bool Remove(const std::string& dir);

    static std::vector<std::string> GetSubFiles(const std::string& dirPath);


    static std::vector<std::string> GetSubDirectories(const std::string& dirPath);
};

inline static void SaveYaml(const std::string& filename, const YAML::Node& node)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("LogError: Failed to open file " + filename);
        return;
    }
    file << node;
    file.close();
}

template <typename T>
inline static std::optional<T> LoadYaml(const std::string& file)
{
    try
    {
        // 从文件中读取YAML数据
        YAML::Node node = YAML::LoadFile(file);

        // 将整个YAML节点转换为指定类型的对象
        return node.as<T>();
    }
    catch (const std::exception& e)
    {
        // 解析异常处理
        LogError("{0}", e.what());
        return std::optional<T>();
    }
}

template <typename T>
inline static YAML::Node toYaml(const T& value)
{
    YAML::Node node;
    node = value;
    return node;
}


#endif //UTILS_H
