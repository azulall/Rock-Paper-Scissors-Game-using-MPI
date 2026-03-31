CC = mpicc
CFLAGS = -Wall -Wextra
TARGET = game
SRC = game.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

