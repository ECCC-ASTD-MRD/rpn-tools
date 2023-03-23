set(FICHIERS_C r.date.c)

add_executable(r.date ${FICHIERS_C})
target_link_libraries(r.date PRIVATE rmn::rmn) 

install(TARGETS r.date
    RUNTIME DESTINATION bin)