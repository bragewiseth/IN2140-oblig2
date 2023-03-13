.PHONY: all clean run



create_fs: allocation.c inode.c
	gcc -g -Wall -Wextra -std=gnu11 create_fs.c allocation.c inode.c -o ../bin/load_2

run: load_2
	../bin/load_2


clean:
	rm -f ../bin/*