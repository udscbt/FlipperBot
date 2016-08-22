ifeq ($(PROFILER),true)
 	FLAGS=$(OTHER_FLAGS) -DSCHEMO_PROFILER
	OPTIONS=$(OTHER_OPTIONS) -p
else
	FLAGS=$(OTHER_FLAGS)
	OPTIONS=$(OTHER_OPTIONS)
endif

TEX=pdflatex

schemo: lib main.cpp schemop schemop_profiler.py
	./schemop -sq $(OPTIONS) main.cpp
	g++ __schemo_main.cpp $(FLAGS) -o schemo -I include -L lib -lschemo

schemo.tex: main.cpp schemop schemop_profiler.py smp2tikz  tex_config/single.template tex_config/preamble.template tex_config/profile.template
ifeq ($(EXEC),true)
	$(MAKE) schemo PROFILER=true
	mv main.cpp.profile schemo.profile
	./schemo
else
	./schemop -sqp main.cpp
	mv main.cpp.profile schemo.profile
endif
	./smp2tikz $(TEX_OPTIONS) schemo.profile
ifneq ($(KEEP),true)
	rename .tex .tex.save *.tex
	$(MAKE) clean
	rename .tex.save .tex *.tex.save
endif

tex: schemo.tex

pdf: schemo.tex
	$(TEX) schemo.tex
ifneq ($(KEEP),true)
	mv schemo.pdf pdf.schemo
	$(MAKE) clean
	mv pdf.schemo schemo.pdf
endif

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
