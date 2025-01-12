# 📢 SO: Simulador da Arquitetura de Von Neumann e Pipeline MIPS

## Introdução

O **Simulador da Arquitetura de Von Neumann e Pipeline MIPS** visa aprofundar o entendimento sobre sistemas operacionais modernos, escalonamento de processos e gerenciamento de memória. Implementado em C++, o simulador modela uma arquitetura multicore baseada na arquitetura de Von Neumann, incorporando um pipeline de 5 estágios (IF, ID, EX, MEM e WB) inspirado no MIPS.

## Instruções para execução

Esse projeto utiliza um arquivo `makefile` para compilar e executar o código-fonte. Seguem os comandos que podem ser utilizados:

- `make clean`: Limpa os binários compilados anteriormente. **Recomendado** para evitar possíveis problemas de execução.
- `make`: Compila os binários.
- `make run`: Executa o código.

### Exemplo de comando completo:

```bash
clear && make clean && make && make run
```

## Arquivos

Os programas a serem executados devem ficar dentro da pasta `dataset` com a extensão `.txt`.

### Exemplo de um conjunto válido de programas:

```
dataset/
  code1.txt
  code2.txt
  code3.txt
  sub.txt
  sum.txt
```

## Exemplo

Ao executar o código, é exibido um menu para realizar configurações iniciais.

```
========== Menu ==========
Digite o número de programas: 5
Digite o número de núcleos: 3

[1] FCFS (First Come First Serve)
[2] SJF (Shortest Job First)
[3] SRTN (Shortest Remaining Time Next)

Escolha uma política de escalonamento: 2
==========================
```

Depois são exibidas algumas informações sobre os programas carregados: o nome do arquivo e qual **PID** o processo respectivo a esse programa irá receber. Após, são listados os processos criados e seus respectivos tamanhos.

```
========== Bootloader ==========
Arquivos e PID

Arquivo: ./dataset/code1.txt
PID: 0

Arquivo: ./dataset/sum.txt
PID: 1

Arquivo: ./dataset/code3.txt
PID: 2

Arquivo: ./dataset/sub.txt
PID: 3

Arquivo: ./dataset/code2.txt
PID: 4


Processos

PID: 0 carregado.
Tamanho: 4

PID: 1 carregado.
Tamanho: 4

PID: 2 carregado.
Tamanho: 12

PID: 3 carregado.
Tamanho: 4

PID: 4 carregado.
Tamanho: 8

==============================
```

Ao fim da execução, os resultados ficam disponíveis na pasta `output`.

## Autores
- **Emanuel Vieira Tavares**: emanuel@aluno.cefetmg.br 
- **Vinicius Alves Pereira**: vinicius@aluno.cefetmg.br
  
  DECOM-DV/CEFET-MG.

  2024/2.
