function(add_cpp_module module_name)
  cmake_parse_arguments(ARG "" "" "FILES;LIBS;PACKAGES" ${ARGN})

  if(NOT ARG_FILES)
    message(
      FATAL_ERROR "add_cpp_module requires at least one source file via FILES")
  endif()

  add_library(${module_name})

  # Optionally find required packages
  foreach(pkg ${ARG_PACKAGES})
    find_package(${pkg} CONFIG REQUIRED)
  endforeach()

  string(TOLOWER ${module_name} file_set_name)

  # Add module source files
  target_sources(${module_name} PUBLIC FILE_SET ${file_set_name}_files TYPE
                                       CXX_MODULES FILES ${ARG_FILES})

  target_compile_features(${module_name} PUBLIC cxx_std_26)

  if(CXX_ALL_FLAGS)
    target_compile_options(${module_name} PRIVATE ${CXX_ALL_FLAGS})
  endif()

  # Link to given libraries
  target_link_libraries(${module_name} PUBLIC ${CXX_SANITIZERS_FLAGS}
                                              ${ARG_LIBS})
endfunction()
