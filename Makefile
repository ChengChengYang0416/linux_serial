ELF_client=client
ELF_server=server

all:
	gcc -o ${ELF_client} client.c
	gcc -o ${ELF_server} server.c

clean:
	rm ${ELF_client} ${ELF_server}
