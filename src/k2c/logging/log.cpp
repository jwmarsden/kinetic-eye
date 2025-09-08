#include "k2c/logging/log.hpp"

#include <memory>

namespace std {
    template <>
    struct hash<k2c::logging::K2CLogReference> {
        size_t operator()(k2c::logging::K2CLogReference const &logReference) const {
            size_t seed = 0;
            k2c::hashCombine(seed, logReference.file, logReference.function, logReference.line);
            return seed;
        }
    };
}

namespace k2c::logging {

    std::unordered_map<K2CLogReference, K2CLogStore> logReferences;

    LogManger::LogManger() {
    }

    LogManger::~LogManger() {
    }

    void LogManger::initialise() {
        auto inConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        inConsoleSink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e %s:%#] >%!%v%$");
        std::vector<spdlog::sink_ptr> inSinks {inConsoleSink};
        auto inLogger = std::make_shared<spdlog::logger>(K2C_IN_LOGGER_NAME, inSinks.begin(), inSinks.end());
        inLogger->set_level(spdlog::level::trace);
        inLogger->flush_on(spdlog::level::trace);
        spdlog::register_logger(inLogger);

        auto outConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        outConsoleSink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e %s:%#] <%!%v%$");
        std::vector<spdlog::sink_ptr> outSinks {outConsoleSink};
        auto outLogger = std::make_shared<spdlog::logger>(K2C_OUT_LOGGER_NAME, outSinks.begin(), outSinks.end());
        outLogger->set_level(spdlog::level::trace);
        outLogger->flush_on(spdlog::level::trace);
        spdlog::register_logger(outLogger);

        auto detailedConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        detailedConsoleSink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e %s:%#] %v%$");
        std::vector<spdlog::sink_ptr> detailSinks {detailedConsoleSink};
        auto detailLogger = std::make_shared<spdlog::logger>(K2C_DETAILED_LOGGER_NAME, detailSinks.begin(), detailSinks.end());
        detailLogger->set_level(spdlog::level::trace);
        detailLogger->flush_on(spdlog::level::trace);
        spdlog::register_logger(detailLogger);
 
        auto defaultConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        defaultConsoleSink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] %v%$");
        std::vector<spdlog::sink_ptr> defaultSinks {defaultConsoleSink};
        auto defaultLogger = std::make_shared<spdlog::logger>(K2C_DEFAULT_LOGGER_NAME, defaultSinks.begin(), defaultSinks.end());
        defaultLogger->set_level(spdlog::level::info);
        defaultLogger->flush_on(spdlog::level::info);
        spdlog::register_logger(defaultLogger);
    }

    void LogManger::shutdown() {
        spdlog::shutdown();
    }

    bool LogManger::check(std::string file, std::string function, int line) {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        
        K2CLogReference logReference {};
        logReference.file = file;
        logReference.function = function;
        logReference.line = line;

        if(logReferences.count(logReference) == 0) {
            K2CLogStore store {0, now};
            logReferences[logReference] = store;
            return true;
        }  else {
            K2CLogStore store = logReferences[logReference];
            std::chrono::steady_clock::time_point basis = store.basis;
            uint64_t delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - basis).count();

            if(delta >= LOG_SPAM_INTERVAL_MS) {
                store.basis = now;
                logReferences[logReference] = store;
                //K2C_TRACE("{}.{}[{}]@{}", logReference.file, logReference.function, logReference.line, delta);
                return true;
            } else {
                store.counter++;
                logReferences[logReference] = store;
                return false;
            }
        }
    }

    int LogManger::rebase(std::string file, std::string function, int line) {
        K2CLogReference logReference {};
        logReference.file = file;
        logReference.function = function;
        logReference.line = line;
        if(logReferences.count(logReference) != 0) {
            K2CLogStore store = logReferences[logReference];
            int counter = store.counter;
            store.counter = 0;
            logReferences[logReference] = store;
            return counter;
        } else {
            return -1;
        }
    }
}
