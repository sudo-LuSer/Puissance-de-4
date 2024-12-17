exe: puissance4.o ai_player.o
	gcc -o exe puissance4.o ai_player.o 
puissance4.o: puissance4.c
	gcc -c puissance4.c 
ai_player.o: ai_player.c ai_player.h
	gcc -c ai_player.c 
