# Hvordan man leser `master_file_table`

En master file table (MFT) er en tabell som inneholder alle filer og mapper i et filsystem. Denne tabellen er en array av `inode`-strukturer.  
For å lese `master_file_table` leser vi først `20` bytes deretter antall bytes som er gitt av `num_entries` x 8. Dvs bytes `16` → `20`. Dette er én `inode`-struktur.

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