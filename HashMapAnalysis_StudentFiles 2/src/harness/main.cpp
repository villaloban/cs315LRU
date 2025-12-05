//
// Created by Anthony Villalobos on 12/3/25.
//

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include "Operations.hpp"
#include "HashTableDictionary.hpp"
#include <chrono>
#include <utility>



bool load_trace_strict_header(const std::string &path,
    std::size_t &N,
                              std::vector<Operation> &out_operations) {
    std::string profile = "";
    int seed = 0;
    out_operations.clear();

    std::ifstream in(path);
    if (!in.is_open())
        return false;

    // --- read FIRST line as header
    std::string header;
    if (!std::getline(in, header))
        return false;

    // Look for a non-while-space character
    const auto first = header.find_first_not_of(" \t\r\n");
    // Since this is the first line, we don't expect it to be blank
    // or start with a comment.
    if (first == std::string::npos || header[first] == '#')
        return false;

    // Create a string stream so that we can read the profile name,
    // N, and the seed more easily.
    std::istringstream hdr(header);
    if (!(hdr >> profile >> N >> seed))
        return false;


    // --- read ops, allowing comments/blank lines AFTER the header ---
    std::string line;
    while (std::getline(in, line)) {
        const auto opCodeIdx = line.find_first_not_of(" \t\r\n");
        if (opCodeIdx == std::string::npos || line[opCodeIdx] == '#')
            continue; // skip blank and comment lines.

        std::istringstream iss(line.substr(opCodeIdx));
        std::string tok; //
        if (!(iss >> tok))
            continue;

        std::string w1, w2; // there are two words after the op code.
                            // Maybe we should add a dash in between in the data file.
        if (tok == "I") {
            if (!(iss >> w1 >> w2)) return false;
            out_operations.emplace_back(OpCode::Insert, w1.append(" ") + w2);
        } else if (tok == "E") {
            if (!(iss >> w1 >> w2)) return false;
//            std::cout << "w1 = " << w1 << " w2 = " << w2 << std::endl;
            out_operations.emplace_back(OpCode::Erase, w1.append(" ") + w2);
        } else {
            std::cout << "Unknown operation in load_trace_strict_header: " << tok << std::endl;
            return false; // unknown token
        }
    }

    return true;
}

std::size_t tableSizeForN(std::size_t N) {
   static const std::vector<std::pair<std::size_t, std::size_t>> N_and_primes = {
        /* N = 2^10 = 1,024  */ { 1024,    1279    },
        /* N = 2^11 = 2,048  */ { 2048,    2551    },
        /* N = 2^12 = 4,096  */ { 4096,    5101    },
        /* N = 2^13 = 8,192  */ { 8192,   10273   },
        /* N = 2^14 = 16,384 */ { 16384,   20479   },
        /* N = 2^15 = 32,768 */ { 32768,   40849   },
        /* N = 2^16 = 65,536 */ { 65536,   81931   },
        /* N = 2^17 = 131,072*/ { 131072,  163861  },
        /* N = 2^18 = 262,144*/ { 262144,  327739  },
        /* N = 2^19 = 524,288*/ { 524288,  655243  },
        /* N = 2^20 = 1,048,576*/{ 1048576, 1310809 }
    };

    for (auto item: N_and_primes) {
        if (item.first == N)
            return item.second;
    }
    std::cout << "Unable to find table size for " << N << " in RunMetaData." << std::endl;
    exit(1);
}


int main(int argc, char *argv[]) {


    if( argc != 2 ) {
        std::cout << "usage: " << argv[0] << " nameOfInputFile\n";
        exit(1);
    }

    std::fstream inputStream;
    inputStream.open(argv[1], std::ios::in);
    if( !inputStream.is_open() ) {
        std::cout << "Unable to open " << argv[1] << ". Terminating...\n";
        exit(2);
    }


    std::vector<Operation> operations;
    std::size_t N = 0;
    load_trace_strict_header(std::string(argv[1]), N, operations);

    auto runTimed = [&](HashTableDictionary::PROBE_TYPE pType,
                        bool doCompact,
                        double &median_out)
    {
        std::vector<double> times;

        {
            HashTableDictionary warm(tableSizeForN(N), pType, doCompact);
            for (const auto &op : operations)
                (op.tag == OpCode::Insert) ? warm.insert(op.key) : warm.remove(op.key);
        }

        for (int rep = 0; rep < 7; rep++) {
            HashTableDictionary table(tableSizeForN(N), pType, doCompact);

            auto start = std::chrono::steady_clock::now();
            for (const auto &op : operations)
                (op.tag == OpCode::Insert) ? table.insert(op.key) : table.remove(op.key);
            auto end = std::chrono::steady_clock::now();

            double ms = std::chrono::duration<double, std::milli>(end - start).count();
            times.push_back(ms);
        }

        std::sort(times.begin(), times.end());
        median_out = times[3]; // median of 7
    };

    double median_single = 0.0;
    double median_double = 0.0;

    runTimed(HashTableDictionary::SINGLE, true, median_single);
    runTimed(HashTableDictionary::DOUBLE, true, median_double);
    HashTableDictionary::PROBE_TYPE pType = HashTableDictionary::DOUBLE;
    auto doWePerformCompaction = true;
    HashTableDictionary hashDictionary(
            tableSizeForN(N), pType, doWePerformCompaction);
    
    std::string filename = "hash_table_results_" + std::to_string(N) + ".csv";
    std::ofstream csvOut(filename);
    if (!csvOut.is_open()) {
        std::cout << "Failed to create CSV file.\n";
        return 3;
    }

    csvOut << "impl,profile,trace_path,N,seed,elapsed_ms,ops_total,average_probes,"
              "eff_load_factor_pct,load_factor_pct,tombstones_pct,full_scans,compactions,compaction_state\n";

    {
        HashTableDictionary t(tableSizeForN(N), HashTableDictionary::SINGLE, true);
        for (auto &op : operations)
            (op.tag == OpCode::Insert) ? t.insert(op.key) : t.remove(op.key);

        csvOut << t.csvStats(
            "hash_map_single",
            "lru_profile",
            argv[1],
            N,
            23,                 
            std::to_string(median_single),
            "compaction_on") << "\n";
    }

    {
        HashTableDictionary t(tableSizeForN(N), HashTableDictionary::DOUBLE, true);
        for (auto &op : operations)
            (op.tag == OpCode::Insert) ? t.insert(op.key) : t.remove(op.key);

        csvOut << t.csvStats(
            "hash_map_double",
            "lru_profile",
            argv[1],
            N,
            23,                  
            std::to_string(median_double),
            "compaction_on") << "\n";
    }

    csvOut.close();
    std::cout << "CSV file 'hash_table_results.csv' created successfully.\n";


    for (const auto &op: operations) {
        // op.print();

        switch (op.tag) {
            case OpCode::Insert:
                hashDictionary.insert(op.key);
            break;
            case OpCode::Erase:
                (void) hashDictionary.remove(op.key);
            break;
        }
    }

    hashDictionary.printMask();
    hashDictionary.printStats();
    if (doWePerformCompaction)
        hashDictionary.printBeforeAndAfterCompactionMaps();
    else
        hashDictionary.printActiveDeleteMap();
    return 0;
}
