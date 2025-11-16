cmake_minimum_required(VERSION 3.10)
project(MiniDJ)

set(CMAKE_CXX_STANDARD 14)

# ----------------------------
# 构建可执行程序
# ----------------------------
add_executable(MiniDJ
    main.cpp
    Song.cpp
)

# ----------------------------
# 启用测试
# ----------------------------
enable_testing()

# 测试编号列表
set(TEST_CASES 1 2 3 4 5 6 7 8)

foreach(TEST_NUM ${TEST_CASES})
    set(OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/output_${TEST_NUM}.txt)

    # 生成输出文件
    add_custom_command(
        OUTPUT ${OUTPUT_FILE}
        COMMAND MiniDJ
                < ${CMAKE_CURRENT_SOURCE_DIR}/testcases/input_${TEST_NUM}.txt
                > ${OUTPUT_FILE}
        DEPENDS MiniDJ
        COMMENT "Running test ${TEST_NUM}..."
    )

    # 为每个测试生成 custom target
    add_custom_target(run_test_${TEST_NUM}
        DEPENDS ${OUTPUT_FILE}
    )

    # 添加 CTest 测试
    add_test(
        NAME test_${TEST_NUM}
        COMMAND ${CMAKE_COMMAND} -E compare_files
            ${CMAKE_CURRENT_SOURCE_DIR}/testcases/expected_${TEST_NUM}.txt
            ${OUTPUT_FILE}
    )

    # 设置 CTest 测试依赖
    set_tests_properties(test_${TEST_NUM}
        PROPERTIES DEPENDS run_test_${TEST_NUM}
    )
endforeach()
