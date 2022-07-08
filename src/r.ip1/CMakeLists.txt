set(FICHIERS_C r.ip.c)

add_executable(r.ip1 ${FICHIERS_C})
target_link_libraries(r.ip1 PRIVATE rmn) 

install(TARGETS r.ip1
    RUNTIME DESTINATION bin)