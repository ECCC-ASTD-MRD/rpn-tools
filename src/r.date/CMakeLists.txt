set(FICHIERS_C r.date.c)

add_executable(r.date ${FICHIERS_C})
add_dependencies(r.date rpn-tools_build_info)
target_link_libraries(r.date PRIVATE rmn::rmn) 
set_target_properties(r.date PROPERTIES 
   COMPILE_DEFINITIONS RDATE_VERSION="${RDATE_VERSION}")

install(TARGETS r.date
    RUNTIME DESTINATION bin)