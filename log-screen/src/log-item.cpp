#include "log-screen.h"
#include <unordered_map>
using namespace SeaDrip::LogScreen;

const LogItem EmptyLogItem = LogItem(LogLevel::EMPTY, "");

const std::unordered_map<std::string, LogLevel> level_name_dict = {
    std::pair<std::string, LogLevel>("info", LogLevel::INFO),
    std::pair<std::string, LogLevel>("warning", LogLevel::WARNING),
    std::pair<std::string, LogLevel>("error", LogLevel::ERROR),
};

const std::unordered_map<int, std::string> level_vendor_dict = {
    std::pair<int, std::string>(static_cast<int>(LogLevel::EMPTY), ""),
    std::pair<int, std::string>(static_cast<int>(LogLevel::INFO), "[Info]"),
    std::pair<int, std::string>(static_cast<int>(LogLevel::WARNING), "\e[33m[Warning]\e[m"),
    std::pair<int, std::string>(static_cast<int>(LogLevel::ERROR), "\e[31m[Error]\e[m"),
};

LogItem LogItem::FromLevelString(const std::string& level, const std::string& content)
{
    auto found_level = level_name_dict.find(level);
    if (found_level == level_name_dict.end())
    {
        return EmptyLogItem;
    }
    return LogItem(found_level->second, content);
}

LogItem::LogItem(const LogLevel level, const std::string& content) : m_e_level(level), m_s_content(content)
{}

const LogLevel LogItem::GetLogLevel() const
{
    return this->m_e_level;
}

const std::string LogItem::GetLogContent() const
{
    return this->m_s_content;
}

const std::string LogItem::vendor() const
{
    auto found_level = level_vendor_dict.find(static_cast<int>(this->m_e_level));
    std::string ret = (found_level == level_vendor_dict.end()) ? "" : found_level->second;
    return ret + this->m_s_content;
}
