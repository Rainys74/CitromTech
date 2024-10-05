project(CitromTech)

# Recursively search for all source files (.cpp)
file(GLOB_RECURSE CITROMTECH_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/CitromTech/src/*.cpp"
)

add_library(CitromTech SHARED ${CITROMTECH_SOURCES})

target_include_directories(CitromTech PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/CitromTech/src)