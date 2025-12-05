//
// Created by Anthony Villalobos on 12/2/25.
//

#include <algorithm>
#include <random>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <list>


std::string makeTraceFilename(unsigned seed, int n) {
    return "lru_profile_N_" + std::to_string(n) + "_S_" + std::to_string(seed) + ".trace";
}

void generateLruTrace(unsigned seed, int n) {
    std::ifstream in("6770_uniq_words.txt");

    if (!in.is_open()) {
        std::cout << "Failed to open 6770_uniq_words.txt" << "\n";
        return;
    }
    std::vector<std::string> words;
    std::string w;
     for (int i = 0; i < 4* n && in >> w; i++)
         words.push_back(w);


    std::vector<std::string> bag;

    for (int i = 0; i < n; i++)
        bag.push_back(words.at(i));

    for (int i = n; i < 2 * n; i++)
        for (int k = 0; k < 5; k++)
            bag.push_back(words[i]);

    for (int i = 2 * n; i < 4 * n; i++)
        for (int k = 0; k < 3; k++)
            bag.push_back(words[i]);

    std::mt19937 rng(seed);
    std::shuffle(bag.begin(), bag.end(), rng);

    std::string outname = makeTraceFilename(seed, n);
    std::ofstream oFS(outname);
    if (!oFS.is_open()) {
        std::cout << "Failed to write to " << outname << "\n";

    }

    oFS << "lru_profile " << n << " " << seed << "\n";

    std::list<std::string> lru_list;
    std::unordered_map<std::string, std::list<std::string>::iterator> resident;

    for (const std::string &key : bag) {
        auto it = resident.find(key);

        if (it != resident.end()) {
            lru_list.erase(it->second);
            lru_list.push_front(key);
            resident[key] = lru_list.begin();
            oFS << "I " << key << " scrunching""\n";
        }
        else{
        if (resident.size() < n) {
            lru_list.push_front(key);
            resident[key] = lru_list.begin();
            oFS << "I " << key << " scrunching""\n";
        }
        else {
            std::string victim = lru_list.back();
            lru_list.pop_back();
            resident.erase(victim);

            oFS << "E " << victim << " scrunching""\n";

            lru_list.push_front(key);
            resident[key] = lru_list.begin();
            oFS << "I " << key << " scrunching""\n";
            }
        }
    }
    oFS.close();
}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./lru_gen <N>\n";
        return 1;
    }

    unsigned seed = 23; 
    int n = std::stoi(argv[1]);

    generateLruTrace(seed, n);
    return 0;
}
