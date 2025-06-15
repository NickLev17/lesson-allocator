cmake_minimum_required(VERSION 3.12)


set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(alloc VERSION ${PROJECT_VERSION})

set(SOURCES main.cpp)
set(HEADERS MyVector.h
           MyAllocator.h)


configure_file(version.h.in version.h)


add_executable(allocator ${SOURCES} ${HEADERS})


set_target_properties(allocator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)


target_include_directories(allocator
    PRIVATE "${CMAKE_BINARY_DIR}"
)


if (MSVC)
    target_compile_options(allocator PRIVATE /W4 )
else()
    target_compile_options(allocator PRIVATE -Wall -Wextra  -pedantic) 
endif()


install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

