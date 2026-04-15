add_executable(vix_log_test
  log_test.cpp
)

target_compile_features(vix_log_test PRIVATE cxx_std_20)

target_link_libraries(vix_log_test
  PRIVATE
    vix::log
)

add_test(NAME vix_log_test COMMAND vix_log_test)
