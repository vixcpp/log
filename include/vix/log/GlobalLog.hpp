/**
 *
 *  @file GlobalLog.hpp
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
#ifndef VIX_LOG_GLOBALLOG_HPP
#define VIX_LOG_GLOBALLOG_HPP

#include <string_view>
#include <utility>

#include <vix/log/LogAdapter.hpp>
#include <vix/log/LogConfig.hpp>
#include <vix/log/LogContext.hpp>
#include <vix/log/LogFormat.hpp>
#include <vix/log/LogLevel.hpp>

namespace vix::log
{

  /**
   * @brief Return the global public log adapter.
   */
  [[nodiscard]] LogAdapter &global() noexcept;

  /**
   * @brief Configure the global logger facade.
   */
  void configure(const LogConfig &config);

  /**
   * @brief Set the global log level.
   */
  void set_level(LogLevel level);

  /**
   * @brief Get the global log level.
   */
  [[nodiscard]] LogLevel level() noexcept;

  /**
   * @brief Check whether the given level is enabled.
   */
  [[nodiscard]] bool enabled(LogLevel level) noexcept;

  /**
   * @brief Set the global log format.
   */
  void set_format(LogFormat format);

  /**
   * @brief Enable or disable async logging.
   */
  void set_async(bool enable);

  /**
   * @brief Set the current per-thread log context.
   */
  void set_context(LogContext ctx);

  /**
   * @brief Clear the current per-thread log context.
   */
  void clear_context();

  /**
   * @brief Get a copy of the current per-thread log context.
   */
  [[nodiscard]] LogContext context();

  /**
   * @brief Set log level from environment.
   */
  void set_level_from_env(std::string_view env_name = "VIX_LOG_LEVEL");

  /**
   * @brief Set log format from environment.
   */
  void set_format_from_env(std::string_view env_name = "VIX_LOG_FORMAT");

  /**
   * @brief Parse a public log level from string.
   */
  [[nodiscard]] LogLevel parse_level(std::string_view value);

  /**
   * @brief Parse a public log format from string.
   */
  [[nodiscard]] LogFormat parse_format(std::string_view value);

  /**
   * @brief Log a TRACE message.
   */
  template <typename... Args>
  void trace(fmt::format_string<Args...> fmtstr, Args &&...args)
  {
    global().trace(fmtstr, std::forward<Args>(args)...);
  }

  /**
   * @brief Log a DEBUG message.
   */
  template <typename... Args>
  void debug(fmt::format_string<Args...> fmtstr, Args &&...args)
  {
    global().debug(fmtstr, std::forward<Args>(args)...);
  }

  /**
   * @brief Log an INFO message.
   */
  template <typename... Args>
  void info(fmt::format_string<Args...> fmtstr, Args &&...args)
  {
    global().info(fmtstr, std::forward<Args>(args)...);
  }

  /**
   * @brief Log a WARN message.
   */
  template <typename... Args>
  void warn(fmt::format_string<Args...> fmtstr, Args &&...args)
  {
    global().warn(fmtstr, std::forward<Args>(args)...);
  }

  /**
   * @brief Log an ERROR message.
   */
  template <typename... Args>
  void error(fmt::format_string<Args...> fmtstr, Args &&...args)
  {
    global().error(fmtstr, std::forward<Args>(args)...);
  }

  /**
   * @brief Log a CRITICAL message.
   */
  template <typename... Args>
  void critical(fmt::format_string<Args...> fmtstr, Args &&...args)
  {
    global().critical(fmtstr, std::forward<Args>(args)...);
  }

  /**
   * @brief Log a message at the given level.
   */
  template <typename... Args>
  void log(LogLevel level_value, fmt::format_string<Args...> fmtstr, Args &&...args)
  {
    global().log(level_value, fmtstr, std::forward<Args>(args)...);
  }

  /**
   * @brief Log a message with key/value pairs.
   */
  template <typename... Args>
  void logf(LogLevel level_value, const std::string &message, Args &&...kvpairs)
  {
    global().logf(level_value, message, std::forward<Args>(kvpairs)...);
  }

} // namespace vix::log

#endif // VIX_LOG_GLOBALLOG_HPP
