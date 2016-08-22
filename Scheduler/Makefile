ifeq ($(PROFILER),true)
 	FLAGS=$(OTHER_FLAGS) -DSCHEMO_PROFILER
	OPTIONS=$(OTHER_OPTIONS) -p
else
	FLAGS=$(OTHER_FLAGS)
	OPTIONS=$(OTHER_OPTIONS)
endif

TEX=pdflatex
NAME=schemo

$(NAME): lib main.cpp schemop schemop_profiler.py
	./schemop -sq $(OPTIONS) main.cpp
	g++ __schemo_main.cpp $(FLAGS) -o $(NAME) -I include -L lib -lschemo

$(NAME).tex: main.cpp schemop schemop_profiler.py smp2tikz tex_config/single.template tex_config/preamble.template tex_config/profile.template
ifeq ($(EXEC),true)
	$(MAKE) $(NAME) PROFILER=true
	mv main.cpp.profile $(NAME).profile
	./$(NAME)
else
	./schemop -sqp main.cpp
	mv main.cpp.profile $(NAME).profile
endif
	./smp2tikz $(TEX_OPTIONS) $(NAME).profile
ifneq ($(KEEP),true)
	rename .tex .tex.save *.tex
	$(MAKE) clean
	rename .tex.save .tex *.tex.save
endif

tex: $(NAME).tex

pdf: $(NAME).tex
	$(TEX) $(NAME).tex
ifneq ($(KEEP),true)
	mv $(NAME).pdf tmp
	$(MAKE) clean
	mv tmp $(NAME).pdf
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
	-rm $(NAME)
	-rm *.profile
	-rm *.tex
	-rm *.log
	-rm *.aux
	-rm *.pdf
	-rm include/*
	-rm lib/*
