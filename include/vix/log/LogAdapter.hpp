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
#include <vix/utils/Logger.hpp>

namespace vix::log
{

  /**
   * @class LogAdapter
   * @brief Public adapter that bridges `vix::log` to `vix::utils::Logger`.
   *
   * This class does not implement logging itself.
   * It provides a stable public API for the `vix::log` module while
   * delegating the actual work to the existing utils logger backend.
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
      backend().log(to_utils_level(level), fmtstr, std::forward<Args>(args)...);
    }

    /**
     * @brief Log a formatted message with key/value pairs.
     *
     * This delegates to `vix::utils::Logger::logf`.
     */
    template <typename... Args>
    void logf(LogLevel level, const std::string &message, Args &&...kvpairs)
    {
      backend().logf(to_utils_level(level), message, std::forward<Args>(kvpairs)...);
    }

  private:
    /**
     * @brief Access the underlying utils logger backend.
     */
    [[nodiscard]] static vix::utils::Logger &backend();

    /**
     * @brief Convert a public log level to utils logger level.
     */
    [[nodiscard]] static vix::utils::Logger::Level to_utils_level(LogLevel level) noexcept;

    /**
     * @brief Convert a utils logger level to public log level.
     */
    [[nodiscard]] static LogLevel from_utils_level(vix::utils::Logger::Level level) noexcept;

    /**
     * @brief Convert a public log format to utils logger format.
     */
    [[nodiscard]] static vix::utils::Logger::Format to_utils_format(LogFormat format) noexcept;

    /**
     * @brief Convert a utils logger format to public log format.
     */
    [[nodiscard]] static LogFormat from_utils_format(vix::utils::Logger::Format format) noexcept;

    /**
     * @brief Convert a public log context to utils logger context.
     */
    [[nodiscard]] static vix::utils::Logger::Context to_utils_context(const LogContext &ctx);

    /**
     * @brief Convert a utils logger context to public log context.
     */
    [[nodiscard]] static LogContext from_utils_context(const vix::utils::Logger::Context &ctx);
  };

} // namespace vix::log

#endif // VIX_LOG_LOGADAPTER_HPP
