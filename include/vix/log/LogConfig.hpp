/**
 *
 *  @file LogConfig.hpp
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
#ifndef VIX_LOG_LOGCONFIG_HPP
#define VIX_LOG_LOGCONFIG_HPP

#include <vix/log/LogFormat.hpp>
#include <vix/log/LogLevel.hpp>

namespace vix::log
{

  /**
   * @struct LogConfig
   * @brief Public configuration for the Vix log facade.
   *
   * This structure represents a simple, stable configuration layer
   * for the public `vix::log` API.
   */
  struct LogConfig
  {
    /**
     * @brief Default logging level.
     */
    LogLevel level{LogLevel::Info};

    /**
     * @brief Default output format.
     */
    LogFormat format{LogFormat::KV};

    /**
     * @brief Whether async logging mode should be enabled.
     */
    bool async{false};
  };

} // namespace vix::log

#endif // VIX_LOG_LOGCONFIG_HPP
