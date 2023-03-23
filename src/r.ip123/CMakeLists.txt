set(FICHIERS_F90 r.ip123.f90)

add_executable(r.ip123 ${FICHIERS_F90})
target_link_libraries(r.ip123 PRIVATE rmn::rmn) 

install(TARGETS r.ip123
    RUNTIME DESTINATION bin)