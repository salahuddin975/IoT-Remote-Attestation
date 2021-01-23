BIN = CarServer

CC = gcc

objects = server.o jpegutil.o

server:$(objects)
	$(CC) -O4 -g -o $(BIN) -fno-stack-protector -z execstack server.o jpegutil.o -lopencv_videoio -lwiringPi -lpthread -lm -lopencv_core -lopencv_ml -lopencv_imgproc -lopencv_highgui -ljpeg

server.o:server.c jpegutil.h
	$(CC) -O4 -g -c server.c

jpegutil.o: jpegutil.c jpegutil.h
	$(CC) -O4 -g -c jpegutil.c

clean:
	rm -rf $(objects) $(BIN)
