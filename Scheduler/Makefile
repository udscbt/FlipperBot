ifeq ($(PROFILER),true)
 	FLAGS=$(OTHER_FLAGS) -DSCHEMO_PROFILER
	OPTIONS=$(OTHER_OPTIONS) -p
else
	FLAGS=$(OTHER_FLAGS)
	OPTIONS=$(OTHER_OPTIONS)
endif

schemo: lib main.cpp schemop schemop_profiler.py
	./schemop -sq $(OPTIONS) main.cpp
	g++ __schemo_main.cpp $(FLAGS) -o schemo -I include -L lib -lschemo

pdf:
	$(MAKE) schemo PROFILER=true
ifeq ($(EXEC),true)
	./schemo
	cat schemo.profile >> main.cpp.profile
endif
	./smp2tikz main.cpp.profile
	pdflatex main.cpp.tex
	mv main.cpp.pdf pdf.out
ifneq ($(KEEP),true)
	$(MAKE) clean
endif
	mv pdf.out out.pdf

schemo_profiler.o: schemo_profiler.h schemo_profiler.cpp
	g++ -c $(FLAGS) schemo_profiler.cpp

schemo.o: schemo.h schemo_profiler.h schemo.cpp
	g++ -c $(FLAGS) schemo.cpp

lib/libschemo.a: schemo.o schemo_profiler.o
	ar r lib/libschemo.a schemo.o schemo_profiler.o

include/schemo.h: schemo.h
	cp schemo.h include/

include/schemo_profiler.h: schemo_profiler.h
	cp schemo_profiler.h include/

lib: lib/libschemo.a include/schemo.h include/schemo_profiler.h

clean:
	-rm *.o
	-rm __schemo*
	-rm schemo
	-rm *.profile
	-rm *.tex
	-rm *.log
	-rm *.aux
	-rm *.pdf
	-rm include/*
	-rm lib/*
