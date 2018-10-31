CXXFLAGS = -Wall -Wextra -pedantic -O
OBJECTS = off-to-js

all: $(OBJECTS)

clean: 
	rm -f *.o $(OBJECTS)
