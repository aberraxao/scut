#include "common.h"
#include "utils.h"
#include "eval.h"

/**
 * Undefine the replacement macros defined in eval.h
 **/
#include "eval-undef.h"

// Global variables for servidor-student
Passagem pedido;                            // Variável que tem o pedido enviado do Cliente para o Servidor
Passagem lista_passagens[NUM_PASSAGENS];    // BD com o Número de pedidos em simultâneo que o servidor suporta
int indice_lista;                           // Índice corrente da Lista, que foi reservado pela função reservaEntradaBD()
Contadores stats;                           // Contadores de estatisticas

/**
 * Wrapper for init()
 **/
int init(Passagem* bd) {
    _student_init( bd );
    return 0;
}

/**
 * Wrapper for loadStats()
 **/
int loadStats(Contadores* pStats) {
    _student_loadStats( pStats );
    return 0;
}

/**
 * Wrapper for criaFicheiroServidor()
 **/
int criaFicheiroServidor() {
    _student_criaFicheiroServidor();
    return 0;
}

/**
 * Wrapper for criaFifo()
 **/
int criaFifo() {
    _student_criaFifo();
    return 0;
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
 * Wrapper for lePedido()
 **/
struct {
    int action;
    Passagem p;
} _lePedido_data;
Passagem lePedido() {
    _lePedido_data.p.tipo_passagem = -1;
    _lePedido_data.p = _student_lePedido();
    return _lePedido_data.p;
}

/**
 * Wrapper for validaPedido()
 **/
struct {
    int action;
    int success;
} _validaPedido_data;
int validaPedido(Passagem pedido) {
    _validaPedido_data.success = -1;
    _validaPedido_data.success = _student_validaPedido( pedido );
    return _validaPedido_data.success;
}

/**
 * Wrapper for reservaEntradaBD()
 **/
struct {
    int action;
    int idx;
} _reservaEntradaBD_data;
int reservaEntradaBD(Passagem* bd, Passagem pedido) {
    _reservaEntradaBD_data.idx = -1;
    _reservaEntradaBD_data.idx = _student_reservaEntradaBD( bd, pedido );
    return _reservaEntradaBD_data.idx;
}

/**
 * Wrapper for criaServidorDedicado()
 **/
struct {
    int action;
    int pid;
} _criaServidorDedicado_data;
int criaServidorDedicado(Passagem* bd, int indice_lista) {
    _criaServidorDedicado_data.pid = 0xFFFFFFFF;
    _criaServidorDedicado_data.pid = _student_criaServidorDedicado(bd, indice_lista);
    return _criaServidorDedicado_data.pid;
}

/**
 * Wrapper for trataSinalSIGINT()
 **/
void trataSinalSIGINT(int sig) {
    _student_trataSinalSIGINT(sig);
}

/**
 * Wrapper for trataSinalSIGHUP()
 **/
void trataSinalSIGHUP(int sig, siginfo_t *info, void *uap) {
    _student_trataSinalSIGHUP( sig, info, uap );
}

/**
 * Wrapper for trataSinalSIGCHLD()
 **/
void trataSinalSIGCHLD(int sig) {
    _student_trataSinalSIGCHLD( sig );
}

/**
 * Wrapper for sd_armaSinais()
 **/
struct {
    int action;
    int success;
} _sd_armaSinais_data;
int sd_armaSinais() {
    _sd_armaSinais_data.success = 1024;
    _sd_armaSinais_data.success = _student_sd_armaSinais();
    return _sd_armaSinais_data.success;
}

/**
 * Wrapper for sd_iniciaProcessamento()
 **/
int sd_iniciaProcessamento(Passagem pedido) {
    _student_sd_iniciaProcessamento(pedido);
    return 0;
}

/**
 * Wrapper for sd_sleepRandomTime()
 **/
int sd_sleepRandomTime() {
    _student_sd_sleepRandomTime();
    return 0;
}

/**
 * Wrapper for sd_terminaProcessamento()
 **/
int sd_terminaProcessamento(Passagem pedido) {
    _student_sd_terminaProcessamento(pedido);
    return 0;
}

/**
 * Wrapper for sd_trataSinalSIGTERM()
 **/
void sd_trataSinalSIGTERM(int sinalRecebido) {
    _student_sd_trataSinalSIGTERM(sinalRecebido);
}


/**
 * Evaluate S1
 **/
void eval_s1( ) {
    printf("\n");
    eval_info("Evaluating S1 ...");

    Passagem bd[NUM_PASSAGENS];
    for( int i = 0; i < NUM_PASSAGENS; i++)
        bd[i].tipo_passagem = -1024;

    EVAL_CATCH( init(bd) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(C1) bad termination");
    }

    for( int i = 0; i < NUM_PASSAGENS; i++) {
        if ( -1 != bd[i].tipo_passagem ) {
            eval_error("(S1) bd not initialized properly");
            break;
        }
    }
    
    eval_check_successlog( "(S1) Init Servidor", "(S1)" );

    eval_close_logs( "(S1)" );
}

/**
 * Evaluate S2
 **/
void eval_s2( ) {
    printf("\n");
    eval_info("Evaluating S2 ...");

    Contadores s = {
        .contadorNormal = 1,
        .contadorViaVerde = 2,
        .contadorAnomalias = 3
    };

    FILE *fb = fopen( FILE_STATS, "w" );
    if ( fb ) {
        fwrite( &s, sizeof(Contadores), 1, fb );
        fclose( fb );
    } else {
        eval_error("(S2) Unable to write test %s", FILE_STATS );
        return;
    }

    s.contadorNormal = -1;
    s.contadorViaVerde = -1;
    s.contadorAnomalias = -1;

    EVAL_CATCH( loadStats( &s ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S2) bad termination");
    }

    if ( (1 != s.contadorNormal) || (2 != s.contadorViaVerde) || (3 != s.contadorAnomalias) )
        eval_error("(S2) Invalid stats loaded");
    
    eval_check_successlog( "(S2) Estatísticas Carregadas", "(S2)" );

    unlink( FILE_STATS );

    Contadores s1 = {
        .contadorNormal = -1,
        .contadorViaVerde = -1,
        .contadorAnomalias = -1
    };

    EVAL_CATCH( loadStats( &s1 ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S2) bad termination");
    }

    if ( (0 != s1.contadorNormal) || (0 != s1.contadorViaVerde) || (0 != s1.contadorAnomalias) )
        eval_error("(S1) Invalid stats");
    
    eval_check_successlog( "(S2) Estatísticas Iniciadas", "(S2)" );

    eval_close_logs( "(S2)" );
}

/**
 * Evaluate S3
 **/
void eval_s3( ) {
    printf("\n");
    eval_info("Evaluating S3 ...");

    EVAL_CATCH( criaFicheiroServidor() );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S3) bad termination");
    }

    FILE* ft = fopen( FILE_SERVIDOR, "r" );
    if ( ft ) {
        pid_t server_pid = -1;
        if ( fscanf( ft, "%d", &server_pid ) != 1 ) {
            eval_error( "(S3) Unable to read server PID from %s", FILE_SERVIDOR );
        }

        if ( getpid() != server_pid ) {
            eval_error( "(S3) Invalid PID read from %s", FILE_SERVIDOR );
        }
        fclose(ft);
    } else {
        eval_error( "(S3) Unable to open file %s for reading", FILE_SERVIDOR );
        eval_check_errorlog("(S3)", "(S3)");
    }

    char str[99];
    snprintf(str, 99, "(S3) %d", getpid());
    eval_check_successlog( str, "(S3)" );

    if (unlink( FILE_SERVIDOR )<0) {
        eval_error("(S3) Unable to remove file %s",FILE_SERVIDOR );
    }

    eval_close_logs( "(S3)" );
}

/**
 * Evaluate S4
 **/
void eval_s4( ) {
    printf("\n");
    eval_info("Evaluating S4 ...");

    _eval_mkfifo_data.status = 0;
    _eval_mkfifo_data.path = NULL;
    _eval_mkfifo_data.mode = 0;

    // Don't create FIFO just catch parameters
    _eval_mkfifo_data.action = 1;

    EVAL_CATCH( criaFifo() );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S4) bad termination");
    }

    if ( 1 != _eval_mkfifo_data.status )
        eval_error("(S4) mkfifo was called %d times, should be 1",
            _eval_mkfifo_data.status );
    
    if ( strcmp(_eval_mkfifo_data.path, FILE_PEDIDOS) != 0 ) {
        eval_error("(S4) mkfifo was called with invalid path %s",
            _eval_mkfifo_data.path );
    }

    mode_t mode = 0700 & _eval_mkfifo_data.mode;
    if ( (0600 != mode) && (0700 != mode) ) {
         eval_error("(S4) mkfifo was called with invalid mode 0%o",
            _eval_mkfifo_data.mode );
    }

    eval_check_successlog( "(S4) Criei FIFO", "(S4)" );

    eval_close_logs( "(S4)" );
}

/**
 * Evaluate S5
 **/
void eval_s5( ) {
    printf("\n");
    eval_info("Evaluating S5 ...");

    _eval_signal_data.status = 0;
    _eval_signal_data.action = 2;

    _eval_sigaction_data.status = 0;
    _eval_sigaction_data.action = 2;

    initlog(&_data_log);

    EVAL_CATCH( armaSinais() );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S5) bad termination");
    }

    if ( 0 != _armaSinais_data.success )
        eval_error("(C3) Bad return value");

    char buffer[100];
 
    snprintf( buffer, 100, "signal,%d,%p", SIGINT, trataSinalSIGINT );
    if ( 0 > findinlog( &_data_log, buffer )) {
        eval_error("(S5) SIGINT not armed properly");
        eval_check_errorlog("(S5)", "(S5)");
    }

    snprintf( buffer, 100, "sigaction,%d,%p", SIGHUP, trataSinalSIGHUP );
    if ( 0 > findinlog( &_data_log, buffer ))  {
        eval_error("(S5) SIGHUP not armed properly");
        eval_check_errorlog("(S5)", "(S5)");
    }

    snprintf( buffer, 100, "signal,%d,%p", SIGCHLD, trataSinalSIGCHLD );
    if ( 0 > findinlog( &_data_log, buffer )) {
        eval_error("(S5) SIGCHLD not armed properly");
        eval_check_errorlog("(S5)", "(S5)");
    }

    eval_check_successlog( "(S5) Armei sinais", "(S5)" );

    eval_close_logs( "(S5)" );
}

/**
 * Evaluate S6
 **/
void eval_s6( ) {
    printf("\n");
    eval_info("Evaluating S6 ...");

    const Passagem p = {
        .tipo_passagem = 2,
        .matricula = "21-36-EN",
        .lanco = "Lisboa-Porto",
        .pid_cliente = 0xD840
    };

    // Create FILE_PEDIDOS as a standard file
    FILE *fb = fopen(FILE_PEDIDOS, "w");
    fwrite(&p, sizeof(Passagem), 1, fb );
    fclose(fb);

    EVAL_CATCH( lePedido() );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S6) bad termination");
    }

    if ( p.tipo_passagem != _lePedido_data.p.tipo_passagem ) {
        eval_error("(S6) Invalid .tipo_passagem read from file");
        eval_check_errorlog("(S6)", "(S6)");
    }

    if ( strcmp(p.matricula, _lePedido_data.p.matricula) ) {
        eval_error("(S6) Invalid .matricula read from file");
        eval_check_errorlog("(S6)", "(S6)");
    }

    if ( strcmp(p.lanco, _lePedido_data.p.lanco) ) {
        eval_error("(S6) Invalid .lanco read from file");
        eval_check_errorlog("(S6)", "(S6)");
    }

    if ( p.pid_cliente != _lePedido_data.p.pid_cliente ) {
        eval_error("(S6) Invalid .pid_cliente read from file");
        eval_check_errorlog("(S6)", "(S6)");
    }

    if ( unlink( FILE_PEDIDOS ) < 0 )
        eval_error("(S6) Unable to remove file %s", FILE_PEDIDOS );

    eval_check_successlog( "(S6) Li FIFO", "(S6)" );

    eval_close_logs( "(S6)" );
}

/**
 * Evaluate S7
 **/
void eval_s7( ) {
    printf("\n");
    eval_info("Evaluating S7 ...");

    Passagem p = {
        .tipo_passagem = 2,
        .matricula = "21-36-EN",
        .lanco = "Lisboa-Porto",
        .pid_cliente = 0xD840
    };

    stats.contadorAnomalias = 0;
    EVAL_CATCH( validaPedido( p ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S7) bad termination");
    }

    if ( _validaPedido_data.success != 0 )
        eval_error( "(S7) Failed with valid pedido");
    
    if ( 0 != stats.contadorAnomalias )
        eval_error( "(S7) Invalid value for stats.contadorAnomalias (should be 0)");

    char str[199];
    snprintf(str, 199, "(S7) Chegou novo pedido de passagem do tipo %s solicitado pela viatura com matrícula %s para o Lanço %s e com PID %d", 1 == p.tipo_passagem ? "Normal" : "Via Verde", p.matricula, p.lanco, p.pid_cliente);
    eval_check_successlog(str, "(S7)");

    // Bad .tipo_passagem
    p.tipo_passagem = 3;

    stats.contadorAnomalias = 0;
    EVAL_CATCH( validaPedido( p ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S7) bad termination");
    }
    p.tipo_passagem = 2;

    if ( _validaPedido_data.success != -1 )
        eval_error( "(S7) Expected return -1 for invalid .tipo_passagem");

    if ( 1 != stats.contadorAnomalias )
        eval_error( "(S7) Invalid value for stats.contadorAnomalias (should be 1)");

    eval_check_errorlog( "(S7)", "(S7)" );

    // Bad .matricula
    strcpy(p.matricula,"");

    stats.contadorAnomalias = 0;
    EVAL_CATCH( validaPedido( p ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S7) bad termination");
    }
    strcpy(p.matricula,"21-36-EN");

    if ( _validaPedido_data.success != -1 )
        eval_error( "(S7) Expected return -1 for invalid .matricula");

    if ( 1 != stats.contadorAnomalias )
        eval_error( "(S7) Invalid value for stats.contadorAnomalias (should be 1)");

    eval_check_errorlog( "(S7)", "(S7)" );

    // Bad .lanco
    strcpy(p.lanco, "");

    stats.contadorAnomalias = 0;
    EVAL_CATCH( validaPedido( p ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S7) bad termination");
    }
    strcpy(p.lanco,"Lisboa-Porto");

    if ( _validaPedido_data.success != -1 )
        eval_error( "(S7) Expected return -1 for invalid .lanco");

    if ( 1 != stats.contadorAnomalias )
        eval_error( "(S7) Invalid value for stats.contadorAnomalias (should be 1)");

    eval_check_errorlog( "(S7)", "(S7)" );

    // Bad .pid_cliente
    p.pid_cliente = 0;

    stats.contadorAnomalias = 0;
    EVAL_CATCH( validaPedido( p ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S7) bad termination");
    }
    p.pid_cliente = 0xD840;

    if ( _validaPedido_data.success != -1 )
        eval_error( "(S7) Expected return -1 for invalid .pid_cliente");

    if ( 1 != stats.contadorAnomalias )
        eval_error( "(S7) Invalid value for stats.contadorAnomalias (should be 1)");

    eval_check_errorlog( "(S7)", "(S7)" );
    eval_close_logs( "(S7)" );
}


int passagem_equal( Passagem a, Passagem b ) {
    int equal = 1;

    if ( a.tipo_passagem != b.tipo_passagem ) equal = 0;
    if ( strcmp( a.matricula, b.matricula ) ) equal = 0;
    if ( strcmp( a.lanco, b.lanco ) ) equal = 0;
    if ( a.pid_cliente != b.pid_cliente ) equal = 0;

    return equal;
}


/**
 * Evaluate S8
 **/
void eval_s8( ) {
    printf("\n");
    eval_info("Evaluating S8 ...");

    pedido = (Passagem) {
        .tipo_passagem = 1,
        .matricula = "52-FZ-76",
        .lanco = "Santarém-Setúbal",
        .pid_cliente = 0xD840
    };

    Passagem bd[NUM_PASSAGENS];
    for(int i = 0; i < NUM_PASSAGENS; i++)
        bd[i].tipo_passagem = 1;
    
    const int idx = NUM_PASSAGENS/2;
    bd[idx].tipo_passagem = -1;

    stats.contadorNormal = 1;
    stats.contadorViaVerde = 5;
    EVAL_CATCH( reservaEntradaBD( bd, pedido ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S8) bad termination");
    }

    if ( idx != _reservaEntradaBD_data.idx )
        eval_error("(S8) Returned invalid idx");
    
    if ( 2 != stats.contadorNormal )
        eval_error("(S8) Invalid value for stats.contadorNormal");

    if ( ! passagem_equal( pedido, bd[idx]))
        eval_error("(S8) Pedido not registered properly");
    
    char str[99];
    snprintf(str, 99, "(S8) Entrada %d preenchida", idx);
    eval_check_successlog(str, "(S8)");

    // Via verde
    pedido.tipo_passagem = 2;

    for( int i = 0; i < NUM_PASSAGENS; i++)
        bd[i].tipo_passagem = 1;
    bd[idx].tipo_passagem = -1;

    stats.contadorNormal = 1;
    stats.contadorViaVerde = 5;
    EVAL_CATCH( reservaEntradaBD(bd, pedido) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S8) bad termination");
    }

    if ( idx != _reservaEntradaBD_data.idx )
        eval_error("(S8) Returned invalid idx");
    
    if ( 6 != stats.contadorViaVerde )
        eval_error("(S8) Invalid value for stats.contadorViaVerde");

    if ( ! passagem_equal( pedido, bd[idx]))
        eval_error("(S8) Pedido not registered properly");
    
    snprintf(str, 99, "(S8) Entrada %d preenchida", idx);
    eval_check_successlog(str, "(S8)");

    // Lista cheia
    for( int i = 0; i < NUM_PASSAGENS; i++)
        bd[i].tipo_passagem = 1;

    stats.contadorAnomalias = 10;

    _eval_kill_data.status = 0;
    _eval_kill_data.action = 1; // Just capture call data
    _eval_kill_data.pid = 0;
    _eval_kill_data.sig = 0;

    EVAL_CATCH( reservaEntradaBD(bd, pedido) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S8) bad termination");
    }

    if ( -1 != _reservaEntradaBD_data.idx )
        eval_error("(S8) Returned invalid idx, should be -1");
    
    if ( 11 != stats.contadorAnomalias )
        eval_error("(S8) Invalid value for stats.contadorAnomalias");
    
    if ( 1 != _eval_kill_data.status )
        eval_error("(S8) kill() should have been called exactly once");
    
    if ( SIGHUP != _eval_kill_data.sig )
        eval_error("(S8) kill() should have been called with SIGHUP");
    
    if ( pedido.pid_cliente != _eval_kill_data.pid )
        eval_error("(S8) kill() called with wrong pid");
    
    eval_check_errorlog("(S8) Lista de Passagens cheia", "(S8)");

    eval_close_logs( "(S8)" );
}


/**
 * Evaluate S9
 **/
void eval_s9( ) {
    printf("\n");
    eval_info("Evaluating S9 ...");

    indice_lista = 0;
    lista_passagens[0].pid_servidor_dedicado = 0xFFFFFFFF;

    // Testa filho
    _eval_fork_data.action = 1;
    EVAL_CATCH( criaServidorDedicado(lista_passagens, indice_lista) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S9) bad termination");
    }
    
    if ( _criaServidorDedicado_data.pid != 0 )
        eval_error( "(S9) Invalid value returned from criaServidorDedicado, should be 0 ");
    
    // Testa pai
    _eval_fork_data.action = 2;

    EVAL_CATCH( criaServidorDedicado(lista_passagens, indice_lista) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S9) bad termination");
    }
    
    if ( _criaServidorDedicado_data.pid != 1 )
        eval_error( "(S9) Invalid value returned from criaServidorDedicado, should be 1 ");
    
    if ( 1 != lista_passagens[0].pid_servidor_dedicado )
        eval_error( "(S9) pid_servidor_dedicado not registered in global DB");

    char str[99];
    snprintf(str, 99, "(S9) Criado Servidor Dedicado com PID %d", 1);
    eval_check_successlog(str, "(S9)");

    // Testa erro
    _eval_fork_data.action = 3;
    EVAL_CATCH( criaServidorDedicado(lista_passagens, indice_lista) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S9) bad termination");
    }
    
    if ( _criaServidorDedicado_data.pid != -1 )
        eval_error( "(S9) Invalid value returned from criaServidorDedicado, should be -1 ");

    eval_check_errorlog("(S9)", "(S9)");

    eval_close_logs( "(S9)" );
}


/**
 * Evaluate S10
 **/
void eval_s10( ) {
    printf("\n");
    eval_info("Evaluating S10 ...");

    for(int i = 0; i < NUM_PASSAGENS; i++ )
        lista_passagens[i].tipo_passagem = -1;
    
    const int idx = NUM_PASSAGENS / 2;
    lista_passagens[idx].tipo_passagem = 2;
    lista_passagens[idx].pid_servidor_dedicado = 0xD840;

    stats.contadorNormal = 1;
    stats.contadorViaVerde = 2;
    stats.contadorAnomalias = 3;

    _eval_kill_data.status = 0;
    _eval_kill_data.action = 1; // Just capture call data
    _eval_kill_data.pid = 0;
    _eval_kill_data.sig = 0;

    _eval_remove_data.status = 0;
    _eval_remove_data.action = 1;

    _eval_unlink_data.status = 0;
    _eval_unlink_data.action = 1;

    initlog(&_data_log);

    _eval_remove_data.action = 2;
    _eval_unlink_data.action = 2;

    EVAL_CATCH( trataSinalSIGINT( SIGINT ) );
    if ( 1 != _eval_env.stat ) { // should terminate with exit()
        eval_error( "(S10) Should terminate with exit()");
    }

    if ( 1 != _eval_kill_data.status )
        eval_error( "(S10) kill should have been called exactly once");
    
    if ( lista_passagens[idx].pid_servidor_dedicado != _eval_kill_data.pid )
        eval_error( "(S10) kill called with wrong pid");

    if ( SIGTERM != _eval_kill_data.sig )
        eval_error( "(S10) kill called with wrong signal");

    Contadores s;
    FILE *f = fopen( FILE_STATS, "r" );
    if ( f ) {
        fread( &s, sizeof(Contadores), 1, f );
        fclose( f );

        if ( s.contadorNormal != stats.contadorNormal )
            eval_error( "(S10) Invalid value .contadorNormal in file %s. Expected '%d', got '%d'", FILE_STATS, stats.contadorNormal, s.contadorNormal);

        if ( s.contadorViaVerde != stats.contadorViaVerde )
            eval_error( "(S10) Invalid value .contadorViaVerde in file %s. Expected '%d', got '%d'", FILE_STATS, stats.contadorViaVerde, s.contadorViaVerde);

        if ( s.contadorAnomalias != stats.contadorAnomalias )
            eval_error( "(S10) Invalid value .contadorNormal in file %s. Expected '%d', got '%d'", FILE_STATS, stats.contadorAnomalias, s.contadorAnomalias);
    } else {
        eval_error("(S10) Unable to open file %s for reading", FILE_STATS );
    }

    if ( unlink(FILE_STATS) < 0 )
        eval_error("(S10) Unable to remove file %s", FILE_STATS );
    

    char buffer[100];
 
    snprintf( buffer, 100, "unlink,%s", FILE_SERVIDOR );
    if ( 0 > findinlog( &_data_log, buffer )) 
        eval_error("(S10) %s not removed properly", FILE_SERVIDOR);

    snprintf( buffer, 100, "unlink,%s", FILE_PEDIDOS );
    if ( 0 > findinlog( &_data_log, buffer )) 
        eval_error("(S10) %s not removed properly", FILE_PEDIDOS);

    eval_check_successlog( "(S10) Shutdown Servidor", "(S10)");
    eval_check_successlog( "(S10.1) Shutdown Servidores Dedicados", "(S10)");
    eval_check_successlog( "(S10.2)", "(S10)");
    eval_check_successlog( "(S10.3)", "(S10)");
    eval_check_successlog( "(S10.4) Estatísticas Guardadas", "(S10)");
    eval_check_successlog( "(S10.5)", "(S10)");

    eval_close_logs( "(S10)" );
}

/**
 * Evaluate S11
 **/
void eval_s11( ) {
    printf("\n");
    eval_info("Evaluating S11 ...");

    for( int i = 0; i < NUM_PASSAGENS; i ++ )
        lista_passagens[i].pid_cliente = -1;
    
    const int idx = NUM_PASSAGENS / 2;
    lista_passagens[idx].tipo_passagem = 1;
    lista_passagens[idx].pid_cliente = 0xD840;

    siginfo_t info;
    info.si_pid = 0xD840;

    _eval_kill_data.status = 0;
    _eval_kill_data.action = 1; // Just capture call data
    _eval_kill_data.pid = 0;
    _eval_kill_data.sig = 0;

    stats.contadorAnomalias = 0;
    EVAL_CATCH( trataSinalSIGHUP( SIGHUP, &info, NULL ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S11) bad termination");
    }

    if ( 1 != _eval_kill_data.status )
        eval_error( "(S11) kill() should have been called exactly once");

    if ( 0xD840 != _eval_kill_data.pid )
        eval_error( "(S11) kill() with wrong PID");

    if ( SIGTERM != _eval_kill_data.sig )
        eval_error( "(S11) kill called with wrong signal");

    if ( -1 != lista_passagens[idx].tipo_passagem )
         eval_error( "(S11) Entry not removed from global BD");

    eval_check_successlog( "(S11) Cancel", "(S11)");

    char str[99];
    snprintf(str, 99, "(S11.1) Cancelamento enviado pelo Processo %d", _eval_kill_data.pid);
    eval_check_successlog(str, "(S11)");
    snprintf(str, 99, "(S11.2) Cancelamento %d", _eval_kill_data.pid);
    eval_check_successlog(str, "(S11)");
    if ( 1 != stats.contadorAnomalias )
        eval_error( "(S11.2) Invalid value for stats.contadorAnomalias (should be 1)");
    eval_check_successlog( "(S11.3) Sinal de Cancelamento enviado ao Servidor Dedicado", "(S11)");

    eval_close_logs( "(S11)" );
}

/**
 * Evaluate S12
 **/
void eval_s12( ) {
    printf("\n");
    eval_info("Evaluating S12 ...");

    for( int i = 0; i < NUM_PASSAGENS; i ++ )
        lista_passagens[i].pid_cliente = -1;
    
    const int idx = NUM_PASSAGENS / 2;
    lista_passagens[idx].tipo_passagem = 1;
    lista_passagens[idx].pid_cliente = 0xD840;

    _eval_wait_data.status = 0;
    _eval_wait_data.action = 2;
    _eval_wait_data.ret = 0xD840;

    EVAL_CATCH( trataSinalSIGCHLD( SIGCHLD ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S11) bad termination");
    }

    if ( 1 != _eval_wait_data.status )
        eval_error( "(S12) wait should have been called exactly once");
    
    if ( -1 != lista_passagens[idx].tipo_passagem )
         eval_error( "(S12) Entry not removed from global BD");

    eval_check_successlog( "(S12) Servidor Dedicado Terminou", "(S12)");
    eval_check_successlog( "(S12.1)", "(S12)");
    eval_check_successlog( "(S12.2)", "(S12)");

    eval_close_logs( "(S12)" );
}

/**
 * Evaluate SD13
 **/
void eval_sd13( ) {
    printf("\n");
    eval_info("Evaluating SD13 ...");

    _eval_signal_data.status = 0;
    _eval_signal_data.action = 2;

    _eval_sigaction_data.status = 0;
    _eval_sigaction_data.action = 2;

    initlog(&_data_log);

    EVAL_CATCH( sd_armaSinais() );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(SD13) bad termination");
    }

    if ( 0 != _sd_armaSinais_data.success )
        eval_error("(C3) Bad return value");

    char buffer[100];
 
    snprintf( buffer, 100, "signal,%d,%p", SIGTERM, sd_trataSinalSIGTERM );
    if ( 0 > findinlog( &_data_log, buffer )) 
        eval_error("(S5) SIGTERM not armed properly");

    snprintf( buffer, 100, "signal,%d,%p", SIGINT, SIG_IGN );
    if ( 0 > findinlog( &_data_log, buffer )) 
        eval_error("(S5) SIGINT not armed properly");

    eval_check_successlog( "(SD13) Servidor Dedicado", "(SD13)");

    eval_close_logs( "(SD13)" );
}

/**
 * Evaluate SD14
 **/
void eval_sd14( ) {
    printf("\n");
    eval_info("Evaluating SD14 ...");

    Passagem p = {
        .tipo_passagem = 2,
        .matricula = "21-36-EN",
        .lanco = "Lisboa-Porto",
        .pid_cliente = 0xD840
    };

    _eval_kill_data.status = 0;
    _eval_kill_data.action = 1; // Just capture call data
    _eval_kill_data.pid = 0;
    _eval_kill_data.sig = 0;

    EVAL_CATCH( sd_iniciaProcessamento(p) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(SD14) bad termination");
    }

    if ( 1 != _eval_kill_data.status )
        eval_error( "(SD14) kill() should have been called exactly once");
    
    if ( 0xD840 != _eval_kill_data.pid )
        eval_error( "(SD14) kill() called with wrong PID");

    if ( SIGUSR1 != _eval_kill_data.sig )
        eval_error( "(SD14) kill() called with wrong signal");

    eval_check_successlog( "(SD14) Início Passagem", "(SD14)");

    eval_close_logs( "(SD14)" );
}

/**
 * Evaluate SD15
 **/
void eval_sd15( ) {
    printf("\n");
    eval_info("Evaluating SD15 ...");

    _eval_sleep_data.status = 0;
    _eval_sleep_data.action = 1;

    EVAL_CATCH( sd_sleepRandomTime() );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(SD15) bad termination");
    }

    if ( 1 != _eval_sleep_data.status )
        eval_error( "(SD15) sleep() should have been called exactly once");
    
    if ( (_eval_sleep_data.seconds < MIN_PROCESSAMENTO) ||
         (_eval_sleep_data.seconds > MAX_PROCESSAMENTO) )
        eval_error( "(SD15) sleep() called with invalid value (%d)", _eval_sleep_data.seconds );

    eval_check_successlog( "(SD15)", "(SD15)" );

    eval_close_logs( "(SD15)" );
}

/**
 * Evaluate SD16
 **/
void eval_sd16( ) {
    printf("\n");
    eval_info("Evaluating SD16 ...");

    Passagem p = {
        .tipo_passagem = 2,
        .matricula = "21-36-EN",
        .lanco = "Lisboa-Porto",
        .pid_cliente = 0xD840
    };

    _eval_kill_data.status = 0;
    _eval_kill_data.action = 1; // Just capture call data
    _eval_kill_data.pid = 0;
    _eval_kill_data.sig = 0;

    EVAL_CATCH( sd_terminaProcessamento(p) );
    if ( 1 != _eval_env.stat ) {
        eval_error( "(SD16) Should have terminated with exit()");
    }

    if ( 1 != _eval_kill_data.status )
        eval_error( "(SD16) kill() should have been called exactly once");
    
    if ( 0xD840 != _eval_kill_data.pid )
        eval_error( "(SD16) kill() called with wrong PID");

    if ( SIGTERM != _eval_kill_data.sig )
        eval_error( "(SD16) kill() called with wrong signal");

    eval_check_successlog("(SD16) Fim Passagem","SD16");

    eval_close_logs( "(SD16)" );
}

/**
 * Evaluate SD17
 **/
void eval_sd17( ) {
    printf("\n");
    eval_info("Evaluating SD17 ...");

    for( int i = 0; i < NUM_PASSAGENS; i++) {
        lista_passagens[i].tipo_passagem = -1;
        lista_passagens[i].pid_cliente = -1;
    }

    Passagem p = {
        .tipo_passagem = 2,
        .matricula = "21-36-EN",
        .lanco = "Lisboa-Porto",
        .pid_cliente = 0xD840
    };

    const int idx = NUM_PASSAGENS / 2;
    lista_passagens[idx] = p;

    indice_lista = idx;

    _eval_kill_data.status = 0;
    _eval_kill_data.action = 1; // Just capture call data
    _eval_kill_data.pid = 0;
    _eval_kill_data.sig = 0;

    EVAL_CATCH( sd_trataSinalSIGTERM( SIGTERM ) );
    if ( 1 != _eval_env.stat ) {
        eval_error( "(SD17) Should have terminated with exit()");
    }

    if ( 1 != _eval_kill_data.status )
        eval_error( "(SD17) kill() should have been called exactly once");
    
    if ( 0xD840 != _eval_kill_data.pid )
        eval_error( "(SD17) kill() called with wrong PID");

    if ( SIGHUP != _eval_kill_data.sig )
        eval_error( "(SD17) kill() called with wrong signal");

    eval_check_successlog("(SD17) Processamento","SD17");


    eval_close_logs( "(SD17)" );
}

int main() {

    initlog( &_success_log );
    initlog( &_error_log );

    eval_info(" %s/servidor.c\n", TOSTRING( _EVAL ) );

    eval_s1(); 
    eval_s2();
    eval_s3();
    eval_s4();
    eval_s5();
    eval_s6();
    eval_s7();
    eval_s8();
    eval_s9();
    eval_s10();
    eval_s11();
    eval_s12();

    eval_sd13();
    eval_sd14();
    eval_sd15();
    eval_sd16();
    eval_sd17();

    printf("\n");
    eval_info("Finished." );

}
