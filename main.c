
#include "Fila.h"


// declara funções
boolean lerArq(char* arquivo, processo *fila0, processo *fila1, processo *fila2, processo *fila3, processo *fila4);

void criaArq();

boolean status(processo *cpu[], int ncpus);

void limpa(int x, processo *cpu[], processo *fila0, processo *fila1, processo *fila2, processo *fila3, processo *fila4);

void clearCpus(processo *cpu[], int cpus);


void run(int cpus, int sliceDuration, int totalMemory, processo *queue0, processo *queue1, processo *queue2, processo *queue3, processo *queue4);


int main(int argc, char *argv[ ])
{
    // lê 3 entradas do main
    int cpus = (int) argv[1];
    int totalMemory = (int)(argv[2]);
    char* fileName = argv[3];
    boolean aux;
    int slice = 500;
    int ncpus = 0, memTotal = 0;
    processo *fila0, *fila1, *fila2, *fila3, *fila4;
    // cria 4 filas, uma para cada prioridade
    fila0 = cria();
    fila1 = cria();
    fila2 = cria();
    fila3 = cria();
    fila4 = cria();


    aux = lerArq(fileName, fila0, fila1, fila2, fila3, fila4);
    if (aux){
      //se conseguir ler o arquivo, cria o arquivo resultado e escalona
        criaArq();
        escalona(ncpus, memTotal, slice, fila0, fila1, fila2, fila3, fila4);
    }
    // para saber que o programa acabou
    printf("Concluído.\n");

    // libera os ponteiros
    free(fila0);
    free(fila1);
    free(fila2);
    free(fila3);
    free(fila4);

    return 0;
}


boolean lerArq(char* arquivo, processo *fila0, processo *fila1, processo *fila2, processo *fila3, processo *fila4){
  
  char linha[100];
  char *aux, *resultado;
  FILE *file;
  int parametros[4]; // 4 parametros a serem lidos
  file = fopen(arquivo, "r");
  if (file == NULL){
    printf("Arquivo vazio\n");
    return 0;
  }

  while (!feof(file)){
    // le a linha até acabar o arquivo
    resultado = fgets(linha, 100, file);

    if (resultado){
      //separa por virgulas
      aux = strtok(resultado, ", ");
      int i = 0;

      while (aux != NULL){
        parametros[i++] = (int) atoi(aux); //força a ser um inteiro e coloca o valor lido no vetor
        aux = strtok(NULL, ", ");
      }

      // aloca o espaço para o processo
      processo *p = (processo*) malloc(sizeof(processo));

      // coloca os valores lidos no vetor em cada variável 
      p->atual = 0;
      p->chegada = parametros[0];
      p->lancamento = parametros[0];
      p->duracao = parametros[1];
      p->memoria = parametros[2];
      p->prioridade = parametros[3];

      //coloca na fila a partir da prioridade
      switch (p->prioridade){
        case 0:
          enqueue(p, fila0);
          break;
        case 1:
          enqueue(p, fila1);
          break;
        case 2:
          enqueue(p, fila2);
          break;
        case 3:
          enqueue(p, fila3);
          break;
        case 4:
          enqueue(p, fila4);
          break;
        }
    }
  }
  fclose(file);
  return 1;
}

// status, para ver o status da cpu, se ainda tem processos
boolean status(processo *cpu[], int ncpus){ 
    for(int i=0; i < ncpus; i++){
      // se a cpu não for nula e nem alcançar o tempo então ainda não acabou
        if(cpu[i] != NULL && cpu[i]->duracao != cpu[i]->atual){
            return 0;
        }
    }
    return 1;
}

//cria o arquivo resultado
void criaArq(){
    FILE *file;
    file = fopen("resultado.txt", "w");
    fclose(file);
}

//limpa por fila os processos, esta ideia foi obtida de um colega, pois estavamos tentando realizar tudo na finaliza, onde limpamos tudo, mas não obtvemos sucesso
void limpa(int x, processo *cpu[], processo *fila0, processo *fila1, processo *fila2, processo *fila3, processo *fila4){
  //se acabarem os procesos
  if(cpu[x]->atual == cpu[x]->duracao){
    FILE *file;
    file = fopen("resultado.txt", "a");

    if(cpu[x] != NULL){
      // coloca resultados no arquivo resultado
      fprintf(file, "%d, %d, %d, %d\n", cpu[x]->chegada, cpu[x]->lancamento, cpu[x]->duracao, cpu[x]->atual);
    }

    fclose(file);

  } else{
    // se não adiciona a fila os processos
    switch(cpu[x]->prioridade){
      case 0:
        enqueue(cpu[x], fila0);
        break;
      case 1:
        enqueue(cpu[x], fila1);
        break;
      case 2:
        enqueue(cpu[x], fila2);
        break;
      case 3:
        enqueue(cpu[x], fila3);
        break;
      case 4:
        enqueue(cpu[x], fila4);
        break;
    }
  }
}

// acaba com os processos 
void finaliza(processo *cpu[], int ncpus){
  int i=0;
  FILE *file;
  file = fopen("resultado.txt", "a");
  while(i < ncpus){
    if(cpu[i] != NULL){
      // se a cpu não for nula, adiciona os resultados no arquivo
      fprintf(file, "%d, %d, %d, %d\n", cpu[i]->chegada, cpu[i]->lancamento, cpu[i]->duracao, cpu[i]->atual);
    }
    i++;
  }
  fclose(file);
}

//realiza escalonamento
void escalona(int ncpus, int memTotal, int slice, processo *fila0, processo *fila1, processo *fila2, processo *fila3, processo *fila4){
  int espaco = memTotal;
  boolean aux;
  processo *p;
  if(ncpus > 0 && slice > 0 && memTotal >= 64) {
    //cria as cpus
    processo *cpu[ncpus];
    //deixa elas todas nulas
    for(int x = 0; x < ncpus; x++){
      cpu[x] = NULL;
    }

    //checa para cada cpu se o processo acabou,e tira da fila
    while(ncpus){
      aux = status(cpu, ncpus);
      p = dequeue(fila0);
      if(p == NULL){
        p = dequeue(fila1);
        if(p == NULL){
          p = dequeue(fila2);
          if(p == NULL){
            p = dequeue(fila3);
            if(p == NULL){
              p = dequeue(fila4);
              if(p == NULL && aux){
                finaliza(cpu, ncpus);
                return;
              };
            }
          }
        }
      }
      // se atingir limite de memória
      if(p != NULL && p->memoria > memTotal){
        free(p);
      } else{
        // se tem espaço
          if(p != NULL && espaco >= p->memoria){
            for(int i = 0; i < ncpus; i++){
              if(cpu[i] == NULL){
                // cpu recebe o processo
                cpu[i] = p;
                // deminuiu o espaço livre
                espaco -= cpu[i]->memoria;
                // se não atingir a duração
                if(cpu[i]->atual + slice < cpu[i]->duracao){
                  // vai pro próximo slice
                  cpu[i]->atual += slice;
                } else{
                  // se atingiu
                    cpu[i]->atual = cpu[i]->duracao;
                    espaco += cpu[i]->memoria;
                    //libera os processos
                    limpa(i, cpu, fila0, fila1, fila2, fila3, fila4);
                    //coloca null na cpu
                    cpu[i] = NULL;
                  }

              } else if(i == ncpus-1){
                // atualiza o tempo de lançamento
                  if(p->atual == 0){
                    ++p->lancamento;
                  }

                  //libera a primeira cpu
                  espaco += cpu[0]->memoria;
                  limpa(0, cpu, fila0, fila1, fila2, fila3, fila4);
                  cpu[0] = NULL;

                  //liebra por prioridade
                  switch(p->prioridade){
                    case 0:
                      enqueue(p, fila0);
                      break;
                    case 1:
                      enqueue(p, fila1);
                      break;
                    case 2:
                      enqueue(p, fila2);
                      break;
                    case 3:
                      enqueue(p, fila3);
                      break;
                    case 4:
                      enqueue(p, fila4);
                      break;
                  }
                }
            }
          } else{
            //acaba o processo
              finaliza(cpu, ncpus);
              //deixa a cpu toda nula novamente
              for(int i=0; i < ncpus; i++){
                cpu[i] = NULL;
              }
              // coloca processos por prioridade
              if(p != NULL)
                switch(p->prioridade){
                  case 0:
                    enqueue(p, fila0);
                    break;
                  case 1:
                    enqueue(p, fila1);
                    break;
                  case 2:
                    enqueue(p, fila2);
                    break;
                  case 3:
                    enqueue(p, fila3);
                    break;
                  case 4:
                    enqueue(p, fila4);
                    break;
                }
            }
        }
    }
  }
}