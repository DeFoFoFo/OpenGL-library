cmake_minimum_required(VERSION 3.15)

include(FetchContent)

# Pin Assimp version
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v6.0.4
)

# --- Configure Assimp options before building ---
# Force static build
set(ASSIMP_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
# Disable warnings as errors (important for MinGW/GCC)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
# Disable everything we don't need
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE) # turn ON if needed
set(ASSIMP_BUILD_FBX_IMPORTER OFF CACHE BOOL "" FORCE)  # turn ON if needed
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
# Force assimp to use zlib for unzip.h
set(ASSIMP_BUILD_ZLIB ON)

# Make it available
FetchContent_MakeAvailable(assimp)
