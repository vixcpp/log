/**
 *
 *  @file LogLevel.hpp
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
#ifndef VIX_LOG_LOGLEVEL_HPP
#define VIX_LOG_LOGLEVEL_HPP

namespace vix::log
{

  /**
   * @enum LogLevel
   * @brief Public logging severity levels for the Vix log module.
   *
   * These values are intentionally stable and map to the underlying
   * `vix::utils::Logger::Level` values through the log adapter.
   */
  enum class LogLevel
  {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Critical,
    Off
  };

} // namespace vix::log

#endif // VIX_LOG_LOGLEVEL_HPP
