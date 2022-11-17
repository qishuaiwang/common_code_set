INCLUDE = -I ./

bd_exe:args_analysis.o format_convert.o main.o
	gcc $(INCLUDE) -g args_analysis.o format_convert.o main.o -o main

main.o:main.c
	gcc $(INCLUDE) -c main.c -o main.o -g
args_analysis.o:args_analysis.c
	gcc $(INCLUDE) -c args_analysis.c -o args_analysis.o -g
format_convert.o:format_convert.c
	gcc $(INCLUDE) -c format_convert.c -o format_convert.o -g

clean:
	rm -f *.o main
