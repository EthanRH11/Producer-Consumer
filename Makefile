CC = gcc
CFLAGS = pthread -lrt

all: producer consumer

producer: producer.cpp
	$(CC) producer.cpp $(CFLAGS) -o producer

consumer: consumer.cpp
	$(CC) producer.cpp $(CFLAGS) -o consumer

clean:
	rm -f producer consumer
