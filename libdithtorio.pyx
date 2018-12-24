# cython: language_level=3
cimport clibdithtorio

def process_file(srcFile, dstFile, bprFile, threshold = 127, split = 65536):
    cdef bytes srcFile_bytes = srcFile.encode();
    cdef char* srcFile_ptr = srcFile_bytes;

    cdef bytes dstFile_bytes = dstFile.encode();
    cdef char* dstFile_ptr = dstFile_bytes;

    cdef bytes bprFile_bytes = bprFile.encode();
    cdef char* bprFile_ptr = bprFile_bytes;

    cdef unsigned char threshold_uchar = threshold
    cdef unsigned int split_uint = split

    clibdithtorio.process_file(srcFile_ptr, dstFile_ptr, bprFile_ptr, threshold_uchar, split_uint)
