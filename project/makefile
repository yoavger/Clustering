FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS = -lm

all: mat.o b_hat.o divide_to_two.o modularity_maximization.o divide_to_modularity_groups.o read_and_write.o main.o
	gcc mat.o b_hat.o divide_to_two.o modularity_maximization.o divide_to_modularity_groups.o read_and_write.o main.o -o cluster $(LIBS)
clean:
	rm -rf *.o cluster

mat.o: mat.c mat.h 
	gcc $(FLAGS) -c mat.c

b_hat.o: b_hat.c b_hat.h mat.h 
	gcc $(FLAGS) -c b_hat.c

divide_to_two.o: divide_to_two.c divide_to_two.h b_hat.h
	gcc $(FLAGS) -c divide_to_two.c

modularity_maximization.o: modularity_maximization.c modularity_maximization.h divide_to_two.h
	gcc $(FLAGS) -c modularity_maximization.c

divide_to_modularity_groups.o: divide_to_modularity_groups.c divide_to_modularity_groups.h modularity_maximization.h
	gcc $(FLAGS) -c divide_to_modularity_groups.c

read_and_write.o: read_and_write.c read_and_write.h divide_to_modularity_groups.h
	gcc $(FLAGS) -c read_and_write.c

main.o: main.c divide_to_modularity_groups.h 
	gcc $(FLAGS) -c main.c