CC = gcc
CFLAGS = -Wall -ggdb -std=c11 -pedantic
LDFLAGS = lib/raylib/libraylib.a
FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

SRC = src/main.c
OUT = src/main

# La regola di default è solo compilare
all: build

# Regola per compilare
build:
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) $(FRAMEWORKS) -o $(OUT)

# Regola per compilare E eseguire
# La dipendenza 'build' assicura che ricompili se hai cambiato il codice
run: build
	./$(OUT)

clean:
	rm -f $(OUT)