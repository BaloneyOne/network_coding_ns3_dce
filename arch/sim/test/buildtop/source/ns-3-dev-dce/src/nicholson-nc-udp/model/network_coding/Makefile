# Flags & Variables
CC = g++
CFLAGS = -Wall -O2
COMPILE = $(CC) $(CFLAGS) -c
CLASSOBJECTS = Matrix.o Galois_Field.o
PROGS = tManual


# Programs names
all: $(PROGS)


# Linking programs
tManual: Test_Manual.o $(CLASSOBJECTS)
	$(CC) -o tManual Test_Manual.o $(CLASSOBJECTS)

# Compiling sources
# TESTS
testManual.o: Test_Manual.cc Matrix.cc Galois_Field.cc
	$(COMPILE) -o Test_Manual.o Test_Manual.cc

# CLASSES
Matrix.o: Matrix.cc Matrix.h Galois_Field.h
	$(COMPILE) -o Matrix.o Matrix.cc

Galois_Field.o: Galois_Field.cc Galois_Field.h
	$(COMPILE) -o Galois_Field.o Galois_Field.cc


# Clean up
clean:
	rm -rf *.o *.out *~ $(PROGS)
