#pragma once

#include "config.h"
#include "utils.hpp"

#include <chrono>
#include <vector>
#include <unordered_map>

#if K2C_BUILD_TYPE == 1
#   if !defined(SPDLOG_ACTIVE_LEVEL)
#       define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#   endif
#elif K2C_BUILD_TYPE == 2
#   if !defined(SPDLOG_ACTIVE_LEVEL)
#       define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#   endif
#endif

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ranges.h"

#define K2C_IN_LOGGER_NAME "k2c_in_log"
#define K2C_OUT_LOGGER_NAME "k2c_out_log"
#define K2C_DETAILED_LOGGER_NAME "k2c_detail_log"
#define K2C_DEFAULT_LOGGER_NAME "k2c_default_log"

#define K2C_NOARG "()"

#if K2C_BUILD_TYPE == 1
#define K2C_IN_SPAM(...) \
    if (k2c::logging::LogManger::getInstance().check(__FILE__, static_cast<const char *>(__FUNCTION__),  __LINE__) && spdlog::get(K2C_IN_LOGGER_NAME) != nullptr) \
    {(spdlog::get(K2C_IN_LOGGER_NAME))->log(spdlog::source_loc{__FILE__, __LINE__, static_cast<const char *>(__FUNCTION__)}, spdlog::level::trace, __VA_ARGS__);k2c::logging::LogManger::getInstance().rebase(__FILE__, static_cast<const char *>(__FUNCTION__),  __LINE__);}
#define K2C_OUT_SPAM(...) \
    if (k2c::logging::LogManger::getInstance().check(__FILE__, static_cast<const char *>(__FUNCTION__),  __LINE__) && spdlog::get(K2C_OUT_LOGGER_NAME) != nullptr) \
    {(spdlog::get(K2C_OUT_LOGGER_NAME))->log(spdlog::source_loc{__FILE__, __LINE__, static_cast<const char *>(__FUNCTION__)}, spdlog::level::trace, __VA_ARGS__);k2c::logging::LogManger::getInstance().rebase(__FILE__, static_cast<const char *>(__FUNCTION__),  __LINE__);}
#define K2C_TRACE_SPAM(...) \
    if (k2c::logging::LogManger::getInstance().check(__FILE__, static_cast<const char *>(__FUNCTION__),  __LINE__) && spdlog::get(K2C_DETAILED_LOGGER_NAME) != nullptr) \
    {(spdlog::get(K2C_DETAILED_LOGGER_NAME))->log(spdlog::source_loc{__FILE__, __LINE__, static_cast<const char *>(__FUNCTION__)}, spdlog::level::trace, __VA_ARGS__);k2c::logging::LogManger::getInstance().rebase(__FILE__, static_cast<const char *>(__FUNCTION__),  __LINE__);}

#define K2C_IN(...)          if (spdlog::get(K2C_IN_LOGGER_NAME) != nullptr) {(spdlog::get(K2C_IN_LOGGER_NAME))->log(spdlog::source_loc{__FILE__, __LINE__, static_cast<const char *>(__FUNCTION__)}, spdlog::level::trace, __VA_ARGS__);}
#define K2C_OUT(...)         if (spdlog::get(K2C_OUT_LOGGER_NAME) != nullptr) {(spdlog::get(K2C_OUT_LOGGER_NAME))->log(spdlog::source_loc{__FILE__, __LINE__, static_cast<const char *>(__FUNCTION__)}, spdlog::level::trace, __VA_ARGS__);}
#define K2C_TRACE(...)       if (spdlog::get(K2C_DETAILED_LOGGER_NAME) != nullptr) {SPDLOG_LOGGER_TRACE(spdlog::get(K2C_DETAILED_LOGGER_NAME), __VA_ARGS__);}
#define K2C_DEBUG(...)       if (spdlog::get(K2C_DETAILED_LOGGER_NAME) != nullptr) {SPDLOG_LOGGER_DEBUG(spdlog::get(K2C_DETAILED_LOGGER_NAME), __VA_ARGS__);}
#define K2C_INFO(...)        if (spdlog::get(K2C_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(K2C_DEFAULT_LOGGER_NAME)->info(__VA_ARGS__);}
#define K2C_WARN(...)        if (spdlog::get(K2C_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(K2C_DEFAULT_LOGGER_NAME)->warn(__VA_ARGS__);}
#define K2C_ERROR(...)       if (spdlog::get(K2C_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(K2C_DEFAULT_LOGGER_NAME)->error(__VA_ARGS__);}
#define K2C_CRITICAL(...)    if (spdlog::get(K2C_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(K2C_DEFAULT_LOGGER_NAME)->critical(__VA_ARGS__);}
#elif K2C_BUILD_TYPE == 2
#define K2C_IN_SPAM(...)     (void) 0;
#define K2C_OUT_SPAM(...)    (void) 0;
#define K2C_TRACE_SPAM(...)  (void) 0;

#define K2C_IN(...)          (void) 0;
#define K2C_OUT(...)         (void) 0;
#define K2C_TRACE(...)       (void) 0;
#define K2C_DEBUG(...)       (void) 0;
#define K2C_INFO(...)        if (spdlog::get(K2C_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(K2C_DEFAULT_LOGGER_NAME)->info(__VA_ARGS__);}
#define K2C_WARN(...)        if (spdlog::get(K2C_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(K2C_DEFAULT_LOGGER_NAME)->warn(__VA_ARGS__);}
#define K2C_ERROR(...)       if (spdlog::get(K2C_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(K2C_DEFAULT_LOGGER_NAME)->error(__VA_ARGS__);}
#define K2C_CRITICAL(...)    if (spdlog::get(K2C_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(K2C_DEFAULT_LOGGER_NAME)->critical(__VA_ARGS__);}
#endif

namespace k2c::logging {

    struct K2CLogReference {
        std::string file;
        std::string function;
        uint32_t line;

        bool operator==(const K2CLogReference other) const {
            return file == other.file && function == other.function && line == other.line;
        }
    };

    struct K2CLogStore {
        int counter;
        std::chrono::steady_clock::time_point basis;
    };

    class LogManger {
        
        public:

            const int LOG_SPAM_INTERVAL_MS = 5000;

            LogManger(LogManger const&)       = delete;
            void operator=(LogManger const&)  = delete;
   
            ~LogManger();

            static LogManger& getInstance() {
                static LogManger instance;
                return instance;
            }

            void initialise();

            void shutdown();

            bool check(std::string file, std::string function, int line);

            int rebase(std::string file, std::string function, int line);

        private:

            LogManger();
            
    };

}