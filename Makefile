CC=gcc
CFLAGS=-c -Wall -Wextra -pedantic -std=c99 -g
LDFLAGS=-lallegro
SOURCES= \
	src/vector.c src/particles.c src/asteroid.c \
	src/ship.c src/game.c src/bullets.c \
	src/main.c
HEADERS=$(SOURCES:.c=.h)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=asteroids

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

format:
	astyle --style=java $(HEADERS) $(SOURCES)

clean:
	rm ./asteroids
	rm src/*.o
