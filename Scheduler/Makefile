#OTHER_FLAGS=-DSCHEMO_PROFILER

schemo: lib main.cpp schemop
	./schemop -sq main.cpp
	g++ __schemo_main.cpp $(OTHER_FLAGS) -o schemo -I include -L lib -lschemo

schemo_profiler.o: schemo_profiler.h schemo_profiler.cpp
	g++ -c $(OTHER_FLAGS) schemo_profiler.cpp

schemo.o: schemo.h schemo_profiler.h schemo.cpp
	g++ -c $(OTHER_FLAGS) schemo.cpp

lib/libschemo.a: schemo.o schemo_profiler.o
	ar r lib/libschemo.a schemo.o schemo_profiler.o

include/schemo.h: schemo.h
	cp schemo.h include/

include/schemo_profiler.h: schemo_profiler.h
	cp schemo_profiler.h include/

lib: lib/libschemo.a include/schemo.h include/schemo_profiler.h

clean:
	-rm *.o
	-rm __schemo_main.cpp
	-rm schemo
	-rm *.profile
	-rm include/*
	-rm lib/*
