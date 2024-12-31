#include "Cache.hpp"

Cache::Cache(Ram* ram) : ram(ram) {
    blocks.resize(CACHE_SIZE);
    pthread_mutex_init(&cache_mutex, NULL);
}

Cache::~Cache() {
    // Write back dirty blocks
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (blocks[i].dirty) {
            write_back(i);
        }
    }
    pthread_mutex_destroy(&cache_mutex);
}

int Cache::read(int address) {
    pthread_mutex_lock(&cache_mutex);
    
    int tag = get_tag(address);
    int index = get_index(address);
    
    // Cache hit
    if (blocks[index].valid && blocks[index].tag == tag) {
        pthread_mutex_unlock(&cache_mutex);
        return blocks[index].data;
    }
    
    // Cache miss
    if (blocks[index].valid && blocks[index].dirty) {
        write_back(index);
    }
    
    // Load from RAM
    blocks[index].data = ram->get_value(address);
    blocks[index].tag = tag;
    blocks[index].valid = true;
    blocks[index].dirty = false;
    
    if (fifo_queue.size() >= CACHE_SIZE) {
        fifo_queue.pop();
    }
    fifo_queue.push(index);
    
    pthread_mutex_unlock(&cache_mutex);
    return blocks[index].data;
}

void Cache::write(int address, int value) {
    pthread_mutex_lock(&cache_mutex);
    ram->set_value(address, value);
    
    int tag = get_tag(address);
    int index = get_index(address);
    
    blocks[index].data = value;
    blocks[index].tag = tag;
    blocks[index].valid = true;
    blocks[index].dirty = false; // Not dirty since we wrote to RAM
    
    pthread_mutex_unlock(&cache_mutex);
}

void Cache::write_back(int index) {
    if (blocks[index].valid && blocks[index].dirty) {
        int address = blocks[index].tag * BLOCK_SIZE + index;
        ram->set_value(address, blocks[index].data);
        blocks[index].dirty = false;
    }
}

int Cache::get_replacement_index() {
    if (!fifo_queue.empty()) {
        int index = fifo_queue.front();
        fifo_queue.pop();
        return index;
    }
    return 0;
}

bool Cache::is_hit(int address) {
    int tag = get_tag(address);
    int index = get_index(address);
    return blocks[index].valid && blocks[index].tag == tag;
}