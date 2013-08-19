EXE = ALSP
OBJ = alsp.c 
all: $(EXE)
$(EXE): 
	g++ -o ALSP alsp.c -Wall
clean:
	rm -fr *.o *~ $(EXE)


