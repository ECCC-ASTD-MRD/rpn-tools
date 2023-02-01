set(FICHIERS_F90 filetyp.f90)

add_executable(r.filetype ${FICHIERS_F90})
target_link_libraries(r.filetype PRIVATE rmn)
add_dependencies(r.filetype rpn-tools_build_info)

if(BUILD_TESTING)
    add_dependencies(check r.filetype)
    add_test(
        NAME r.filetype-nofile-expect-fail
        COMMAND ${CMAKE_SOURCE_DIR}/test/expect-exit-code.sh 253 $<TARGET_FILE:r.filetype>)
endif()

install(TARGETS r.filetype
    RUNTIME DESTINATION bin)