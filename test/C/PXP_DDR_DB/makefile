INCLUDE = -I ./
# CFLAG = -lm
# CFLAG = -g -lm
# CFLAG = -pg -lm
CFLAG = -ltcmalloc -lprofiler -g -lm
CPUPROFILE=./profile ./img_backdoor_2_ddr

main:cJSON.o linked_list.o data_repackage.o args_analysis.o format_convert.o main.o
	gcc $(INCLUDE) cJSON.o linked_list.o args_analysis.o data_repackage.o format_convert.o main.o -o img_backdoor_2_ddr $(CFLAG)

main.o:main.c
	gcc $(INCLUDE) -c main.c -o main.o $(CFLAG)
cJSON.o:cJSON.c
	gcc $(INCLUDE) -c cJSON.c -o cJSON.o $(CFLAG)
args_analysis.o:args_analysis.c
	gcc $(INCLUDE) -c args_analysis.c -o args_analysis.o $(CFLAG)
format_convert.o:format_convert.c
	gcc $(INCLUDE) -c format_convert.c -o format_convert.o $(CFLAG)
linked_list.o:linked_list.c
	gcc $(INCLUDE) -c linked_list.c -o linked_list.o $(CFLAG)
data_repackage.o:data_repackage.c
	gcc $(INCLUDE) -c data_repackage.c -o data_repackage.o $(CFLAG)

clean:
	rm -f *.o main

# INCLUDE = -I ./
#
# main:cJSON.o cjson_test.o
# 	gcc $(INCLUDE) -g cJSON.o cjson_test.o -o main
#
# cjson_test.o:cjson_test.c
# 	gcc $(INCLUDE) -c cjson_test.c -o cjson_test.o -g
# cJSON.o:cJSON.c
# 	gcc $(INCLUDE) -c cJSON.c -o cJSON.o -g
#
# clean:
# 	rm -f *.o main
#
