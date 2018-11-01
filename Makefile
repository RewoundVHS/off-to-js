CXXFLAGS = -Wall -Wextra -pedantic -O
OBJECTS = off-to-js

all: off-to-js

clean: 
	rm -f *.o $(OBJECTS)
