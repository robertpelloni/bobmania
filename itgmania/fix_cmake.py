with open("CMake/SMDefs.cmake", "r") as f:
    content = f.read()

head_part = """file(READ "${SM_ROOT_DIR}/VERSION.md" SM_VERSION_STRING)
string(STRIP "${SM_VERSION_STRING}" SM_VERSION_STRING)
string(REPLACE "." ";" SM_VERSION_LIST "${SM_VERSION_STRING}")
list(GET SM_VERSION_LIST 0 SM_VERSION_MAJOR)
list(GET SM_VERSION_LIST 1 SM_VERSION_MINOR)
list(GET SM_VERSION_LIST 2 SM_VERSION_PATCH)

"""

conflict_block = """file(READ "${SM_ROOT_DIR}/VERSION.md" SM_VERSION_STRING)
string(STRIP "${SM_VERSION_STRING}" SM_VERSION_STRING)
string(REPLACE "." ";" SM_VERSION_LIST "${SM_VERSION_STRING}")
list(GET SM_VERSION_LIST 0 SM_VERSION_MAJOR)
list(GET SM_VERSION_LIST 1 SM_VERSION_MINOR)
list(GET SM_VERSION_LIST 2 SM_VERSION_PATCH)


content = content.replace(conflict_block, head_part)

with open("CMake/SMDefs.cmake", "w") as f:
    f.write(content)
