```markdown
# Simulador de Sistema Operacional - Documentação de Implementação

## 🔄 Fases Implementadas

### 1. Gerenciamento de Processos

#### 1.1 Expansão do PCB (Process Control Block)

**Melhorias Implementadas:**

- Adição de estados do processo: NEW, READY, RUNNING, BLOCKED, TERMINATED
- Implementação de controle de quantum para preempção.
- Contadores de tempo: waiting_time, cpu_time
- Gerenciamento de recursos alocados.
- Separação entre dados e código na memória.

#### 1.2 Gerenciamento de Estados de Processo

**Funcionalidades Implementadas:**

- Transições de estado: NEW → READY → RUNNING → TERMINATED
- Preempção por quantum.
- Escalonamento FCFS (First-Come-First-Served).
- Sincronização entre cores usando mutex.
- Gerenciamento de filas de processos.

### 2. Sistema de Memória (Cache)

#### 2.1 Estrutura da Cache

```cpp
// Definição do bloco de cache
struct CacheBlock {
    bool valid;     // Indica se o bloco contém dados válidos
    bool dirty;     // Indica se o bloco foi modificado
    int tag;        // Tag do endereço de memória
    int data;       // Dados armazenados
};

class Cache {
private:
    vector<CacheBlock> blocks;     // Blocos de cache (16 blocos)
    queue<int> fifo_queue;         // Fila para política FIFO
    Ram* ram;                      // Ponteiro para a RAM
    pthread_mutex_t cache_mutex;   // Mutex para sincronização
};
```

#### 2.2 Integração com o Sistema

**Modificações na CPU:**

```cpp
class Cpu {
private:
    Cache* cache;  // Adicionado ponteiro para cache
    Ram* ram;      // RAM mantida para compatibilidade

public:
    // Construtor atualizado
    Cpu(int id, Ram* ram, Cache* cache) 
        : id(id), cache(cache), ram(ram) {}
};
```

**Modificações no OperatingSystem:**

```cpp
class OperatingSystem {
private:
    Cache* cache;  // Cache compartilhada entre cores
    
public:
    OperatingSystem(Scheduler* scheduler, Ram* ram) {
        this->cache = new Cache(ram);  // Inicialização da cache
        // Passa cache para cada core
        for (int i = 0; i < CORES_COUNT; i++) {
            Cpu core(i, ram, cache);
            cores.push_back(core);
        }
    }
};
```

#### 2.3 Operações Fundamentais

**Leitura (Read):**

```cpp
int Cache::read(int address) {
    pthread_mutex_lock(&cache_mutex);
    
    // Verifica hit
    if (blocks[index].valid && blocks[index].tag == tag) {
        return blocks[index].data;  // Cache hit
    }
    
    // Cache miss - carrega da RAM
    blocks[index].data = ram->get_value(address);
    blocks[index].tag = tag;
    blocks[index].valid = true;
    
    return blocks[index].data;
}
```

**Escrita (Write):**

```cpp
void Cache::write(int address, int value) {
    pthread_mutex_lock(&cache_mutex);
    
    // Write-through
    ram->set_value(address, value);
    
    // Atualiza cache
    blocks[index].data = value;
    blocks[index].tag = tag;
    blocks[index].valid = true;
}
```

### 3. Memory Logger

#### 3.1 Objetivo

O MemoryLogger foi implementado para registrar as operações de memória e o estado final da RAM e da Cache durante a execução do simulador. Isso permite acompanhar as alterações na memória e depurar o comportamento do sistema.

#### 3.2 Funcionamento

**Singleton Pattern:**

O MemoryLogger utiliza o padrão Singleton, garantindo uma única instância durante a execução do sistema.

```cpp
MemoryLogger* MemoryLogger::getInstance(Ram* ram, Cache* cache) {
    if (instance == nullptr) {
        instance = new MemoryLogger();
        instance->ram = ram;
        instance->cache = cache;
        instance->open_log_file();  
    }
    return instance;
}
```

**Registro de Operações:**

As operações de memória, como LOAD e STORE, são logadas durante a execução do simulador.

```cpp
void MemoryLogger::log_memory_operation(const string& op, int address, int value) {
    log_file << "\n[Memory Operation] " 
             << "Time: " << time(nullptr) << endl
             << "Operation: " << op << endl
             << "Address: " << address << endl
             << "Value: " << value << endl;
}
```

**Estado Final da Memória:**

Ao término da execução, o logger registra o estado completo da RAM e da Cache.

```cpp
void MemoryLogger::log_final_state() {
    if (!ram || !cache || !log_file.is_open()) {
        return;
    }

    log_file << "\n=== Final Memory State ===" << endl;
    log_file << "RAM Contents:" << endl;
    
    // Print RAM contents
    for (int i = 0; i < 32; i++) {
        log_file << "Address " << setw(2) << i 
                << ": " << setw(5) << ram->get_value(i) << endl;
    }
    
    // Print Cache contents
    log_file << "\nCache Contents:" << endl;
    for (int i = 0; i < CACHE_SIZE; i++) {
        CacheBlock block = cache->get_block(i);
        if (cache && block.valid) {
            log_file << "Block " << setw(2) << i 
                    << ": Tag=" << block.tag 
                    << " Data=" << block.data 
                    << " Dirty=" << block.dirty << endl;
        }
    }
    
    log_file << "\n=== End of Memory State ===" << endl;
    log_file.flush();
}
```

#### 3.3 Integração com o Simulador

- **Inicialização:** O MemoryLogger é iniciado junto com o simulador.
- **Operações de Memória:** Cada operação executada pela CPU é registrada.
- **Estado Final:** O estado completo da memória é salvo ao final da execução ou em casos de interrupção.

```cpp
// Inicialização no main.cpp
auto logger = MemoryLogger::getInstance(&ram, &cache);
logger->log_memory_operation("INIT", 0, 0);

// Registro no signal handler
void signal_handler(int signum) {
    if (global_ram && global_cache) {
        auto logger = MemoryLogger::getInstance(global_ram, global_cache);
        logger->log_final_state();
        logger->close_log_file();
    }
    exit(signum);
}
```

## 🔧 Melhorias Técnicas

1. **Sincronização**
   - Implementação de mutex para proteger acessos à cache e memória compartilhada.
   - Redução significativa de erros em ambientes multicore.

2. **Execução Multicore**
   - Cache compartilhada entre os núcleos.
   - Escalonamento independente e preempção com controle por quantum.

3. **Integração com Pipeline MIPS**
   - Controle básico de hazards e integração em cinco estágios: IF, ID, EX, MEM, WB.

## 📊 Estado Atual

- Suporte a múltiplos núcleos.
- Cache implementada e funcionando.
- Gerenciamento de processos básico operacional.
- Registro detalhado de operações com o MemoryLogger.

## 🔜 Próximos Passos

-  memória secundária?
- testar a cache
- tratar as interrupuções I/O
