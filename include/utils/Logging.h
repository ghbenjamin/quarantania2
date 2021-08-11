#pragma once

#include <fmt/format.h>
#include <spdlog/logger.h>


class Logger
{
public:
    ~Logger() = default;

    static Logger& get()
    {
        static Logger l;
        return l;
    }
    
    
    template <typename... Args>
    void info(Args... args)
    {
        m_logger->info( std::forward<Args>(args)... );
    }
    
    template <typename... Args>
    void error(Args... args)
    {
        std::string buf = fmt::format( std::forward<Args>(args)... );
        m_logger->error( buf );
    }
    
    template <typename... Args>
    void debug(Args... args)
    {
        std::string buf = fmt::format( std::forward<Args>(args)... );
        m_logger->debug( buf );
    }
    
    template <typename... Args>
    void warn(Args... args)
    {
        std::string buf = fmt::format( std::forward<Args>(args)... );
        m_logger->warn( buf );
    }

private:
    Logger();
    
    std::shared_ptr<spdlog::logger> m_logger;
};

