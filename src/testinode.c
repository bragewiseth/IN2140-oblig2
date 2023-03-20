#include "inode.h"

inode * load_inodes();

int main()
{
    inode * root = load_inodes();
    inode * kernel = (inode*) ( root -> entries[0] );
    printf("kernel: %s\n", kernel -> name );
    return 0;
}