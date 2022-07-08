set(FICHIERS_F90 filetyp.f90)

add_executable(r.filetype ${FICHIERS_F90})
target_link_libraries(r.filetype PRIVATE rmn) 

install(TARGETS r.filetype
    RUNTIME DESTINATION bin)