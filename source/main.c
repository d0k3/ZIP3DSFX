#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/dirent.h>
#include "archive_zip.h"
#define MINIZ_HEADER_FILE_ONLY
#include "miniz.c"
#include "hid.h"


void rmkdir (const char* path) {
    char tmp[256];
    size_t len;
    char* p;

    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    for (p = tmp + 1; *p; p++) {
        if((*p == '/') || (*p == '\\')) {
            char s = *p;
            *p = 0;
            mkdir(tmp, 0777);
            *p = s;
        }
    }
    mkdir(path, 0777);
}

s32 main (int argc, char **argv) {
    char path[256] = { 0 };
    mz_zip_archive mz_archive;
    mz_uint n_files;
       
    // Initialize GFX services / console
    gfxInitDefault();
    gfxSwapBuffers();
    consoleInit(GFX_BOTTOM, NULL );

    // Say hello
    printf ("** ZIP3DSFX v0.1 by d0k3 **\n\n");
    
    // init internal ZIP archive
    memset(&mz_archive, 0, sizeof(mz_archive));
    if (!mz_zip_reader_init_mem(&mz_archive, archive_zip, archive_zip_size, 0) || !(n_files = mz_zip_reader_get_num_files(&mz_archive))) {
        printf(" ERROR opening internal archive!\n");
    } else {
        u32 n;
        for (n = 0; n < n_files; n++) {
            mz_zip_archive_file_stat mz_stat;
            if (!mz_zip_reader_file_stat(&mz_archive, n, &mz_stat)) {
               printf(" ERROR in internal ZIP structure!\n");
               break;
            }
            printf(" %s\n", mz_stat.m_filename);
            if (!mz_zip_reader_is_file_a_directory(&mz_archive, n)) {
                if (!mz_zip_reader_extract_to_file(&mz_archive, n, mz_stat.m_filename, 0)) {
                    printf(" ERROR extracting file!\n");
                    break;
                }
            } else rmkdir(mz_stat.m_filename);
        }
        if (n == n_files) printf(" -> Done !\n", n);
    }
    mz_zip_reader_end(&mz_archive);
    printf("\nPress any key to reboot");
    wait_key();
    
    // Deinitialize GFX
    gfxExit();
    
    return 0;
}
