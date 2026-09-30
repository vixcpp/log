/**
 * @file ConsoleSync.hpp
 * @brief Shared console synchronization used by the logging implementation.
 */
#ifndef VIX_LOG_CONSOLE_SYNC_HPP
#define VIX_LOG_CONSOLE_SYNC_HPP

#include <condition_variable>
#include <mutex>

namespace vix::log
{
  inline std::mutex &console_mutex()
  {
    static std::mutex mutex;
    return mutex;
  }

  inline std::mutex &banner_mutex()
  {
    static std::mutex mutex;
    return mutex;
  }

  inline std::condition_variable &console_cv()
  {
    static std::condition_variable cv;
    return cv;
  }

  inline bool &console_banner_done()
  {
    static bool done = true;
    return done;
  }

  inline void console_wait_banner()
  {
    std::unique_lock<std::mutex> lock(banner_mutex());
    console_cv().wait(lock, [] { return console_banner_done(); });
  }

  inline void console_mark_banner_done()
  {
    {
      std::lock_guard<std::mutex> lock(banner_mutex());
      console_banner_done() = true;
    }
    console_cv().notify_all();
  }

  inline void console_reset_banner()
  {
    std::lock_guard<std::mutex> lock(banner_mutex());
    console_banner_done() = false;
  }
}

#endif // VIX_LOG_CONSOLE_SYNC_HPP
