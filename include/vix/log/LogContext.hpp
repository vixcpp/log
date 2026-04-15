/**
 *
 *  @file LogContext.hpp
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
#ifndef VIX_LOG_LOGCONTEXT_HPP
#define VIX_LOG_LOGCONTEXT_HPP

#include <string>
#include <unordered_map>

namespace vix::log
{

  /**
   * @struct LogContext
   * @brief Public per-thread log context.
   *
   * LogContext carries request-scoped metadata that can be attached
   * to logs produced through the Vix log module.
   *
   * Typical fields include:
   * - request id
   * - module name
   * - arbitrary key/value metadata
   */
  struct LogContext
  {
    /**
     * @brief Request identifier for correlation.
     */
    std::string request_id;

    /**
     * @brief Logical module name.
     */
    std::string module;

    /**
     * @brief Arbitrary structured metadata fields.
     */
    std::unordered_map<std::string, std::string> fields;
  };

} // namespace vix::log

#endif // VIX_LOG_LOGCONTEXT_HPP
