#OTHER_FLAGS=-DSCHEMO_PROFILER

schemo: lib
	./schemop -sq main.cpp
	g++ __schemo_main.cpp $(OTHER_FLAGS) -o schemo -I include -L lib -lschemo

schemo.o: schemo.h schemo_profiler.h
	g++ -c $(OTHER_FLAGS) schemo.cpp

lib/libschemo.a: schemo.o
	ar r lib/libschemo.a schemo.o

include/schemo.h: schemo.h
	cp schemo.h include/

include/schemo_profiler.h: schemo_profiler.h
	cp schemo_profiler.h include/

lib: lib/libschemo.a include/schemo.h include/schemo_profiler.h

clean:
	-rm *.o
	-rm __schemo_main.cpp
	-rm schemo
	-rm include/*
	-rm lib/*
