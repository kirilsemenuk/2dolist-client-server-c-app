all:clienet.c 02_add.c 01_add.c 03_add.c 04_add.c main_server.c
	gcc -g3 clienet.c 02_add.c 01_add.c 03_add.c 04_add.c -lpthread -o cli
	gcc -g3 main_server.c 02_add.c 01_add.c 03_add.c 04_add.c -lpthread -o serv
