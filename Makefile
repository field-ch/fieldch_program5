CC = gcc -g

TARGET_1 = enc_server
TARGET_2 = enc_client
TARGET_3 = dec_server
TARGET_4 = dec_client
TARGET_5 = keygen

SOURCE_1 = enc_server.c
SOURCE_2 = enc_client.c
SOURCE_3 = dec_server.c
SOURCE_4 = dec_client.c
SOURCE_5 = keygen.c

all:
	$(CC) -o $(TARGET_1) $(SOURCE_1)
	$(CC) -o $(TARGET_2) $(SOURCE_2)
	$(CC) -o $(TARGET_3) $(SOURCE_3)
	$(CC) -o $(TARGET_4) $(SOURCE_4)
	$(CC) -o $(TARGET_5) $(SOURCE_5)

clean:
	rm -r -f $(TARGET_1) $(TARGET_2) $(TARGET_3) $(TARGET_4) $(TARGET_5)
