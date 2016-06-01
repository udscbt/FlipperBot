schemo: lib
	./schemop -sq main.cpp
	g++ __schemo_main.cpp -o schemo -I include -L lib -lschemo

schemo.o:
	g++ -c schemo.cpp

lib/libschemo.a: schemo.o
	ar r lib/libschemo.a schemo.o

include/schemo.h:
	cp schemo.h include/schemo.h

lib: lib/libschemo.a include/schemo.h

clean:
	-rm *.o
	-rm __schemo_main.cpp
	-rm schemo
	-rm include/*
	-rm lib/*
