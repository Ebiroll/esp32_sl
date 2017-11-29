.POSIX:
CC     = cc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -I ../main

json: json.o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ json.o main.o $(LDLIBS)

json.o: ../main/json.c
	$(CC) $(CFLAGS) -c -o json.o ../main/json.c 

main.o: main.c
	$(CC) $(CFLAGS) -c $(LDFLAGS) -o main.o main.c $(LDLIBS)	

clean:
	rm -f json json.o main.o

run: json
	./json  ../doc/realtimedeparturesv4.json 
