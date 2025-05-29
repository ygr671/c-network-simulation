cmake_minimum_required(VERSION 3.10)
project(main C)

# Répertoires
set(SRC_DIR src)
set(INC_DIR ${SRC_DIR}/include)
set(BUILD_DIR build)

# Options de compilation
set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Wextra -Werror -pedantic)

# Inclure les headers
include_directories(${INC_DIR})

# Fichiers sources automatiquement
file(GLOB SRCS ${SRC_DIR}/*.c)

# Déclaration de l'exécutable
add_executable(main ${SRCS})

# Optionnel : définir le dossier de sortie
set_target_properties(main PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
