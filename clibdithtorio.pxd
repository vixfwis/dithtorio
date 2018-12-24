cdef extern from "dithering.h":
    void process_file(const char* srcFile, const char* dstFile, const char* bprFile, unsigned char threshold, unsigned int split)
