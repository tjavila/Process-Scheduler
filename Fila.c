#include "Fila.h"


// função que cria um processo, setando seus parâmetros iniciais para 0
processo *cria(){
  processo *head = (processo*) malloc(sizeof(processo));
  head->chegada = 0;
  head->duracao = 0;
  head->memoria = 0;
  head->prioridade = 0;
  head->lancamento = 0;
  head->atual = 0;
  head->next = NULL;
  return head;
}

// adiciina na fila
void enqueue(processo *pr, processo *fila){
  pr->next = fila->next;
  fila->next = pr;
}

//tira da fila
processo* dequeue(processo *fila){
  processo *p = fila->next;
  processo *q = fila;
  if(p == NULL){
    return 0;
  }
  while(p->next != NULL){
      p = p->next;
      q = q->next;
  }
  q->next = NULL;
  return p;
}


