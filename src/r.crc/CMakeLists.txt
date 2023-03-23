set(FICHIERS_C r.crc.c)

add_executable(r.crc ${FICHIERS_C})
target_link_libraries(r.crc PRIVATE rmn::rmn) 

install(TARGETS r.crc
    RUNTIME DESTINATION bin)