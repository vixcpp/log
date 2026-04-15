/**
 *
 *  @file GlobalLog.cpp
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

#include <vix/log/GlobalLog.hpp>

namespace vix::log
{

  LogAdapter &global() noexcept
  {
    return LogAdapter::instance();
  }

  void configure(const LogConfig &config)
  {
    global().configure(config);
  }

  void set_level(LogLevel level_value)
  {
    global().set_level(level_value);
  }

  LogLevel level() noexcept
  {
    return global().level();
  }

  bool enabled(LogLevel level_value) noexcept
  {
    return global().enabled(level_value);
  }

  void set_format(LogFormat format)
  {
    global().set_format(format);
  }

  void set_async(bool enable)
  {
    global().set_async(enable);
  }

  void set_context(LogContext ctx)
  {
    global().set_context(std::move(ctx));
  }

  void clear_context()
  {
    global().clear_context();
  }

  LogContext context()
  {
    return global().context();
  }

  void set_level_from_env(std::string_view env_name)
  {
    global().set_level_from_env(env_name);
  }

  void set_format_from_env(std::string_view env_name)
  {
    global().set_format_from_env(env_name);
  }

  LogLevel parse_level(std::string_view value)
  {
    return LogAdapter::parse_level(value);
  }

  LogFormat parse_format(std::string_view value)
  {
    return LogAdapter::parse_format(value);
  }

} // namespace vix::log
