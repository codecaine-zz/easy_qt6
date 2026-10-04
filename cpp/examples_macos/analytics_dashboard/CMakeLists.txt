add_executable(example_analytics_dashboard
    main.cpp
)

target_link_libraries(example_analytics_dashboard PRIVATE simplegui::simplegui)

if(APPLE)
    set_target_properties(example_analytics_dashboard PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()
