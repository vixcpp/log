/**
 *
 *  @file log_test.cpp
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

#include <cstdlib>
#include <iostream>
#include <string>

#include <vix/log/GlobalLog.hpp>
#include <vix/log/Log.hpp>
#include <vix/log/LogAdapter.hpp>
#include <vix/log/LogConfig.hpp>
#include <vix/log/LogContext.hpp>
#include <vix/log/LogFormat.hpp>
#include <vix/log/LogLevel.hpp>

namespace
{

  void assert_true(bool condition, const std::string &message)
  {
    if (!condition)
    {
      std::cerr << "[FAIL] " << message << '\n';
      std::exit(EXIT_FAILURE);
    }
  }

  void test_parse_level()
  {
    using namespace vix::log;

    assert_true(parse_level("trace") == LogLevel::Trace,
                "parse_level(trace) should return Trace.");
    assert_true(parse_level("debug") == LogLevel::Debug,
                "parse_level(debug) should return Debug.");
    assert_true(parse_level("info") == LogLevel::Info,
                "parse_level(info) should return Info.");
    assert_true(parse_level("warn") == LogLevel::Warn,
                "parse_level(warn) should return Warn.");
    assert_true(parse_level("error") == LogLevel::Error,
                "parse_level(error) should return Error.");
    assert_true(parse_level("critical") == LogLevel::Critical,
                "parse_level(critical) should return Critical.");
    assert_true(parse_level("off") == LogLevel::Off,
                "parse_level(off) should return Off.");
  }

  void test_parse_format()
  {
    using namespace vix::log;

    assert_true(parse_format("kv") == LogFormat::KV,
                "parse_format(kv) should return KV.");
    assert_true(parse_format("json") == LogFormat::JSON,
                "parse_format(json) should return JSON.");
    assert_true(parse_format("json_pretty") == LogFormat::JSON_PRETTY,
                "parse_format(json_pretty) should return JSON_PRETTY.");
  }

  void test_set_and_get_level()
  {
    using namespace vix::log;

    set_level(LogLevel::Debug);
    assert_true(level() == LogLevel::Debug,
                "Global log level should be Debug after set_level(Debug).");

    set_level(LogLevel::Error);
    assert_true(level() == LogLevel::Error,
                "Global log level should be Error after set_level(Error).");
  }

  void test_enabled()
  {
    using namespace vix::log;

    set_level(LogLevel::Warn);

    assert_true(!enabled(LogLevel::Info),
                "Info should be disabled when global level is Warn.");
    assert_true(enabled(LogLevel::Warn),
                "Warn should be enabled when global level is Warn.");
    assert_true(enabled(LogLevel::Error),
                "Error should be enabled when global level is Warn.");
  }

  void test_context_roundtrip()
  {
    using namespace vix::log;

    LogContext ctx;
    ctx.request_id = "req-123";
    ctx.module = "auth";
    ctx.fields["user_id"] = "42";
    ctx.fields["ip"] = "127.0.0.1";

    set_context(ctx);

    const LogContext current = context();

    assert_true(current.request_id == "req-123",
                "Context request_id should round-trip correctly.");
    assert_true(current.module == "auth",
                "Context module should round-trip correctly.");
    assert_true(current.fields.count("user_id") == 1,
                "Context should contain user_id.");
    assert_true(current.fields.at("user_id") == "42",
                "Context user_id value should round-trip correctly.");
    assert_true(current.fields.count("ip") == 1,
                "Context should contain ip.");
    assert_true(current.fields.at("ip") == "127.0.0.1",
                "Context ip value should round-trip correctly.");

    clear_context();

    const LogContext cleared = context();
    assert_true(cleared.request_id.empty(),
                "Context request_id should be empty after clear_context().");
    assert_true(cleared.module.empty(),
                "Context module should be empty after clear_context().");
    assert_true(cleared.fields.empty(),
                "Context fields should be empty after clear_context().");
  }

  void test_configure()
  {
    using namespace vix::log;

    LogConfig config;
    config.level = LogLevel::Critical;
    config.format = LogFormat::KV;
    config.async = false;

    configure(config);

    assert_true(level() == LogLevel::Critical,
                "configure() should apply configured log level.");
  }

  void test_adapter_instance_and_state()
  {
    using namespace vix::log;

    LogAdapter &a = LogAdapter::instance();
    LogAdapter &b = LogAdapter::instance();

    assert_true(&a == &b,
                "LogAdapter::instance() should return the same singleton instance.");

    a.set_level(LogLevel::Info);
    assert_true(a.level() == LogLevel::Info,
                "Adapter level should reflect set_level(Info).");
    assert_true(a.enabled(LogLevel::Info),
                "Info should be enabled when adapter level is Info.");
    assert_true(!a.enabled(LogLevel::Trace),
                "Trace should be disabled when adapter level is Info.");
  }

  void test_logging_calls_smoke()
  {
    using namespace vix::log;

    set_level(LogLevel::Trace);
    set_format(LogFormat::KV);
    set_async(false);

    trace("trace message {}", 1);
    debug("debug message {}", 2);
    info("info message {}", 3);
    warn("warn message {}", 4);
    error("error message {}", 5);
    critical("critical message {}", 6);

    log(LogLevel::Info, "generic log {}", 7);
    logf(LogLevel::Info, "structured message", "status", 200, "method", "GET");

    assert_true(true,
                "Smoke logging calls should complete without crashing.");
  }

} // namespace

int main()
{
  test_parse_level();
  test_parse_format();
  test_set_and_get_level();
  test_enabled();
  test_context_roundtrip();
  test_configure();
  test_adapter_instance_and_state();
  test_logging_calls_smoke();

  std::cout << "[PASS] vix_log_test\n";
  return EXIT_SUCCESS;
}
