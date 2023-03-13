# Hvordan man leser `master_file_table`

# Eventuelle implementasjonskrav som ikke er oppfylt

# Eventuelle deler av implementasjonen som avviker fra prekoden. Dersom du for eksempel oppretter egne filer, forklar hva hensikten er

endret inode definisjonen til en typedef:

~~~ c
typedef struct inode
{
	int        id;
	char*      name;
	char       is_directory;
	char       is_readonly;
	int        filesize;
	int        num_entries;
	uintptr_t* entries;
} inode;
~~~

# Eventuelle tester som feiler og hva du tror årsaken kan være