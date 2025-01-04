# 📢 SO: Simulador da Arquitetura de Von Neumann e Pipeline MIPS

O **Simulador da Arquitetura de Von Neumann e Pipeline MIPS** visa aprofundar o entendimento sobre sistemas operacionais modernos, escalonamento de processos e gerenciamento de memória. Implementado em C++, o simulador modela uma arquitetura multicore baseada na arquitetura de Von Neumann, incorporando um pipeline de 5 estágios (IF, ID, EX, MEM e WB) inspirado no MIPS.

## Objetivos do Simulador

- **Arquitetura Multicore:** Suporta múltiplos núcleos de CPU, permitindo a execução paralela de processos e simulando ambientes de computação modernos.
- **Preempção de Processos:** Implementa a preempção baseada em quantum, permitindo a interrupção e troca de processos para otimizar a utilização dos recursos do sistema.
- **Gerenciamento de Processos:** Utiliza Process Control Blocks (PCBs) para armazenar informações críticas de cada processo, incluindo estado, prioridade e alocação de memória.
- **Hierarquia de Memórias:** Simula a interação entre CPU, memória principal (RAM) e memória cache, destacando a importância das políticas de substituição e escrita na cache.
- **Sincronização com Mutexes:** Garante a integridade dos dados compartilhados em ambientes multicore através do uso de mutexes, prevenindo condições de corrida.
- **Escalonamento de Processos:** Implementa algoritmos de escalonamento como FCFS, permitindo a análise do impacto no desempenho do sistema.
- **Registro de Operações:** Registra operações de memória e estados dos processos para facilitar a análise e depuração.

## 📌 Estrutura Geral

### 1. CPU

A classe **CPU** é responsável pelo controle do fluxo de instruções dentro de cada núcleo. As principais funções são:

- **InstructionFetch()**: Lê a próxima instrução da ROM (`codigo.txt`) e a carrega para a execução. O Program Counter (PC) é incrementado a cada instrução buscada até que todas as instruções sejam processadas.
- **InstructionDecode()**: Separa a instrução ativa em partes utilizando a função `split()` para identificar a operação (op) e os operandos. Valores válidos dos operandos são armazenados no banco de registradores.
- **Execute()**: Simula a Unidade Lógica e Aritmética (ULA), executando operações como ADD, SUB, MUL, DIV, SLT, BNE, BEQ e J. As operações de comparação e salto alteram o valor do PC.
- **MemoryAccess()**: Gerencia o acesso à memória, implementando as instruções LOAD, ILOAD e STORE. Utiliza a memória RAM para armazenar e recuperar valores.
- **WriteBack()**: Escreve o valor calculado de volta no banco de registradores se houver um valor para ser escrito.
- **Multicore Implementation**: O simulador suporta múltiplos núcleos de CPU, permitindo a execução paralela de processos. Cada núcleo opera independentemente, sincronizando-se através de mutexes para garantir a consistência dos dados compartilhados.
- **Gerenciamento de Mutexes**: Utiliza mutexes para sincronizar o acesso a recursos compartilhados, evitando condições de corrida e garantindo a integridade dos dados durante operações concorrentes.

### 2. RAM

A classe **RAM** simula a memória do sistema, composta por um array de 32 inteiros, onde cada posição pode ser lida ou escrita através de funções de acesso. Principais funcionalidades:

- **Gerenciamento de Memória**: Permite a inserção de programas, gerenciamento de PCB (Process Control Blocks) e manipulação de valores na memória.
- **Exibição do Estado da RAM**: O estado atual da RAM pode ser exibido na saída padrão para monitoramento.

### 3. RegisterBank

A classe **RegisterBank** simula o banco de registradores do processador, armazenando 32 registradores. Cada registrador possui um valor e um flag *dirty* que indica se foi alterado. Principais funcionalidades:

- **Acesso aos Registradores**: Permite a leitura e escrita dos registradores através de funções específicas.
- **Flag Dirty**: Mantém o controle sobre quais registradores foram modificados, facilitando a escrita de volta na memória quando necessário.

### 4. Cache

A classe **Cache** implementa uma memória cache para otimizar o acesso à RAM. Detalhes da implementação:

- **Estrutura da Cache**: Composta por blocos de cache (`CacheBlock`) que contêm informações sobre validade, modificações (dirty), tags e dados.
- **Política de Substituição**: Utiliza FIFO (First-In, First-Out) para gerenciar a substituição de blocos quando a cache atinge sua capacidade máxima (`CACHE_SIZE`).
- **Acesso Concorrente**: Utiliza mutexes para sincronizar o acesso aos blocos da cache, garantindo operações seguras em ambientes multicore.
- **Operações de Leitura e Escrita**: Implementa métodos para leitura (`read`) e escrita (`write`) de dados, gerenciando hits e misses na cache.
- **Write-Back**: Quando necessário, realiza a escrita de volta dos blocos modificados para a RAM, mantendo a consistência dos dados.

### 5. Scheduler

A classe **Scheduler** gerencia a fila de processos prontos para execução. Principais funcionalidades:

- **Fila de Prontos:** Armazena os PIDs dos processos que estão prontos para serem executados.
- **Gerenciamento de Quantum:** Implementa a preempção baseada em quantum, garantindo que cada processo receba um tempo de execução justo antes de ser trocado por outro.
- **Gerenciamento de Mutexes:** Utiliza mutexes para sincronizar o acesso à fila de prontos, garantindo a integridade dos dados em ambientes multithread.
- **Chaveamento de Processos:** Facilita o chaveamento eficiente entre múltiplos jobs, assegurando uma execução balanceada e a integridade dos dados compartilhados.

### 6. Operating System

A classe **OperatingSystem** coordena todos os componentes do simulador. Principais funcionalidades:

- **Inicialização Multicore**: Configura múltiplos núcleos de CPU para execução paralela de processos.
- **Gerenciamento de Interrupções**: Implementa a lógica para decisão e tratamento de interrupções, garantindo a resposta adequada a eventos do sistema.
- **Gerenciamento de Quantum**: Controla o tempo de execução alocado para cada processo, implementando o escalonamento de quantum para garantir uma distribuição justa dos recursos.
- **Log de Estados**: Mantém o registro do estado dos processos e das operações de memória através da classe `MemoryLogger`.

### 7. MemoryLogger

A classe **MemoryLogger** gerencia o registro das operações de memória e mudanças de estado dos processos. Principais funcionalidades:

- **Singleton Pattern**: Garante uma única instância de `MemoryLogger` durante a execução do sistema.
- **Registro de Operações**: Loga operações de memória, mudanças de estado dos processos, expiração de quantum e finalização de processos.
- **Sincronização com Mutexes**: Utiliza mutexes para garantir que os logs sejam escritos de maneira consistente em ambientes multicore.

### 8. Bootloader

A classe **Bootloader** é responsável por carregar os programas e processos na RAM. Principais funcionalidades:

- **Carregamento de Programas**: Lê arquivos de código (`codigo.txt`) e os insere na RAM.
- **Inicialização de PCBs**: Cria e configura os Process Control Blocks para cada processo, preparando-os para execução.

## ➕ Novos Tópicos

### Implementação Multicore

O simulador utiliza um processador dividido em múltiplos núcleos, permitindo a execução paralela de processos. Cada núcleo opera independentemente, mas sincroniza-se com os demais através de mutexes para acessar recursos compartilhados como a RAM e a Cache.

### Decisão das Interrupções

As interrupções são gerenciadas para responder a eventos como a expiração do quantum ou a conclusão de instruções críticas. O simulador utiliza mutexes para garantir que as interrupções sejam tratadas de maneira segura, evitando condições de corrida.

### Funcionamento do Quantum

O quantum define o tempo máximo de execução alocado para cada processo antes que seja realizado um escalonamento. O sistema incrementa um contador de ciclos de clock e, ao atingir o quantum, realiza a troca de contexto para o próximo processo na fila de prontos.

### Detalhamento da Implementação da Cache

A cache está implementada como uma série de blocos (`CacheBlock`) que armazenam dados da RAM. Utiliza uma política de substituição FIFO para gerenciar quais blocos devem ser substituídos quando a cache está cheia. Cada operação de leitura ou escrita verifica se há um hit na cache e, em caso de miss, carrega os dados da RAM para a cache, possivelmente efetuando write-back de blocos modificados.

### Funcionamento dos Mutexes

Mutexes são utilizados para sincronizar o acesso a recursos compartilhados como a RAM, Cache e filas de processos. Garantem que apenas um thread possa acessar ou modificar um recurso crítico por vez, prevenindo condições de corrida e assegurando a integridade dos dados em um ambiente multicore.

## 📚 Tecnologias Utilizadas

- **Linguagem:** C++
- **Framework:** Pthreads para gerenciamento de threads e sincronização
- **Versionamento:** GitHub


## ⚙️ Como executar

O projeto possui um arquivo Makefile que realiza todo o procedimento de compilação e execução. Para tanto, temos as seguintes diretrizes de execução para distribuições Linux:

| Comando         | Função                                                              |
|-----------------|---------------------------------------------------------------------|
| `make clean`    | Apaga a última compilação realizada contida na pasta build          |
| `make`          | Executa a compilação do programa utilizando o gcc, e o resultado vai para a pasta build |
| `make run`      | Executa o programa da pasta build após a realização da compilação   |

**Tabela 1:** Comandos úteis para compilar e executar o programa de computador

<section>

## 📝 Instruções Definidas para o Simulador

### 1. ILOAD
- **Formato**: ILOAD \<dest\> \<valor\>
- **Descrição**: Carrega um valor imediato no registrador \<dest\>.
- **Exemplo**: ILOAD 16 10
  - **Explicação**: Carrega o valor 10 no registrador 16.

### 2. LOAD
- **Formato**: LOAD \<dest\> \<endereco\>
- **Descrição**: Carrega o valor da posição de memória especificada por \<endereco\> no registrador \<dest\>.
- **Exemplo**: LOAD 16 100
  - **Explicação**: Carrega o valor da posição de memória 100 no registrador 16.

### 3. ADD
- **Formato**: ADD \<dest\> \<src1\> \<src2\>
- **Descrição**: Soma o valor dos registradores \<src1\> e \<src2\> e armazena o resultado em \<dest\>.
- **Exemplo**: ADD 18 16 17
  - **Explicação**: Soma o valor nos registradores 16 e 17 e armazena o resultado no registrador 18.

### 4. STORE
- **Formato**: STORE \<src\> \<endereco\>
- **Descrição**: Armazena o valor do registrador \<src\> na posição de memória \<endereco\>.
- **Exemplo**: STORE 18 16
  - **Explicação**: Armazena o valor do registrador 18 na posição de memória definida pelo valor no registrador 16.

### 5. BEQ (Branch if Equal)
- **Formato**: BEQ \<reg1\> \<reg2\> \<label\>
- **Descrição**: Se os valores de \<reg1\> e \<reg2\> são iguais, o programa salta para a instrução de número \<label\>.
- **Exemplo**: BEQ 16 17 9
  - **Explicação**: Se o valor no registrador 16 for igual ao valor no registrador 17, o programa pula para a linha 9 do código.

### 6. J (Jump)
- **Formato**: J \<label\>
- **Descrição**: Altera o fluxo de execução do código para a linha especificada por \<label\>.
- **Exemplo**: J 5
  - **Explicação**: Altera o fluxo de execução para a linha 5.

### 7. SUB
- **Formato**: SUB \<dest\> \<src1\> \<src2\>
- **Descrição**: Subtrai o valor em \<src2\> do valor em \<src1\> e armazena o resultado em \<dest\>.
- **Exemplo**: SUB 18 16 17
  - **Explicação**: Subtrai o valor no registrador 17 do valor no registrador 16 e armazena o resultado em 18.

### 8. MUL
- **Formato**: MUL \<dest\> \<src1\> \<src2\>
- **Descrição**: Multiplica o valor dos registradores \<src1\> e \<src2\> e armazena o resultado em \<dest\>.
- **Exemplo**: MUL 18 16 17
  - **Explicação**: Multiplica os valores nos registradores 16 e 17 e armazena o resultado em 18.

### 9. DIV
- **Formato**: DIV \<dest\> \<src1\> \<src2\>
- **Descrição**:Divide o valor dos registradores \<src1\> e \<src2\> e armazena o resultado em \<dest\>.
- **Exemplo**: DIV 18 16 17
  - **Explicação**: Divide os valores nos registradores 16 e 17 e armazena o resultado em 18.

### 10. BNE (Branch if Not Equal) 
- **Formato**: BNE \<reg1\> \<reg2\> \<label\>
- **Descrição**: Se os valores de \<reg1\> e \<reg2\> são diferentes, o programa salta para a instrução de número \<label\>.
- **Exemplo**: BNE 16 17 9
  - **Explicação**: Se o valor no registrador 16 for diferente do valor no registrador 17, o programa pula para a linha 9 do código.

### 11. SLT (Set if Less Than)
- **Formato**: SLT \<dest\> \<src1\> \<src2\>
- **Descrição**: Compara os valores de \<src1\> e \<src2\>. Se o valor em \<src1\> for menor que o valor em \<src2\>, armazena 1 em \<dest\>; caso contrário, armazena 0.
- **Exemplo**: SLT 18 16 17
  - **Explicação**: Se o valor no registrador 16 for menor que o valor no registrador 17, armazena 1 no registrador 18; caso contrário, armazena 0.

---

## Análise Detalhada de Código 

O arquivo codigo.txt contém o código de instruções que será executado pelo simulador.
Abaixo, segue a explicação de cada linha de um código de exemplo para detalhar a execução do programa:

````plaintext
    ILOAD 16 10
    ILOAD 17 12
    ADD 18 16 17
    STORE 18 16
    BEQ 16 17 9
    ILOAD 18 1
    ADD 16 18 16
    J 5
    SUB 18 16 17
    MUL 18 16 17
````

### Explicando o código:

1. **`ILOAD 16 10`**
   - Carrega o valor `10` no registrador `16`.

2. **`ILOAD 17 12`**
   - Carrega o valor `12` no registrador `17`.

3. **`ADD 18 16 17`**
   - Soma os valores dos registradores `16` e `17` (`10 + 12 = 22`) e armazena o resultado no registrador `18`.

4. **`STORE 18 16`**
   - Armazena o valor do registrador `18` (`22`) na posição de memória cujo endereço está armazenado no registrador `16` (`10`).

5. **`BEQ 16 17 9`**
   - Se os valores dos registradores `16` e `17` forem iguais, o fluxo de execução salta para a linha `9`. Como os valores são diferentes (`10` e `12`), o código continua na próxima linha.

6. **`ILOAD 18 1`**
   - Carrega o valor `1` no registrador `18`.

7. **`ADD 16 18 16`**
   - Soma o valor do registrador `18` (`1`) ao valor do registrador `16` (`10`), resultando em `11`, e armazena o resultado no registrador `16`.

8. **`J 5`**
   - Salta para a linha `5` do código, voltando para a instrução `BEQ 16 17 9`. Este salto cria um loop, verificando continuamente se os valores dos registradores `16` e `17` são iguais.

9. **`SUB 18 16 17`**
   - Caso o loop seja quebrado (valores iguais), esta linha executará a subtração do valor no registrador `17` (`12`) do valor no registrador `16`, com o resultado armazenado no registrador `18`.

10. **`MUL 18 16 17`**
    - Multiplica os valores dos registradores `16` e `17`

## Instrução de Loop 
É possível usar uma combinação das instruções BEQ, ILOAD, ADD, e J para criar um loop no simulador. A seguir, explicamos como funciona o loop representado pelo trecho:

````plaintext
    BEQ 16 17 9
    ILOAD 18 1
    ADD 16 18 16
    J 5
````
### Descrição do funcionamento:

**`BEQ 16 17 9`** <br>
 Esta instrução verifica se os valores nos registradores 16 e 17 são iguais. Se forem, o programa pula para a linha 9, o que, neste caso, significa sair do loop. Se não forem iguais, o código continua com as próximas instruções.

**`ILOAD 18 1`** <br>
 Carrega o valor 1 no registrador 18.

**`ADD 16 18 16`** <br>
 Soma o valor no registrador 18 (que é 1) ao valor no registrador 16 e armazena o resultado em 16. Isso incrementa o valor em 16 por 1.

**`J 5`** <br>
 Salta para a linha 5, onde a instrução BEQ é executada novamente, formando um loop que continua até que o valor no registrador 16 se iguale ao valor no registrador 17.



</section>

## 📚 Referências

1. TENENBAUM, Andrew S. **Sistemas Operacionais: Projeto e Implementação**. 3ª edição. Editora Campus, 2018.

2. BERTINI, Luciano. **Capítulo 5 Pipeline**. 2019. Disponível em: [https://www.professores.uff.br/lbertini/wp-content/uploads/sites/108/2017/08/Cap-5-Pipeline.pdf](https://www.professores.uff.br/lbertini/wp-content/uploads/sites/108/2017/08/Cap-5-Pipeline.pdf). Acesso em: 24 out. 2024.



# Autores
- **Emanuel Vieira Tavares**:emanuel@aluno.cefetmg.br 
- **Vinicius Alves Pereira**:vinicius@aluno.cefetmg.br
  DECOM-DV/CEFET-MG.
  2024/2.

