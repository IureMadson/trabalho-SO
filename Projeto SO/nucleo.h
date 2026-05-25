#ifndef NUCLEO_H
#define NUCLEO_H

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
#else
  #error "Esta versão (fibers) foi preparada para Windows (_WIN32)."
#endif

typedef struct desc_p {
    char nome[35];
    ESTADO_PROC estado;
    /* contexto (fiber) */
    PTR_DESC contexto;
    /* ponteiros de filas */
    struct desc_p *fila_sem; /* encadeamento na fila de bloqueados do semáforo */
    struct desc_p *prox_desc; /* fila circular de prontos */
        /* função do processo (para o trampolim) */
    void (*codigo)(void);
} DESCRITOR_PROC;
typedef DESCRITOR_PROC* PTR_DESC_PROC;






#endif // NUCLEO_H_INCLUDED
