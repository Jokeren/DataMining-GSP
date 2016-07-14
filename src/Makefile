CXXFLAGS=-g -O2

objects=main.o gsp_read_input.o gsp_execute.o gsp_generate_candidates.o gsp_print_patterns.o \
seperator.o database_count.o database_init.o database_prune.o database_create.o pattern.o\
gsp_utils.o gsp_hash_tree.o sequence.o\

all : data_provider gsp

gsp : $(objects)
	g++ -o gsp $(objects) -g -O2

main.o : gsp.h seperator.h database.h
seperator.o : seperator.h
gsp_execute.o : gsp.h
gsp_generate_candidates.o : gsp.h database.h
gsp_print_patterns.o : gsp.h database.h
gsp_read_input.o : gsp.h database.h
gsp_utils.o : gsp_utils.h
gsp_hash_tree.o : gsp_hash_tree.h
pattern.o : gsp.h
database_count.o : gsp.h database.h
database_init.o : gsp.h database.h
database_prune.o : gsp.h database.h
database_create.o : gsp.h database.h
sequence.o : gsp.h

.PHONY : clean 

clean :
	rm gsp data_provider $(objects) 
