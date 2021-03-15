aux_source_directory(. CoreFiles)
MESSAGE(STATUS ${CoreFiles})
add_library(core ${CoreFiles})