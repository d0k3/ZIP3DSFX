#include <3ds.h>
#include <sys/dirent.h>
#include "config.h"
#include "hid.h"
#define MINIZ_HEADER_FILE_ONLY
#include "miniz.c"
#ifdef FORCE_ROOT
#include <sys/unistd.h>
#endif
#ifndef SFX_STUB
#include "archive_zip.h"
#else
#include "selfsize.h"
#endif
#if defined SFX_STUB || !defined OVERWRITE_ALWAYS
#include <stdio.h>
#endif


#ifdef NO_CREATE_DIRS
// check if the containing dir exists
bool dir_exists(const char* path) {
    bool res = false;
    char tmp[256];
    char* p;
    
    snprintf(tmp, sizeof(tmp), "%s", path);
    for (p = tmp + strnlen(tmp, 256) - 1; p > tmp; p--) {
        if((*p == '/') || (*p == '\\')) {
            DIR* dir;
            char s = *p;
            *p = 0;
            res = (dir = opendir(tmp));
            if (dir) closedir(dir);
            *p = s;
            break;
        }
    }
    
    return res;
}
#endif

#ifndef OVERWRITE_ALWAYS
// checks if a file exists
bool file_exists (const char* path) {
    bool res = false;
    FILE* fp;
    
    res = (fp = fopen(path, "rb"));
    if (fp) fclose(fp);
    
    return res;
}
#endif

// recursively creates a directory tree
void rmkdir (const char* path) {
    char tmp[256];
    char* p;

    snprintf(tmp, sizeof(tmp), "%s", path);
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

#ifdef SFX_STUB
// reads a file into memory
u32 read_file_to_mem (u8** data, const char* path, u32 offset) {
    FILE* fp;
    u32 file_size;
    
    fp = fopen(path, "rb");
    if (fp == NULL) return 0;
    
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    if (file_size <= offset) {
        fclose(fp);
        return 0;
    }
    fseek(fp, offset, SEEK_SET);
    file_size -= offset;
    
    *data = (u8*) malloc(file_size);
    if (*data == NULL) {
        fclose(fp);
        return 0;
    }
    
    if (fread(*data, 1, file_size, fp) != file_size) {
        free(*data);
        fclose(fp);
        return 0;
    }
    
    fclose(fp);
    
    return file_size;
}
#endif

s32 main (int argc, char **argv) {
    mz_zip_archive mz_archive;
    mz_uint n_files;
       
    // Initialize GFX services / console
    gfxInitDefault();
    gfxSwapBuffers();
    consoleInit(GFX_BOTTOM, NULL );

    // Say hello
    printf ("** ZIP3DSFX v%i.%i by d0k3 **\n", ZIP3DSFX_VER / 10, ZIP3DSFX_VER % 10);
    
    #ifdef SFX_STUB
    u8* archive_zip = NULL;
    u32 archive_zip_size = 0;
    if (argc < 1) {
        printf("\n ! ERROR loader not compatible with stub!");
    } else {
        archive_zip_size = read_file_to_mem(&archive_zip, *argv, SELF_SIZE);
    }
    #endif
    
    #ifdef FORCE_ROOT
    chdir("/");
    #endif
    
    // init internal ZIP archive
    memset(&mz_archive, 0, sizeof(mz_archive));
    if (!mz_zip_reader_init_mem(&mz_archive, archive_zip, archive_zip_size, 0) || !(n_files = mz_zip_reader_get_num_files(&mz_archive))) {
        printf("\n ! ERROR opening internal archive!");
    } else {
        u32 n;
        u32 n_x = 0;
        u32 n_s = 0;
        u32 n_d = 0;
        for (n = 0; n < n_files; n++) {
            printf("\n");
            mz_zip_archive_file_stat mz_stat;
            if (!mz_zip_reader_file_stat(&mz_archive, n, &mz_stat)) {
               printf(" ! ERROR in internal ZIP structure!");
               break;
            }
            printf(" - %s\r", mz_stat.m_filename);
            #ifdef NO_CREATE_DIRS
            if (mz_zip_reader_is_file_a_directory(&mz_archive, n) || !dir_exists(mz_stat.m_filename)) {
                printf(" S %s\r", mz_stat.m_filename);
                n_s++;
                continue;
            }
            #endif
            #ifndef OVERWRITE_ALWAYS
            if (file_exists(mz_stat.m_filename)) {
                #ifndef OVERWRITE_NEVER
                printf(" ? %s\n", mz_stat.m_filename);
                printf("   <A> overwrite <B> skip\r");
                u32 last_key;
                for (last_key = 0; !(last_key & (KEY_A|KEY_B)); last_key = wait_key());
                printf("                         \r");
                if (last_key != KEY_A) {
                    printf(" S %s\r", mz_stat.m_filename);
                    n_s++;
                    continue;
                } else printf(" - %s\r", mz_stat.m_filename);
                #else
                printf(" S %s\r", mz_stat.m_filename);
                n_s++;
                continue;
                #endif
            }
            #endif
            if (!mz_zip_reader_is_file_a_directory(&mz_archive, n)) {
                if (!mz_zip_reader_extract_to_file(&mz_archive, n, mz_stat.m_filename, 0)) {
                    printf("\n ! ERROR extracting file!");
                    break;
                }
                printf(" X %s\r", mz_stat.m_filename);
                n_x++;
            } else {
                printf(" D %s\r", mz_stat.m_filename);
                n_d++;
                rmkdir(mz_stat.m_filename);
            }
        }
        if (n == n_files) printf("\n\n > %i eXtracted, %i Skipped, %i Dirs\n", n_x, n_s, n_d);
    }
    mz_zip_reader_end(&mz_archive);
    printf("\n\nPress any key to reboot");
    wait_key();
    
    // Deinitialize GFX
    gfxExit();
    
    return 0;
}
