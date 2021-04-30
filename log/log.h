#pragma once

/**
 * @file log.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief thin warpper of spdlog
 * @version 0.1
 * @date 2021-04-30
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <memory>

#include "spdlog/common.h"

#define LOGD SPDLOG_DEBUG
#define LOGW SPDLOG_WARN
#define LOGE SPDLOG_ERROR

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"

namespace log_details {
class Log {
   public:
    Log() {
        auto std_cout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink =
            std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/learn_opengl_log.txt", true);

        spdlog::set_default_logger(std::make_shared<spdlog::logger>(
            "OpenGL", spdlog::sinks_init_list{std_cout_sink, file_sink}));

        spdlog::enable_backtrace(10);
        spdlog::flush_on(spdlog::level::debug);
        spdlog::set_pattern("[%Y-%m-%d %T.%e] [%L] [%s:%#] [%!] %v");

        SPDLOG_INFO("App started");
    }

    Log(const Log&) = delete;
    Log(Log&&) = delete;
    auto operator=(const Log&) -> Log& = delete;
    auto operator=(Log&&) -> Log& = delete;

    ~Log() {
        SPDLOG_INFO("App terminated");
    };
};
const static Log global_log;
}  // namespace log_details