
# Since most of the applications are server applications they do not return a value
# but waits for requests. To check if they are running successfully, a PassedTimeInMillisWithStop exe which depends on
# the actual app will run. If the app runs successfully it will pass otherwise it will fail.

function(CREATE_APP_TEST APP_NAME)

    message("Creating app test for: ${APP_NAME}")

    get_property(CMD GLOBAL PROPERTY TEST_EXEC)
    add_test(
            NAME "${APP_NAME}_TEST"
            COMMAND ${CMD}
    )
    set_tests_properties("${APP_NAME}_TEST" PROPERTIES DEPENDS ${APP_NAME})

endfunction()