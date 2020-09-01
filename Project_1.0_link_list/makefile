FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS = -lm

all: mat.o bHat.o divideToTwo.o ModularityMaximization.o DivideToModularityGroups.o ReadAndWrite.o main.o
	gcc mat.o bHat.o divideToTwo.o ModularityMaximization.o DivideToModularityGroups.o ReadAndWrite.o main.o -o cluster $(LIBS)
clean:
	rm -rf *.o cluster

mat.o: mat.c mat.h 
	gcc $(FLAGS) -c mat.c

bHat.o: bHat.c bHat.h mat.h 
	gcc $(FLAGS) -c bHat.c

divideToTwo.o: divideToTwo.c divideToTwo.h bHat.h
	gcc $(FLAGS) -c divideToTwo.c

ModularityMaximization.o: ModularityMaximization.c ModularityMaximization.h divideToTwo.h
	gcc $(FLAGS) -c ModularityMaximization.c

DivideToModularityGroups.o: DivideToModularityGroups.c DivideToModularityGroups.h ModularityMaximization.h
	gcc $(FLAGS) -c DivideToModularityGroups.c

ReadAndWrite.o: ReadAndWrite.c ReadAndWrite.h DivideToModularityGroups.h
	gcc $(FLAGS) -c ReadAndWrite.c

main.o: main.c DivideToModularityGroups.h 
	gcc $(FLAGS) -c main.c