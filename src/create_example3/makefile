.PHONY: all clean run


create_fs: allocation.c inode.c
	gcc -g -Wall -Wextra -std=gnu11 create_fs.c allocation.c inode.c -o ../bin/create_3

run: create_3
	../bin/create_3


clean:
	rm -f ../bin/*