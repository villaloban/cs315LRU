Student Info: Name: Anthony Villalobos Student ID: 008394627 Repo Link: https://github.com/villaloban/cs315LRU

Collaboration & Sources: In this project I collaberated with classmates, and also looked up several tutorials such as how to use and unordered map, how to use a clock, write to csv's and also used ai for debugging since a lot of that went on in this project. 

Implementation & Details: In this project we were tasked to create a LRU profile that would test single probing and double probing that was being tested before and after compaction as well as several other metrics, it printed an active and deleted map and generated a map of 0/1's to use for a histogram. As well as outoputting a csv for reading in a graph as well.

Testing & Status: 
There were a lot of trials with this project and the way we tested them for the generator was, g++ -std=c++17 -0 src/trace-generators/lru-generator/main.cpp -o lru_g 
./lru_gen 2048
Giving it the N and then the harness
 g++ -std=c++17 -O2 src/harness/main.cpp HashTableDictionary.cpp -I. -o harness_run

 ./harness_run lru_profile_N_4096_S_23.trace > output_histogram.txt


ISSUES: I had a lot of issues with the program when running large data sets i would get an alloc error /lru_gen 32768
terminate called after throwing an instance of 'std::out_of_range'
  what():  vector::_M_range_check: __n (which is 6770) >= this->size() (which is 6770)
Aborted (core dumped)

Which would stop me from running large files. This stopped me from being able to get data necesarry to complete the written part of the project. I also had issues with my csv thats stopped me from fully being able to take this project in as a whole. I still enjoyed and soaked up all the information from the project but wished I had more time to fully get everything working.  
