CC=g++

all: compare

compare: soname
	$(CC) -g -Wall compare.cpp -Wl,-rpath=. -L. -lcompbranches -o compare

soname: lib
	ln -s libcompbranches.so.1.0.1 libcompbranches.so.1 && ln -s libcompbranches.so.1.0.1 libcompbranches.so 
	
lib: compbranches.o
	$(CC) -shared -Wl,-soname,libcompbranches.so.1 -o libcompbranches.so.1.0.1 compbranches.o -lcurl -lrpm

compbranches.o:
	$(CC) -fPIC -c compbranches.cpp -o compbranches.o

clean:
	rm -f *.o
	
