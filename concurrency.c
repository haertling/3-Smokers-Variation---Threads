//  Steven Haertling, Danny Nomura, Konstantin Sanosyan
//  concurrency.c
//  This program demonstrates a similar solution to that of "the 3 smokers" problem, using concurrency primitives.
//  Here we have 4 particular threads, one for the chef, a customer who has infinite burgers,
//  a customer with infinite fries, and a customer with infinite soda. They each desire the other items.
//  The chef will produce 2 items randomly, out of burgers, fries, and soda.

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h> // Null pointer and sleep()

// Function Initialization
void *gordonRamsay(void *a);// chef function, in honor of the great gordon ramsay
void *giveBurger(void *a);// chef giving burger to customer
void *giveFries(void *a);// chef giving fries to customer
void *giveSoda(void *a);// chef giving soda to customer
void *burgerGuy(void *a);// customer with infinite burgers
void *fryGuy(void *a);// customer with infinite fries
void *sodaGuy(void *a);// customer with infinite soda

// Global variable initialization
// Counters (for keeping track)
int generalCount;
int burgerCount;
int friesCount;
int sodaCount;
// keep track of jobs running
int chefJobs;
int burgerJobs;
int friesJobs;
int sodaJobs;
// Boolean values to determine who has an item currently
// Note that for the 3 customers, one already has infinite fries, one has infinite burgers, and another infinite soda
bool hasBurger = false;
bool hasFries = false;
bool hasSoda = false;

// Initialize mutexes for chef and customer
pthread_mutex_t chefLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t customerLock = PTHREAD_MUTEX_INITIALIZER;

// Initialize conditionals for individual items
pthread_cond_t burger = PTHREAD_COND_INITIALIZER;
pthread_cond_t fries = PTHREAD_COND_INITIALIZER;
pthread_cond_t soda = PTHREAD_COND_INITIALIZER;

// Initialize conditionals for chef and assigned items - Our designated threads
pthread_cond_t condChef = PTHREAD_COND_INITIALIZER;
pthread_cond_t condCustmerBurger = PTHREAD_COND_INITIALIZER;
pthread_cond_t condCustumerFries = PTHREAD_COND_INITIALIZER;
pthread_cond_t condCustomerSoda = PTHREAD_COND_INITIALIZER;

// Main is simple: all the work goes down in the give/take functions
int main(int argc, char *argv[])
{
    // thread creation
    pthread_t chefThread;
    pthread_t giveBurgerThread;
    pthread_t giveFriesThread;
    pthread_t giveSodaThread;
    pthread_t burgerGuyThread;
    pthread_t fryGuyThread;
    pthread_t sodaGuyThread;
    
    // Check that each thread creation worked with proper notation
    // Thread creation returns 0, if nothing returned 0, thread was not created
    if(pthread_create(&chefThread, NULL, gordonRamsay, NULL) != 0)
    {
        printf("Unable to create chef thread, exiting\n");
        exit(1);
    }
    
    if(pthread_create(&giveBurgerThread, NULL, giveBurger, NULL) != 0)
    {
        printf("Unable to create giveBurger thread, exiting\n");
        exit(1);
    }
    
    if(pthread_create(&giveFriesThread, NULL, giveFries, NULL) != 0)
    {
        printf("Unable to create giveFries thread, exiting\n");
        exit(1);
    }
    
    if(pthread_create(&giveSodaThread, NULL, giveSoda, NULL) != 0)
    {
        printf("Unable to create giveFries thread, exiting\n");
        exit(1);
    }
    
    if(pthread_create(&burgerGuyThread, NULL, burgerGuy, NULL) != 0)
    {
        printf("Unable to create burgerGuy thread, exiting\n");
        exit(1);
    }
    
    if(pthread_create(&fryGuyThread, NULL, fryGuy, NULL) != 0)
    {
        printf("Unable to create fryGuy thread, exiting\n");
        exit(1);
    }
    
    if(pthread_create(&sodaGuyThread, NULL, sodaGuy, NULL) != 0)
    {
        printf("Unable to create sodaGuy thread, exiting\n");
        exit(1);
    }
    
    pthread_join(chefThread, NULL);
    pthread_join(giveBurgerThread, NULL);
    pthread_join(giveFriesThread, NULL);
    pthread_join(giveSodaThread, NULL);
    pthread_join(burgerGuyThread, NULL);
    pthread_join(fryGuyThread, NULL);
    pthread_join(sodaGuyThread, NULL);
}

void *gordonRamsay(void *a)
{
    while(1)
    {
        sleep(1); // sleep temporarily at every iteration (1s)
        // If you want to skip the wait, you can comment out the previous line; results are the same
        
        // Set the lock
        pthread_mutex_lock(&chefLock);
        
        // Update counter
        generalCount += 1;
        
        // Format output for each run
        // If we are still running, indicate number of passes
        // If we have reached max number of runs (100) output total stats
        if(generalCount <= 100)
        {
            printf("-----Run Number: %d-----\n", generalCount);
        }
        if(generalCount > 100)
        {
            printf("----------------------------------------\n");
            printf("-----Overall Statistics-----------------\n");
            printf("-----Burger guy ate: %d times-----------\n", burgerCount);
            printf("-----Fry guy ate:    %d times-----------\n", friesCount);
            printf("-----Soda guy ate:   %d times-----------\n", sodaCount);
            printf("----------------------------------------\n");
            break; // exit loop
        }
        
        // randomly cook either burger and fries, soda and fries, or burger and soda
        // generate random number from 0-2
        int choice = rand() % 3;
        
        // Burger and fries chosen to be cooked
        if(choice == 0)
        {
            // gets burger and fries
            hasBurger = true;
            hasFries = true;
            // sets chef jobs and missing item
            chefJobs = 0;
            sodaCount += 1;
            // signal to conditionals
            pthread_cond_signal(&burger);
            pthread_cond_signal(&fries);
            // Some output to keep track
            printf("Chef Ramsay Decides to Cook Burger and Fries\n");
        }
        
        // Soda and Fries chosen to be cooked
        if(choice == 1)
        {
            // gets soda and fries
            hasSoda = true;
            hasFries = true;
            // sets chef jobs and missing item
            chefJobs = 0;
            burgerCount += 1;
            // signal to conditionals
            pthread_cond_signal(&soda);
            pthread_cond_signal(&fries);
            // Some output to keep track
            printf("Chef Ramsay Decides to Cook Soda and Fries\n");
        }
        
        // Burger and Soda Chosen to be cooked
        if(choice == 2)
        {
            // gets burger and soda
            hasBurger = true;
            hasSoda = true;
            // sets chef jobs and missing item
            chefJobs = 0;
            friesCount += 1;
            // signal to conditionals
            pthread_cond_signal(&burger);
            pthread_cond_signal(&soda);
            // Some output to keep track
            printf("Chef Ramsay Decides to Cook Burger and Soda\n");
        }
        
        // End the loop, Unlock
        pthread_mutex_unlock(&chefLock);
    }
    return 0;
}

void *giveBurger(void *a)
{
    while(1)
    {
        // Set the lock
        pthread_mutex_lock(&chefLock);
        
        // wait for customer to be ready for a burger
        if(!hasBurger)
        {
            printf("Waiting for burger...\n");
            pthread_cond_wait(&burger, &chefLock);
        }
        
        // Check if customer has fries or soda
        // If so, call customers who dont
        if(hasSoda)
        {
            hasSoda = false;
            friesJobs = 1;
            chefJobs = 0;
            // call customer waiting on fries instead
            pthread_cond_signal(&condCustumerFries);
        }
        
        if(hasFries)
        {
            hasFries = false;
            sodaJobs = 1;
            chefJobs = 0;
            // call customer waiting on soda instead
            pthread_cond_signal(&condCustomerSoda);
        }
        // Unlock
        pthread_mutex_unlock(&chefLock);
    }
    return 0;
}

void *giveFries(void *a)
{
    while(1)
    {
        // Set the lock
        pthread_mutex_lock(&chefLock);
        
        // wait for customer to be ready for fries
        if(!hasFries)
        {
            printf("Waiting for fries...\n");
            pthread_cond_wait(&fries, &chefLock);
        }
        
        // Check if customer has burger or soda
        // If so, call customers who dont
        if(hasSoda)
        {
            hasSoda = false;
            burgerJobs = 1;
            chefJobs = 0;
            // call customer waiting on burger instead
            pthread_cond_signal(&condCustmerBurger);
        }
        
        if(hasBurger)
        {
            hasBurger = false;
            sodaJobs = 1;
            chefJobs = 0;
            // call customer waiting on soda instead
            pthread_cond_signal(&condCustomerSoda);
        }
        // Unlock
        pthread_mutex_unlock(&chefLock);
    }
    return 0;
}

void *giveSoda(void *a)
{
    while(1)
    {
        // Set the lock
        pthread_mutex_lock(&chefLock);
        
        // wait for customer to be ready for soda
        if(!hasSoda)
        {
            printf("Waiting for soda...\n");
            pthread_cond_wait(&soda, &chefLock);
        }
        
        // Check if customer has burger or fries
        // If so, call customers who dont
        if(hasBurger)
        {
            hasBurger = false;
            friesJobs = 1;
            chefJobs = 0;
            // call customer waiting on fries instead
            pthread_cond_signal(&condCustumerFries);
        }
        
        if(hasFries)
        {
            hasFries = false;
            burgerJobs = 1;
            chefJobs = 0;
            // call customer waiting on burger instead
            pthread_cond_signal(&condCustmerBurger);
        }
        // Unlock
        pthread_mutex_unlock(&chefLock);
    }
    return 0;
}

void *burgerGuy(void *a)
{
    while(1)
    {
        // Set the lock
        pthread_mutex_lock(&customerLock);
        
        // wait for customer to be ready for food
        while(burgerJobs == 0)
        {
            pthread_cond_wait(&condCustmerBurger, &customerLock);
        }
        
        // Doesnt have fries or soda
        hasSoda = false;
        hasFries = false;
        burgerJobs = 0;
        chefJobs = 1;
        
        printf("Burger guy takes fries and soda...\n");
        
        // Unlock
        pthread_mutex_unlock(&customerLock);
        
        printf("Burger guy eats and is hungry again\n");
    }
    return 0;
}

void *fryGuy(void *a)
{
    while(1)
    {
        // Set the lock
        pthread_mutex_lock(&customerLock);
        
        // wait for customer to be ready for food
        while(friesJobs == 0)
        {
            pthread_cond_wait(&condCustumerFries, &customerLock);
        }
        
        // Doesnt have burger or soda
        hasSoda = false;
        hasBurger = false;
        friesJobs = 0;
        chefJobs = 1;
        
        printf("Fry guy takes Burger and soda...\n");
        
        // Unlock
        pthread_mutex_unlock(&customerLock);
        
        printf("Fry guy eats and is hungry again\n");
    }
    return 0;
}

void *sodaGuy(void *a)
{
    while(1)
    {
        // Set the lock
        pthread_mutex_lock(&customerLock);
        
        // wait for customer to be ready for food
        while(sodaJobs == 0)
        {
            pthread_cond_wait(&condCustomerSoda, &customerLock);
        }
        
        // Doesnt have fries or soda
        hasBurger = false;
        hasFries = false;
        sodaJobs = 0;
        chefJobs = 1;
        
        printf("Soda guy takes fries and burger...\n");
        
        // Unlock
        pthread_mutex_unlock(&customerLock);
        
        printf("Soda guy eats and is hungry again\n");
    }
    return 0;
}
