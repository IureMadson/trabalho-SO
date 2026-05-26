#include "nucleo.h"
#include <stdlib.h>
#include <stdio.h>


    /* Fila circular (Variáveis reais) */
    PTR_DESC_PROC prim = NULL;
    PTR_DESC_PROC atual = NULL;
    /* Contexto do main (como fiber - substitui o conceito do antigo "d_esc") */
    static descritor main_desc;
    static PTR_DESC main_ctx = &main_desc;
    static int main_ready = 0;
    /* ... outras funções auxiliares como processo_trampolim ... */
    /* Função Básica de Preparação */

void inicia_fila_prontos(void){
    prim = NULL;
    atual = NULL;
}
void cria_processo(void (*end_proc)(void), const char *nome_p){
    DESCRITOR_PROC novo = (DESCRITOR_PROC)malloc(sizeof(DESCRITOR_PROC));
    strcpy(proc->nome, nome_p);
    novo->estado = ATIVO;
    novo->contexto = cria_desc();
    novo->codigo = end_proc;
    
}
static PTR_DESC_PROC proximo_ativo_depois(PTR_DESC_PROC a_partir){
    //verifica se a fila circular (prim) existe e não está vazia; 
    if(prim==NULL){
        printf("Fila não existe ou não foi alocada corretamente!");
        return;
    }
    PTR_DESC_PROC aux = a_partir->prox_desc;
    //define o ponto inicial da busca (usa prim se a_partir for nulo); 
    if(a_partir==NULL){
        PTR_DESC_PROC aux = prim;
        while(aux!=prim){
            if(aux->estado==ATIVO){
                return aux;
            }
            aux = aux->prox_desc;
        }
    } else {
        PTR_DESC_PROC aux = a_partir;
        while(aux!=a_partir){
            if(aux->estado==ATIVO){
                return aux;
            }
            aux = aux->prox_desc;
        }
    }
    return (a_partir->estado == ATIVO) ? a_partir : NULL;
}
static void processo_trampolim(void* arg){
    PTR_DESC_PROC qlqr = (PTR_DESC_PROC)*arg;
}
void dispara_sistema(void){
    if(prim==NULL){
        return
    }
    system_init_main(main_ctx);

    atual = (prim->estado==ATIVO) ? prim : proximo_ativo_depois(prim);

    if (atual!=NULL){
        transferir(main_ctx, atual->contexto);
    }
}
void yield(void){
    PTR_DESC_PROC prox;
    if(atual==NULL){
        return;
    }
    prox = proximo_ativo_depois(atual);

    if(prox!=atual&&prox==NULL){
        PTR_DESC_PROC antigo = atual;
        atual = prox;
        transfer(antigo->contexto, atual->contexto);
    }
}
void termina_processo(void){
    
}
