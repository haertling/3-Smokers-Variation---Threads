use 4 threads : 1 chef, 3 patrons
use concurrency primitives to ensure items are distrubuted and no deadlocks
chef: produces two random unique components, and sleeps until items are gone
Patrons: needs 3 items for meal(burger, fries, soda), #1 has infinite burgers, #2 fries, and #3 sodas
run 100 chefs, track eat time a customer eats and print stats of each patron at the end
