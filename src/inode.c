#include "allocation.h"
#include "inode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BLOCKSIZE 4096
#define EMPTY_INODE_SIZE 20
#define PRINT_STRUCT(STRUCT) {\
    printf("id: %d\n", STRUCT -> id);\
    printf("name: %s\n", STRUCT -> name);\
    printf("is_directory: %d\n", STRUCT -> is_directory);\
    printf("is_readonly: %d\n", STRUCT -> is_readonly);\
    printf("filesize: %d\n", STRUCT -> filesize);\
    printf("num_entries: %d\n", STRUCT -> num_entries);\
}


static int next_inode_id = 0;

struct inode* create_file( struct inode* parent, char* name, char readonly, int size_in_bytes )
{
    inode * entries = (inode*) ( parent -> entries );
    num_entries = parent -> num_entries;
    for (int i=0; i < entries; i++)
    {
        if (strcmp(entries[i], name)) return NULL;
    }

}

struct inode* create_dir( struct inode* parent, char* name )
{   
    inode * entries = (inode*) ( parent -> entries );
    num_entries = parent -> num_entries;
    for (int i=0; i < entries; i++)
    {
        if (strcmp(entries[i], name)) return NULL;
    }
    // copy name into new memory
    char * name_copy = malloc(sizeof(char) * strlen(name));
    strcpy(name_copy, name);
    inode * new_directory = malloc(sizeof(inode));
    // set values
    new_directory -> id = next_inode_id; 
    new_directory -> name = name_copy;
    new_directory -> is_directory = 1;
    new_directory -> is_readonly = 0;
    new_directory -> filesize = 0;
    new_directory -> num_entries = 0;
    // increment number of entries in parent
    parent -> num_entries++;
    // realloc parent entries
    realloc(entries, parent -> num_entries + sizeof(uintptr_t*))
    entries[num_entries + 1] = new_directory;

    return new_directory;
}



struct inode* find_inode_by_name( struct inode* parent, char* name )
{   
    // trevese linked list
    return NULL;
}





// write a function that takes a file and writes it to the disk
struct inode* write_file( struct inode* parent, char* name, char* data, int size_in_bytes )
{
    return NULL;
}




struct inode* load_inodes()
{
    FILE * fp = fopen("master_file_table", "r");
    if (!fp) { perror("fopen"); }
    
    inode * inodes[10]; // large enough for 10 inodes
    int META[2]; // buffer to read ID and length of name
    int i = 0;
    while (1) 
    {

        // read the first 8 bytes of the file into the buffer
        int read = fread(META, sizeof(META), 1, fp);
        if (read != 1) 
        { 
            if (read == 0) { printf("EOF\n"); break; }
            fprintf(stderr, "fread failed: %zu bytes read\n", read); break; 
        }

        char BUF[META[1] + 10]; // buffer to read the rest of the inode

        // read the rest of the inode into the buffer
        read = fread(BUF, sizeof(BUF), 1, fp);
        if (read != 1) { fprintf(stderr, "fread failed: %zu\n", read); break; }
        
        // allocate memory for the name and copy it from the buffer 
        char * name = malloc(META[1]);
        strcpy(name, BUF);


        // create an inode struct and copy the data from the buffer into it
        inode * new_inode = malloc(sizeof(inode));
        new_inode -> id = META[0];
        new_inode -> name = name;
        new_inode -> is_directory = BUF[META[1]];
        new_inode -> is_readonly = BUF[META[1] + 1];
        new_inode -> filesize = *((int *)(BUF + META[1] + 2));    // get the first filesize byte and interpret it as an int
        new_inode -> num_entries = *((int *)(BUF + META[1] + 6)); // get the num_entries byte and interpret it as an int

        // read the entries into the buffer
        int * entries = malloc(sizeof(long) * new_inode -> num_entries);
        read = fread(entries, sizeof(long), new_inode -> num_entries, fp);
        if (read != new_inode -> num_entries) { fprintf(stderr, "fread failed reading entries: %zu\n", read); break; }

        new_inode -> entries = entries;

        // put inode in a list of inodes. The ID of an inode is its index in the list:
        inodes[i] = new_inode;
        next_inode_id++; // since inodes is numbered 0 -> in the master_file_table 
        i++;
    }

    // after all nodes are read change the entries to point to the actual inodes:
    for (int k = 0; k < i; k++)
    {
        for ( int j=0 ; j < inodes[k] -> num_entries; j++)
        {
            int ID = inodes[k] -> entries[j];
            inodes[k] -> entries[j] = inodes[ID];
        }
    }
    

    if (fclose(fp))  perror("fclose() error");
    else printf("File closed successfully.\n");
    // return the root inode
    return inodes[0];
}




void fs_shu_tdown( struct inode* inode )
{
}



/* This static variable is used to change the indentation while debug_fs
 * is walking through the tree of inodes and prints information.
 */
// static int indent = 0;

// void debug_fs( struct inode* node )
// {
//     if( node == NULL ) return;
//     for( int i=0; i<indent; i++ )
//         printf("  ");
//     if( node->is_directory )
//     {
//         printf("%s (id %d)\n", node->name, node->id );
//         indent++;
//         for( int i=0; i<node->num_entries; i++ )
//         {
//             struct inode* child = (struct inode*)node->entries[i];
//             debug_fs( child );
//         }
//         indent--;
//     }
//     else
//     {
//         printf("%s (id %d size %db blocks ", node->name, node->id, node->filesize );
//         for( int i=0; i<node->num_entries; i++ )
//         {
//             printf("%d ", (int)node->entries[i]);
//         }
//         printf(")\n");
//     }
// }

