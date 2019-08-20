#-------------------------------------------------------
# Slurm job launching script generator
#
# We would like to generate a script which can be used to generate batch jobs
# to submit benchmarks to slurm using many combinations of settings.
#
# We add a custom command which takes our template script and
# expands out all the variables we need to pass into it.
# Unfortunately, due to the way cmake works, some variables are only known
# at build time, and not at cmake configure time. Using a custom command which
# calls cmake to run our script at build time, allows us to pass variables
# into the final script which is placed in our build dir.
#
# Note that we generate these scripts in the build dir instead
# of the install dir as they are intended for development testing.
# A version could be supported for installation later.
#-------------------------------------------------------

set(BENCHMARK_SCRIPTS_PATH
  "${CMAKE_BINARY_DIR}/scripts"
  CACHE PATH
  "Directory to place batch scripts in"
)

# Make sure scripts dir exists
execute_process(COMMAND "${CMAKE_COMMAND}" -E make_directory "${BENCHMARK_SCRIPTS_PATH}/scripts")

#--------------------------------------------------
# Slurm script generator for benchmarks
#--------------------------------------------------
set(SCRIPTS "parallel_for")

foreach(script ${SCRIPTS})
  ADD_CUSTOM_COMMAND(
    DEPENDS   "${CMAKE_CURRENT_SOURCE_DIR}/${script}.sh.in"
    COMMAND   "${CMAKE_COMMAND}"
    ARGS      -DSCRIPT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
              -DSCRIPT_NAME=${script}
              -DSCRIPT_DEST_DIR="${BENCHMARK_SCRIPTS_PATH}"
              -DBINARY_DIR="${CMAKE_BINARY_DIR}/bin"
              -P "${CMAKE_CURRENT_SOURCE_DIR}/copy_script.cmake"
    OUTPUT    "${BENCHMARK_SCRIPTS_PATH}/${script}.sh"
    VERBATIM
  )

  add_custom_target(script-${script}
    DEPENDS "${BENCHMARK_SCRIPTS_PATH}/${script}.sh"
  )
endforeach(script)
