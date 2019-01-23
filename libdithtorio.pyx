# cython: language_level=3
cimport clibdithtorio

def process_file(srcFile, dstFile, bprFile, forceNT = False, threshold = 0, split = 32768):
    cdef bytes srcFile_bytes = srcFile.encode()
    cdef char* srcFile_ptr = srcFile_bytes

    cdef bytes dstFile_bytes = dstFile.encode()
    cdef char* dstFile_ptr = dstFile_bytes

    cdef bytes bprFile_bytes = bprFile.encode()
    cdef char* bprFile_ptr = bprFile_bytes

    cdef bint forceNT_bool = (1 if forceNT else 0)
    cdef unsigned char threshold_uchar = threshold
    cdef unsigned int split_uint = split

    clibdithtorio.process_file(srcFile_ptr, dstFile_ptr, bprFile_ptr, forceNT_bool, threshold_uchar, split_uint)

def add_factorio_entity(name, red, green, blue):
    cdef bytes name_bytes = name.encode()
    cdef char* name_ptr = name_bytes

    cdef unsigned char red_uchar = red
    cdef unsigned char green_uchar = green
    cdef unsigned char blue_uchar = blue

    clibdithtorio.add_factorio_entity(name_ptr, red, green, blue)

def add_factorio_tile(name, red, green, blue):
    cdef bytes name_bytes = name.encode()
    cdef char* name_ptr = name_bytes

    cdef unsigned char red_uchar = red
    cdef unsigned char green_uchar = green
    cdef unsigned char blue_uchar = blue

    clibdithtorio.add_factorio_entity(name_ptr, red, green, blue)

def clear_data():
    clibdithtorio.clear_data()

def preload_modded():
    clibdithtorio.preload_modded()

def preload_vanilla():
    clibdithtorio.preload_vanilla()
