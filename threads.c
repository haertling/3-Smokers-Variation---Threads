//Alexandra, Danny, Konstantin, Steven

/*
use 4 threads : 1 chef, 3 patrons
use concurrency primitives to ensure items are distrubuted and no deadlocks
chef: produces two random unique components, and sleeps until items are gone
Patrons: needs 3 items for meal(burger, fries, soda), #1 has infinite burgers, #2 fries, and #3 sodas
run 100 chefs, track eat time a customer eats and print stats of each patron at the end
*/

// includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <err.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// function declarations
void chef();
void patron();
void pusher();

// global variables
char* patron_needs[3] = {"fries and soda", "burger and soda", "burger and fries"};
bool food[3] = { false, false, false };//burger then fries then soda
sem_t food_gone;
sem_t patron_lock[3];
//main
int main(int argc, char **argv){
	
	srand(time(NULL));
	
	
	
	
	printf("");
	exit(0);
}

//chef
void chef(){
	int food = rand()%3;
	
	usleep(rand()%20000);
	//waiting for bell, for food to be taken
	sem_wait(&food_gone);
	//pass food out
	sem_post(&pusher_semaphores[food]);
	sem_post(&pusher_semaphores[(food +1)%3]);
	
	return 0;
}

void patron(){
	int patron = ;
	int patron_type = patron % 3;
	//wait for food
	sem_wait(&patron_lock[patron_type]);
	//eat
	
	//increment id eaten counter
	
}

void pusher(){
	
}