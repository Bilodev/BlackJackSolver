link: AI.o queue.o seed.o deck.o player.o main.o
	gcc AI.o queue.o seed.o deck.o player.o main.o -o main.exe -lraylib

queue.o:
	gcc -c lib/queue.c

AI.o:
	gcc -c lib/AI.c

player.o:
	gcc -c lib/player.c

deck.o:
	gcc -c lib/deck.c

seed.o:
	gcc -c lib/seed.c

main.o:
	gcc -c main.c -lm


clean:
	rm -r *.exe *.o