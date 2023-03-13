#include "allocation.h"
#include "inode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BLOCKSIZE 4096

struct inode* create_file( struct inode* parent, char* name, char readonly, int size_in_bytes )
{
    if ( unique name )
    {
        inode new_file = { new id, name, 0, readonly, size_in_bytes }
        parent -> entries add new_file
    }
    else 
    return NULL;
}

struct inode* create_dir( struct inode* parent, char* name )
{
    return NULL;
}

struct inode* find_inode_by_name( struct inode* parent, char* name )
{   
    trevese linked list
    return NULL;
}

struct inode* load_inodes()
{
    char BUF[100];
    FILE * fp = fopen("master_file_table", "r");
    if (!fp) { perror("fopen"); }

    size_t read;
    size_t nmemb = 3;
    read = fread(BUF, sizeof(BUF[0]) ,nmemb, fp);
    if (read != nmemb)
    {
        fprintf(stderr, "fread() failed: %zu\n", read);
        exit(EXIT_FAILURE);
    }

    if (fclose(fp))  
      perror("fclose() error");
    else printf("File closed successfully.\n");
    return NULL;
}

void fs_shutdown( struct inode* inode )
{
}

/* This static variable is used to change the indentation while debug_fs
 * is walking through the tree of inodes and prints information.
 */
static int indent = 0;

void debug_fs( struct inode* node )
{
    if( node == NULL ) return;
    for( int i=0; i<indent; i++ )
        printf("  ");
    if( node->is_directory )
    {
        printf("%s (id %d)\n", node->name, node->id );
        indent++;
        for( int i=0; i<node->num_entries; i++ )
        {
            struct inode* child = (struct inode*)node->entries[i];
            debug_fs( child );
        }
        indent--;
    }
    else
    {
        printf("%s (id %d size %db blocks ", node->name, node->id, node->filesize );
        for( int i=0; i<node->num_entries; i++ )
        {
            printf("%d ", (int)node->entries[i]);
        }
        printf(")\n");
    }
}

