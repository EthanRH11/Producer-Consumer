CC = g++
CFLAGS = -Wall -g
LDFLAGS = -pthread -lrt
INCLUDES = -I./Include 
SRC_DIR = src
TARGET_CONSUMER = consumer
TARGET_PRODUCER = producer

all: $(TARGET_CONSUMER) $(TARGET_PRODUCER)

$(TARGET_CONSUMER): $(SRC_DIR)/consumer.o
	$(CC) $(CFLAGS) -o $(TARGET_CONSUMER) $(SRC_DIR)/consumer.o $(LDFLAGS)

$(TARGET_PRODUCER): $(SRC_DIR)/producer.o
	$(CC) $(CFLAGS) -o $(TARGET_PRODUCER) $(SRC_DIR)/producer.o $(LDFLAGS)

$(SRC_DIR)/consumer.o: $(SRC_DIR)/consumer.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC_DIR)/consumer.cpp -o $(SRC_DIR)/consumer.o

$(SRC_DIR)/producer.o: $(SRC_DIR)/producer.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC_DIR)/producer.cpp -o $(SRC_DIR)/producer.o

clean:
	rm -f $(SRC_DIR)/*.o $(TARGET_CONSUMER) $(TARGET_PRODUCER)

.PHONY: all clean