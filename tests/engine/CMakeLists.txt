add_executable(engine_tester "../tests_main.cpp")

target_sources(engine_tester
    PRIVATE
        "core/component_uuid_tests.cpp"
    )

target_link_libraries(engine_tester PRIVATE catch engine)

add_test(NAME engine_tests COMMAND engine_tester)