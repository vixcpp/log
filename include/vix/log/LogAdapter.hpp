/**
 *
 *  @file LogAdapter.hpp
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
#ifndef VIX_LOG_LOGADAPTER_HPP
#define VIX_LOG_LOGADAPTER_HPP

#include <string_view>
#include <utility>

#include <vix/log/LogConfig.hpp>
#include <vix/log/LogContext.hpp>
#include <vix/log/LogFormat.hpp>
#include <vix/log/LogLevel.hpp>
#include <vix/log/Logger.hpp>

namespace vix::log
{

  /**
   * @class LogAdapter
   * @brief Public adapter over the canonical Vix logger implementation.
   *
   * This class provides the public `vix::log` API while delegating to the
   * logging implementation owned by this module.
   */
  class LogAdapter
  {
  public:
    /**
     * @brief Get the global adapter instance.
     */
    static LogAdapter &instance();

    /**
     * @brief Apply a public log configuration.
     */
    void configure(const LogConfig &config);

    /**
     * @brief Set the active log level.
     */
    void set_level(LogLevel level);

    /**
     * @brief Get the active log level.
     */
    [[nodiscard]] LogLevel level() const noexcept;

    /**
     * @brief Check whether the given log level is enabled.
     */
    [[nodiscard]] bool enabled(LogLevel level) const noexcept;

    /**
     * @brief Set the active output format.
     */
    void set_format(LogFormat format);

    /**
     * @brief Enable or disable async logging mode.
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
    [[nodiscard]] LogContext context() const;

    /**
     * @brief Read log level from environment.
     *
     * Default variable name: `VIX_LOG_LEVEL`.
     */
    void set_level_from_env(std::string_view env_name = "VIX_LOG_LEVEL");

    /**
     * @brief Read log format from environment.
     *
     * Default variable name: `VIX_LOG_FORMAT`.
     */
    void set_format_from_env(std::string_view env_name = "VIX_LOG_FORMAT");

    /**
     * @brief Parse a public log level from string.
     */
    [[nodiscard]] static LogLevel parse_level(std::string_view value);

    /**
     * @brief Parse a public log format from string.
     */
    [[nodiscard]] static LogFormat parse_format(std::string_view value);

    /**
     * @brief Log a TRACE message.
     */
    template <typename... Args>
    void trace(fmt::format_string<Args...> fmtstr, Args &&...args)
    {
      backend().trace(fmtstr, std::forward<Args>(args)...);
    }

    /**
     * @brief Log a DEBUG message.
     */
    template <typename... Args>
    void debug(fmt::format_string<Args...> fmtstr, Args &&...args)
    {
      backend().debug(fmtstr, std::forward<Args>(args)...);
    }

    /**
     * @brief Log an INFO message.
     */
    template <typename... Args>
    void info(fmt::format_string<Args...> fmtstr, Args &&...args)
    {
      backend().info(fmtstr, std::forward<Args>(args)...);
    }

    /**
     * @brief Log a WARN message.
     */
    template <typename... Args>
    void warn(fmt::format_string<Args...> fmtstr, Args &&...args)
    {
      backend().warn(fmtstr, std::forward<Args>(args)...);
    }

    /**
     * @brief Log an ERROR message.
     */
    template <typename... Args>
    void error(fmt::format_string<Args...> fmtstr, Args &&...args)
    {
      backend().error(fmtstr, std::forward<Args>(args)...);
    }

    /**
     * @brief Log a CRITICAL message.
     */
    template <typename... Args>
    void critical(fmt::format_string<Args...> fmtstr, Args &&...args)
    {
      backend().critical(fmtstr, std::forward<Args>(args)...);
    }

    /**
     * @brief Log a message at the given public level.
     */
    template <typename... Args>
    void log(LogLevel level, fmt::format_string<Args...> fmtstr, Args &&...args)
    {
      backend().log(to_logger_level(level), fmtstr, std::forward<Args>(args)...);
    }

    /**
     * @brief Log a formatted message with key/value pairs.
     *
     * This delegates to the canonical logger implementation.
     */
    template <typename... Args>
    void logf(LogLevel level, const std::string &message, Args &&...kvpairs)
    {
      backend().logf(to_logger_level(level), message, std::forward<Args>(kvpairs)...);
    }

  private:
    /**
     * @brief Access the underlying logger implementation.
     */
    [[nodiscard]] static Logger &backend();

    /**
     * @brief Convert a public log level to implementation level.
     */
    [[nodiscard]] static Logger::Level to_logger_level(LogLevel level) noexcept;

    /**
     * @brief Convert an implementation level to public log level.
     */
    [[nodiscard]] static LogLevel from_logger_level(Logger::Level level) noexcept;

    /**
     * @brief Convert a public log format to implementation format.
     */
    [[nodiscard]] static Logger::Format to_logger_format(LogFormat format) noexcept;

    /**
     * @brief Convert an implementation format to public log format.
     */
    [[nodiscard]] static LogFormat from_logger_format(Logger::Format format) noexcept;

    /**
     * @brief Convert a public log context to implementation context.
     */
    [[nodiscard]] static Logger::Context to_logger_context(const LogContext &ctx);

    /**
     * @brief Convert an implementation context to public log context.
     */
    [[nodiscard]] static LogContext from_logger_context(const Logger::Context &ctx);
  };

} // namespace vix::log

#endif // VIX_LOG_LOGADAPTER_HPP
