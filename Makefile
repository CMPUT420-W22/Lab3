all: datagen main

datagen:
	gcc datagen.c Lab3IO.c -o datagen

datagen.o: datagen.c Lab3IO.h
	gcc -c -o datagen.o datagen.c

main: main.o Lab3IO.o
	g++ -o main main.o Lab3IO.o -fopenmp

main.o: main.cpp Lab3IO.h timer.h 
	g++ -c -o main.o main.cpp 

Lab3IO.o: Lab3IO.c Lab3IO.h 
	gcc -c -o Lab3IO.o Lab3IO.c 

test: 
	gcc serialtester.c 	Lab3IO.c -o serialtester -lm
	./serialtester

clean:
	rm *.o
	rm main
	rm datagen
	rm data_*