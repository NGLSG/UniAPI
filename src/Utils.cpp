#include "../include/Utils.h"
#include <filesystem>
#include <fstream>

#include "Logger.h"

bool UFile::Exists(const std::string& filename)
{
    std::ifstream file(filename);
    return file.good();
}

std::vector<std::string> UFile::GetFilesInDirectory(const std::string& folder)
{
    std::vector<std::string> result;

    // 使用filesystem遍历目录
    for (const auto& entry : std::filesystem::directory_iterator(folder))
    {
        result.push_back(entry.path().string());
    }

    return result;
}

std::string UFile::PlatformPath(std::string path)
{
    return std::filesystem::path(path).make_preferred().string();
}

bool UFile::EndsWith(const std::string& str, const std::string& suffix)
{
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

bool UDirectory::CreateDirIfNotExists(const std::string& dir)
{
    if (!Exists(dir))
    {
        return Create(dir);
    }
    return true;
}

bool UFile::UCopyFile(const std::string& src, const std::string& dst)
{
    try
    {
        std::filesystem::copy_file(
            std::filesystem::path(src),
            std::filesystem::path(dst)
        );
    }
    catch (std::filesystem::filesystem_error& e)
    {
        // 处理错误
        return false;
    }

    return true;
}

bool UDirectory::Create(const std::string& dirname)
{
    try
    {
        std::filesystem::create_directories(dirname);
        return true;
    }
    catch (std::filesystem::filesystem_error& e)
    {
        LogError(fmt::format("creating directory: {0}", e.what()));
        return false;
    }
}

bool UDirectory::Exists(const std::string& dirname)
{
    return std::filesystem::is_directory(dirname);
}

bool UDirectory::Remove(const std::string& dir)
{
    try
    {
        // 使用 std::filesystem::remove_all 删除目录及其所有内容
        if (std::filesystem::remove_all(dir))
        {
            return true;
        }
        else
        {
            LogError("Failed to remove directory '{0}{1}", dir, "'.");
            return false;
        }
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        LogError("Error occurred while removing directory: {0}", e.what());
        return false;
    }
}

std::vector<std::string> UDirectory::GetSubFiles(const std::string& dirPath)
{
    std::filesystem::path path(dirPath);
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (std::filesystem::is_regular_file(entry))
        {
            files.push_back(entry.path().filename().string());
        }
    }
    return files;
}

std::vector<std::string> UDirectory::GetSubDirectories(const std::string& dirPath)
{
    std::filesystem::path path(dirPath);
    std::vector<std::string> directories;
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (std::filesystem::is_directory(entry))
        {
            directories.push_back(entry.path().filename().string());
            // 递归获取子文件夹下的子文件夹
            // auto subdirectories = ListDirectories(entry);
            // directories.insert(directories.end(), subdirectories.begin(), subdirectories.end());
        }
    }
    return directories;
}
