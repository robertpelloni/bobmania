cmake_minimum_required(VERSION 3.5)
project(StepMania)

<<<<<<< HEAD
# Forward to the actual src directory
=======
include(StepmaniaCore.cmake)

# The external libraries need to be included.
add_subdirectory(extern)
add_subdirectory(extern/bobcoin)

# The internal libraries and eventual executable to be used.
>>>>>>> origin/unified-ui-features-13937230807013224518
add_subdirectory(src)
