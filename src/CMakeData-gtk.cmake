if(NOT GTK3_FOUND)
  return()
endif()

<<<<<<< HEAD
add_library("LoadingWindowGtk"
            OBJECT
            "arch/LoadingWindow/LoadingWindow_Gtk.cpp"
            "arch/LoadingWindow/LoadingWindow_Gtk.h")

set_property(TARGET "LoadingWindowGtk" PROPERTY FOLDER "Internal Libraries")
set_property(TARGET "LoadingWindowGtk" PROPERTY CXX_STANDARD 17)
set_property(TARGET "LoadingWindowGtk" PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET "LoadingWindowGtk" PROPERTY CXX_EXTENSIONS ON)
=======
add_library("GtkModule"
            SHARED
            "arch/LoadingWindow/LoadingWindow_GtkModule.cpp"
            "arch/LoadingWindow/LoadingWindow_GtkModule.h")

sm_add_compile_flag("GtkModule" "-std=${SM_CPP_STANDARD}")

# It is normally not appropriate to set the prefix to the empty string. This is
# to maintain compatibility with the current source. At some point, it may be
# worth being more flexible.
set_target_properties("GtkModule" PROPERTIES PREFIX "")
set_target_properties("GtkModule" PROPERTIES OUTPUT_NAME "GtkModule")
set_target_properties("GtkModule"
                      PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${SM_ROOT_DIR}")
set_target_properties("GtkModule"
                      PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE
                                 "${SM_ROOT_DIR}")
set_target_properties(
  "GtkModule"
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${SM_ROOT_DIR}")
set_target_properties("GtkModule"
                      PROPERTIES LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL
                                 "${SM_ROOT_DIR}")
set_target_properties("GtkModule"
                      PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO
                                 "${SM_ROOT_DIR}")
target_link_libraries("GtkModule" ${GTK3_LIBRARIES})
set_property(TARGET "GtkModule" PROPERTY FOLDER "Internal Libraries")
list(APPEND SM_GTK_INCLUDE_DIRS
            "${SM_SRC_DIR}"
            "${SM_SRC_DIR}/generated"
            "${SM_SRC_DIR}/arch/LoadingWindow"
            "${GTK3_INCLUDE_DIRS}")
>>>>>>> main

target_include_directories("LoadingWindowGtk" PRIVATE "${SM_SRC_DIR}"
                                                      "${SM_GENERATED_SRC_DIR}"
                                                      "${SM_SRC_DIR}/arch/LoadingWindow"
                                                      "${GTK3_INCLUDE_DIRS}")

target_link_libraries("LoadingWindowGtk" ${GTK3_LIBRARIES})

list(APPEND SMDATA_LINK_LIB "LoadingWindowGtk")
list(APPEND SMDATA_ARCH_LOADING_HPP
            "arch/LoadingWindow/LoadingWindow_Gtk.h")
