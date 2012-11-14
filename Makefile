CC=gcc
CFLAGS=-c -Wall -Wextra -Werror -g -fPIC
LDFLAGS=-lallegro -lallegro_primitives -lallegro_font -lallegro_ttf \
	 -lm -lallegro_color
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
	rm -f src/*.o src/*.orig ./asteroids ./asteroids.zip

tags:
	ctags -o TAGS -e $(HEADERS) $(SOURCES)

test: $(OBJECTS)
	$(CC) -shared -Wl,-soname,lib$(EXECUTABLE).so -o lib$(EXECUTABLE).so $(OBJECTS) $(LDFLAGS)
	nosetests3

asteroids.zip: $(SOURCES)
	rm -rf dist
	mkdir dist
	git clone . dist
	rm dist/.git -rf
	zip -r asteroids.zip dist
	rm -rf dist
