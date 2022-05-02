/******************************************************************************
 ** ISCTE-IUL: Trabalho prático 3 de Sistemas Operativos
 ** Avaliação automática
 ******************************************************************************/
#ifndef __EVAL_H__
#define __EVAL_H__

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <sys/stat.h>

#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include <setjmp.h>

#define LOGSIZE 128
#define LOGLINE 256

typedef struct {
    char buffer[LOGSIZE][LOGLINE];
    int start;
    int end;
} log_t;

extern log_t _success_log;
extern log_t _error_log;
extern log_t _data_log;

void initlog( log_t * );
char *newline( log_t * );
void printlog( log_t * );
int rmheadmsg( log_t*, const char [] );

int findinlog( log_t*, const char [] );

const char* loghead( log_t* );

// Eval error
#define eval_error(fmt, ...) printf("\033[0;31m[error]\033[0m " fmt "\n", ## __VA_ARGS__)
// Eval info
#define eval_info(fmt, ...) printf("\033[0;32m[info]\033[0m " fmt "\n", ## __VA_ARGS__)


#define datalog(fmt, ...) do {\
    snprintf( newline( &_data_log ), LOGLINE, fmt , ## __VA_ARGS__); \
} while(0)


int eval_check_successlog( const char msg[], const char section[]);
int eval_check_errorlog( const char msg[], const char section[]);
void eval_clear_logs( void );
void eval_close_logs( char msg[] );

#define EVAL_VAR( name ) _eval_##name##_type _eval_##name##_data 

#define RESET(var) memset(&(var), 0, sizeof(var))

/**
 * Convert a a macro definition to a string
 * This requires a double-expansion
 **/
#define _TOSTRING(x) #x
#define TOSTRING(x) _TOSTRING(x)

typedef struct {
    jmp_buf jmp;
    int signal;
    int stat;

    sig_t old_sighndlr[4];
} _eval_env_type;

extern _eval_env_type _eval_env;

void _eval_sighandler( int sig );
void _eval_arm_signals( );
void _eval_disarm_signals( );


int eval_checkptr( void* ptr );

/**
 * EVAL_CATCH( code ) macro
 * Runs the specified code catching:
 * - exit() calls
 * - SIGSEGV, SIGBUS, SIGFPE and SIGILL signals
 **/

#define EVAL_CATCH( _code ) { \
    _eval_exit_data.status = -1; \
    _eval_env.signal = -1; \
    _eval_arm_signals(); \
    eval_info("%s running...", #_code ); \
    _eval_env.stat = setjmp( _eval_env.jmp ); \
    if ( !_eval_env.stat ) { \
        _code; \
        eval_info("%s completed normally", #_code); \
    } else { \
        switch( _eval_env.stat ) { \
        case(1): \
            eval_info("%s terminated by exit(%d)", #_code, _eval_exit_data.status ); \
            break; \
        case(2): \
            eval_info("%s terminated by signal %d", #_code, _eval_env.signal);\
            break; \
        default: \
            eval_error("%s abnormal termination", #_code); \
        } \
    } \
    _eval_disarm_signals(); \
}

/**
 * EVAL_CATCH_IO( code, stdin, stdout ) macro
 * Same as EVAL_CATCH but redirects input/output from/to specified files
 **/

typedef struct {
    int stdin;
    int stdout;
    int stdin_old;
    int stdout_old;
} _eval_stdio_t;

int _eval_io_redirect( const char* , const char*  );
int _eval_io_restore();

#define EVAL_CATCH_IO( _code, _stdin, _stdout ) { \
    _eval_exit_data.status = -1; \
    _eval_env.signal = -1; \
    _eval_arm_signals(); \
    eval_info("%s running...", #_code ); \
    _eval_io_redirect( _stdin, _stdout ); \
    _eval_env.stat = setjmp( _eval_env.jmp ); \
    if ( !_eval_env.stat ) { \
        _code; \
        _eval_io_restore();\
        eval_info("%s completed normally", #_code); \
    } else { \
        _eval_io_restore();\
        switch( _eval_env.stat ) { \
        case(1): \
            eval_info("%s terminated by exit(%d)", #_code, _eval_exit_data.status ); \
            break; \
        case(2): \
            eval_info("%s terminated by signal %d", #_code, _eval_env.signal);\
            break; \
        default: \
            eval_error("%s abnormal termination", #_code); \
        } \
    } \
    _eval_disarm_signals(); \
}

#define FILE_STDIN "eval.stdin"
#define FILE_STDOUT "eval.stdout"
#define FILE_DEVNULL "/dev/null"

/******************************************************************************
 * exit
 *****************************************************************************/
typedef struct {
    int action;
    int status; // Actual exit status, not number of times called
} _eval_exit_type;

extern _eval_exit_type _eval_exit_data;

void _eval_exit(int);

#define exit( status ) _eval_exit( status );

/******************************************************************************
 * sleep
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    int ret;

    unsigned int seconds;
} _eval_sleep_type;

extern _eval_sleep_type _eval_sleep_data;

unsigned int _eval_sleep(unsigned int seconds);

#define sleep( seconds ) _eval_sleep( seconds )

/******************************************************************************
 * fork
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    pid_t ret;
} _eval_fork_type;

extern _eval_fork_type _eval_fork_data;

pid_t _eval_fork(void);

#define fork() _eval_fork()

/******************************************************************************
 * wait
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    pid_t ret;

    int *stat_loc;
} _eval_wait_type;

extern _eval_wait_type _eval_wait_data;

pid_t _eval_wait(int *stat_loc);

#define wait( stat_loc ) _eval_wait( stat_loc )

/******************************************************************************
 * waitpid
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    pid_t ret;

    pid_t pid;
    int *stat_loc;
    int options;
} _eval_waitpid_type;

extern _eval_waitpid_type _eval_waitpid_data;

pid_t _eval_waitpid(pid_t pid, int *stat_loc, int options);

#define waitpid( pid, stat_loc, options ) _eval_waitpid( pid, stat_loc, options )

/******************************************************************************
 * kill
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    int ret;

    pid_t pid;
    int sig;
} _eval_kill_type;

extern _eval_kill_type _eval_kill_data;

int _eval_kill(pid_t pid, int sig);

#define kill( pid, sig ) _eval_kill( pid, sig )

/******************************************************************************
 * signal
 *****************************************************************************/
typedef void (*sighandler_t)(int);

typedef struct {
    int action;
    int status;
    sighandler_t ret;

    int signum;
    sighandler_t handler;
} _eval_signal_type;

extern _eval_signal_type _eval_signal_data;

sighandler_t _eval_signal(int signum, sighandler_t handler);

#define signal( signum, handler ) _eval_signal( signum, handler )

/******************************************************************************
 * sigaction
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    int ret;

    int signum;
    struct sigaction *act;
    struct sigaction *oldact;
} _eval_sigaction_type;

extern _eval_sigaction_type _eval_sigaction_data;

int _eval_sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

#define sigaction( signum, act, oldact ) _eval_sigaction( signum, act, oldact )

/******************************************************************************
 * pause
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    int ret;
} _eval_pause_type;

extern _eval_pause_type _eval_pause_data;

int _eval_pause(void);

#define pause( ) _eval_pause( )

/******************************************************************************
 * msgget
 *****************************************************************************/
#define msgget( key, msgflg ) _eval_msgget( key, msgflg )

typedef struct {
    int action;
    int status;
    int ret;

    key_t key;
    int msgflg;
} _eval_msgget_type;
extern _eval_msgget_type _eval_msgget_data;

int _eval_msgget(key_t key, int msgflg);

/******************************************************************************
 * msgsnd
 *****************************************************************************/
#define msgsnd( msqid, msgp, msgsz, msgflg) _eval_msgsnd( msqid, msgp, msgsz, msgflg)

typedef struct {
    int action;
    int status;
    int ret;

    int msqid;
    void *msgp;
    size_t msgsz;
    int msgflg;
} _eval_msgsnd_type;

extern _eval_msgsnd_type _eval_msgsnd_data;

int _eval_msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);

/******************************************************************************
 * msgrcv
 *****************************************************************************/
#define msgrcv( msqid, msgp, msgsz, msgtyp ,msgflg) _eval_msgrcv( msqid, msgp, msgsz, msgtyp, msgflg)

typedef struct {
    int action;
    int status;
    ssize_t ret;

    int msqid;
    void *msgp;
    size_t msgsz;
    long msgtyp;
    int msgflg;
} _eval_msgrcv_type;

extern _eval_msgrcv_type _eval_msgrcv_data;

ssize_t _eval_msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);

/******************************************************************************
 * msgctl
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    int ret;

    int msqid;
    int cmd;
    struct msqid_ds *buf;
} _eval_msgctl_type;

extern _eval_msgctl_type _eval_msgctl_data;

int _eval_msgctl(int msqid, int cmd, struct msqid_ds *buf);

#define msgctl(msqid, cmd, buf) _eval_msgctl(msqid, cmd, buf)

/******************************************************************************
 * semget
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    int ret;

    key_t key;
    int nsems;
    int semflg;
} _eval_semget_type;

extern _eval_semget_type _eval_semget_data;

int _eval_semget(key_t key, int nsems, int semflg);

#define semget( key, nsems, semflg) _eval_semget( key, nsems, semflg )

/******************************************************************************
 * semctl
 *****************************************************************************/
#ifdef _SEM_SEMUN_UNDEFINED

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};

#endif

typedef struct {
    int action;
    int status;
    int ret;
    int semid;
    int semnum;
    int cmd;
    union semun arg;
} _eval_semctl_type;

extern _eval_semctl_type _eval_semctl_data;

int _eval_semctl(int semid, int semnum, int cmd, ... );

#define semctl( semid, semnum, ... ) _eval_semctl( semid, semnum, ## __VA_ARGS__ )

/******************************************************************************
 * semop
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    int ret;
    int semid;
    struct sembuf *sops;
    size_t nsops;
} _eval_semop_type;

extern _eval_semop_type _eval_semop_data;

int _eval_semop(int semid, struct sembuf *sops, size_t nsops);

#define semop( semid, sops, nsops ) _eval_semop( semid, sops, nsops )

/******************************************************************************
 * shmget
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    int ret;
    key_t key;
    size_t size;
    int shmflg;
} _eval_shmget_type;

extern _eval_shmget_type _eval_shmget_data;

int _eval_shmget(key_t key, size_t size, int shmflg);

#define shmget( key, size, shmflg) _eval_shmget( key, size, shmflg )

/******************************************************************************
 * shmat
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    void *ret;

    int shmid;
    void *shmaddr;
    int shmflg;
} _eval_shmat_type;

extern _eval_shmat_type _eval_shmat_data;

void *_eval_shmat( int shmid, const void *shmaddr, int shmflg);

#define shmat( shmid, shmaddr, shmflg ) _eval_shmat( shmid, shmaddr, shmflg )

/******************************************************************************
 * shmdt
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    int ret;
    void *shmaddr;
} _eval_shmdt_type;

extern _eval_shmdt_type _eval_shmdt_data;

int _eval_shmdt(const void *shmaddr);

#define shmdt(shmaddr) _eval_shmdt(shmaddr)

/******************************************************************************
 * shmctl
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    int ret;
    int shmid;
    int cmd;
    struct shmid_ds *buf;
} _eval_shmctl_type;

extern _eval_shmctl_type _eval_shmctl_data;

int _eval_shmctl(int shmid, int cmd, struct shmid_ds *buf);

#define shmctl( shmid, cmd, buf ) _eval_shmctl( shmid, cmd, buf )


/******************************************************************************
 * mkfifo
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    char *path;
    mode_t mode;
    int ret;
} _eval_mkfifo_type;

extern _eval_mkfifo_type _eval_mkfifo_data;

int _eval_mkfifo(const char *path, mode_t mode);

#define mkfifo( path, mode ) _eval_mkfifo( path, mode )

/******************************************************************************
 * S_ISFIFO
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    mode_t mode;
    int ret;
} _eval_isfifo_type;

extern _eval_isfifo_type _eval_isfifo_data;

int _eval_isfifo(mode_t mode);

#undef S_ISFIFO
#define S_ISFIFO(st_mode) _eval_isfifo(st_mode)

/******************************************************************************
 * alarm
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    unsigned int seconds;
    unsigned int ret;
} _eval_alarm_type;

extern _eval_alarm_type _eval_alarm_data;

unsigned int _eval_alarm( unsigned int seconds );

#define alarm( seconds ) _eval_alarm( seconds )


/******************************************************************************
 * remove
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    char *path;
    int ret;
} _eval_remove_type;

extern _eval_remove_type _eval_remove_data;

int _eval_remove(const char * path);

#define remove( path ) _eval_remove( path )

/******************************************************************************
 * unlink
 *****************************************************************************/
typedef struct {
    int action;
    int status;
    char *path;
    int ret;
} _eval_unlink_type;

extern _eval_unlink_type _eval_unlink_data;

int _eval_unlink(const char * path);

#define unlink( path ) _eval_unlink( path )

/**
 * Specific prototypes and wrapper function control variables
 **/

#include "proto-eval.h"


#endif