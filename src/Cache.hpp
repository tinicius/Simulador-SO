#ifndef CACHE_HPP
#define CACHE_HPP

#include <vector>
#include <queue>
#include <pthread.h>
#include "Ram.hpp"

#define CACHE_SIZE 16
#define BLOCK_SIZE 4

struct CacheBlock {
    bool valid;     // Block contains valid data
    bool dirty;     // Block was modified
    int tag;        // Memory address tag
    int data;       // Actual data
    
    CacheBlock() : valid(false), dirty(false), tag(-1), data(0) {}
};

class Cache {
private:
    vector<CacheBlock> blocks;
    queue<int> fifo_queue;
    Ram* ram;
    pthread_mutex_t cache_mutex;
    

public:
    Cache(Ram* ram);
    ~Cache();
    
    int read(int address);
    void write(int address, int value);
    bool is_hit(int address);
    void write_back(int block_index);
    int get_replacement_index();

private:
    int get_tag(int address) { return address / BLOCK_SIZE; }
    int get_index(int address) { return address % CACHE_SIZE; }
};

#endif