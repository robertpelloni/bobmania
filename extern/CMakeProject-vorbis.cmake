set(VORBIS_SRC "vorbis/lib/analysis.c"
               "vorbis/lib/bitrate.c"
               "vorbis/lib/block.c"
               "vorbis/lib/codebook.c"
               "vorbis/lib/envelope.c"
               "vorbis/lib/floor0.c"
               "vorbis/lib/floor1.c"
               "vorbis/lib/info.c"
               "vorbis/lib/lookup.c"
               "vorbis/lib/lpc.c"
               "vorbis/lib/lsp.c"
               "vorbis/lib/mapping0.c"
               "vorbis/lib/mdct.c"
               "vorbis/lib/psy.c"
               "vorbis/lib/registry.c"
               "vorbis/lib/res0.c"
               "vorbis/lib/sharedbook.c"
               "vorbis/lib/smallft.c"
               "vorbis/lib/synthesis.c"
               "vorbis/lib/vorbisenc.c"
               "vorbis/lib/window.c")

<<<<<<< HEAD
set(VORBIS_HPP "vorbis/lib/backends.h"
               "vorbis/lib/bitrate.h"
               "vorbis/lib/codebook.h"
               "vorbis/include/vorbis/codec.h"
               "vorbis/lib/codec_internal.h"
               "vorbis/lib/envelope.h"
               "vorbis/lib/modes/floor_all.h"
               "vorbis/lib/books/floor/floor_books.h"
               "vorbis/lib/highlevel.h"
               "vorbis/lib/lookup.h"
               "vorbis/lib/lookup_data.h"
               "vorbis/lib/lpc.h"
               "vorbis/lib/lsp.h"
               "vorbis/lib/masking.h"
               "vorbis/lib/mdct.h"
               "vorbis/lib/misc.h"
               "vorbis/lib/os.h"
               "vorbis/lib/psy.h"
               "vorbis/lib/modes/psych_11.h"
               "vorbis/lib/modes/psych_16.h"
               "vorbis/lib/modes/psych_44.h"
               "vorbis/lib/modes/psych_8.h"
               "vorbis/lib/registry.h"
               "vorbis/lib/books/coupled/res_books_stereo.h"
               "vorbis/lib/books/uncoupled/res_books_uncoupled.h"
               "vorbis/lib/modes/residue_16.h"
               "vorbis/lib/modes/residue_44.h"
               "vorbis/lib/modes/residue_44u.h"
               "vorbis/lib/modes/residue_8.h"
               "vorbis/lib/scales.h"
               "vorbis/lib/modes/setup_11.h"
               "vorbis/lib/modes/setup_16.h"
               "vorbis/lib/modes/setup_22.h"
               "vorbis/lib/modes/setup_32.h"
               "vorbis/lib/modes/setup_44.h"
               "vorbis/lib/modes/setup_44u.h"
               "vorbis/lib/modes/setup_8.h"
               "vorbis/lib/modes/setup_X.h"
               "vorbis/lib/window.h")
=======
list(APPEND VORBIS_SRC
            "${VORBIS_DIR}/lib/analysis.c"
            "${VORBIS_DIR}/lib/bitrate.c"
            "${VORBIS_DIR}/lib/block.c"
            "${VORBIS_DIR}/lib/codebook.c"
            "${VORBIS_DIR}/lib/envelope.c"
            "${VORBIS_DIR}/lib/floor0.c"
            "${VORBIS_DIR}/lib/floor1.c"
            "${VORBIS_DIR}/lib/info.c"
            "${VORBIS_DIR}/lib/lookup.c"
            "${VORBIS_DIR}/lib/lpc.c"
            "${VORBIS_DIR}/lib/lsp.c"
            "${VORBIS_DIR}/lib/mapping0.c"
            "${VORBIS_DIR}/lib/mdct.c"
            "${VORBIS_DIR}/lib/psy.c"
            "${VORBIS_DIR}/lib/registry.c"
            "${VORBIS_DIR}/lib/res0.c"
            "${VORBIS_DIR}/lib/sharedbook.c"
            "${VORBIS_DIR}/lib/smallft.c"
            "${VORBIS_DIR}/lib/synthesis.c"
            "${VORBIS_DIR}/lib/vorbisenc.c"
            "${VORBIS_DIR}/lib/window.c")

list(APPEND VORBIS_HPP
            "${VORBIS_DIR}/lib/backends.h"
            "${VORBIS_DIR}/lib/bitrate.h"
            "${VORBIS_DIR}/lib/codebook.h"
            "${VORBIS_DIR}/include/vorbis/codec.h"
            "${VORBIS_DIR}/lib/codec_internal.h"
            "${VORBIS_DIR}/lib/envelope.h"
            "${VORBIS_DIR}/lib/modes/floor_all.h"
            "${VORBIS_DIR}/lib/books/floor/floor_books.h"
            "${VORBIS_DIR}/lib/highlevel.h"
            "${VORBIS_DIR}/lib/lookup.h"
            "${VORBIS_DIR}/lib/lookup_data.h"
            "${VORBIS_DIR}/lib/lpc.h"
            "${VORBIS_DIR}/lib/lsp.h"
            "${VORBIS_DIR}/lib/masking.h"
            "${VORBIS_DIR}/lib/mdct.h"
            "${VORBIS_DIR}/lib/misc.h"
            "${VORBIS_DIR}/lib/os.h"
            "${VORBIS_DIR}/lib/psy.h"
            "${VORBIS_DIR}/lib/modes/psych_11.h"
            "${VORBIS_DIR}/lib/modes/psych_16.h"
            "${VORBIS_DIR}/lib/modes/psych_44.h"
            "${VORBIS_DIR}/lib/modes/psych_8.h"
            "${VORBIS_DIR}/lib/registry.h"
            "${VORBIS_DIR}/lib/books/coupled/res_books_stereo.h"
            "${VORBIS_DIR}/lib/books/uncoupled/res_books_uncoupled.h"
            "${VORBIS_DIR}/lib/modes/residue_16.h"
            "${VORBIS_DIR}/lib/modes/residue_44.h"
            "${VORBIS_DIR}/lib/modes/residue_44u.h"
            "${VORBIS_DIR}/lib/modes/residue_8.h"
            "${VORBIS_DIR}/lib/scales.h"
            "${VORBIS_DIR}/lib/modes/setup_11.h"
            "${VORBIS_DIR}/lib/modes/setup_16.h"
            "${VORBIS_DIR}/lib/modes/setup_22.h"
            "${VORBIS_DIR}/lib/modes/setup_32.h"
            "${VORBIS_DIR}/lib/modes/setup_44.h"
            "${VORBIS_DIR}/lib/modes/setup_44u.h"
            "${VORBIS_DIR}/lib/modes/setup_8.h"
            "${VORBIS_DIR}/lib/modes/setup_X.h"
            "${VORBIS_DIR}/lib/window.h")
>>>>>>> main

source_group("Source Files" FILES ${VORBIS_SRC})
source_group("Header Files" FILES ${VORBIS_HPP})

add_library("vorbis" STATIC ${VORBIS_SRC} ${VORBIS_HPP})

set_property(TARGET "vorbis" PROPERTY FOLDER "External Libraries")

disable_project_warnings("vorbis")

<<<<<<< HEAD
target_include_directories("vorbis" PUBLIC
  "ogg"
  "vorbis/lib"
  "vorbis/include"
)
=======
list(APPEND VORBIS_INCLUDE_DIRS
            "${VORBIS_DIR}/lib"
            "${VORBIS_DIR}/include"
            "${SM_EXTERN_DIR}/newogg/include")

target_include_directories("vorbis" PUBLIC ${VORBIS_INCLUDE_DIRS})

list(APPEND VORBIS_LINK_LIBS "ogg")
>>>>>>> main

target_link_libraries("vorbis" "ogg")
