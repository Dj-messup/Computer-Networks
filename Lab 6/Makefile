CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11

SERVER = evidence_server
CLIENT = investigator_client

all: $(SERVER) $(CLIENT)

$(SERVER): evidence_server.c
	$(CC) $(CFLAGS) evidence_server.c -o $(SERVER)

$(CLIENT): investigator_client.c
	$(CC) $(CFLAGS) investigator_client.c -o $(CLIENT)

run-server:
	./$(SERVER) 3490

test-echo:
	./$(CLIENT) localhost 3490 echo

test-file:
	./$(CLIENT) localhost 3490 file

clean:
	rm -f $(SERVER) $(CLIENT) received_case_file.txt

.PHONY: all run-server test-echo test-file clean
