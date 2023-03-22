#include "allocation.h"
#include "inode.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BLOCKSIZE 4096
#define PRINT_STRUCT(STRUCT) {\
    printf("id: %d\n", STRUCT -> id); \
    printf("name: %s\n", STRUCT -> name);\
    printf("is_directory: %d\n", STRUCT -> is_directory);\
    printf("is_readonly: %d\n", STRUCT -> is_readonly);\
    printf("filesize: %d\n", STRUCT -> filesize);\
    printf("num_entries: %d\n", STRUCT -> num_entries);\
    for (int i=0; i < STRUCT -> num_entries; i++)\
    {\
        printf("entry %d: %d\n", i, STRUCT -> entries[i]);\
    }\
}





static int next_inode_id = 0;






struct inode* create_file( struct inode* parent, char* name, char readonly, int size_in_bytes )
{
    int num_entries = parent -> num_entries;
    // check if file already exists
    if ( find_inode_by_name(parent, name) ) return NULL; 
    // allocate blocks, if it failes it will give back the blocks and return NULL
    int num_blocks = size_in_bytes / BLOCKSIZE; 
    if (size_in_bytes % BLOCKSIZE != 0) num_blocks++; 
    int blocks[num_blocks];  
    for (int i=0; i < num_blocks; i++) 
    { 
        blocks[i] = allocate_block(); 
        if ( blocks[i] == -1 )
        {
            i--; 
            for ( ; i >= 0; i--) 
            { 
                free_block(i);
            } 
            fprintf(stderr, "failed to create file, not enough space\n");
            return NULL;
        } 
    } 

    // copy name into new memory
    char * name_copy = malloc(strlen(name) + 10);
    if ( name_copy == NULL ) perror("malloc failed in create_file() giving memory to name_copy");
    strcpy(name_copy, name);
    // make new_file
    inode * new_file = malloc(sizeof(inode));
    // place block number in the entries
    new_file -> entries = malloc(sizeof(uintptr_t) * num_blocks +1);
    for (int i=0; i < num_blocks; i++) { new_file -> entries[i] = blocks[i]; }

    // set values
    new_file -> id = next_inode_id;
    new_file -> name = name_copy;
    new_file -> is_directory = 0;
    new_file -> is_readonly = readonly;
    new_file -> filesize = size_in_bytes;
    new_file -> num_entries = num_blocks;
    // add the file to the parent entries
    parent -> entries = realloc( parent->entries, (num_entries * sizeof(uintptr_t*)) + sizeof(uintptr_t*));
    parent -> entries[num_entries] = (uintptr_t) new_file;
    parent -> num_entries++;
    // increment ID
    next_inode_id++;
    return new_file;
}





struct inode* create_dir( struct inode* parent, char* name )
{   
    int num_entries;
    if ( parent != NULL )
    {
        num_entries = parent -> num_entries;
        if ( find_inode_by_name(parent, name) ) return NULL;
        // increment number of entries in parent
        parent -> num_entries++;
        // realloc parent entries
        parent -> entries = realloc(parent -> entries, (num_entries * sizeof(uintptr_t*)) + sizeof(uintptr_t*));
    }

    // copy name into new memory
    char * name_copy = malloc(strlen(name) + 1);
    strcpy(name_copy, name);
    inode * new_directory = malloc(sizeof(inode));
    // set values
    new_directory -> id = next_inode_id; 
    new_directory -> name = name_copy;
    new_directory -> is_directory = 1;
    new_directory -> is_readonly = 0;
    new_directory -> filesize = 0;
    new_directory -> num_entries = 0;
    new_directory -> entries = malloc(sizeof(uintptr_t*)); // add one size to avoid segfault when debugging
    if ( parent != NULL) parent -> entries[num_entries] = (uintptr_t) new_directory;
    next_inode_id++;
    return new_directory;
}









struct inode* find_inode_by_name( struct inode* parent, char* name )
{   
    inode ** entries = (inode**) ( parent -> entries );
    int num_entries = parent -> num_entries;
    for (int i=0; i < num_entries; i++)
    {
        if (strcmp(entries[i] -> name, name) == 0) return (inode*) entries[i];
    }
    return NULL;
}



// this functionality is integrated in create_file( ) and load_inodes( ) directly
// write a function that takes a file and writes it to the disk
// struct inode* write_file( struct inode* parent, char* name, char* data, int size_in_bytes )
// {
//     return NULL;
// }



struct inode* load_inodes()
{
    FILE * fp = fopen("master_file_table", "r");
    if (!fp) { perror("fopen"); }
    fseek(fp, 0L, SEEK_END);
    long length = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    inode * inodes[length / 18]; // large enough for length/18 inodes. 18 is the smallest an inode can be

    int META[2]; // buffer to read ID and length of name
    int i = 0;
    while (1) 
    {

        // read the first 8 bytes of the file into the buffer
        int read = fread(META, sizeof(META), 1, fp);
        if (read != 1) 
        { 
            if (read == 0) { printf("EOF\n"); break; }
            fprintf(stderr, "fread failed: %d bytes read\n", read); break; 
        }

        char BUF[META[1] + 10]; // buffer to read the rest of the inode

        // read the rest of the inode into the buffer
        read = fread(BUF, sizeof(BUF), 1, fp);
        if (read != 1) { fprintf(stderr, "fread failed: %d\n", read); break; }
        
        // allocate memory for the name and copy it from the buffer 
        char * name = malloc(META[1]);
        if ( !name ) { perror("malloc failed"); }
        strcpy(name, BUF);


        // create an inode struct and copy the data from the buffer into it
        inode * new_inode = malloc(sizeof(inode));
        if ( !new_inode ) { perror("malloc failed"); }
        new_inode -> id = META[0];
        new_inode -> name = name;
        new_inode -> is_directory = BUF[META[1]];
        new_inode -> is_readonly = BUF[META[1] + 1];
        new_inode -> filesize = *((int *)(BUF + META[1] + 2));    // get the first filesize byte and interpret it as an int
        new_inode -> num_entries = *((int *)(BUF + META[1] + 6)); // get the num_entries byte and interpret it as an int

        // read the entries into the buffer
        uintptr_t * entries = malloc(sizeof(uintptr_t) * (new_inode -> num_entries + 1));
        if ( !entries ) { perror("malloc failed"); }
        read = fread(entries, sizeof(uintptr_t), new_inode -> num_entries, fp);
        if (read != new_inode -> num_entries) { fprintf(stderr, "fread failed reading entries: %d\n", read); break; }

        new_inode -> entries = entries;

        // put inode in a list of inodes. The ID of an inode is its index in the list:
        inodes[META[0]] = new_inode;
        next_inode_id++; // since inodes is numbered 0 -> in the master_file_table 
        i++;
    }

    // after all nodes are read change the entries to point to the actual inodes or allocate blocks:
    for (int k = 0; k < i; k++)
    {
        if (inodes[k] -> is_directory)
        {
        for ( int j=0 ; j < inodes[k] -> num_entries; j++)
            {
                int ID = inodes[k] -> entries[j];
                inodes[k] -> entries[j] = (uintptr_t) inodes[ID];
            }
        }
        else
        {
            for ( int j=0 ; j < inodes[k] -> num_entries; j++)
            {
                allocate_block(); 
            }
        }
    }
    
    if (fclose(fp))  perror("fclose() error");
    else printf("File closed successfully.\n");
    // return the root inode
    return inodes[0];
}









void fs_shutdown( inode* inod )
{
    if ( inod == NULL ) return;

    inode ** entries = (inode**) ( inod -> entries );
    int num_entries = inod -> num_entries; 
    if ( inod -> is_directory )
    {
        for (int i=0; i < num_entries; i++) { fs_shutdown(entries[i]); }
    } 
    else
    {
        for (int i=0; i < num_entries; i++) { free_block( (int) entries[i]); }
    } 
    printf("closing %s\n", inod -> name);
    free ( inod -> entries );
    free ( inod -> name );
    free ( inod );
    return;
}






/* This static variable is used to change the indentation while debug_fs
 * is walking through the tree of inodes and prints information.
 */
static int indent = 0;

void debug_fs( struct inode* node )
{
    // PRINT_STRUCT(node);
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

