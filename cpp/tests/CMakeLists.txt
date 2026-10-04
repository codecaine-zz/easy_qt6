add_executable(simplegui_api_smoke_test api_smoke_test.cpp)
target_link_libraries(simplegui_api_smoke_test PRIVATE simplegui)

add_test(NAME api_smoke_test COMMAND simplegui_api_smoke_test)
# Run headless so the tests work on CI machines without a display.
set_tests_properties(api_smoke_test PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
