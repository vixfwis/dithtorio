cdef extern from "dithtorio.h":
    void process_file(const char* srcFile, const char* dstFile, const char* bprFile, bint forceNT, unsigned char threshold, unsigned int split)
    void add_factorio_entity(const char* name, unsigned char red, unsigned char green, unsigned char blue)
    void add_factorio_tile(const char* name, unsigned char red, unsigned char green, unsigned char blue)
    void clear_data()
    void preload_modded()
    void preload_vanilla()
