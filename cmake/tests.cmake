enable_testing()

set(SuccessTests 
    basic_run 
    hello_world
    two_words_in_two_sentences
    problem_statement_correct
)

set(FailTests 
    problem_statement_original_wrong
)

foreach(test ${SuccessTests})
    add_test(NAME ${test} COMMAND ${PROJECT_NAME} ../tests/${test}/input ../tests/${test}/output)
endforeach()

foreach(test ${FailTests})
    add_test(NAME ${test} COMMAND ${PROJECT_NAME} ../tests/${test}/input ../tests/${test}/output)
    set_tests_properties(${test} PROPERTIES WILL_FAIL True)
endforeach()

