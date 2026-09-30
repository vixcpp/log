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

  Logger &LogAdapter::backend()
  {
    return Logger::getInstance();
  }

  void LogAdapter::configure(const LogConfig &config)
  {
    set_level(config.level);
    set_format(config.format);
    set_async(config.async);
  }

  void LogAdapter::set_level(LogLevel level)
  {
    backend().setLevel(to_logger_level(level));
  }

  LogLevel LogAdapter::level() const noexcept
  {
    return from_logger_level(backend().level());
  }

  bool LogAdapter::enabled(LogLevel level) const noexcept
  {
    return backend().enabled(to_logger_level(level));
  }

  void LogAdapter::set_format(LogFormat format)
  {
    backend().setFormat(to_logger_format(format));
  }

  void LogAdapter::set_async(bool enable)
  {
    backend().setAsync(enable);
  }

  void LogAdapter::set_context(LogContext ctx)
  {
    backend().setContext(to_logger_context(ctx));
  }

  void LogAdapter::clear_context()
  {
    backend().clearContext();
  }

  LogContext LogAdapter::context() const
  {
    return from_logger_context(backend().getContext());
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
    return from_logger_level(Logger::parseLevel(value));
  }

  LogFormat LogAdapter::parse_format(std::string_view value)
  {
    return from_logger_format(Logger::parseFormat(value));
  }

  Logger::Level LogAdapter::to_logger_level(LogLevel level) noexcept
  {
    using LoggerLevel = Logger::Level;

    switch (level)
    {
    case LogLevel::Trace:
      return LoggerLevel::Trace;
    case LogLevel::Debug:
      return LoggerLevel::Debug;
    case LogLevel::Info:
      return LoggerLevel::Info;
    case LogLevel::Warn:
      return LoggerLevel::Warn;
    case LogLevel::Error:
      return LoggerLevel::Error;
    case LogLevel::Critical:
      return LoggerLevel::Critical;
    case LogLevel::Off:
      return LoggerLevel::Off;
    }

    return LoggerLevel::Info;
  }

  LogLevel LogAdapter::from_logger_level(Logger::Level level) noexcept
  {
    using LoggerLevel = Logger::Level;

    switch (level)
    {
    case LoggerLevel::Trace:
      return LogLevel::Trace;
    case LoggerLevel::Debug:
      return LogLevel::Debug;
    case LoggerLevel::Info:
      return LogLevel::Info;
    case LoggerLevel::Warn:
      return LogLevel::Warn;
    case LoggerLevel::Error:
      return LogLevel::Error;
    case LoggerLevel::Critical:
      return LogLevel::Critical;
    case LoggerLevel::Off:
      return LogLevel::Off;
    }

    return LogLevel::Info;
  }

  Logger::Format LogAdapter::to_logger_format(LogFormat format) noexcept
  {
    using LoggerFormat = Logger::Format;

    switch (format)
    {
    case LogFormat::KV:
      return LoggerFormat::KV;
    case LogFormat::JSON:
      return LoggerFormat::JSON;
    case LogFormat::JSON_PRETTY:
      return LoggerFormat::JSON_PRETTY;
    }

    return LoggerFormat::KV;
  }

  LogFormat LogAdapter::from_logger_format(Logger::Format format) noexcept
  {
    using LoggerFormat = Logger::Format;

    switch (format)
    {
    case LoggerFormat::KV:
      return LogFormat::KV;
    case LoggerFormat::JSON:
      return LogFormat::JSON;
    case LoggerFormat::JSON_PRETTY:
      return LogFormat::JSON_PRETTY;
    }

    return LogFormat::KV;
  }

  Logger::Context LogAdapter::to_logger_context(const LogContext &ctx)
  {
    Logger::Context out;
    out.request_id = ctx.request_id;
    out.module = ctx.module;
    out.fields = ctx.fields;
    return out;
  }

  LogContext LogAdapter::from_logger_context(const Logger::Context &ctx)
  {
    LogContext out;
    out.request_id = ctx.request_id;
    out.module = ctx.module;
    out.fields = ctx.fields;
    return out;
  }

} // namespace vix::log
