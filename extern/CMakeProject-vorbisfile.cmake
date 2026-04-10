set(VORBISFILE_SRC "vorbis/lib/vorbisfile.c")

<<<<<<< HEAD
set(VORBISFILE_HPP "vorbis/include/vorbis/vorbisfile.h")
=======
list(APPEND VORBISFILE_SRC "${VORBISFILE_DIR}/lib/vorbisfile.c")

list(APPEND VORBISFILE_HPP "${VORBISFILE_DIR}/include/vorbis/vorbisfile.h")
>>>>>>> main

source_group("Source Files" FILES ${VORBISFILE_SRC})
source_group("Header Files" FILES ${VORBISFILE_HPP})

add_library("vorbisfile" STATIC ${VORBISFILE_SRC} ${VORBISFILE_HPP})

set_property(TARGET "vorbisfile" PROPERTY FOLDER "External Libraries")

disable_project_warnings("vorbisfile")

<<<<<<< HEAD
target_include_directories("vorbisfile" PUBLIC "vorbis/include")

target_link_libraries("vorbisfile" "ogg")
=======
list(APPEND VORBIS_INCLUDE_DIRS "${VORBISDIR_DIR}/include")

target_include_directories("vorbisfile" PUBLIC ${VORBIS_INCLUDE_DIRS})

list(APPEND VORBISFILE_LINK_LIBS "ogg")

target_link_libraries("vorbisfile" ${VORBISFILE_LINK_LIBS})
>>>>>>> main
