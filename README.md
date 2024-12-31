# Simulador de Sistema Operacional - Documentação de Implementação

## 🔄 Fases Implementadas

### **Fase 1: Gerenciamento de Processos**

#### 1.1 Expansão do PCB (Process Control Block)
**Melhorias Implementadas:**
- Adição de estados do processo:
  - `NEW`, `READY`, `RUNNING`, `BLOCKED`, `TERMINATED`
- Implementação de controle de quantum para preempção.
- Contadores de tempo:
  - `waiting_time`, `cpu_time`
- Gerenciamento de recursos alocados.
- Separação entre dados e código na memória.

#### 1.2 Gerenciamento de Estados de Processo
**Funcionalidades Implementadas:**
- Transições de estado:
  - `NEW → READY → RUNNING → TERMINATED`
- Preempção por quantum.
- Escalonamento FCFS (First-Come-First-Served).
- Sincronização entre cores usando mutex.
- Gerenciamento de filas de processos.

---

### **Fase 2: Sistema de Memória**

#### 2.1 Cache
### Implementação da Cache - Detalhamento Técnico

#### 1. Estrutura da Cache
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

#### 2. Integração com Sistema

##### 2.1 Modificações na CPU
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

##### 2.2 Modificações no OperatingSystem
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

#### 3. Operações da Cache

##### 3.1 Leitura (Read)
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

##### 3.2 Escrita (Write)
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

#### 4. Fluxo de Dados
**CPU → Cache → RAM:**

- CPU solicita leitura/escrita
- Cache verifica hit/miss
- Se miss, acessa RAM
- Se write, atualiza RAM (write-through)

**Sincronização:**

- Mutex protege acessos concorrentes
- FIFO gerencia substituição de blocos
- Write-through mantém consistência

#### 5. Comunicação com Pipeline MIPS
```cpp
void Cpu::MemoryAccess() {
    switch (op) {
        case LOAD: {
            write_value = cache->read(get_register(2));
            break;
        }
        case STORE: {
            cache->write(get_register(2), get_register(get_register(1)));
            break;
        }
    }
}
```

#### 6. Melhorias Implementadas

**Consistência de Memória:**

- Write-through para debugging
- Sincronização entre cores
- Proteção de acessos concorrentes

**Performance:**

- Cache de 16 blocos
- Política FIFO
- Hit/miss tracking

**Integração:**

- Transparente para CPU
- Compartilhada entre cores
- Consistente com RAM

## 🔧 Melhorias Técnicas

### **1. Sincronização**
- Implementação de mutex para recursos compartilhados.
- Proteção de regiões críticas.
- Controle de acesso à cache.
- Gerenciamento de filas de processos.

### **2. Multicore**
- Suporte a múltiplos cores (2 cores).
- Escalonamento entre cores.
- Cache compartilhada.
- Preempção independente por core.

### **3. Pipeline MIPS**
- Cinco estágios:
  - `IF`, `ID`, `EX`, `MEM`, `WB`.
- Integração com cache na fase MEM.
- Controle de hazards básico.
- Suporte a instruções básicas.

---

## 📊 Estado Atual
- Sistema base funcionando.
- Execução multicore operacional.
- Cache implementada e integrada.
- Gerenciamento de processos básico completo.
- Preempção por quantum funcionando.

---

## 🔜 Próximos Passos
1. Implementação de memória secundária (Fase 2.2).
2. Otimização do cálculo de endereços na cache.
3. Implementação de mais tipos de hazards.
4. Melhorias no escalonamento.
