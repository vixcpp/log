/**
 *
 *  @file LogAdapter.cpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2025, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/vixcpp/vix
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  Vix.cpp
 */

#include <string>

#include <vix/log/LogAdapter.hpp>

namespace vix::log
{

  LogAdapter &LogAdapter::instance()
  {
    static LogAdapter adapter;
    return adapter;
  }

  vix::utils::Logger &LogAdapter::backend()
  {
    return vix::utils::Logger::getInstance();
  }

  void LogAdapter::configure(const LogConfig &config)
  {
    set_level(config.level);
    set_format(config.format);
    set_async(config.async);
  }

  void LogAdapter::set_level(LogLevel level)
  {
    backend().setLevel(to_utils_level(level));
  }

  LogLevel LogAdapter::level() const noexcept
  {
    return from_utils_level(backend().level());
  }

  bool LogAdapter::enabled(LogLevel level) const noexcept
  {
    return backend().enabled(to_utils_level(level));
  }

  void LogAdapter::set_format(LogFormat format)
  {
    backend().setFormat(to_utils_format(format));
  }

  void LogAdapter::set_async(bool enable)
  {
    backend().setAsync(enable);
  }

  void LogAdapter::set_context(LogContext ctx)
  {
    backend().setContext(to_utils_context(ctx));
  }

  void LogAdapter::clear_context()
  {
    backend().clearContext();
  }

  LogContext LogAdapter::context() const
  {
    return from_utils_context(backend().getContext());
  }

  void LogAdapter::set_level_from_env(std::string_view env_name)
  {
    backend().setLevelFromEnv(env_name);
  }

  void LogAdapter::set_format_from_env(std::string_view env_name)
  {
    backend().setFormatFromEnv(env_name);
  }

  LogLevel LogAdapter::parse_level(std::string_view value)
  {
    return from_utils_level(vix::utils::Logger::parseLevel(value));
  }

  LogFormat LogAdapter::parse_format(std::string_view value)
  {
    return from_utils_format(vix::utils::Logger::parseFormat(value));
  }

  vix::utils::Logger::Level LogAdapter::to_utils_level(LogLevel level) noexcept
  {
    using UtilsLevel = vix::utils::Logger::Level;

    switch (level)
    {
    case LogLevel::Trace:
      return UtilsLevel::Trace;
    case LogLevel::Debug:
      return UtilsLevel::Debug;
    case LogLevel::Info:
      return UtilsLevel::Info;
    case LogLevel::Warn:
      return UtilsLevel::Warn;
    case LogLevel::Error:
      return UtilsLevel::Error;
    case LogLevel::Critical:
      return UtilsLevel::Critical;
    case LogLevel::Off:
      return UtilsLevel::Off;
    }

    return UtilsLevel::Info;
  }

  LogLevel LogAdapter::from_utils_level(vix::utils::Logger::Level level) noexcept
  {
    using UtilsLevel = vix::utils::Logger::Level;

    switch (level)
    {
    case UtilsLevel::Trace:
      return LogLevel::Trace;
    case UtilsLevel::Debug:
      return LogLevel::Debug;
    case UtilsLevel::Info:
      return LogLevel::Info;
    case UtilsLevel::Warn:
      return LogLevel::Warn;
    case UtilsLevel::Error:
      return LogLevel::Error;
    case UtilsLevel::Critical:
      return LogLevel::Critical;
    case UtilsLevel::Off:
      return LogLevel::Off;
    }

    return LogLevel::Info;
  }

  vix::utils::Logger::Format LogAdapter::to_utils_format(LogFormat format) noexcept
  {
    using UtilsFormat = vix::utils::Logger::Format;

    switch (format)
    {
    case LogFormat::KV:
      return UtilsFormat::KV;
    case LogFormat::JSON:
      return UtilsFormat::JSON;
    case LogFormat::JSON_PRETTY:
      return UtilsFormat::JSON_PRETTY;
    }

    return UtilsFormat::KV;
  }

  LogFormat LogAdapter::from_utils_format(vix::utils::Logger::Format format) noexcept
  {
    using UtilsFormat = vix::utils::Logger::Format;

    switch (format)
    {
    case UtilsFormat::KV:
      return LogFormat::KV;
    case UtilsFormat::JSON:
      return LogFormat::JSON;
    case UtilsFormat::JSON_PRETTY:
      return LogFormat::JSON_PRETTY;
    }

    return LogFormat::KV;
  }

  vix::utils::Logger::Context LogAdapter::to_utils_context(const LogContext &ctx)
  {
    vix::utils::Logger::Context out;
    out.request_id = ctx.request_id;
    out.module = ctx.module;
    out.fields = ctx.fields;
    return out;
  }

  LogContext LogAdapter::from_utils_context(const vix::utils::Logger::Context &ctx)
  {
    LogContext out;
    out.request_id = ctx.request_id;
    out.module = ctx.module;
    out.fields = ctx.fields;
    return out;
  }

} // namespace vix::log
