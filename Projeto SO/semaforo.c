#include "system.h"
#include "semaforo.h"
#include "pausa.h"
#include "nucleo.h"
#include <stdlib.h>
#include <stdio.h>

void inicia_semaforo(semaforo *sem, int n){
  if (!sem){
    return;
  }
  sem->s = n;
  sem->Q = NULL;
}
void P(semaforo* sem){
  PTR_DESC_PROC aux;
  PTR_DESC_PROC prox;

  if (!sem){
    return;
  }
  if(!atual){
    fprintf(stderr, "P: nenhum processo atual.\n");
    exit(1);
  }
  if (sem->s> 0){
    sem->s--;
    return;
  }
  atual->estado = BLOQ_P;
  atual->fila_sem = NULL;
  if(!sem->Q){
    sem->Q = atual;
  } else {
    aux = sem->Q;
    while(aux->fila_sem){
      aux = aux->fila_sem;
    }
    aux->fila_sem = atual;
    /* precisa existir alguém ativo para rodar, senão deadlock */ 
    prox = NULL;
    if(prim){
      /* procura um ativo a partir do atual */ 
      PTR_DESC_PROC scan = atual->prox_desc;
      while(scan&&scan!=atual){
        if(scan->estado == ATIVO) {
          prox = scan;
          break;
        }
        scan = scan->prox_desc;
      }
      if(!prox&&atual->estado==ATIVO){
        prox = atual;
      }
    }
    if(!prox){
      fprintf(stderr, "Deadlock: Todos os processos estao bloqueados em P().\n");
      exit(1);
    }
    /* cede CPU para outro processo ativo */ 
    yield();
    /* quando for acordado por V(), retorna e continua */ 
  }
}
void V(semaforo *sem){
  PTR_DESC_PROC p;

  if(!sem){
    return;
  }
  /* Se a fila de espera Q estiver vazia, incrementamos o contador s */
  if(sem->Q==NULL){
    sem->s++;
  } else{
    /* Retira o primeiro processo (p) da fila de bloqueados Q */
    p = sem->Q;
    sem->Q = p->fila_sem;
    p->fila_sem = NULL;
    /* Muda o estado deste processo para ATIVO */
    /* Na próxima varredura do escalonador, este processo poderá rodar */
    p->estado = ATIVO;
  }
}
