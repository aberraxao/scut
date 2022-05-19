#define _SERVIDOR 1
#include "common.h"
#include "utils.h"

#include <sys/stat.h>
#include <fcntl.h>

/**
 * Undefine the replacement macros defined in eval.h
 **/
#include "eval-undef.h"

question_t questions[] = {
    {"1.1",   "Ligar à memória partilhada (existente)", 0.0 },
    {"1.1.1", "Imprime mensagem sucesso", 0.0 },
    {"1.2",   "Ligar à memória partilhada (inexistente)", 0.0 },
    {"1.3",   "Ligar à memória partilhada (outro erro)", 0.0 },
    {"1.3.1", "Imprime mensagem erro", 0.0 },

    {"2.1",   "Cria memória partilhada", 0.0 },
    {"2.1.1", "Imprime mensagem sucesso", 0.0 },
    {"2.2",   "Inicializa lista de passagens", 0.0 },
    {"2.2.1", "Imprime mensagem sucesso", 0.0 },
    {"2.3a",  "Carrega ficheiro FILE_STATS ", 0.0 },
    {"2.3a.1","Imprime mensagem sucesso", 0.0 },
    {"2.3b",  "Inicializa estatísticas", 0.0 },
    {"2.3b.1","Imprime mensagem sucesso", 0.0 },

    {"3.1",   "Cria Fila mensagens", 0.0 },
    {"3.2",   "Arma sinal SIGINT", 0.0 },
    {"3.3",   "Ignora sinal SIGCHLD", 0.0 },
    {"3.4",   "Apaga fila mensagens existente e cria nova", 0.0 },

    {"4.1",   "Recebe mensagem corretamente", 0.0 },
    {"4.1.1", "Imprime mensagem sucesso", 0.0 },
    {"4.2",   "Recebe mensagem (action inválida)", 0.0 },
    {"4.2.1", "Imprime mensagem erro", 0.0 },
    {"4.3",   "Recebe mensagem (erro)", 0.0 },
    {"4.3.1", "Imprime mensagem erro", 0.0 },

    {"5.1",   "Cria servidor dedicado (child)", 0.0 },
    {"5.2",   "Cria servidor dedicado (parent)", 0.0 },
    {"5.2.1", "Imprime mensagem sucesso", 0.0},
    {"5.3",   "Cria servidor dedicado (erro)", 0.0 },

    {"6.1",   "Shutdown servidores dedicados", 0.0 },
    {"6.2",   "Grava ficheiro FILE_STATS", 0.0 },
    {"6.3",   "Fecha IPCs e termina o servidor", 0.0 },

    {"7.1",   "Arma sinal SIGHUP", 0.0 },
    {"7.2",   "Ignora sinal SIGINT", 0.0 },
    {"7.3",   "Imprime mensagem sucesso", 0.0 },

    {"8.1",   "Valida pedido (ok)", 0.0 },
    {"8.1.1", "Imprime mensagem sucesso", 0.0 },
    {"8.2",   "Valida pedido (tipo errado)", 0.0 },
    {"8.2.1", "Imprime mensagem erro", 0.0 },
    {"8.3",   "Valida pedido (lanço errado)", 0.0 },
    {"8.3.1", "Imprime mensagem erro", 0.0 },
    {"8.4",   "Valida pedido (matrícula errada)", 0.0 },
    {"8.4.1", "Imprime mensagem erro", 0.0 },
    {"8.5",   "Valida pedido (lanço errado + bad pid)", 0.0 },
    {"8.5.1", "Imprime mensagem erro", 0.0 },

    {"9.1",   "Reserva entrada (Normal)", 0.0 },
    {"9.1.1", "Imprime mensagem sucesso", 0.0 },
    {"9.2",   "Reserva entrada (Via Verde)", 0.0 },
    {"9.2.1", "Imprime mensagem sucesso", 0.0 },
    {"9.3",   "Reserva entrada (Lista Cheia)", 0.0 },
    {"9.3.1", "Imprime mensagem erro", 0.0 },

    {"10.1",  "Inicia processamento corretamente", 0.0},
    {"10.1.1", "Imprime mensagem sucesso", 0.0 },

    {"11.1",  "Aguarda alguns segundos", 0.0},
    {"11.1.1", "Imprime mensagem sucesso", 0.0 },

    {"12.1",  "Envia mensagem corretamente", 0.0},
    {"12.2",  "Apaga a entrada da BD", 0.0},
    {"12.3",  "Termina o processo Servidor Dedicado", 0.0},
    {"12.3.1", "Imprime mensagem sucesso", 0.0 },
    {"12.4",  "Verifica apagaEntradaBD()", 0.0},

    {"13.1",  "Envia mensagem corretamente", 0.0},
    {"13.2",  "Apaga a entrada da BD", 0.0},
    {"13.3",  "Termina o processo Servidor Dedicado", 0.0},
    {"13.3.1", "Imprime mensagem sucesso", 0.0 },

    {"14.1",  "Cria conjunto semáforos", 0.0},
    {"14.2",  "Sincronização estatísticas A", 0.0},
    {"14.3",  "Sincronização base de dados e estatísticas B (normal)", 0.0},
    {"14.4",  "Sincronização base de dados e estatísticas B (cheia)", 0.0},
    {"14.5",  "Fecha conjunto semáforos", 0.0},

    {"---", "_end_",0.0}
};

/* Variáveis globais */
int shmId;                                              // Variável que tem o ID da Shared Memory
int msgId;                                              // Variável que tem o ID da Message Queue
int semId;                                              // Variável que tem o ID do Array de Semáforos
Mensagem mensagem;                                      // Variável que tem o pedido enviado do Cliente para o Servidor
DadosServidor *dadosServidor;                           // Variável que vai ficar com a memória partilhada
int indice_lista;                                       // Índice corrente da Lista, que foi reservado pela função reservaEntradaBD()


DadosServidor localDadosServidor;

struct {
    int action;
    int ret;
} _shmGet_data;

int shmGet() {
    _shmGet_data.ret = 0;
    _shmGet_data.ret = _student_shmGet();
    return _shmGet_data.ret;
}

/**
 * Evaluate S1
 **/
int eval_s1( ) {
    eval_info("Evaluating S1.1 - %s...", question_text(questions,"1.1"));
    eval_reset_stats();

    _eval_shmget_data.status = 0;
    _eval_shmget_data.action = 1;

    _eval_shmget_data.key = 0;
    _eval_shmget_data.size = 0;
    _eval_shmget_data.shmflg = 0xFFFF;

    _eval_shmget_data.shmid = 0xD840;

    _eval_shmat_data.status = 0;
    _eval_shmat_data.action = 1;
    _eval_shmat_data.shmid = -1;
    _eval_shmat_data.shmaddr = &localDadosServidor;
    _eval_shmat_data.shmflg = 0xFFFF;

    EVAL_CATCH( shmGet() );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S1) bad termination");
    }

    if ( 1 != _eval_shmget_data.status ) {
        eval_error("(S1) shmget() should have been called exactly once, was called %d times.", _eval_shmget_data.status );
    } else {
        if ( IPC_KEY != _eval_shmget_data.key ) {
            eval_error("(S1) shmget() called with invalid key: Expected 0x%x, got 0x%x", IPC_KEY, _eval_shmget_data.key);
        }
        if ( sizeof(DadosServidor) < _eval_shmget_data.size ) {
            eval_error("(S1) shmget() called with invalid size: Expected %d (max), got %d", sizeof(DadosServidor), _eval_shmget_data.size);
        }
        if ( 0 != _eval_shmget_data.shmflg ) {
            eval_error("(S1) shmget() called with invalid shmflg: Expected 0, got %d", _eval_shmget_data.shmflg);
        }
    }

    if ( 1 != _eval_shmat_data.status ) {
        eval_error("(S1) shmat() should have been called exactly once, was called %d times.", _eval_shmat_data.status );
    } else {
        if ( 0xD840 != _eval_shmat_data.shmid) {
            eval_error("(S1) shmat called with wrong shmid");
        }
        if ( NULL != _eval_shmat_data.shmaddr ) {
            eval_error("(S1) shmat called with wrong shmaddr");
        }
        if ( 0 != _eval_shmat_data.shmflg ) {
            eval_error("(S1) shmat called with wrong shmflg");
        }

        if ( dadosServidor != &localDadosServidor ) {
            eval_error("(S1) dadosServidor not initialized properly");
        }
    }

    if ( ! _shmGet_data.ret ) {
        eval_error("(S1) invalid return value");
    }

    eval_check_successlog( "S1", "(S1) Abri Shared Memory já existente com ID");
    eval_close_logs( "(S1)" );

    eval_info("Evaluating S1.2 - %s...", question_text(questions,"1.2"));

    _eval_shmget_data.status = 0;
    _eval_shmget_data.action = 2;
    errno = ENOENT;

    EVAL_CATCH( shmGet() );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S1) bad termination");
    }

    eval_info("Evaluating S1.3 - %s...", question_text(questions,"1.3"));

    _eval_shmget_data.status = 0;
    _eval_shmget_data.action = 2;
    errno = -1;

    EVAL_CATCH( shmGet() );
    if ( 1 != _eval_env.stat ) {
        eval_error( "(S1) should have been terminated by exit()");
    } else {
        if ( -1 != _eval_exit_data.status ) {
            eval_error( "(S1) Invalid exit status");
        }
    }

    eval_check_errorlog("S1", "(S1)");

    eval_close_logs( "(S1)" );
    return eval_complete("(S1)");
}


struct {
    int action;
    int ret;
} _shmCreateAndInit_data;

int shmCreateAndInit() {
    _shmCreateAndInit_data.ret = 0;
    _shmCreateAndInit_data.ret = _student_shmCreateAndInit();
    return _shmCreateAndInit_data.ret;
}

struct {
    int action;
    int status;
    int ret;

    Contadores* pStats;
} _loadStats_data;

int loadStats( Contadores* pStats ) {

    _loadStats_data.status++ ;
    _loadStats_data.pStats = pStats;

    if ( _loadStats_data.action == 0 ) {
        _loadStats_data.ret = _student_loadStats( pStats );
    } else {
        _loadStats_data.ret = 0;
    }
    
    return _loadStats_data.ret;
}


/**
 * Evaluate S2
 **/
int eval_s2( ) {
    eval_info("Evaluating S2.1 - %s...", question_text(questions,"2.1"));
    eval_reset_stats();
    
    for( int i = 0; i < NUM_PASSAGENS; i++ ) {
        localDadosServidor.lista_passagens[i].tipo_passagem = 0xFFFF;
    }

    _eval_shmget_data.status = 0;
    _eval_shmget_data.action = 1;

    _eval_shmget_data.key = 0;
    _eval_shmget_data.size = 0;
    _eval_shmget_data.shmflg = 0xFFFF;

    _eval_shmget_data.shmid = 0xD840;

    _eval_shmat_data.status = 0;
    _eval_shmat_data.action = 1;
    _eval_shmat_data.shmid = -1;
    _eval_shmat_data.shmaddr = &localDadosServidor;
    _eval_shmat_data.shmflg = 0xFFFF;

    _loadStats_data.status = 0;
    _loadStats_data.action = 1; // ignore loadStats

    EVAL_CATCH( shmCreateAndInit() );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S1) bad termination");
    }

    if ( 1 != _eval_shmget_data.status ) {
        eval_error("(S2) shmget() should have been called exactly once, was called %d times.", _eval_shmget_data.status );
    } else {
        if ( IPC_KEY != _eval_shmget_data.key ) {
            eval_error("(S2) shmget() called with invalid key.");
        }
        if ( sizeof(DadosServidor) != _eval_shmget_data.size ) {
            eval_error("(S2) shmget() called with invalid size.");
        }
        if ( ! ( 0600 | IPC_CREAT | _eval_shmget_data.shmflg ) ) {
            eval_error("(S2) shmget() called with invalid shmflg.");
        }
    }

    if ( 1 != _eval_shmat_data.status ) {
        eval_error("(S2) shmat() should have been called exactly once, was called %d times.", _eval_shmat_data.status );
    } else {
        if ( 0xD840 != _eval_shmat_data.shmid) {
            eval_error("(S2) shmat called with wrong shmid");
        }
        if ( NULL != _eval_shmat_data.shmaddr ) {
            eval_error("(S2) shmat called with wrong shmaddr");
        }
        if ( 0 != _eval_shmat_data.shmflg ) {
            eval_error("(S2) shmat called with wrong shmflg");
        }

        if ( dadosServidor != &localDadosServidor ) {
            eval_error("(S2) dadosServidor not initialized properly");
        }
    }

    if ( 0xD840 != _shmCreateAndInit_data.ret ) {
        eval_error("(S2) invalid return value");
    }

    eval_check_successlog("S2", "(S2.1) Criei Shared Memory");

    eval_info("Evaluating S2.2 - %s...", question_text(questions,"2.2"));

    int err = 0;
    for( int i = 0; i < NUM_PASSAGENS; i++ ) {
        if ( localDadosServidor.lista_passagens[i].tipo_passagem != -1 ) {
            err = 1;
            break;
        }
    }

    if ( err ) {
        eval_error("(S2) dadosServidor->lista_passagens not initialized properly");
    }

    eval_check_successlog("S2", "(S2.2) Iniciei Shared Memory Passagens" );

    eval_close_logs( "(S2) - debug" );


    eval_info("Evaluating S2.3a - %s...", question_text(questions,"2.3a"));

    if ( 1 != _loadStats_data.status ) {
        eval_error("(S2.3) loadStats() should have been called exactly once, was called %d times.", _loadStats_data.status );
    } else if ( _loadStats_data.pStats != &localDadosServidor.contadores ) {
        eval_error("(S2.3) loadStats() called with wrong pointer");
    }

    _loadStats_data.action = 0;

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
        return _eval_stats.error;
    }

    s.contadorNormal = -1;
    s.contadorViaVerde = -1;
    s.contadorAnomalias = -1;

    EVAL_CATCH( loadStats( &s ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S2) bad termination");
    }

    if ( (1 != s.contadorNormal) || (2 != s.contadorViaVerde) || (3 != s.contadorAnomalias) ) {
        eval_error("(S2) Invalid stats loaded");
    }
    
    eval_check_successlog( "S2", "(S2.3) Estatísticas Carregadas" );

    unlink( FILE_STATS );

    eval_close_logs( "(S2)" );

    // Test with missing file
    eval_info("Evaluating S2.3b - %s...", question_text(questions,"2.3b"));

    s.contadorNormal = -1;
    s.contadorViaVerde = -1;
    s.contadorAnomalias = -1;

    EVAL_CATCH( loadStats( &s ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S2) bad termination");
    }

    if ( (0 != s.contadorNormal) || (0 != s.contadorViaVerde) || (0 != s.contadorAnomalias) ) {
        eval_error("(S2) Invalid stats initialized");
    } else {
        question_setgrade( questions,"2.2", 1.0f );
    }
    
    eval_check_successlog( "S2", "(S2.3) Estatísticas Iniciadas" );

    eval_close_logs( "(S2)" );
    return eval_complete("(S2)");
}


int createIPC() {
    return _student_createIPC();
}


/**
 * Evaluate S3
 **/
int eval_s3( ) {
    eval_info("Evaluating S3.1 - %s...", question_text(questions,"3.1"));
    eval_reset_stats();

    _eval_msgget_data.status = 0;
    _eval_msgget_data.action = 3;
    _eval_msgget_data.msqid = 0xD840;

    _eval_msgctl_data.status = 0;
    _eval_msgctl_data.action = 1;
    _eval_msgctl_data.msqid = -1;
    _eval_msgctl_data.cmd = -1;
    _eval_msgctl_data.buf = NULL;

    _eval_signal_data.status = 0;
    _eval_signal_data.action = 2;

    _eval_sigaction_data.status = 0;
    _eval_sigaction_data.action = 2;

    // These are needed because of SD14
    _eval_semget_data.status = 0;
    _eval_semget_data.action = 1;

    _eval_semctl_data.status = 0;
    _eval_semctl_data.action = 1;
    _eval_semget_data.semid = 0xFFFF;

    initlog(&_data_log);

    EVAL_CATCH( createIPC() );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S3) bad termination");
    }

    if ( IPC_KEY != _eval_msgget_data.key ) {
        eval_error( "(S3) invalid key on msgget() call");
    }

    if ( !((IPC_CREAT|0600) & _eval_msgget_data.msgflg ) ) {
        eval_error("(S3) invalid msgflg on msgget() call");
    }

    eval_info("Evaluating S3.2 - %s...", question_text(questions,"3.2"));
    if ( 0 > findinlog( &_data_log, "signal,%d,%p", SIGINT, trataSinalSIGINT ))
        eval_error("(S5) SIGINT not armed properly");

    eval_info("Evaluating S3.3 - %s...", question_text(questions,"3.3"));
    if ( 0 > findinlog( &_data_log, "signal,%d,%p", SIGCHLD, SIG_IGN ))
        eval_error("(S5) SIGCHLD not armed properly");

    eval_check_successlog("S3", "(S3) Criei mecanismos IPC" );

    eval_info("Evaluating S3.4 - %s...", question_text(questions,"3.4"));

    _eval_msgget_data.status = 0;
    _eval_msgget_data.action = 1;
    _eval_msgget_data.msqid = 0xD840;

    _eval_msgctl_data.status = 0;
    _eval_msgctl_data.action = 1;
    _eval_msgctl_data.msqid = -1;
    _eval_msgctl_data.cmd = -1;
    _eval_msgctl_data.buf = NULL;

    _eval_signal_data.status = 0;
    _eval_signal_data.action = 1;

    _eval_sigaction_data.status = 0;
    _eval_sigaction_data.action = 1;

    EVAL_CATCH( createIPC() );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(S3) bad termination");
    }

    if ( 1 != _eval_msgctl_data.status ) {
        eval_error("(S3) msgctl() should have been called exactly once, was called %d times.", _eval_msgctl_data.status );
    } else {
        if ( 0xD840 != _eval_msgctl_data.msqid ) {
            eval_error("(S3) Invalid msqid in msgctl() call");
        }

        if ( IPC_RMID != _eval_msgctl_data.cmd ) {
            eval_error("(S3) Invalid cmd in msgctl() call");
        }
    }

    if ( IPC_KEY != _eval_msgget_data.key ) {
        eval_error( "(S3) invalid key on msgget() call");
    }

    if ( !((IPC_CREAT|0600) & _eval_msgget_data.msgflg ) ) {
        eval_error("(S3) invalid msgflg on msgget() call");
    }

    eval_check_successlog("S3", "(S3) Criei mecanismos IPC" );
    
    eval_close_logs( "(S3)" );
    return eval_complete("(S3)");
}

struct {
    int action;
    Mensagem ret;

} _recebePedido_data;

Mensagem recebePedido() {
    Mensagem m = {};
    _recebePedido_data.ret = m;
    _recebePedido_data.ret = _student_recebePedido();
    return _recebePedido_data.ret;
}

/**
 * Evaluate S4
 **/
int eval_s4( ) {
    eval_info("Evaluating S4.1 - %s...", question_text(questions,"4.1"));
    eval_reset_stats();

    const Passagem p = {
        .tipo_passagem = 2,
        .matricula = "21-36-EN",
        .lanco = "Lisboa-Porto",
        .pid_cliente = 0xD840
    };

    Mensagem snd;
    snd.tipoMensagem = 1;
    snd.conteudo.action = 1;
    snd.conteudo.dados.pedido_cliente = p;

    _eval_msgrcv_data.status = 0;
    _eval_msgrcv_data.action = 2; // inject

    _eval_msgrcv_data.msqid = -1;
    _eval_msgrcv_data.msgtyp = -1;
    _eval_msgrcv_data.msgflg = -1;

    _eval_msgrcv_data.msgp = &snd;
    _eval_msgrcv_data.msgsz = sizeof(snd.conteudo);

    msgId = 0x1234;

    EVAL_CATCH( recebePedido() );

    if ( 0 != _eval_env.stat ) {
        eval_error("(S4) bad termination");
    }
    
    if ( 1 != _eval_msgrcv_data.status ) {
        eval_error("(S4) msgrcv() should have been called exactly once, was called %d times.", _eval_msgrcv_data.status );
    } else {
        if ( 0x1234 != _eval_msgrcv_data.msqid ) {
            eval_error("(S4) Invalid msqid in msgrcv() call.");
        }
        if ( sizeof( snd.conteudo ) != _eval_msgrcv_data.msgsz ) {
            eval_error("(S4) Invalid msgsz in msgrcv() call.");
        }
        if ( 1 != _eval_msgrcv_data.msgtyp ) {
            eval_error("(S4) Invalid msgtyp in msgrcv() call.");
        }
        if ( 0 != _eval_msgrcv_data.msgflg ) {
            eval_error("(S4) Invalid msgflg in msgrcv() call.");
        }

        Mensagem rcv = _recebePedido_data.ret;
        if ( ( rcv.tipoMensagem != snd.tipoMensagem ) ||
                ( rcv.conteudo.action != snd.conteudo.action) ||
                ( rcv.conteudo.dados.pedido_cliente.tipo_passagem != snd.conteudo.dados.pedido_cliente.tipo_passagem ) ||
                ( rcv.conteudo.dados.pedido_cliente.pid_cliente != snd.conteudo.dados.pedido_cliente.pid_cliente ) ||
                ( strncmp( rcv.conteudo.dados.pedido_cliente.matricula, snd.conteudo.dados.pedido_cliente.matricula , 9 ) ) ||
                ( strncmp( rcv.conteudo.dados.pedido_cliente.lanco, snd.conteudo.dados.pedido_cliente.lanco, 50 ) )
        ) {
            eval_error("(S4) A mensagem devolvida não corresponde à mensagem enviada");
        }
    }

    eval_check_successlog("S4", "(S4) Li Pedido do Cliente" );

    eval_info("Evaluating S4.2 - %s...", question_text(questions,"4.2"));

    snd.tipoMensagem = 0xFFFF;
    snd.conteudo.action = -1;
    _eval_msgrcv_data.msgp = &snd;
    EVAL_CATCH( recebePedido() );

    if ( 1 != _eval_env.stat ) {
        eval_error("(S4) should have called exit()");
    } else {
        if ( -1 != _eval_exit_data.status ) {
            eval_error("(S4) Invalid exit status");
        }
    }
    eval_check_errorlog( "S4","(S4)");

    eval_info("Evaluating S4.3 - %s...", question_text(questions,"4.3"));

    _eval_msgrcv_data.action = 3;
    EVAL_CATCH( recebePedido() );

    if ( 1 != _eval_env.stat ) {
        eval_error("(S4) should have called exit()");
    } else {
        if ( -1 != _eval_exit_data.status ) {
            eval_error("(S4) Invalid exit status");
        }
    }
    eval_check_errorlog( "S4","(S4)");

    eval_close_logs( "(S4)" );
    return eval_complete("(S4)");
}

struct {
    int action;
    int pidFilho;
} _criaServidorDedicado_data;

int criaServidorDedicado(  ) {
    _criaServidorDedicado_data.pidFilho = -1;
    _criaServidorDedicado_data.pidFilho = _student_criaServidorDedicado( );
    return _criaServidorDedicado_data.pidFilho;
}

/**
 * Evaluate S5
 **/
int eval_s5( ) {
    eval_info("Evaluating S5.1 - %s...", question_text(questions,"5.1"));
    eval_reset_stats();

    // Testa filho
    _eval_fork_data.status = 0;
    _eval_fork_data.action = 1;

    EVAL_CATCH( criaServidorDedicado() );

    if ( 0 != _eval_env.stat ) {
        eval_error("(S5) bad termination");
    }

    if ( 1 != _eval_fork_data.status ) {
        eval_error("(S5) Fork should have been called exactly once, was called %d times.", _eval_fork_data.status );
    } else {
        if ( 0 != _criaServidorDedicado_data.pidFilho ) {
            eval_error("(S5) Invalid return value");
        }
    }

    // Testa pai
     eval_info("Evaluating S5.2 - %s...", question_text(questions,"5.2"));
    _eval_fork_data.status = 0;
    _eval_fork_data.action = 2;

    EVAL_CATCH( criaServidorDedicado() );

    if ( 0 != _eval_env.stat ) {
        eval_error("(S5) bad termination");
    }

    if ( 1 != _eval_fork_data.status ) {
        eval_error("(S5) Fork should have been called exactly once, was called %d times.", _eval_fork_data.status );
    } else {
        if ( 1 != _criaServidorDedicado_data.pidFilho ) {
            eval_error("(S5) Invalid return value");
        }
    }

    eval_check_successlog( "S5", "(S5) Criado Servidor Dedicado" );

    // Testa error
    eval_info("Evaluating S5.3 - %s...", question_text(questions,"5.3"));
    _eval_fork_data.status = 0;
    _eval_fork_data.action = 3;

    EVAL_CATCH( criaServidorDedicado() );

    if ( 0 != _eval_env.stat ) {
        eval_error("(S5) bad termination");
    }

    if ( 1 != _eval_fork_data.status ) {
        eval_error("(S5) Fork should have been called exactly once, was called %d times.", _eval_fork_data.status );
    } else {
        if ( -1 != _criaServidorDedicado_data.pidFilho ) {
            eval_error("(S5) Invalid return value");
        }
    }

    eval_check_errorlog("S5", "(S5) Fork");

    eval_close_logs( "(S5)" );
    return eval_complete("(S5)");
}


void trataSinalSIGINT( int sinalRecebido ) {
    _student_trataSinalSIGINT( sinalRecebido );
}

/**
 * Evaluate S6
 **/
int eval_s6( ) {
    eval_info("Evaluating S6.1 - %s...", question_text(questions,"6.1"));
    eval_reset_stats();

    for( int i = 0; i < NUM_PASSAGENS; i++ ) {
        localDadosServidor.lista_passagens[i].tipo_passagem = -1;
    }
    const int idx = NUM_PASSAGENS/3;
    localDadosServidor.lista_passagens[idx].tipo_passagem = 1;
    localDadosServidor.lista_passagens[idx].pid_servidor_dedicado = 0xD840;

    localDadosServidor.contadores.contadorNormal = 1;
    localDadosServidor.contadores.contadorViaVerde = 2;
    localDadosServidor.contadores.contadorAnomalias = 3;


    dadosServidor = &localDadosServidor;

    _eval_kill_data.status = 0;
    _eval_kill_data.action = 1;

    _eval_msgctl_data.status = 0;
    _eval_msgctl_data.action = 1;
    _eval_msgctl_data.msqid = -1;
    _eval_msgctl_data.cmd = -1;
    _eval_msgctl_data.buf = NULL;
    msgId = 0xD840;

    _eval_semctl_data.status = 0;
    _eval_semctl_data.action = 1;

    EVAL_CATCH( trataSinalSIGINT( SIGINT ) );

    eval_check_successlog( "S6", "(S6) Shutdown Servidor" );

    if ( 1 != _eval_kill_data.status ) {
        eval_error("(S6) kill() should have been called exactly once, was called %d times.", _eval_kill_data.status );
    } else {
        if ( _eval_kill_data.sig != SIGHUP ) {
            eval_error("(S6) kill() called with invalid signal");
        }
        if ( _eval_kill_data.pid != 0xD840 ) {
            eval_error("(S6) kill() called with invalid pid");
        }
    }

    eval_check_successlog( "S6", "(S6.1) Shutdown Servidores Dedicados" );

    eval_info("Evaluating S6.2 - %s...", question_text(questions,"6.2"));

    Contadores s;
    FILE *f = fopen( FILE_STATS, "r" );
    if ( f ) {
        fread( &s, sizeof(Contadores), 1, f );
        fclose( f );

        if ( s.contadorNormal != localDadosServidor.contadores.contadorNormal ) {
            eval_error( "(S6) Invalid value .contadorNormal in file %s", FILE_STATS );
        }
        if ( s.contadorViaVerde != localDadosServidor.contadores.contadorViaVerde ) {
            eval_error( "(S6) Invalid value .contadorViaVerde in file %s", FILE_STATS );
        }
        if ( s.contadorAnomalias != localDadosServidor.contadores.contadorAnomalias ) {
            eval_error( "(S6) Invalid value .contadorNormal in file %s", FILE_STATS );
        }
    } else {
        eval_error("(S6) Unable to open file %s for reading", FILE_STATS );
    }

    if ( unlink(FILE_STATS) < 0 )
        eval_error("(S6) Unable to remove file %s", FILE_STATS );

    eval_check_successlog( "S6", "(S6.2) Estatísticas Guardadas" );

    eval_info("Evaluating S6.3 - %s...", question_text(questions,"6.3"));

    if ( 1 != _eval_msgctl_data.status ) {
        eval_error("(S6) msgctl() should have been called exactly once, was called %d times.", _eval_msgctl_data.status );
    } else {
        if ( 0xD840 != _eval_msgctl_data.msqid ) {
            eval_error("(S6) Invalid msqid in msgctl() call");
        }

        if ( IPC_RMID != _eval_msgctl_data.cmd ) {
            eval_error("(S6) Invalid cmd in msgctl() call");
        }
    }

    if ( 1 != _eval_env.stat ) {
        eval_error("(S6) Should have called exit()");
    } else {
        if (0 != _eval_exit_data.status ) {
            eval_error("(S6) Invalid exit status");
        }
    }

    eval_check_successlog( "S6", "(S6.3) Shutdown Servidor completo" );

    eval_close_logs( "(S6)" );
    return eval_complete("(S6)");
}

int sd_armaSinais() {
    return _student_sd_armaSinais();
}

/**
 * Evaluate SD7
 **/
int eval_sd7( ) {
   
    eval_info("Evaluating SD7.1 - %s...", question_text(questions,"7.1"));
    eval_reset_stats();

    _eval_signal_data.status = 0;
    _eval_signal_data.action = 2;

    _eval_sigaction_data.status = 0;
    _eval_sigaction_data.action = 2;

    initlog(&_data_log);

    EVAL_CATCH( sd_armaSinais(  ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(SD7) bad termination");
    }

    if ( 0 > findinlog( &_data_log, "signal,%d,%p", SIGHUP, sd_trataSinalSIGHUP ) )
        eval_error("(S7) SIGHUP not armed properly");

    eval_info("Evaluating S7.2 - %s...", question_text(questions,"7.2"));

    if ( 0 > findinlog( &_data_log, "signal,%d,%p", SIGINT, SIG_IGN ))
        eval_error("(S7) SIGINT not armed properly");

    eval_info("Evaluating S7.3 - %s...", question_text(questions,"7.3"));
    eval_check_successlog( "S7", "(SD7) Servidor Dedicado Armei sinais" );

    eval_close_logs( "(SD7)" );
    return eval_complete("(SD7)");
}

int sd_validaPedido( Mensagem pedido ) {
    return _student_sd_validaPedido( pedido );
}

/**
 * Evaluate SD8
 **/
int eval_sd8( ) {
   
    eval_info("Evaluating SD8.1 - %s...", question_text(questions,"8.1"));
    eval_reset_stats();

    Passagem p = {
        .tipo_passagem = 2,
        .matricula = "21-36-EN",
        .lanco = "Lisboa-Porto",
        .pid_cliente = 0xD840
    };

    Mensagem m;
    m.tipoMensagem = 1;
    m.conteudo.action = 1;
    m.conteudo.dados.pedido_cliente = p;

    localDadosServidor.contadores.contadorNormal = 1;
    localDadosServidor.contadores.contadorViaVerde = 2;
    localDadosServidor.contadores.contadorAnomalias = 3;
    dadosServidor = &localDadosServidor;

    // Disable semaphores and messages
    _eval_msgsnd_data.action = 1;
    _eval_msgsnd_data.msgp = NULL;
    _eval_semop_data.action = 1;

    EVAL_CATCH( sd_validaPedido( m ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(SD8) bad termination");
    }

    if ( 1 != localDadosServidor.contadores.contadorNormal ||
         2 != localDadosServidor.contadores.contadorViaVerde || 
         3 != localDadosServidor.contadores.contadorAnomalias ) {
        
        eval_error("(SD8) As estatísticas foram actualizadas indevidamente");
    }

    if ( _eval_msgsnd_data.msgp ) {
        eval_error("(SD8) No message should have been sent");
        free(_eval_msgsnd_data.msgp);
        _eval_msgsnd_data.msgp = NULL;
    }

    eval_check_successlog( "SD8", "(SD8) Chegou novo pedido de passagem do tipo" );


    eval_info("Evaluating SD8.2 - %s...", question_text(questions,"8.2"));
    eval_clear_logs();

    localDadosServidor.contadores.contadorNormal = 1;
    localDadosServidor.contadores.contadorViaVerde = 3;
    localDadosServidor.contadores.contadorAnomalias = 3;


    m.conteudo.dados.pedido_cliente.tipo_passagem = -1;
    EVAL_CATCH( sd_validaPedido( m ) );
    m.conteudo.dados.pedido_cliente.tipo_passagem = 1;

    if ( 1 != _eval_env.stat ) {
        eval_error( "(SD8) should have called exit()");
    } else {
        if ( -1 != _eval_exit_data.status ) {
            eval_error("(SD8) Invalid exit status");
        }
    }

    if ( 1 != localDadosServidor.contadores.contadorNormal ||
         3 != localDadosServidor.contadores.contadorViaVerde || 
         4 != localDadosServidor.contadores.contadorAnomalias ) {
        
        eval_error("(SD8) As estatísticas não foram actualizadas convenientemente");
    }

    if ( _eval_msgsnd_data.msgp ) {
        if ( _eval_msgsnd_data.msgsz != sizeof(m.conteudo) ) {
            eval_error("(SD8) Invalid msgz parameter on msgsnd() call");
        } else {
            Mensagem * snd = _eval_msgsnd_data.msgp;
            if ( snd -> tipoMensagem != 0xD840 ) {
                eval_error( "(SD8) Invalid tipoMensagem" );
            }

            if ( snd -> conteudo.action != 4 ) {
                eval_error( "(SD8) Invalid conteudo.action" );
            }
        }
        free(_eval_msgsnd_data.msgp);
        _eval_msgsnd_data.msgp = NULL;
    } else {
        eval_error("(SD8) No message sent");
    }

    eval_check_errorlog("SD8", "(SD8)");

    eval_info("Evaluating SD8.3 - %s...", question_text(questions,"8.3"));
    eval_clear_logs();

    m.conteudo.dados.pedido_cliente.matricula[0] = 0;
    EVAL_CATCH( sd_validaPedido( m ) );
    m.conteudo.dados.pedido_cliente.matricula[0] = '2';
    if ( 1 != _eval_env.stat ) {
        eval_error( "(SD8) should have called exit()");
    } else {
        if ( -1 != _eval_exit_data.status ) {
            eval_error("(SD8) Invalid exit status");
        }
    }

    if ( 1 != localDadosServidor.contadores.contadorNormal ||
         3 != localDadosServidor.contadores.contadorViaVerde || 
         5 != localDadosServidor.contadores.contadorAnomalias ) {
        
        eval_error("(SD8) As estatísticas não foram actualizadas convenientemente");
    }

    if ( _eval_msgsnd_data.msgp ) {
        if ( _eval_msgsnd_data.msgsz != sizeof(m.conteudo) ) {
            eval_error("(SD8) Invalid msgz parameter on msgsnd() call");
        } else {
            Mensagem * snd = _eval_msgsnd_data.msgp;
            if ( snd -> tipoMensagem != 0xD840 ) {
                eval_error( "(SD8) Invalid tipoMensagem" );
            }

            if ( snd -> conteudo.action != 4 ) {
                eval_error( "(SD8) Invalid conteudo.action" );
            }
        }
        free(_eval_msgsnd_data.msgp);
        _eval_msgsnd_data.msgp = NULL;
    } else {
        eval_error("(SD8) No message sent");
    }

    eval_check_errorlog("SD8", "(SD8)");

    eval_info("Evaluating SD8.4 - %s...", question_text(questions,"8.4"));
    eval_clear_logs();

    m.conteudo.dados.pedido_cliente.lanco[0] = 0;
    EVAL_CATCH( sd_validaPedido( m ) );
    m.conteudo.dados.pedido_cliente.lanco[0] = 'L';
    if ( 1 != _eval_env.stat ) {
        eval_error( "(SD8) should have called exit()");
    } else {
        if ( -1 != _eval_exit_data.status ) {
            eval_error("(SD8) Invalid exit status");
        }
    }

    if ( 1 != localDadosServidor.contadores.contadorNormal ||
         3 != localDadosServidor.contadores.contadorViaVerde || 
         6 != localDadosServidor.contadores.contadorAnomalias ) {
        
        eval_error("(SD8) As estatísticas não foram actualizadas convenientemente");
    }

    if ( _eval_msgsnd_data.msgp ) {
        if ( _eval_msgsnd_data.msgsz != sizeof(m.conteudo) ) {
            eval_error("(SD8) Invalid msgz parameter on msgsnd() call");
        } else {
            Mensagem * snd = _eval_msgsnd_data.msgp;
            if ( snd -> tipoMensagem != 0xD840 ) {
                eval_error( "(SD8) Invalid tipoMensagem" );
            }

            if ( snd -> conteudo.action != 4 ) {
                eval_error( "(SD8) Invalid conteudo.action" );
            }
        }
        free(_eval_msgsnd_data.msgp);
        _eval_msgsnd_data.msgp = NULL;
    } else {
        eval_error("(SD8) No message sent");
    }

    eval_check_errorlog("SD8", "(SD8)");

    eval_info("Evaluating SD8.5 - %s...", question_text(questions,"8.5"));
    eval_clear_logs();

    m.conteudo.dados.pedido_cliente.pid_cliente = -1;

    EVAL_CATCH( sd_validaPedido( m ) );
    if ( 1 != _eval_env.stat ) {
        eval_error( "(SD8) should have called exit()");
    } else {
        if ( -1 != _eval_exit_data.status ) {
            eval_error("(SD8) Invalid exit status");
        }
    }

    if ( 1 != localDadosServidor.contadores.contadorNormal ||
         3 != localDadosServidor.contadores.contadorViaVerde || 
         7 != localDadosServidor.contadores.contadorAnomalias ) {
        
        eval_error("(SD8) As estatísticas não foram actualizadas convenientemente");
    }

    if ( _eval_msgsnd_data.msgp ) {
        eval_error("(SD8) No message should have been sent with invalid .pid_cliente");
        free(_eval_msgsnd_data.msgp);
        _eval_msgsnd_data.msgp = NULL;
    }

    eval_check_errorlog("SD8", "(SD8)");

    eval_close_logs( "(SD8)" );
    return eval_complete("(SD8)");
}

struct {
    int action;
    int ret;
} _sd_reservaEntradaBD_data;

int sd_reservaEntradaBD( DadosServidor* dadosServidor, Mensagem pedido ) {
    _sd_reservaEntradaBD_data.ret = -1;
    _sd_reservaEntradaBD_data.ret = _student_sd_reservaEntradaBD( dadosServidor, pedido );
    return _sd_reservaEntradaBD_data.ret;
}

/**
 * Evaluate SD9
 **/
int eval_sd9( ) {
   
    eval_info("Evaluating SD9.1 - %s...", question_text(questions,"9.1"));
    eval_reset_stats();

    for( int i = 0; i < NUM_PASSAGENS; i++ ) {
        localDadosServidor.lista_passagens[i].tipo_passagem = 2;
    }
    const int idx = NUM_PASSAGENS/3;
    localDadosServidor.lista_passagens[idx].tipo_passagem = -1;
    localDadosServidor.lista_passagens[idx].pid_servidor_dedicado = -1;

    localDadosServidor.contadores.contadorNormal = 1;
    localDadosServidor.contadores.contadorViaVerde = 2;
    localDadosServidor.contadores.contadorAnomalias = 3;

    Passagem p = {
        .tipo_passagem = TIPO_PASSAGEM_NORMAL,
        .matricula = "21-36-EN",
        .lanco = "Lisboa-Porto",
        .pid_cliente = 0xD840
    };

    Mensagem m;
    m.tipoMensagem = 1;
    m.conteudo.action = 1;
    m.conteudo.dados.pedido_cliente = p;

    // Disable semaphores and messages
    _eval_msgsnd_data.action = 1;
    _eval_msgsnd_data.msgp = NULL;
    _eval_semop_data.action = 1;

    EVAL_CATCH( sd_reservaEntradaBD( &localDadosServidor, m ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(SD9) bad termination");
    }

    if ( _eval_msgsnd_data.msgp ) {
        eval_error("(SD9) No message should have been sent");
        free( _eval_msgsnd_data.msgp );
        _eval_msgsnd_data.msgp = NULL;
    }

    if ( idx != _sd_reservaEntradaBD_data.ret ) {
        eval_error("(SD9) Invalid indiceLista returned");
    } else {
        Passagem p2 = localDadosServidor.lista_passagens[idx];

        if ((  p.tipo_passagem != p2.tipo_passagem ) ||
            ( p.pid_cliente != p2.pid_cliente ) ||
            ( strncmp( p.matricula, p2.matricula , 9 ) ) ||
            ( strncmp( p.lanco, p2.lanco, 50 ) )
        ) {
            eval_error("(SD9) A passagem não foi inserida corretamente na BD");
        }

        if ( getpid() != localDadosServidor.lista_passagens[idx].pid_servidor_dedicado )
            eval_error("(SD9) O campo .pid_servidor_dedicado não foi corretamente preenchido. Expected: %d, got %d", getpid(), localDadosServidor.lista_passagens[idx].pid_servidor_dedicado);

        if ( 2 != localDadosServidor.contadores.contadorNormal ||
            2 != localDadosServidor.contadores.contadorViaVerde || 
            3 != localDadosServidor.contadores.contadorAnomalias ) {
            
            eval_error("(SD9) As estatísticas não foram actualizadas convenientemente");
        }

    }

    eval_check_successlog("SD9", "(SD9) Entrada" );

    eval_info("Evaluating SD9.2 - %s...", question_text(questions,"9.2"));

    for( int i = 0; i < NUM_PASSAGENS; i++ ) {
        localDadosServidor.lista_passagens[i].tipo_passagem = 2;
    }
    localDadosServidor.lista_passagens[idx].tipo_passagem = -1;
    localDadosServidor.lista_passagens[idx].pid_servidor_dedicado = -1;

    localDadosServidor.contadores.contadorNormal = 1;
    localDadosServidor.contadores.contadorViaVerde = 2;
    localDadosServidor.contadores.contadorAnomalias = 3;

    p.tipo_passagem = TIPO_PASSAGEM_VIAVERDE;
    m.conteudo.dados.pedido_cliente = p;

    // Disable semaphores and messages
    _eval_msgsnd_data.action = 1;
    _eval_msgsnd_data.msgp = NULL;
    _eval_semop_data.action = 1;

    EVAL_CATCH( sd_reservaEntradaBD( &localDadosServidor, m ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(SD9) bad termination");
    }

    if ( _eval_msgsnd_data.msgp ) {
        eval_error("(SD9) No message should have been sent");
        free( _eval_msgsnd_data.msgp );
        _eval_msgsnd_data.msgp = NULL;
    }

    if ( idx != _sd_reservaEntradaBD_data.ret ) {
        eval_error("(SD9) Invalid indiceLista returned");
    } else {
        Passagem p2 = localDadosServidor.lista_passagens[idx];

        if ((  p.tipo_passagem != p2.tipo_passagem ) ||
            ( p.pid_cliente != p2.pid_cliente ) ||
            ( strncmp( p.matricula, p2.matricula , 9 ) ) ||
            ( strncmp( p.lanco, p2.lanco, 50 ) )
        ) {
            eval_error("(SD9) A passagem não foi inserida corretamente na BD");
        }

        if ( getpid() != localDadosServidor.lista_passagens[idx].pid_servidor_dedicado )
            eval_error("(SD9) O campo .pid_servidor_dedicado não foi corretamente preenchido. Expected: %d, got %d", getpid(), localDadosServidor.lista_passagens[idx].pid_servidor_dedicado);

        if ( 1 != localDadosServidor.contadores.contadorNormal ||
            3 != localDadosServidor.contadores.contadorViaVerde || 
            3 != localDadosServidor.contadores.contadorAnomalias ) {
            
            eval_error("(SD9) As estatísticas não foram actualizadas convenientemente");
        }

    }

    eval_check_successlog( "SD9", "(SD9) Entrada" );

    eval_info("Evaluating SD9.3 - %s...", question_text(questions,"9.3"));

    for( int i = 0; i < NUM_PASSAGENS; i++ ) {
        localDadosServidor.lista_passagens[i].tipo_passagem = 2;
    }

    localDadosServidor.contadores.contadorNormal = 1;
    localDadosServidor.contadores.contadorViaVerde = 2;
    localDadosServidor.contadores.contadorAnomalias = 3;

    // Disable semaphores and messages
    _eval_msgsnd_data.status = 0;
    _eval_msgsnd_data.action = 1;
    _eval_msgsnd_data.msgp = NULL;
    _eval_semop_data.action = 1;

    EVAL_CATCH( sd_reservaEntradaBD( &localDadosServidor, m ) );
    if ( 1 != _eval_env.stat ) {
        eval_error( "(SD9) should have called exit");
    } else {
        if ( -1 != _eval_exit_data.status ) {
            eval_error( "(SD9) Invalid exit status");
        }
    }

    if ( 1 != _eval_msgsnd_data.status ) {
        eval_error("(SD9) msgsnd() should have been called exactly once, was called %d times.", _eval_msgsnd_data.status );
    } else {
        if ( _eval_msgsnd_data.msgflg != 0 ) {
            eval_error("(SD9) Invalid msgflg on msgsnd() call");
        }

        if ( _eval_msgsnd_data.msgsz != sizeof(m.conteudo) ) {
            eval_error("(SD9) Invalid msgsz on msgsnd() call");
        } else {
       
            Mensagem * snd = _eval_msgsnd_data.msgp;

            if ( snd -> tipoMensagem != 0xD840 ) {
                eval_error("(SD9) Invalid .tipoMensagem on msgsnd() call.");
            }

            if ( snd -> conteudo.action != 4 ) {
                eval_error("(SD9) Invalid .conteudo.action.");
            }
        }
    }

    eval_check_errorlog( "SD9", "(SD9) Lista de Passagens cheia" );

    if (_eval_msgsnd_data.msgp) {
        free( _eval_msgsnd_data.msgp );
        _eval_msgsnd_data.msgp = NULL;
    }

    eval_close_logs( "(SD9)" );
    return eval_complete("(SD9)");
}


int sd_iniciaProcessamento( Mensagem pedido ) {
    return _student_sd_iniciaProcessamento( pedido );
}

/**
 * Evaluate SD10
 **/
int eval_sd10( ) {
   
    eval_info("Evaluating SD10.1 - %s...", question_text(questions,"10.1"));
    eval_reset_stats();

    Passagem p = {
        .tipo_passagem = 1,
        .matricula = "21-36-EN",
        .lanco = "Lisboa-Porto",
        .pid_cliente = 0xD840
    };

    Mensagem m;
    m.tipoMensagem = 1;
    m.conteudo.action = 1;
    m.conteudo.dados.pedido_cliente = p;

    _eval_msgsnd_data.status = 0;
    _eval_msgsnd_data.action = 1;
    _eval_msgsnd_data.msgp = NULL;
    _eval_semop_data.action = 1;

    EVAL_CATCH( sd_iniciaProcessamento( m ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(SD10) bad termination");
    }

    if ( 1 != _eval_msgsnd_data.status ) {
        eval_error("(SD10) msgsnd() should have been called exactly once, was called %d times.", _eval_msgsnd_data.status );
    } else {
        if ( msgId != _eval_msgsnd_data.msqid ) {
            eval_error("(SD10) Invalid msqid in msgsnd() call");
        }

        if ( sizeof(m.conteudo) != _eval_msgsnd_data.msgsz ) {
            eval_error("(SD10) Invalid msgsz in msgsnd() call");
        }
        
        if ( _eval_msgsnd_data.msgp ) {
            Mensagem* snd = _eval_msgsnd_data.msgp;

            if ( 0xD840 != snd -> tipoMensagem ) {
                eval_error("(SD10) Invalid tipoMensagem in msgsnd() call");
            }

            if ( 2 != snd -> conteudo.action ) {
                eval_error("(SD10) Invalid conteudo.action in msgsnd() call");
            }
        } else {
            eval_error("(SD10) no message sent");
        }
    }

    eval_check_successlog( "SD10", "(SD10) Início Passagem" );

    if (_eval_msgsnd_data.msgp) {
        free( _eval_msgsnd_data.msgp );
        _eval_msgsnd_data.msgp = NULL;
    }

    eval_close_logs( "(SD10)" );
    return eval_complete("(SD10)");
}

int sd_sleepRandomTime() {
    return _student_sd_sleepRandomTime();
}

/**
 * Evaluate SD11
 **/
int eval_sd11() {
    eval_info("Evaluating SD11.1 - %s...", question_text(questions,"11.1"));
    eval_reset_stats();

    _eval_sleep_data.status = 0;
    _eval_sleep_data.action = 1;

    EVAL_CATCH( sd_sleepRandomTime() );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(SD11) bad termination");
    }


    if ( 1 != _eval_sleep_data.status ) {
        eval_error( "(SD11) sleep() should have been called exactly once, was called %d times.", _eval_sleep_data.status );
    }
    
    if ( (_eval_sleep_data.seconds < MIN_PROCESSAMENTO) ||
         (_eval_sleep_data.seconds > MAX_PROCESSAMENTO) ) {
        eval_error( "(SD11) sleep() called with invalid value (%d)", _eval_sleep_data.seconds );
    }

    eval_check_successlog( "SD11", "(SD11)" );

    eval_close_logs( "(SD11)" );
    return eval_complete("(SD11)");
}

struct {
    int action;
    int status;
    int ret;

    DadosServidor* dadosServidor;
    int indice_lista;
} _apagaEntradaBD_data;

int apagaEntradaBD( DadosServidor* dadosServidor, int indice_lista ) {
    _apagaEntradaBD_data.status ++;
    _apagaEntradaBD_data.dadosServidor = dadosServidor;
    _apagaEntradaBD_data.indice_lista = indice_lista;

    switch ( _apagaEntradaBD_data.action ) {
        case(1):
            _apagaEntradaBD_data.ret = 0;
            break;
        default:
            _apagaEntradaBD_data.ret = -1;
            _apagaEntradaBD_data.ret = _student_apagaEntradaBD( dadosServidor, indice_lista );
    }
    return _apagaEntradaBD_data.ret;
}

int sd_terminaProcessamento( Mensagem pedido ) {
    return _student_sd_terminaProcessamento( pedido );
}

/**
 * Evaluate SD12
 **/
int eval_sd12() {
    eval_info("Evaluating SD12.1 - %s...", question_text(questions,"12.1"));
    eval_reset_stats();

    localDadosServidor.contadores.contadorNormal = 1;
    localDadosServidor.contadores.contadorViaVerde = 2;
    localDadosServidor.contadores.contadorAnomalias = 3;
    dadosServidor = & localDadosServidor;

    indice_lista = NUM_PASSAGENS / 3;

    Passagem p = {
        .tipo_passagem = 1,
        .matricula = "21-36-EN",
        .lanco = "Lisboa-Porto",
        .pid_cliente = 0xD840
    };

    Mensagem m;
    m.tipoMensagem = 1;
    m.conteudo.action = 1;
    m.conteudo.dados.pedido_cliente = p;

    _eval_msgsnd_data.status = 0;
    _eval_msgsnd_data.action = 1;
    _eval_msgsnd_data.msgp = NULL;
    _eval_semop_data.action = 1;

    _apagaEntradaBD_data.status = 0;
    _apagaEntradaBD_data.action = 1;

    EVAL_CATCH( sd_terminaProcessamento( m ) );

    if ( 1 != _eval_msgsnd_data.status ) {
        eval_error("(SD12) msgsnd() should have been called exactly once, was called %d times.", _eval_msgsnd_data.status );
    } else {
        if ( msgId != _eval_msgsnd_data.msqid ) {
            eval_error("(SD12) invalid msqid parameter in msgsnd() call");
        }
        if ( sizeof(m.conteudo) != _eval_msgsnd_data.msgsz ) {
            eval_error("(SD12) invalid msgsz parameter in msgsnd() call");
        }
        if ( 0 != _eval_msgsnd_data.msgflg ) {
            eval_error("(SD12) invalid msgflg parameter in msgsnd() call");
        }

        if ( _eval_msgsnd_data.msgp ) {
            Mensagem* snd = _eval_msgsnd_data.msgp; 
            if ( 0xD840 != snd -> tipoMensagem ) {
                eval_error("(SD12) Invalid tipoMensagem in message");
            }
            if ( 3 != snd -> conteudo.action ) {
                eval_error("(SD12) Invalid conteudo.action in message");
            }
            if ( 1 != snd -> conteudo.dados.contadores_servidor.contadorNormal ||
                 2 != snd -> conteudo.dados.contadores_servidor.contadorViaVerde || 
                 3 != snd -> conteudo.dados.contadores_servidor.contadorAnomalias ) {
                
                eval_error("(SD10) Invalid conteudo.dados.contadores_servidor in msgsnd() call");
            }
  
        } else {
            eval_error("(SD12) no message sent");
        }
    }

    eval_info("Evaluating SD12.2 - %s...", question_text(questions,"12.2"));

    if ( 1 != _apagaEntradaBD_data.status ) {
        eval_error("(SD12) apagaEntradaBD() should have been called exactly once, was called %d times.", _apagaEntradaBD_data.status );
    } else {
        if ( dadosServidor != _apagaEntradaBD_data.dadosServidor ) {
            eval_error("(SD12) Invalid dadosServidor parameter on apagaEntradaBD() call");
        }
        if ( indice_lista != _apagaEntradaBD_data.indice_lista ) {
            eval_error("(SD12) Invalid indice_lista parameter on apagaEntradaBD() call");
        }
    }

    eval_info("Evaluating SD12.3 - %s...", question_text(questions,"12.3"));

    eval_check_successlog( "SD12", "(SD12) Fim Passagem" );

    if ( 1 != _eval_env.stat ) {
        eval_error( "(SD12) should have called exit()");
    } else {
        if ( 0 != _eval_exit_data.status ) {
            eval_error("(SD12) Invalid exit status");
        }
    }

    eval_info("Evaluating SD12.4 - %s...", question_text(questions,"12.4"));
    
    const int idx = 2;
    localDadosServidor.lista_passagens[idx].tipo_passagem = 2;

    _eval_semop_data.action = 1;

    _apagaEntradaBD_data.status = 0;
    _apagaEntradaBD_data.action = 0;

    EVAL_CATCH( apagaEntradaBD( &localDadosServidor, idx ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(SD12) sbad termination");
    } 
    
    if ( -1 != localDadosServidor.lista_passagens[idx].tipo_passagem ) {
        eval_error("(SD12) apagaEntradaBD() not working properly");
    }

    eval_close_logs( "(SD12)" );
    return eval_complete("(SD12)");
}

void sd_trataSinalSIGHUP( int sinalRecebido ) {
    return _student_sd_trataSinalSIGHUP( sinalRecebido );
}

/**
 * Evaluate SD13
 **/
int eval_sd13() {
    eval_info("Evaluating SD13.1 - %s...", question_text(questions,"13.1"));
    eval_reset_stats();

    indice_lista = NUM_PASSAGENS / 3;
    mensagem.conteudo.dados.pedido_cliente.pid_cliente = 0xD840;
    localDadosServidor.lista_passagens[ indice_lista ].pid_cliente = 0xD840;
    dadosServidor = & localDadosServidor;

    _eval_msgsnd_data.status = 0;
    _eval_msgsnd_data.action = 1;
    _eval_msgsnd_data.msgp = NULL;
    _eval_semop_data.action = 1;

    _apagaEntradaBD_data.status = 0;
    _apagaEntradaBD_data.action = 1;

    EVAL_CATCH( sd_trataSinalSIGHUP( SIGHUP ) );

    if ( 1 != _eval_msgsnd_data.status ) {
        eval_error("(SD13) msgsnd() should have been called exactly once, was called %d times.", _eval_msgsnd_data.status );
    } else {
        Mensagem m;
        if ( msgId != _eval_msgsnd_data.msqid ) {
            eval_error("(SD13) invalid msqid parameter in msgsnd() call");
        }
        if ( sizeof(m.conteudo) != _eval_msgsnd_data.msgsz ) {
            eval_error("(SD13) invalid msgsz parameter in msgsnd() call");
        }
        if ( 0 != _eval_msgsnd_data.msgflg ) {
            eval_error("(SD13) invalid msgflg parameter in msgsnd() call");
        }

        if ( _eval_msgsnd_data.msgp ) {
            Mensagem* snd = _eval_msgsnd_data.msgp; 
            if ( 0xD840 != snd -> tipoMensagem ) {
                eval_error("(SD13) Invalid tipoMensagem in message");
            }
            if ( 4 != snd -> conteudo.action ) {
                eval_error("(SD13) Invalid conteudo.action in message");
            }
  
        } else {
            eval_error("(SD13) no message sent");
        }
    }

    eval_info("Evaluating SD13.2 - %s...", question_text(questions,"13.2"));

    if ( 1 != _apagaEntradaBD_data.status ) {
        eval_error("(SD13) apagaEntradaBD() should have been called exactly once, was called %d times.", _apagaEntradaBD_data.status );
    } else {
        if ( dadosServidor != _apagaEntradaBD_data.dadosServidor ) {
            eval_error("(SD13) Invalid dadosServidor parameter on apagaEntradaBD() call");
        }
        if ( indice_lista != _apagaEntradaBD_data.indice_lista ) {
            eval_error("(SD13) Invalid indice_lista parameter on apagaEntradaBD() call");
        }
    }

    eval_info("Evaluating SD13.3 - %s...", question_text(questions,"13.3"));

    eval_check_successlog( "SD13", "(SD13) Processamento Cancelado" );

    if ( 1 != _eval_env.stat ) {
        eval_error( "(SD13) should have called exit()");
    } else {
        if ( 0 != _eval_exit_data.status ) {
            eval_error("(SD13) Invalid exit status");
        }
    }

    eval_close_logs( "(SD13)" );
    return eval_complete("(SD13)");
}


/**
 * Evaluate SD14
 **/
int eval_sd14() {
    eval_info("Evaluating SD14.1 - %s...", question_text(questions,"14.1"));
    eval_reset_stats();

    _eval_msgget_data.status = 0;
    _eval_msgget_data.action = 1;
    _eval_msgget_data.msqid = 0xD840;

    _eval_signal_data.status = 0;
    _eval_signal_data.action = 1;

    _eval_sigaction_data.status = 0;
    _eval_sigaction_data.action = 1;

    _eval_semget_data.status = 0;
    _eval_semget_data.action = 1;
    _eval_semget_data.semid  = 0xABCD;

    _eval_semctl_data.status = 0;
    _eval_semctl_data.action = 1;
    _eval_semctl_data.arg.array = NULL;

    EVAL_CATCH( createIPC( ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(SD14) createIPC() bad termination");
    }

    if ( 1 != _eval_semget_data.status ) {
        eval_error("(SD14) semget() should have been called exactly once, was called %d times.", _eval_semget_data.status );
    } else {
        if ( IPC_KEY != _eval_semget_data.key ) {
            eval_error("(SD14) invalid key parameter in call to semget(): Expected 0x%x, got 0x%x", IPC_KEY, _eval_semget_data.key);
        }
        if ( 2 != _eval_semget_data.nsems ) {
            eval_error("(SD14) invalid nsems parameter in call to semget(): Expected %d, got %d", 2, _eval_semget_data.nsems);
        }
        if ( ! ((IPC_CREAT | 0600) & _eval_semget_data.semflg ) ) {
            eval_error("(SD14) invalid semflg parameter in call to semget(): Expected (IPC_CREAT | 0600)");
        } 
    }

    if ( 0xABCD != semId ) {
        eval_error("(SD14) Global value of semId not set properly: Expected: %d, gpt %d", 0xABCD, semId);
    }

    switch( _eval_semctl_data.status ) {
        case(1):
            if ( SETALL != _eval_semctl_data.cmd ) {
                eval_error("(SD14) Semaphores not initialized properly (invalid command). Expected status SETALL, got %d", _eval_semctl_data.cmd);
            }
            if ( ! _eval_semctl_data.arg.array ) {
                eval_error("(SD14) Invalid array on semctl() call");
            }
            break;
        case(2):
            if ( SETVAL != _eval_semctl_data.cmd ) {
                eval_error("(SD14) Semaphores not initialized properly (invalid command). Expected command SETVAL, got %d", _eval_semctl_data.cmd);
            }
            if ( 1 != _eval_semctl_data.arg.val ) {
                eval_error("(SD14) Invalid val on semctl() call");
            }
            break;
        default:
            eval_error("(SD14) Semaphores not initialized properly. Status received %d", _eval_semctl_data.status);
    }

    eval_info("Evaluating SD14.2 - %s...", question_text(questions,"14.2"));

    Mensagem m;
    m.conteudo.dados.pedido_cliente.tipo_passagem = TIPO_PASSAGEM_INVALIDO;

    semId = 0xABCD;

    initlog(&_data_log);

    _eval_semop_data.status = 0;
    _eval_semop_data.action = 2;

    _eval_msgsnd_data.action = 4;

    EVAL_CATCH( sd_validaPedido( m ) );
    if ( 1 != _eval_env.stat ) {
        eval_error( "(SD14) sd_validaPedido() should have called exit()");
    }

    if ( 2 != _eval_semop_data.status ) {
        eval_error("(SD14) sd_validaPedido(), semop() should have been called twice ");
    } else {

        int lower = findinlog( &_data_log, "semop,%d,%d,%d,%d", semId,SEM_ESTATISTICAS,-1,0);
        int raise = findinlog( &_data_log, "semop,%d,%d,%d,%d", semId,SEM_ESTATISTICAS,+1,0);


        if ( lower < 0 || raise < 0 ) {
            eval_error("(SD14) sd_validaPedido(), semaphores not set properly");
        }
        if ( raise < lower ) {
            eval_error("(SD14) sd_validaPedido(), semaphores not set properly");
        }
    }

    eval_info("Evaluating SD14.3 - %s...", question_text(questions,"14.3"));

    initlog(&_data_log);

    _eval_semop_data.status = 0;
    _eval_semop_data.action = 2;

    _eval_msgsnd_data.action = 4;

    localDadosServidor.lista_passagens[0].tipo_passagem = TIPO_PASSAGEM_INVALIDO;

    EVAL_CATCH( sd_reservaEntradaBD( &localDadosServidor, m ) );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(SD14) sd_reservaEntradaBD() bad termination");
    }

    if ( 4 != _eval_semop_data.status ) {
        eval_error("(SD14) sd_reservaEntradaBD(), semop() should have been called 4 times ");
    } else {
        int lower;
        int raise;
        
        lower = findinlog( &_data_log, "semop,%d,%d,%d,%d", semId,SEM_ESTATISTICAS,-1,0);
        raise = findinlog( &_data_log,"semop,%d,%d,%d,%d", semId,SEM_ESTATISTICAS,+1,0);

        if ( lower < 0 || raise < 0 ) {
            eval_error("(SD14) sd_reservaEntradaBD(), semaphores(SEM_ESTATISTICAS) not set properly");
        }
        if ( raise < lower ) {
            eval_error("(SD14) sd_reservaEntradaBD(), semaphores(SEM_ESTATISTICAS) not set properly");
        }

        lower = findinlog( &_data_log, "semop,%d,%d,%d,%d", semId,SEM_LISTAPASSAGENS,-1,0);
        raise = findinlog( &_data_log, "semop,%d,%d,%d,%d", semId,SEM_LISTAPASSAGENS,+1,0);

        if ( lower < 0 || raise < 0 ) {
            eval_error("(SD14) sd_reservaEntradaBD(), semaphores(SEM_LISTAPASSAGENS) not set properly");
        }
        if ( raise < lower ) {
            eval_error("(SD14) sd_reservaEntradaBD(), semaphores(SEM_LISTAPASSAGENS) not set properly");
        }

    }

    eval_info("Evaluating SD14.4 - %s...", question_text(questions,"14.4"));

    initlog(&_data_log);

    _eval_semop_data.status = 0;
    _eval_semop_data.action = 2;

    _eval_msgsnd_data.action = 4;

    for(int i = 0; i < NUM_PASSAGENS; i++)
        localDadosServidor.lista_passagens[i].tipo_passagem = TIPO_PASSAGEM_VIAVERDE;

    EVAL_CATCH( sd_reservaEntradaBD( &localDadosServidor, m ) );
    if ( 1 != _eval_env.stat ) {
        eval_error( "(SD14) sd_reservaEntradaBD() should have called exit()");
    }

    if ( 4 != _eval_semop_data.status ) {
        eval_error("(SD14) sd_reservaEntradaBD(), semop() should have been called 4 times ");
    } else {
        int lower;
        int raise;
        
        lower = findinlog( &_data_log, "semop,%d,%d,%d,%d", semId,SEM_ESTATISTICAS,-1,0);
        raise = findinlog( &_data_log, "semop,%d,%d,%d,%d", semId,SEM_ESTATISTICAS,+1,0);

        if ( lower < 0 || raise < 0 ) {
            eval_error("(SD14) sd_reservaEntradaBD(), semaphores(SEM_ESTATISTICAS) not set properly");
        }
        if ( raise < lower ) {
            eval_error("(SD14) sd_reservaEntradaBD(), semaphores(SEM_ESTATISTICAS) not set properly");
        }

        lower = findinlog( &_data_log, "semop,%d,%d,%d,%d", semId,SEM_LISTAPASSAGENS,-1,0);
        raise = findinlog( &_data_log, "semop,%d,%d,%d,%d", semId,SEM_LISTAPASSAGENS,+1,0);

        if ( lower < 0 || raise < 0 ) {
            eval_error("(SD14) sd_reservaEntradaBD(), semaphores(SEM_LISTAPASSAGENS) not set properly");
        }
        if ( raise < lower ) {
            eval_error("(SD14) sd_reservaEntradaBD(), semaphores(SEM_LISTAPASSAGENS) not set properly");
        }

    }

    eval_info("Evaluating SD14.5 - %s...", question_text(questions,"14.5"));

    for( int i = 0; i < NUM_PASSAGENS; i++ ) {
        localDadosServidor.lista_passagens[i].tipo_passagem = -1;
    }
    dadosServidor = &localDadosServidor;

    _eval_kill_data.status = 0;
    _eval_kill_data.action = 1;

    _eval_msgctl_data.status = 0;
    _eval_msgctl_data.action = 1;

    _eval_semctl_data.status = 0;
    _eval_semctl_data.action = 1;

    semId = 0x1234;

    EVAL_CATCH( trataSinalSIGINT( SIGINT ) );
    if ( 1 != _eval_env.stat ) {
        eval_error( "(SD14) bad termination");
    }

    if ( 1 > _eval_semctl_data.status ) {
        eval_error("(SD14) semctl() should have been called at least once, was called %d times.", _eval_semctl_data.status );
    } else {
        if ( 0x1234 != _eval_semctl_data.semid ) {
            eval_error("(SD14) bad semId parameter on semctl() call. Expected: %d, got %d", 0x1234, _eval_semctl_data.semid);
        }
        if ( IPC_RMID != _eval_semctl_data.cmd ) {
            eval_error("(SD14) bad cmd parameter on semctl() call.");
        }
    }

    eval_clear_logs();

    eval_close_logs( "(SD14)" );
    return eval_complete("(SD14)");
}

int main(int argc, char *argv[]) {

    initlog( &_success_log );
    initlog( &_error_log );

    eval_info(" %s/servidor.c\n", TOSTRING( _EVAL ) );

    int nerr = 0;

    nerr += eval_s1(); 
    nerr += eval_s2();
    nerr += eval_s3();
    nerr += eval_s4();
    nerr += eval_s5();
    nerr += eval_s6();

    nerr += eval_sd7();
    nerr += eval_sd8();
    nerr += eval_sd9();
    nerr += eval_sd10();
    nerr += eval_sd11();
    nerr += eval_sd12();
    nerr += eval_sd13();


    nerr += eval_sd14();

    eval_info("Finished." );
    _eval_stats.error = nerr;
    eval_complete("servidor");

    /* Print out full evaluation report */
    if ( argc >= 2 ) {
        if ( ! strcmp( argv[1], "-x" ) ) question_export(questions, "servidor" );
        if ( ! strcmp( argv[1], "-l" ) ) question_list(questions, "servidor" );
    }

}