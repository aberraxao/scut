
#include "common.h"
#include "eval.h"

#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>

/**
 * Undefine the replacement macros defined in eval.h
 **/
#include "eval-undef.h"

_eval_stdio_t _eval_stdio;

log_t _success_log;
log_t _error_log;
log_t _data_log;

/**
 * initialize a log_t variable
 * @param log   Log variable to initialize
 **/
void initlog( log_t* log ) {
    log -> start = -1;
    log -> end = 0;
}

/**
 * Returns a pointer to a new line in the log and updates internal pointers
 * @param log   Log variable
 * @return      char* to a new line
 **/
char *newline( log_t* log ) {
    char *line = log -> buffer[ log -> end ];
    if ( log -> start < 0 ) log -> start = 0;
    log -> end++;
    if ( log -> end >= LOGSIZE ) {
        fprintf(stderr,"(*critical*) No more space in log buffer, aborting\n");
        printlog( log );
        exit(1);
    }
    return line;
}

/**
 * Prints entire log
 * @param log   Log variable
 **/
void printlog( log_t* log ) {
    if ( log -> start < 0 || log -> start >= log -> end ) {
        printf("<empty>\n");
    } else {
        for( int i = log -> start, j = 0; i != log -> end; i++, j++ ) {
            printf("%3d - %s", j, log -> buffer[i]);
        }
    }
}


int findinlog( log_t* log, const char msg[] ) {
    int idx = -1;
    for( int i = log->start; i != log->end; i++ ) {
        if ( !strcmp( msg, log->buffer[i]) ) {
            idx = i;
            break;
        }
    }
    return idx;
}


/**
 * Removes the head line from the log if it contains the supplied message
 * 
 * @param msg   Messagem to look for
 * @return      0 on success, -1 on error (empty log or msg not found)
 **/
int rmheadmsg( log_t* log, const char msg[] ) {
    if ( log -> start < 0 ) return -1;
    if ( strstr( log -> buffer[ log -> start], msg ) ) {
        // Remove the line
        log -> start ++;
        return 0;
    } else {
        return -1;
    }
}

/**
 * Returns the string at the head of the log or "empty" if empty
 * 
 * @param log   Log
 * @return      Pointer to string
 **/
const char* loghead( log_t* log ) {
    static const char empty[] = "<empty>";
    if ( log -> start < 0 ) return empty;
    else return log -> buffer[log -> start];
}

/**
 * Checks if the supplied msg is at the head of the success log.
 * If true, removes head line and returns 0, otherwise issues an error message
 * 
 * @param msg       Message to check for
 * @param section   Section id (for error message)
 * @return          1 on success, 0 if msg not found at head
 **/
int eval_check_successlog( const char msg[], const char section[]) {
    const char *currLog = loghead(&_success_log);
    if ( rmheadmsg( &_success_log, msg ) ) {
        eval_error( "%s Invalid success log message, expected '%s', got '%s'", section, msg, currLog);
        return 0;
    } else {
        eval_info("Success log message OK as expected: '%s'", currLog);
        return 1;
    }
}

/**
 * Checks if the supplied msg is at the head of the error log.
 * If true, removes head line and returns 0, otherwise issues an error message
 * 
 * @param msg       Message to check for
 * @param section   Section id (for error message)
 * @return          1 on success, 0 if msg not found at head
 **/
int eval_check_errorlog( const char msg[], const char section[]) {
    // Vamos assumir que o fato do aluno ter detetado um erro já está OK, seria complicado o texto ser igual ao esperado!
    const char *currLog = loghead(&_error_log);
    if ( rmheadmsg( &_error_log, msg ) ) {
        eval_error( "%s Invalid error log message, expected '%s', got '%s'", section, msg, currLog);
        return 0;
    } else { 
        eval_info("Error log message OK as expected: '%s'", currLog);
        return 1;
    }
}

/**
 * Clears both success and error logs, and prints remaning messages if any
 **/
void eval_close_logs( char msg[] ) {
    if ( _success_log.start >= 0 && _success_log.start < _success_log.end ) {
        eval_info( "%s @@ Success log @@", msg );
        for( int i = _success_log.start; i != _success_log.end; i++ ) {
            printf("%3d - %s", i, _success_log.buffer[i]);
        }
    } 

    if ( _error_log.start >= 0 && _error_log.start < _error_log.end ) {
        eval_info( "%s @@ Error log @@", msg );
        for( int i = _error_log.start; i != _error_log.end; i++ ) {
            printf("%3d - %s", i, _error_log.buffer[i]);
        }
    } 
    
    initlog( &_success_log );
    initlog( &_error_log );
}

/**
 * @brief Redirects stdin and stdout to files
 * 
 * @param stdin     File name for stdin. If set to NULL no redirection takes place
 * @param stdout    File name for stdout. If set to NULL no redirection takes place
 * @return int      0 on success
 */
int _eval_io_redirect( const char* stdin, const char* stdout ) {

    if ( stdin ) {
        if ( (_eval_stdio.stdin_old = dup( STDIN_FILENO ))< 0 ) {
            eval_error("Unable to duplicate STDIN_FILENO");
            return -1;
        }
        if ( (_eval_stdio.stdin = open( stdin, O_RDONLY )) < 0 ) {
            eval_error("Unable to open file %s as read-only", stdin);
            return -1;
        }
        if ( (dup2( _eval_stdio.stdin, STDIN_FILENO )) < 0 ) {
            eval_error("Unable to associate file %s with stdin", stdin);
            return -1;
        };
    } else {
        _eval_stdio.stdin_old = -1;
        _eval_stdio.stdin = -1;
    }

    if ( stdout ) {
        if ( (_eval_stdio.stdout_old = dup( STDOUT_FILENO )) < 0 ) {
            eval_error("Unable to duplicate STDOUT_FILENO");
            return -1;
        }
        if ( (_eval_stdio.stdout = open( stdout, O_CREAT | O_WRONLY )) < 0 ) {
            eval_error("Unable to open file %s as write-only", stdout);
            return -1;
        }
        if ( (dup2( _eval_stdio.stdout, STDOUT_FILENO )) < 0 ) {
            eval_error("Unable to associate file %s with stdout", stdout);
            return -1;
        }
    } else {
        _eval_stdio.stdout_old = -1;
        _eval_stdio.stdout = -1;
    }

    return 0;
}

/**
 * @brief Restores stdin and stdout to console
 * 
 * @return int      0 on success
 */
int _eval_io_restore() {
    if ( _eval_stdio.stdin != -1 ) {
        close( _eval_stdio.stdin );
        dup2( _eval_stdio.stdin_old, STDIN_FILENO );
    }

    if ( _eval_stdio.stdout != -1 ) {
        close( _eval_stdio.stdout );
        dup2( _eval_stdio.stdout_old, STDOUT_FILENO );
    }

    return 0;
}


struct {
    int stat;
    int sig;
    jmp_buf jmp;
}_eval_checkptr_data;

void _eval_checkptr_sighndlr( int sig ) {
    _eval_checkptr_data.stat = 1;
    _eval_checkptr_data.sig = sig;
    longjmp( _eval_checkptr_data.jmp, 1 );
}

/**
 * Checks if a pointer is valid by reading/writing 1 byte from/to address
 * @param ptr   Pointer to be evaluated
 * @return      0 is pointer is valid
 *              1 NULL pointer
 *              2 (-1) pointer
 *              3 Segmentation fault when accessing pointer
 *              4 Bus Error when accessing pointer
 *              5 Invalid signal caught (should never happen)
 **/
int eval_checkptr( void* ptr ) {

    if ( ptr == NULL ) {
        eval_error("NULL pointer");
        return 1;
    }

    if ( ptr == (void *) -1 ) {
        eval_error("Invalid pointer %p", ptr );
        return 2;
    }

    // Catch SIGSEGV and SIGBUS
    sig_t tmp[2];
    if ( ( tmp[0] = signal( SIGSEGV, _eval_checkptr_sighndlr ) ) == SIG_ERR ) {
        perror("(*critical*) Unable to set SIGSEGV handler");
        exit(1);
    };

    if ( ( tmp[1] = signal( SIGBUS, _eval_checkptr_sighndlr ) ) == SIG_ERR ) {
        perror("(*critical*) Unable to set SIGBUS handler");
        exit(1);
    };

    // Reset _eval_checkptr_data
    _eval_checkptr_data.stat = 0;
    _eval_checkptr_data.sig = -1;

    // Access memory at position ptr, invalid pointers will throw a signal
    int stat = setjmp( _eval_checkptr_data.jmp );
    if ( !stat ) {
        char *p = (char *) ptr;
        char b = *p;
        *p = b;
    }

    // Restore previous signal handlers
    if ( ( tmp[0] = signal( SIGSEGV, tmp[0] ) ) == SIG_ERR ) {
        perror("(*critical*) Unable to reset SIGSEGV handler");
        exit(1);
    };

    if ( ( tmp[1] = signal( SIGBUS, tmp[1] ) ) == SIG_ERR ) {
        perror("(*critical*) Unable to reset SIGBUS handler");
        exit(1);
    };

    if ( _eval_checkptr_data.stat ) {
        switch( _eval_checkptr_data.sig ) {
        case(SIGSEGV):
            eval_error("Accessing %p caused Segmentation Fault", ptr);
            return 3;
        case(SIGBUS):
            eval_error("Accessing %p caused Bus Error", ptr);
            return 4;
        default:
            eval_error("Accessing %p caused unknonown signal", ptr);
            return 5;
        }
    }

    return 0;
}


/**
 * Global variables for controlling wrapper function behaviors
 **/

_eval_env_type _eval_env;

EVAL_VAR(exit);

EVAL_VAR(sleep);

EVAL_VAR(fork);
EVAL_VAR(wait);
EVAL_VAR(waitpid);


EVAL_VAR(kill);
EVAL_VAR(signal);
EVAL_VAR(sigaction);
EVAL_VAR(pause);
EVAL_VAR(alarm);


EVAL_VAR(msgget);
EVAL_VAR(msgsnd);
EVAL_VAR(msgrcv);
EVAL_VAR(msgctl);

EVAL_VAR(semget);
EVAL_VAR(semctl);
EVAL_VAR(semop);

EVAL_VAR(shmget);
EVAL_VAR(shmat);
EVAL_VAR(shmdt);
EVAL_VAR(shmctl);

EVAL_VAR(mkfifo);
EVAL_VAR(isfifo);

EVAL_VAR(remove);
EVAL_VAR(unlink);


/**
 * Signal handler for the EVAL_CATCH macro
 * requires data in the _eval_env variable
 **/
void _eval_sighandler( int sig ) {

    char *name;

    switch( sig ) {
    case( SIGSEGV ):
        eval_error("Segmentation fault (SIGSEGV)");
        break;
    case( SIGBUS ):
        eval_error("Bus error (SIGBUS)");
        break;
    case( SIGFPE ):
        eval_error("Floating point exception / division by 0 (SIGFPE)");
        break;
    case( SIGILL ):
        eval_error("Illegal instruction (SIGILL)");
        break;
    default:
        name = strsignal(sig);
        if ( name )
            eval_error("Unexepected signal %s (%d) caught!", name, sig);
        else
            eval_error("Unexepected signal %d caught!", sig);
    }

    _eval_env.signal = sig;
    longjmp(_eval_env.jmp, 2);
}

/**
 * Arms SIGSEGV and SIGBUS for the EVAL_CATCH macro
 **/
void _eval_arm_signals( void ) {
    _eval_env.old_sighndlr[0] = signal( SIGSEGV, _eval_sighandler );
    if ( _eval_env.old_sighndlr[0] == SIG_ERR ) {
        perror("(*critical*) Unable to set signal handler for SIGSEGV");
        exit(1);
    }

    _eval_env.old_sighndlr[1] = signal( SIGBUS, _eval_sighandler );
    if ( _eval_env.old_sighndlr[1] == SIG_ERR ) {
        perror("(*critical*) Unable to set signal handler for SIGBUS");
        exit(1);
    }

    _eval_env.old_sighndlr[2] = signal( SIGFPE, _eval_sighandler );
    if ( _eval_env.old_sighndlr[2] == SIG_ERR ) {
        perror("(*critical*) Unable to set signal handler for SIGFPE");
        exit(1);
    }

    _eval_env.old_sighndlr[3] = signal( SIGILL, _eval_sighandler );
    if ( _eval_env.old_sighndlr[3] == SIG_ERR ) {
        perror("(*critical*) Unable to set signal handler for SIGILL");
        exit(1);
    }
}

/**
 * Restores previous signal handlers
 **/
void _eval_disarm_signals( void ) {
    signal( SIGSEGV, _eval_env.old_sighndlr[0] );
    signal( SIGBUS,  _eval_env.old_sighndlr[1] );
    signal( SIGFPE,  _eval_env.old_sighndlr[2] );
    signal( SIGILL,  _eval_env.old_sighndlr[3] );
}

/**
 * Evaluate implementation calling of exit function
 * Requires data in global _eval_exit_data and _eval_env
 * 
 * @param seconds   Number of seconds to sleep
 * @return          Evalation result or result of sleep operation
 **/
void _eval_exit( int status ) {
    _eval_exit_data.status = status;
    if ( _eval_exit_data.action )
         eval_info("exit(%d) caught!", status );
    longjmp(_eval_env.jmp, 1);
}

/**
 * Evaluate implementation calling of sleep function
 * Requires data in global _eval_sleep_data
 * 
 * @param seconds   Number of seconds to sleep
 * @return          Evalation result or result of sleep operation
 **/
unsigned int _eval_sleep(unsigned int seconds) {
    _eval_sleep_data.status ++;
    _eval_sleep_data.seconds = seconds;
    switch( _eval_sleep_data.action ) {
    case(1):
        _eval_sleep_data.ret = 0;
        break;
    default:
        _eval_sleep_data.ret = sleep( seconds );
    }
    return _eval_sleep_data.ret;
}

/**
 * Evaluate implementation calling of fork function
 * Requires data in global _eval_fork_data
 * 
 * @return          Evalation result or result of fork operation
 **/
pid_t _eval_fork(void) {
    _eval_fork_data.status ++;
    switch( _eval_fork_data.action ) {
    case(3):
        _eval_fork_data.ret = -1;
        break;
    case(2):
        _eval_fork_data.ret = 1;
        break;
    case(1):
        _eval_fork_data.ret = 0;
        break;
    default:
        _eval_fork_data.ret = fork( );
    }
    return _eval_fork_data.ret;
}

/**
 * Evaluate implementation calling of wait function
 * Requires data in global _eval_wait_data
 * 
 * @return          Evalation result or result of wait operation
 **/
pid_t _eval_wait(int *stat_loc) {

    _eval_wait_data.status ++;
    _eval_wait_data.stat_loc = stat_loc;

    switch( _eval_wait_data.action ) {
    case(2):
        // Don't change the value of _eval_wait_data.ret
        break;
    case(1):
        _eval_wait_data.ret = 0;
        break;
    default:
        _eval_wait_data.ret = wait( stat_loc );
    }
    return _eval_wait_data.ret;
}

/**
 * Evaluate implementation calling of waitpid function
 * Requires data in global _eval_wait_data
 * 
 * @return          Evalation result or result of wait operation
 **/
pid_t _eval_waitpid(pid_t pid, int *stat_loc, int options) {

    _eval_waitpid_data.status ++;

    _eval_waitpid_data.pid = pid;
    _eval_waitpid_data.stat_loc = stat_loc;
    _eval_waitpid_data.options = options;

    switch( _eval_waitpid_data.action ) {
    case(1):
        _eval_waitpid_data.ret = pid;
        break;
    default:
        _eval_waitpid_data.ret = waitpid( pid, stat_loc, options );
    }
    return _eval_waitpid_data.ret;
}

/**
 * Evaluate implementation calling of kill function
 * Requires data in global _eval_sleep_data
 * 
 * @param pid   Process(es) to send signal to
 * @param sig   Signal to be sent
 * @return      error status
 **/
int _eval_kill(pid_t pid, int sig) {
    _eval_kill_data.status ++;
    _eval_kill_data.pid = pid;
    _eval_kill_data.sig = sig;

    switch( _eval_kill_data.action ) {
    case(1):
        _eval_kill_data.ret = 0;
        break;
    default:
        _eval_kill_data.ret = kill( pid, sig );
    }
    return _eval_kill_data.ret;
}

/**
 * Evaluate implementation calling of signal function
 * Requires data in global _eval_signal_data
 * 
 * @param signum    Signal to be handled
 * @param handler   Signal handler
 * @return          Previsous value of signal handler, SIG_ERR on error
 **/
sighandler_t _eval_signal(int signum, sighandler_t handler) {
    _eval_signal_data.status ++;
    _eval_signal_data.signum = signum;
    _eval_signal_data.handler = handler;

    switch( _eval_signal_data.action ) {
    case(2):
        datalog("signal,%d,%p", signum, handler );
        _eval_signal_data.ret = SIG_DFL;
        break;
    case(1):
        _eval_signal_data.ret = SIG_DFL;
        break;
    default:
        _eval_signal_data.ret = signal( signum, handler );
    }
    return _eval_signal_data.ret;
}

/**
 * Evaluate implementation calling of sigaction function
 * Requires data in global _eval_sleep_data
 * 
 * @param signum    Signal to be handled
 * @param act       Signal handler
 * @param oldaction Signal handler
 * @return          Previsous value of signal handler, SIG_ERR on error
 **/
int _eval_sigaction(int signum, const struct sigaction *act, struct sigaction *oldact) {
    _eval_sigaction_data.status ++;
    _eval_sigaction_data.signum = signum;
    _eval_sigaction_data.act = (struct sigaction *) act;

    switch( _eval_sigaction_data.action ) {
    case(2):
        if ( act -> sa_flags & SA_SIGINFO ) {
            datalog("sigaction,%d,%p", signum, act->sa_sigaction );
        } else {
            datalog("signal,%d,%p", signum, act->sa_handler );
        }
        _eval_sigaction_data.ret = 0;
        break;
    case(1):
        _eval_sigaction_data.ret = 0;
        break;
    default:
        _eval_sigaction_data.ret = sigaction( signum, act, oldact );
    }
    return _eval_sigaction_data.ret;
}

/**
 * Evaluate implementation calling of pause() function
 * Requires data in global _eval_pause_data
 * 
 * @return          Always returns -1
 **/
int _eval_pause(void) {
    _eval_pause_data.status ++;

    switch( _eval_pause_data.action ) {
    case(2):
        eval_error("pause() called, aborting");
        longjmp(_eval_env.jmp, 1);
        break;
    case(1):
        _eval_pause_data.ret = -1;
        break;
    default:
        _eval_pause_data.ret = pause( );
    }
    return _eval_pause_data.ret;
}

/**
 * @brief Evaluate implementation calling of alarm() function
 * 
 * @param seconds       Number of seconds until SIGALARM
 * @return              Number of seconds remaining on previous alarm 
 */
unsigned int _eval_alarm( unsigned int seconds ) {
    _eval_alarm_data.status ++;
    _eval_alarm_data.ret = _eval_alarm_data.seconds;
    _eval_alarm_data.seconds = seconds;

    switch( _eval_alarm_data.action ) {
    case(1):
        // Ignore alarm call
        break;
    default:
        _eval_alarm_data.ret = alarm( seconds );
    }
    return _eval_alarm_data.ret;
}

/**
 * Evaluate implementation calling of msgget function
 * 
 * @param key       IPC key of msg queue
 * @param msgflg    Flag controlling msg queue creation / connection
 * @return          Evalation result or result of msgget operation
 **/
int _eval_msgget(key_t key, int msgflg) {
    _eval_msgget_data.status ++;

    _eval_msgget_data.key = key;
    _eval_msgget_data.msgflg = msgflg;

    _eval_msgget_data.ret = msgget( key, msgflg );
    
    return _eval_msgget_data.ret;
}

/**
 * Evaluate implementation calling of msgsnd function
 * Requires data in global _eval_msgsnd_data
 * 
 * @param msqid     Queue ID to use
 * @param msgp      Pointer to message structure
 * @param msgsz     Size of message body
 * @param msgflg    
 * @return          Evalation result or result of msgsnd operation
 **/
int _eval_msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg) {

    _eval_msgsnd_data.status ++;

    switch( _eval_msgsnd_data.action ) {

    case(1): // capture
        _eval_msgsnd_data.msqid = msqid;
        _eval_msgsnd_data.msgp = (void *) msgp;
        _eval_msgsnd_data.msgsz = msgsz;
        _eval_msgsnd_data.msgflg = msgflg;
        return 0;
    
    case(2): // inject
        return msgsnd( _eval_msgsnd_data.msqid, _eval_msgsnd_data.msgp,
            _eval_msgsnd_data.msgsz, _eval_msgsnd_data.msgflg );

    default:
        return msgsnd( msqid, msgp, msgsz, msgflg );
    }
}

/**
 * Evaluate implementation calling of msgsnd function
 * Requires data in global _eval_msgrcv_data
 * 
 * @param msqid     Queue ID to use
 * @param msgp      Pointer to message structure
 * @param msgsz     Size of message body
 * @param msgflg    
 * @return          Evalation result or result of msgsnd operation
 **/
ssize_t _eval_msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg) {
    _eval_msgrcv_data.status ++;

    switch( _eval_msgrcv_data.action ) {

    case(1): // capture
        _eval_msgrcv_data.msqid = msqid;
        _eval_msgrcv_data.msgp = msgp;
        _eval_msgrcv_data.msgsz = msgsz;
        _eval_msgrcv_data.msgtyp = msgtyp;
        _eval_msgrcv_data.msgflg = msgflg;
        return msgsz;
    
    case(2): // inject
        return msgrcv( _eval_msgrcv_data.msqid,_eval_msgrcv_data.msgp, 
            _eval_msgrcv_data.msgsz, _eval_msgrcv_data.msgtyp,
            _eval_msgrcv_data.msgflg );

    default:
        _eval_msgrcv_data.msqid = msqid;
        _eval_msgrcv_data.msgp = msgp;
        _eval_msgrcv_data.msgsz = msgsz;
        _eval_msgrcv_data.msgtyp = msgtyp;
        _eval_msgrcv_data.msgflg = msgflg;
        return msgrcv( msqid, msgp, msgsz, msgtyp, msgflg );
    }

}

/**
 * Evaluate implementation calling of shmctl function
 * Requires data in global _eval_shmctl_data
 * 
 * @param shmid     Shared memory ID
 * @param cmd       Command to perform
 * @param buf       struct shmid_ds for input / output of command
 * @return          success
 **/
int _eval_msgctl(int msqid, int cmd, struct msqid_ds *buf) {
    _eval_msgctl_data.status ++;
    _eval_msgctl_data.msqid = msqid;
    _eval_msgctl_data.cmd = cmd;
    _eval_msgctl_data.buf = buf;
    _eval_msgctl_data.ret = msgctl( msqid, cmd, buf );

    return _eval_msgctl_data.ret;
}

/**
 * Evaluate implementation calling of semget function
 * Requires data in global _eval_semget_data
 * 
 * @param key       IPC key
 * @param nsems     Number of semaphores in the array
 * @param semflg    Flags for semaphore creation / connection
 * @return          semaphore id
 **/
int _eval_semget(key_t key, int nsems, int semflg) {

    _eval_semget_data.status ++;

    _eval_semget_data.key = key;
    _eval_semget_data.nsems = nsems;
    _eval_semget_data.semflg = semflg;

    _eval_semget_data.ret = semget( key, nsems, semflg );

    return _eval_semget_data.ret;
}

/**
 * Evaluate implementation calling of semctl function
 * Requires data in global _eval_semctl_data
 * 
 * @param semid     semaphore id
 * @param sops      Semaphore operations
 * @param nsops     nsops
 * @return          semop result
 **/
int _eval_semctl(int semid, int semnum, int cmd, ... ) {
    _eval_semctl_data.status ++;

    _eval_semctl_data.semid = semid;
    _eval_semctl_data.semnum = semnum;
    _eval_semctl_data.cmd = cmd;

    switch (cmd) {
    case(SETVAL):
    case(IPC_STAT):
    case(IPC_SET):
    case(GETALL):
    case(SETALL):
        {
            va_list valist;
            va_start(valist, cmd );
            _eval_semctl_data.arg = va_arg(valist, union semun );
            va_end(valist);
        }
        _eval_semctl_data.ret = semctl( semid, semnum, cmd, _eval_semctl_data.arg );
        break;
    default:
         _eval_semctl_data.ret = semctl( semid, semnum, cmd );
    }

    return _eval_semctl_data.ret;
}

/**
 * Evaluate implementation calling of semop function
 * Requires data in global _eval_semop_data
 * 
 * @param semid     semaphore id
 * @param sops      Semaphore operations
 * @param nsops     nsops
 * @return          semop result
 **/
int _eval_semop(int semid, struct sembuf *sops, size_t nsops) {
    _eval_semop_data.status ++;

    _eval_semop_data.sops = sops;
    _eval_semop_data.nsops = nsops;

    _eval_semop_data.ret = semop( semid, sops, nsops );

    return _eval_semop_data.ret;
}

/**
 * Evaluate implementation calling of shmget function
 * Requires data in global _eval_semget_data
 * 
 * @param key       IPC key
 * @param size      Size of shared memory region
 * @param semflg    Flags for shmem creation / connection
 * @return          shm id
 **/
int _eval_shmget(key_t key, size_t size, int shmflg) {

    _eval_shmget_data.status ++;

    _eval_shmget_data.key = key;
    _eval_shmget_data.size = size;
    _eval_shmget_data.shmflg = shmflg;

    _eval_shmget_data.ret = shmget( key, size, shmflg );

    return _eval_shmget_data.ret;
}

/**
 * Evaluate implementation calling of shmat function
 * Requires data in global _eval_semget_data
 * 
 * @param shmid     Shared memory ID
 * @param shmaddr   Choice for logical address
 * @param shmflg    Flags for address choice
 * @return          Logical address of shared memory region
 **/
void *_eval_shmat( int shmid, const void *shmaddr, int shmflg) {
    _eval_shmat_data.status ++;

    _eval_shmat_data.shmid = shmid;
    _eval_shmat_data.shmaddr = (void *) shmaddr;
    _eval_shmat_data.shmflg = shmflg;

    _eval_shmat_data.ret = shmat( shmid, shmaddr, shmflg );

    return _eval_shmat_data.ret;
}

/**
 * Evaluate implementation calling of shmdt function
 * Requires data in global _eval_shmdt_data
 * 
 * @param shmaddr   Logical address of shared memory region to detach
 * @return          success
 **/
int _eval_shmdt(const void *shmaddr) {
    _eval_shmdt_data.status ++;
    _eval_shmdt_data.shmaddr = (void *) shmaddr;
    _eval_shmdt_data.ret = shmdt( shmaddr );

    return _eval_shmdt_data.ret;
}

/**
 * Evaluate implementation calling of shmctl function
 * Requires data in global _eval_shmctl_data
 * 
 * @param shmid     Shared memory ID
 * @param cmd       Command to perform
 * @param buf       struct shmid_ds for input / output of command
 * @return          success
 **/
int _eval_shmctl(int shmid, int cmd, struct shmid_ds *buf) {
    _eval_shmctl_data.status ++;
    _eval_shmctl_data.shmid = shmid;
    _eval_shmctl_data.cmd = cmd;
    _eval_shmctl_data.buf = buf;
    _eval_shmctl_data.ret = shmctl( shmid, cmd, buf );

    return _eval_shmctl_data.ret;
}

/**
 * @brief Evaluate implementation calling of mkfifo function
 * Requires data in global _eval_mkfifo_data
 * 
 * @param path      Path to FIFO
 * @param mode      Creation mode
 * @return int      Success
 */
int _eval_mkfifo(const char *path, mode_t mode) {
    
    _eval_mkfifo_data.status ++;
    _eval_mkfifo_data.path = (char *) path;
    _eval_mkfifo_data.mode = mode;

    switch( _eval_mkfifo_data.action ) {
        case(1):
            _eval_mkfifo_data.ret = 0;
            break;
        default:
            _eval_mkfifo_data.ret = mkfifo( path, mode );
    }
    return _eval_mkfifo_data.ret;
}


/**
 * @brief Evaluate implementation calling of S_ISFIFO macro
 * Requires data in global _eval_isfifo_data
 * 
 * @param mode  File mode from stat or fstat
 * @return      For action = 1 always returns true, for action = 0
 *              has the same behavior as the S_ISFIFO macro
 */
int _eval_isfifo(mode_t mode) {
    
    _eval_isfifo_data.status ++;
    _eval_isfifo_data.mode = mode;
    _eval_isfifo_data.ret = S_ISFIFO(mode);

    switch( _eval_isfifo_data.action ) {
        case(1):
            return 1;
        default:
            return _eval_isfifo_data.ret;
    }
    return 0;
}


int _eval_remove(const char * path) {
    _eval_remove_data.status ++;
    _eval_remove_data.path = (char *) path;
    _eval_remove_data.ret = 0;

    switch( _eval_remove_data.action ) {
    case(2):
        // We use "unlink" deliberately
        datalog("unlink,%s", path );
        _eval_remove_data.ret = 0;
        break;
    case(1):
        _eval_remove_data.ret = 0;
        break;
    default:
        _eval_remove_data.ret = remove( path );
    }
    return _eval_remove_data.ret;
}

int _eval_unlink(const char * path) {
    _eval_unlink_data.status ++;
    _eval_unlink_data.path = (char *) path;
    _eval_unlink_data.ret = 0;

    switch( _eval_unlink_data.action ) {
    case(2):
        datalog("unlink,%s", path );
        _eval_unlink_data.ret = 0;
        break;
    case(1):
        _eval_unlink_data.ret = 0;
        break;
    default:
        _eval_unlink_data.ret = remove( path );
    }
    return _eval_unlink_data.ret;
}
