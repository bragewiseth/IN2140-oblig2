.PHONY: all clean run



create_fs: allocation.c inode.c
	gcc -g -Wall -Wextra -std=gnu11 create_fs.c allocation.c inode.c -o ../bin/load_3

run: load_3
	../bin/load_3


clean:
	rm -f ../bin/*