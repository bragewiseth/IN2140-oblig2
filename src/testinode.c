#include "inode.h"
#include "allocation.h"
#define PRINT_STRUCT(STRUCT) {\
    printf("id: %d\n", STRUCT -> id);\
    printf("name: %s\n", STRUCT -> name);\
    printf("is_directory: %d\n", STRUCT -> is_directory);\
    printf("is_readonly: %d\n", STRUCT -> is_readonly);\
    printf("filesize: %d\n", STRUCT -> filesize);\
    printf("num_entries: %d\n", STRUCT -> num_entries);\
    for (int i=0; i < STRUCT -> num_entries; i++)\
    {\
        printf("entry %d: %p\n", i, STRUCT -> entries[i]);\
    }\
}


inode * load_inodes();

int main()
{
    format_disk();
    inode * root = load_inodes();
    debug_fs( root );
    debug_disk();
    return 0;
}