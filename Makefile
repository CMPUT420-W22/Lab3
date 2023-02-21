all: Lab3IO datagen main 

datagen:
	gcc datagen.c Lab3IO.c -o datagen

datagen.o: datagen.c Lab3IO.h
	gcc -c -o datagen.o datagen.c

main: main.o
	g++ -o main Lab3IO.o main.o

main.o: main.cpp Lab3IO.h
	g++ -c -o main.o main.cpp

Lab3IO.o: Lab3IO.c Lab3IO.h
	gcc -c -o Lab3IO.o Lab3IO.c

clean:
	rm *.o
	rm main
	rm datagen
	rm data_*