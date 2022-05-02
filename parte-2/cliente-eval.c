#include <fcntl.h>
#include <sys/stat.h>

#include "common.h"
#include "eval.h"

/**
 * Undefine the replacement macros defined in eval.h
 **/
#include "eval-undef.h"

// Global variables for cidadao-student
Passagem pedido;                        // Variável que tem o pedido enviado do Cliente para o Servidor
int pidServidor;                        // Variável que tem o PID do processo Servidor
char passagemIniciada = 0;          // Variável que indica que o Servidor já enviou sinal de início da passagem

/**
 * Wrapper for getPidServidor()
 **/
struct {
    int action;
    int pidServidor;
} _getPidServidor_data;

int getPidServidor() {
    _getPidServidor_data.pidServidor = -1;
    _getPidServidor_data.pidServidor = _student_getPidServidor();
    return -1;
}

/**
 * Wrapper for getDadosPedidoUtilizador()
 **/
struct {
    int action;
    Passagem p;
} _getDadosPedidoUtilizador_data;

Passagem getDadosPedidoUtilizador() {
    _getDadosPedidoUtilizador_data.p = (Passagem) {.tipo_passagem = -1};
    _getDadosPedidoUtilizador_data.p = _student_getDadosPedidoUtilizador();
    return _getDadosPedidoUtilizador_data.p;
}

/**
 * Wrapper for armaSinais()
 **/
struct {
    int action;
    int success;
} _armaSinais_data;

int armaSinais() {
    _armaSinais_data.success = -1024;
    _armaSinais_data.success = _student_armaSinais();
    return 0;
}

/**
 * Wrapper for escrevePedido()
 **/
struct {
    int action;
    int success;
} _escrevePedido_data;

int escrevePedido(Passagem p) {
    _escrevePedido_data.success = -1024;
    _escrevePedido_data.success = _student_escrevePedido(p);
    return 0;
}

/**
 * Wrapper for configuraTemporizador()
 **/
int configuraTemporizador() {
    return _student_configuraTemporizador();
}

/**
 * Wrapper for trataSinalSIGUSR1()
 **/
void trataSinalSIGUSR1(int sig) {
    _student_trataSinalSIGUSR1(sig);
}

/**
 * Wrapper for trataSinalSIGTERM()
 **/
void trataSinalSIGTERM(int sig) {
    _student_trataSinalSIGTERM(sig);
}

/**
 * Wrapper for trataSinalSIGHUP()
 **/
void trataSinalSIGHUP(int sig) {
    _student_trataSinalSIGHUP(sig);
}

/**
 * Wrapper for trataSinalSIGINT()
 **/
void trataSinalSIGINT(int sig) {
    _student_trataSinalSIGINT(sig);
}

/**
 * Wrapper for trataSinalSIGALRM()
 **/
void trataSinalSIGALRM(int sig) {
    _student_trataSinalSIGALRM(sig);
}

/**
 * Evaluate C1
 **/
void eval_c1( ) {
    printf("\n");
    eval_info("Evaluating C1 ...");

    const int testpid = 12345;

    FILE* ft = fopen(FILE_SERVIDOR,"w");
    fprintf(ft,"%d\n", testpid);
    fclose(ft);

    EVAL_CATCH( getPidServidor() );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(C1) bad termination");
    }

    if ( testpid != _getPidServidor_data.pidServidor ) {
        eval_error( "(C1) Wrong value read from file");
    }

    unlink(FILE_SERVIDOR );

    eval_check_successlog( "(C1)", "(C1)" );

    eval_close_logs( "(C1)" );
}


/**
 * Evaluate C2
 **/
void eval_c2( ) {
    printf("\n");
    eval_info("Evaluating C2 ...");

    const int tipo_passagem = 1;
    const char matricula[] = "21-36-EN";
    const char lanco[] = "Lisboa-Porto";

    FILE* ft = fopen(FILE_STDIN,"w");
    fprintf(ft,"%d\n", tipo_passagem );
    fprintf(ft,"%s\n", matricula );
    fprintf(ft,"%s\n", lanco );
    for(int i = 0; i < 5; i++ ) fprintf(ft,"\n" );
    fclose(ft);

    // Redirect standard I/O to files
    EVAL_CATCH_IO( getDadosPedidoUtilizador(), FILE_STDIN, FILE_STDOUT );

    if ( 0 != _eval_env.stat ) {
        eval_error( "(C2) bad termination");
    }

    // Remove I/O files
    unlink( FILE_STDIN );
    unlink( FILE_STDOUT );

    if ( tipo_passagem != _getDadosPedidoUtilizador_data.p.tipo_passagem ) {
        eval_error( "(C2.1) invalid p.tipo_passagem" );
        eval_check_errorlog( "(C2)", "(C2)" );
    }

    if ( strcmp(matricula,_getDadosPedidoUtilizador_data.p.matricula ) ) {
        eval_error( "(C2.2) invalid p.matricula");
        eval_check_errorlog( "(C2)", "(C2)" );
    }

    if ( strcmp(lanco,_getDadosPedidoUtilizador_data.p.lanco ) ) {
        eval_error( "(C2.3) invalid p.lanco");
        eval_check_errorlog( "(C2)", "(C2)" );
    }
    
    if ( getpid() != _getDadosPedidoUtilizador_data.p.pid_cliente ) {
        eval_error( "(C2.3) invalid p.pid_cliente");
        eval_check_errorlog( "(C2)", "(C2)" );
    }

    char str[120];
    snprintf(str, 120, "(C2) Passagem do tipo %s solicitado pela viatura com matrícula %s para o Lanço %s e com PID %d", 1 == tipo_passagem ? "Normal" : "Via Verde", matricula, lanco, getpid());
    eval_check_successlog(str, "(C2)");

    eval_close_logs( "(C2)" );
}

/**
 * Evaluate C3
 **/
void eval_c3( ) {
    printf("\n");
    eval_info("Evaluating C3 ...");

    _eval_signal_data.status = 0;
    _eval_signal_data.action = 2;

    _eval_sigaction_data.status = 0;
    _eval_sigaction_data.action = 2;

    initlog(&_data_log);

    EVAL_CATCH( armaSinais() );

    if ( 0 != _eval_env.stat ) {
        eval_error( "(C3) bad termination");
    }

    if ( 0 != _armaSinais_data.success ) {
        eval_error("(C3) Bad return value");
        eval_check_errorlog( "(C3)", "(C3)");
    }

    char buffer[100];
 
    snprintf( buffer, 100, "signal,%d,%p", SIGUSR1, trataSinalSIGUSR1 );
    if ( 0 > findinlog( &_data_log, buffer ))  {
        eval_error("(C3.1) SIGUSR1 not armed properly");
        eval_check_errorlog( "(C3)", "(C3)");
    }

    snprintf( buffer, 100, "signal,%d,%p", SIGTERM, trataSinalSIGTERM );
    if ( 0 > findinlog( &_data_log, buffer )) {
        eval_error("(C3.2) SIGTERM not armed properly");
        eval_check_errorlog( "(C3)", "(C3)");
    }

    snprintf( buffer, 100, "signal,%d,%p", SIGHUP, trataSinalSIGHUP );
    if ( 0 > findinlog( &_data_log, buffer )) {
        eval_error("(C3.3) SIGHUP not armed properly");
        eval_check_errorlog( "(C3)", "(C3)");
    }

    snprintf( buffer, 100, "signal,%d,%p", SIGINT, trataSinalSIGINT );
    if ( 0 > findinlog( &_data_log, buffer )) {
        eval_error("(C3.4) SIGINT not armed properly");
        eval_check_errorlog( "(C3)", "(C3)");
    }

    snprintf( buffer, 100, "signal,%d,%p", SIGALRM, trataSinalSIGALRM );
    if ( 0 > findinlog( &_data_log, buffer )) {
        eval_error("(C3.5) SIGALRM not armed properly");
        eval_check_errorlog( "(C3)", "(C3)");
    }

    eval_check_successlog( "(C3) Armei sinais", "(C3)" );

    eval_close_logs( "(C3)" );
}

/**
 * Evaluate C4
 **/
void eval_c4( ) {
    printf("\n");
    eval_info("Evaluating C4 ...");

    const Passagem p = {
        .tipo_passagem = 2,
        .matricula = "21-36-EN",
        .lanco = "Lisboa-Porto",
        .pid_cliente = 0xD840
    };

    // Create FILE_PEDIDOS as a standard file
    FILE *fb = fopen(FILE_PEDIDOS, "w");
    fprintf(fb,"empty\n");
    fclose(fb);

    // Always return true on S_ISFIFO
    _eval_isfifo_data.action = 1;
    EVAL_CATCH( escrevePedido(p) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(C4) bad termination");
    }

    if ( 0 != _escrevePedido_data.success ) {
        eval_error( "(C4) returned error");
        printlog( &_error_log );
    }

    struct stat st = {
        .st_size = 0
    };

    if ( stat( FILE_PEDIDOS, &st ) < 0) {
        eval_error( "(C4) Unable to stat %s", FILE_PEDIDOS );
        eval_check_errorlog( "(C4)", "(C4)");
    }

    if ( st.st_size < sizeof(Passagem) ) {
        eval_error( "(C4) Invalid or insufficient data on %s. Got %ld, expected %ld.", FILE_PEDIDOS, st.st_size, sizeof(Passagem));
    } else {
        fb = fopen(FILE_PEDIDOS,"r");
        if ( fb ) {
            Passagem p2;
            fread( &p2, sizeof(Passagem),1,fb);
            fclose( fb );
            
            if ( p.tipo_passagem != p2.tipo_passagem ) {
                eval_error("(C4) Invalid .tipo_passagem found in file");
                eval_check_errorlog( "(C4)", "(C4)");
            }

            if ( strcmp(p.matricula, p2.matricula ) ) {
                eval_error("(C4) Invalid .matricula found in file");
                eval_check_errorlog( "(C4)", "(C4)");
            }

            if ( strcmp(p.lanco, p2.lanco ) ) {
                eval_error("(C4) Invalid .lanco found in file");
                eval_check_errorlog( "(C4)", "(C4)");
            }

            if ( p.pid_cliente != p2.pid_cliente ) {
                eval_error("(C4) Invalid .pid_cliente found in file");
                eval_check_errorlog( "(C4)", "(C4)");
            }
        } else {
            eval_error("(C4) Unable to open %s for reading", FILE_PEDIDOS);
            eval_check_errorlog( "(C4)", "(C4)");
        }
    }

    unlink( FILE_PEDIDOS );

    eval_check_successlog("(C4) Escrevi FIFO", "(C4)");

    eval_close_logs( "(C4)" );
}

/**
 * Evaluate C5
 **/
void eval_c5( ) {
    printf("\n");
    eval_info("Evaluating C5 ...");

    // Intercept call to alarm() function
    _eval_alarm_data.status = 0;
    _eval_alarm_data.action = 1;

    EVAL_CATCH( configuraTemporizador() );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(C5) bad termination");
    }

    if ( 1 != _eval_alarm_data.status )
        eval_error("(C3.0) alarm was called %d times, should be 1",
            _eval_alarm_data.status );
        
    if ( MAX_ESPERA != _eval_alarm_data.seconds )
        eval_error("(C3.0) alarm was called with %d parameter, should be %d",
            _eval_alarm_data.seconds, MAX_ESPERA );

    char str[99];
    snprintf(str, 99, "(C5) Inicia espera de %d segundos", MAX_ESPERA);
    eval_check_successlog(str, "(C5)");

    eval_close_logs( "(C5)" );
}

/**
 * Evaluate C6
 **/
void eval_c6( ) {
    printf("\n");
    eval_info("Evaluating C6 ...");

    passagemIniciada = 0;

    EVAL_CATCH( trataSinalSIGUSR1(SIGUSR1) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(C6) bad termination");
    }

    if ( ! passagemIniciada )
        eval_error("(C6) passagemIniciada was not set to true");

    eval_check_successlog( "(C6) Passagem Iniciada", "(C6)" );

    eval_close_logs( "(C6)" );
}

/**
 * Evaluate C7
 **/
void eval_c7( ) {
    printf("\n");
    eval_info("Evaluating C7 ...");

    _eval_exit_data.status = -1024;

    passagemIniciada = 1;

    EVAL_CATCH( trataSinalSIGTERM( SIGTERM ) );
    if ( 1 != _eval_env.stat ) {
        eval_error( "(C7) should have been terminated by exit");
    }

    if ( 0 != _eval_exit_data.status  )
        eval_error("(C7) Invalid exit value, %d, should be 0", _eval_exit_data.status );

    eval_check_successlog( "(C7) Passagem Concluída", "(C7)" );

    passagemIniciada = 0;

    EVAL_CATCH( trataSinalSIGTERM( SIGTERM ) );
    if ( 1 != _eval_env.stat ) {
        eval_error( "(C7) should have been terminated by exit");
    }

    if ( 0 == _eval_exit_data.status  )
        eval_error("(C7) Invalid exit value, %d, should be !=0", _eval_exit_data.status );

    eval_check_errorlog( "(C7)", "(C7)" );

    eval_close_logs( "(C7)" );
}

/**
 * Evaluate C8
 **/
void eval_c8( ) {
    printf("\n");
    eval_info("Evaluating C8 ...");

    _eval_exit_data.status = -1024;

    passagemIniciada = 1;

    EVAL_CATCH( trataSinalSIGHUP( SIGHUP ) );
    if ( 1 != _eval_env.stat ) {
        eval_error( "(C8) should have been terminated by exit");
    }

    if ( 0 != _eval_exit_data.status  )
        eval_error("(C8) Invalid exit value, %d, should be 0", _eval_exit_data.status );

    eval_check_successlog( "(C8) Processo Não Concluído e Incompleto", "(C8)" );

    eval_close_logs( "(C8)" );
}

/**
 * Evaluate C9
 **/
void eval_c9( ) {
    printf("\n");
    eval_info("Evaluating C9 ...");

    pidServidor = 0x12345;

    // Don't send signal
    _eval_kill_data.status = 0;
    _eval_kill_data.action = 1;

    EVAL_CATCH( trataSinalSIGINT( SIGINT ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(C9) bad termination");
    }

    if ( 1 != _eval_kill_data.status )
        eval_error( "(C9) kill() was called %d times, should have been called once",
            _eval_kill_data.status );

    if ( SIGHUP != _eval_kill_data.sig )
        eval_error( "(C9) kill() was called with wrong signal, should be SIGHUP");

    if ( pidServidor != _eval_kill_data.pid )
        eval_error( "(C9) kill() was called with invalid pid");

    eval_check_successlog( "(C9) Processo Cancelado pelo Cliente", "(C9)" );

    eval_close_logs( "(C9)" );
}

/**
 * Evaluate C10
 **/
void eval_c10( ) {
    printf("\n");
    eval_info("Evaluating C10 ...");

    pidServidor = 0x12345;

    // Don't send signal
    _eval_kill_data.status = 0;
    _eval_kill_data.action = 1;

    EVAL_CATCH( trataSinalSIGALRM( SIGALRM ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(C10) bad termination");
    }

    if ( 1 != _eval_kill_data.status )
        eval_error( "(C10) kill() was called %d times, should have been called once",
            _eval_kill_data.status );

    if ( SIGHUP != _eval_kill_data.sig )
        eval_error( "(C10) kill() was called with wrong signal, should be SIGHUP");

    if ( pidServidor != _eval_kill_data.pid )
        eval_error( "(C10) kill() was called with invalid pid");

    eval_check_successlog( "(C10) Timeout Cliente", "(C10)" );

    eval_close_logs( "(C10)" );
}


int main() {

    initlog( &_success_log );
    initlog( &_error_log );

    eval_info(" %s/cliente.c\n", TOSTRING( _EVAL ) );

    _eval_exit_data.action = 0;

    // abort test whenever pause() is called
    _eval_pause_data.action = 2; 

    eval_c1();
    eval_c2();
    eval_c3();
    eval_c4();
    eval_c5();
    eval_c6();
    eval_c7();
    eval_c8();
    eval_c9();
    eval_c10();

    // Cleanup
    eval_info("Cleaning up ...");

    eval_info("Finished." );

}
