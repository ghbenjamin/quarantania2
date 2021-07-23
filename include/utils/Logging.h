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
        m_logger->error( std::forward<Args>(args)... );
    }
    
    template <typename... Args>
    void debug(Args... args)
    {
        m_logger->debug( std::forward<Args>(args)... );
    }
    
    template <typename... Args>
    void warn(Args... args)
    {
        m_logger->warn( std::forward<Args>(args)... );
    }

private:
    Logger();
    
    std::shared_ptr<spdlog::logger> m_logger;
};

