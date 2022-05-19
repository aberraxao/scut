/******************************************************************************
 ** ISCTE-IUL: Trabalho prático 3 de Sistemas Operativos
 ** Avaliação automática
 ******************************************************************************/
#ifndef __UTILS_EVAL_H__
#define __UTILS_EVAL_H__

#undef DEBUG_MODE
#define DEBUG_MODE 0



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


#undef success
#define success(passo,fmt, ...) do {\
    snprintf( newline( &_success_log ), LOGLINE,"(" passo ") " fmt, ## __VA_ARGS__); \
} while(0)

#undef error
#define error(passo,fmt, ...) do {\
    snprintf( newline( &_error_log ), LOGLINE,"(" passo ") " fmt, ## __VA_ARGS__); \
} while(0)

/*

#undef exit_on_error
#define exit_on_error(status, errorMsg) do { \
    if (status < 0) { \
        snprintf( newline( &_error_log ), LOGLINE, "%s : %s", errorMsg, strerror(errno) ); \
        exit(-1); \
    } \
} while (0)

#undef exit_on_null
#define exit_on_null(status, errorMsg) do { \
    if (NULL == status) { \
        snprintf( newline( &_error_log ), LOGLINE, "%s : %s", errorMsg, strerror(errno) ); \
        exit(-1); \
    } \
} while (0)

*/

#endif