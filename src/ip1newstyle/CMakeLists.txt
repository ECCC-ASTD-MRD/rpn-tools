set(FICHIERS_F90 ip1newstyle.f90)

add_executable(ip1newstyle ${FICHIERS_F90})
target_link_libraries(ip1newstyle PRIVATE rmn::rmn) 

install(TARGETS ip1newstyle
    RUNTIME DESTINATION bin)