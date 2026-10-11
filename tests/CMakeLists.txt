# Tests
enable_testing()

add_executable(EntityManagerTests
   EntityManagerTests.cpp
)

target_compile_features(EntityManagerTests PUBLIC cxx_std_26)
target_compile_options(EntityManagerTests PRIVATE ${CXX_ALL_FLAGS})
target_link_libraries(EntityManagerTests PUBLIC TinyEngine_EntityManager ${CXX_SANITIZERS_FLAGS})

add_test(
   NAME EntityManagerTests
   COMMAND EntityManagerTests
)
