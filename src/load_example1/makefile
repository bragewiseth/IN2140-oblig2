.PHONY: all clean run



create_fs: allocation.c inode.c
	gcc -g -Wall -Wextra -std=gnu11 create_fs.c allocation.c inode.c -o ../bin/load_1

run: load_1
	../bin/load_1


clean:
	rm -f ../bin/*