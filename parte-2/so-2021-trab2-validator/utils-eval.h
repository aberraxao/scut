/******************************************************************************
 ** ISCTE-IUL: Trabalho prático 3 de Sistemas Operativos
 ** Avaliação automática
 ******************************************************************************/
#ifndef __UTILS_EVAL_H__
#define __UTILS_EVAL_H__

#include "utils.h"
#include "eval.h"

#undef DEBUG_MODE
#define DEBUG_MODE 0

#undef success
#undef error


/**
 * Disable warning: ‘%s’ directive output may be truncated
 * which may be triggered by the sucesso() and erro() macros
 **/ 
#ifdef __GNUC__
#if __GNUC__ > 7
#pragma GCC diagnostic ignored "-Wformat-truncation"
#pragma GCC diagnostic push
#endif
#endif


#define success(passo,fmt, ...) do {\
    snprintf( newline( &_success_log ), LOGLINE,"(" passo ") " fmt "\n", ## __VA_ARGS__); \
} while(0)

#define error(passo,fmt, ...) do {\
    snprintf( newline( &_error_log ), LOGLINE,"(" passo ") " fmt "\n", ## __VA_ARGS__); \
} while(0)


#endif
