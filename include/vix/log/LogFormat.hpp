/**
 *
 *  @file LogFormat.hpp
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
#ifndef VIX_LOG_LOGFORMAT_HPP
#define VIX_LOG_LOGFORMAT_HPP

namespace vix::log
{

  /**
   * @enum LogFormat
   * @brief Public structured logging output formats.
   *
   * These formats map to the underlying `vix::utils::Logger::Format`
   * values through the log adapter.
   */
  enum class LogFormat
  {
    /**
     * @brief Key-value text format.
     */
    KV,

    /**
     * @brief Single-line JSON format.
     */
    JSON,

    /**
     * @brief Pretty JSON format with indentation.
     */
    JSON_PRETTY
  };

} // namespace vix::log

#endif // VIX_LOG_LOGFORMAT_HPP
