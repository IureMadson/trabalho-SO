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
    //Inicialização dos campos
    strcpy(novo->nome, nome_p);
    novo->estado = ATIVO;
    novo->codigo = end_proc;
    novo->fila_sem= NULL;
    novo->contexto = cria_desc();
    newprocess(processo_trampolim, novo, novo->contexto);
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
    PTR_DESC_PROC *antigo;
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
    PTR_DESC_PROC aux;
    if(!atual){
        printf("\nNao ha processos rodando!");
        return;
    }
    atual = TERMINADO;
    aux = proximo_estado_ativo(atual);
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
