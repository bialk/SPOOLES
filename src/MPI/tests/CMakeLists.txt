add_executable(tester_spooles_mpi_bcast test_mpi_bcast.c)
target_link_libraries(tester_spooles_mpi_bcast PRIVATE SPOOLES::spooles)

add_test(NAME spooles_mpi_bcast
  COMMAND ${MPIEXEC_EXECUTABLE}
    ${MPIEXEC_NUMPROC_FLAG} 2
    ${MPIEXEC_PREFLAGS}
    $<TARGET_FILE:tester_spooles_mpi_bcast>
    ${MPIEXEC_POSTFLAGS})
set_tests_properties(spooles_mpi_bcast PROPERTIES PROCESSORS 2)
