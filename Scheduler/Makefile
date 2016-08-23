ifeq ($(PROFILER),true)
 	FLAGS=$(OTHER_FLAGS) -DSCHEMO_PROFILER
	OPTIONS=$(OTHER_OPTIONS) -p
	LIB=-lschemo_profiler
else
	FLAGS=$(OTHER_FLAGS)
	OPTIONS=$(OTHER_OPTIONS)
	LIB=-lschemo
endif

TEX=pdflatex
NAME=schemo

$(NAME): lib main.cpp schemop schemop_profiler.py
	./schemop -sq $(OPTIONS) main.cpp
	g++ __schemo_main.cpp $(FLAGS) -o $(NAME) -I include -L lib $(LIB)

$(NAME).profile: main.cpp schemop schemop_profiler.py
ifeq ($(EXEC),true)
	$(MAKE) $(NAME) PROFILER=true
	mv main.cpp.profile $(NAME).profile
	./$(NAME)
ifneq ($(NAME), schemo)
	cat schemo.profile >> $(NAME).profile
	rm schemo.profile
endif
else
	./schemop -sqp main.cpp
	mv main.cpp.profile $(NAME).profile
endif

profile: $(NAME).profile

$(NAME).tex: $(NAME).profile smp2tikz tex_config/single.template tex_config/schemotex.preamble.tex tex_config/profile.template tex_config/schemotex.preamble.full.tex tex_config/schemotex.standalone.tex
	./smp2tikz $(TEX_OPTIONS) $(NAME).profile
ifneq ($(KEEP),true)
	rename .tex .tex.save *.tex
	$(MAKE) clean
	rename .tex.save .tex *.tex.save
endif

tex: $(NAME).tex

pdf: $(NAME).tex
	$(TEX) schemotex.standalone.tex
	mv schemotex.standalone.pdf $(NAME).pdf
ifneq ($(KEEP),true)
	mv $(NAME).pdf tmp
	$(MAKE) clean
	mv tmp $(NAME).pdf
endif

schemo_profiler.o: schemo_profiler.h schemo_profiler.cpp
	g++ -c -DSCHEMO_PROFILER schemo_profiler.cpp

schemo.o: schemo.h schemo_profiler.h schemo.cpp
	g++ -c schemo.cpp

schemo_p.o: schemo.h schemo_profiler.h schemo.cpp
	g++ -c -DSCHEMO_PROFILER schemo.cpp -o schemo_p.o

lib/libschemo.a: schemo.o
	ar r lib/libschemo.a schemo.o

lib/libschemo_profiler.a: schemo_p.o schemo_profiler.o
	ar r lib/libschemo_profiler.a schemo_p.o schemo_profiler.o

include/schemo.h: schemo.h
	cp schemo.h include/

include/schemo_profiler.h: schemo_profiler.h
	cp schemo_profiler.h include/

lib: lib/libschemo.a lib/libschemo_profiler.a include/schemo.h include/schemo_profiler.h

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
