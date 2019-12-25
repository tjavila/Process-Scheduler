#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// declaração da struct para os processos e funções auxiliares de fila
typedef struct aux{
    int chegada; 
    int duracao;
    int memoria; 
    int prioridade; 
    int atual;
    int lancamento; 
    struct aux *next;
    //int delay; 
} processo;

typedef enum tipos{
  falso,
  verdadeiro
} boolean;

processo *cria();

void enqueue(processo *pr, processo *fila);

processo* dequeue(processo *fila);


