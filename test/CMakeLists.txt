add_executable(vix_log_test
  log_test.cpp
)

target_compile_features(vix_log_test PRIVATE cxx_std_20)

target_link_libraries(vix_log_test
  PRIVATE
    vix::log
)

if (VIX_ENABLE_SANITIZERS AND NOT MSVC)
  target_compile_options(vix_log_test
    PRIVATE
      -fno-omit-frame-pointer
      -fsanitize=address,undefined
  )

  target_link_options(vix_log_test
    PRIVATE
      -fsanitize=address,undefined
  )
endif()

add_test(NAME vix_log_test COMMAND vix_log_test)
