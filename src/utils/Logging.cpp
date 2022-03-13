#include <utils/Logging.h>
#include <spdlog/sinks/stdout_color_sinks.h>

Logger::Logger()
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);
    console_sink->set_pattern("[%^%l%$] %v");
    
    m_logger = std::make_shared<spdlog::logger>( "logger", console_sink );
}
