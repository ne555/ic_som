CXX = g++
#CPPFLAGS = -Wall -pedantic-errors -ggdb 
CPPFLAGS = -Wall -pedantic-errors -O2 
LIBRARIES_grapher = $(addprefix -l,GL GLU glut)
LIBRARIES_som = 

object_som = $(addprefix obj/,main.o neurona_som.o som.o util.o neurona.o)
object_grapher = $(addprefix obj/,grapher.o)
object_input = $(addprefix obj/,input.o util.o)

objects = $(object_som) $(object_grapher) $(object_input)

binaries = $(addprefix bin/,som grapher input) 

project: $(binaries)

all: $(objects)

bin/som: $(object_som)
	$(CXX) $(LIBRARIES_som) $(object_som) -o $@

bin/grapher: $(object_grapher)
	$(CXX) $(LIBRARIES_grapher) $(object_grapher) -o $@

bin/input: $(object_input)
	$(CXX) $(object_input) -o $@

obj/input.o: $(addprefix header/,util.h)
obj/main.o: $(addprefix header/,som.h neurona_som.h)
obj/neurona.o: $(addprefix header/,util.h neurona.h)
obj/neurona_som.o: $(addprefix header/,util.h neurona_som.h)
obj/som.o: $(addprefix header/,som.h neurona_som.h util.h)
obj/util.o: $(addprefix header/,util.h)

obj/%.o : src/%.cpp
	$(CXX) $< -c $(CPPFLAGS) -Iheader -o $@

obj/main.o: src/main.cpp
	$(CXX) $< -c $(CPPFLAGS) -Iheader -o $@

$(objects): | obj

$(binaries): | bin

obj:
	mkdir $@

bin:
	mkdir $@


.PHONY: clean test

clean:
	-rm $(objects) $(binaries)
	-rmdir bin obj
