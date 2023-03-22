# Hvordan man leser `master_file_table`

En master file table (MFT) er en tabell som inneholder alle filer og mapper i et filsystem. Denne tabellen er en array av `inode`-strukturer.  
For å lese `master_file_table` leser vi først `8` bytes, dette er ID og lendge på navn deretter leser vi navnet + `10` bytes. Til slutt leser vi `entries` det antallet er gitt av `num_entries`. Dette utgjør én `inode`-struktur.

# Eventuelle implementasjonskrav som ikke er oppfylt

Funksjonen `write_file()` er ikke implementer siden dette ikke var et krav. Funksjonalitet for å skrive til disk er integrert direkte inn i `create_file` of `load_inodes()`.

# Eventuelle deler av implementasjonen som avviker fra prekoden. Dersom du for eksempel oppretter egne filer, forklar hva hensikten er

endret inode definisjonen til en typedef for å gjøre det mindre ordrikt å jobbe med:

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

Alle navn blir kopiert til heapen. I denne oppgaven er det egentlig ikke ikke nødvendig siden alle navn blir allokert i main og blir ikke poppet av stacken før programmet terminerer. Hvis testprogrammene hadde laget inoder i funksjoner ville dette vært annerledes.

# Eventuelle tester som feiler og hva du tror årsaken kan være