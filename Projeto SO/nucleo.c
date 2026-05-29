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
    if (novo==NULL){
        printf("Erro na alocacao de memoria!");
        return;
    }
    strcpy(novo->nome, nome_p);
    novo->estado = ATIVO;
    novo->codigo = end_proc;
    novo->fila_sem= NULL;
    novo->contexto = cria_desc();
    newprocess(processo_trampolim, novo, novo->contexto);
    prim = novo;
    novo->prox_desc = novo;
}

static PTR_DESC_PROC proximo_ativo_depois(PTR_DESC_PROC a_partir){
    //verifica se a fila circular (prim) existe e não está vazia; 
    if(prim==NULL){
        printf("Fila não existe ou não foi alocada corretamente!");
        return NULL;
    }
    PTR_DESC_PROC aux;
    //define o ponto inicial da busca (usa prim se a_partir for nulo); 
    if(a_partir==NULL){
        aux = prim->prox_desc;
        while(aux!=prim){
            if(aux->estado==ATIVO){
                return aux;
            }
            aux = aux->prox_desc;
        }
    } else {
        aux = a_partir->prox_desc;
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
    PTR_DESC_PROC aux = (PTR_DESC_PROC)arg;
    if(aux==NULL||aux->codigo==NULL){
        printf("\nCodigo ou descritor invalido!");
        return;
    }
    aux->codigo();
    termina_processo();
}

void dispara_sistema(void){
    if(prim==NULL){
        return;
    }
    system_init_main(main_ctx);
    main_ready = 1;
    if(prim->estado==ATIVO){
        atual = prim;
    } else {
        atual = proximo_ativo_depois(prim);
        if(atual==NULL){
            printf("\nNenhum processo ativo encontrado!");
            return;
        }
    }
    transfer(main_ctx, atual->contexto);
}

void yield(void){
    PTR_DESC_PROC prox;
    PTR_DESC_PROC antigo;
    if(atual==NULL){
        return;
    }
    prox = proximo_ativo_depois(atual);

    if(prox!=atual&&prox!=NULL){
        antigo = atual;
        atual = prox;
        transfer(antigo->contexto, atual->contexto);
    } else {
        return;
    }
}

void termina_processo(void){
    PTR_DESC_PROC aux;
    PTR_DESC_PROC antigo;
    if(!atual){
        printf("\nNao ha processos rodando!");
        return;
    }
    atual->estado = TERMINADO;
    aux = proximo_ativo_depois(atual);
    if (aux!=atual&&aux!=NULL){
        antigo = atual;
        atual = aux;
        transfer(antigo->contexto, atual->contexto); // Pode estar errado os parametros REVER DEPOIS
        free(antigo);
    }else {
        printf("\nNão exite mais processos ativos na fila circular!");
        //VERIFICA SE O PROGRAMA PRINCIPAL ESTÁ PRONTO
        if(main_ctx){
            transfer(atual->contexto, main_ctx);
        } else{
            printf("\nPrograma principal nao disponivel!");
        }
    }
}
