# INCLUDE = -I ./
# 
# main:linked_list.o data_repackage.o args_analysis.o format_convert.o main.o
# 	gcc $(INCLUDE) -g args_analysis.o data_repackage.o format_convert.o main.o -o main
# 
# main.o:main.c
# 	gcc $(INCLUDE) -c main.c -o main.o -g
# args_analysis.o:args_analysis.c
# 	gcc $(INCLUDE) -c args_analysis.c -o args_analysis.o -g
# format_convert.o:format_convert.c
# 	gcc $(INCLUDE) -c format_convert.c -o format_convert.o -g
# linked_list.o:linked_list.c
# 	gcc $(INCLUDE) -c linked_list.c -o linked_list.o -g
# data_repackage.o:data_repackage.c
# 	gcc $(INCLUDE) -c data_repackage.c -o data_repackage.o -g
# 
# clean:
# 	rm -f *.o main

INCLUDE = -I ./
CFLAG = -g -lm
main:cJSON.o cjson_test.o
	gcc $(INCLUDE) cJSON.o cjson_test.o -o main $(CFLAG)

cjson_test.o:cjson_test.c
	gcc $(INCLUDE) -c cjson_test.c -o cjson_test.o $(CFLAG)
# test.o:cjson_test.c
# 	gcc $(INCLUDE) -c test.c -o cjson_test.o $(CFLAG)
cJSON.o:cJSON.c
	gcc $(INCLUDE) -c cJSON.c -o cJSON.o $(CFLAG)

clean:
	rm -f *.o main
