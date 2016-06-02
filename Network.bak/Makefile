test: lib
	g++ *.cpp -o test -L lib -L ../FBCP/lib -I include -I ../FBCP/include -lfbcp -lfbnet

include/FBNet.h: FBNet.h
	cp FBNet.h include/FBNet.h

FBNet.o: FBNet.h FBNet.cpp fbcp
	g++ -c -o FBNet.o -I include -I ../FBCP/include FBNet.cpp

fbcp:
	$(MAKE) -C ../FBCP/ lib

lib/libfbnet.a: FBNet.o
	ar r lib/libfbnet.a FBNet.o

lib: include/FBNet.h lib/libfbnet.a

clean:
	-rm *.o
	-rm include/*
	-rm lib/*
	-rm test
