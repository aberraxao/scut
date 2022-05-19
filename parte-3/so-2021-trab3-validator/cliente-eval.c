#define _CLIENTE 1
#include "common.h"

#include <fcntl.h>
#include <sys/stat.h>

/**
 * Undefine the replacement macros defined in eval.h
 **/
#include "eval-undef.h"

// Question list
question_t questions[] = {
    {"1.1",   "Abre fila de mensagens", 0.0 },
    {"1.2",   "Termina cliente em caso de erro", 0.0 },

    {"2.1",   "Obtém dados do utilizador", 0.0 },
    {"2.1.1", "Imprime mensagem sucesso", 0.0 },
    {"2.2",   "Falha com .tipo_passagem inválido", 0.0 },
    {"2.2.1", "Imprime messagem erro", 0.0 },
    {"2.3",   "Falha com .matricula inválida", 0.0 },
    {"2.3.1", "Imprime messagem erro", 0.0 },
    {"2.4",   "Falha com .lanco inválido", 0.0 },
    {"2.4.1", "Imprime messagem erro", 0.0 },

    {"3.1"  , "Envia mensagem corretamente", 0.0 },
    {"3.1.1", "Imprime mensagem sucesso", 0.0 },
    {"3.2"  , "Termina cliente em caso de erro", 0.0 },
    {"3.2.1", "Imprime messagem erro", 0.0 },

    {"4.1"  , "Recebe mensagem corretamente", 0.0 },
    {"4.1.1", "Imprime mensagem sucesso", 0.0 },
    {"4.2"  , "Termina cliente em caso de erro", 0.0 },
    {"4.2.1", "Imprime messagem erro", 0.0 },

    {"5.1"  , "Inicia passagem", 0.0 },
    {"5.1.1", "Imprime mensagem sucesso", 0.0 },

    {"6.1"  , "Pedido concluído (passagem iniciada)", 0.0 },
    {"6.1.1", "Imprime mensagem sucesso", 0.0 },

    {"6.1"  , "Pedido concluído (passagem iniciada)", 0.0 },
    {"6.1.1", "Imprime mensagem sucesso", 0.0 },
    {"6.2"  , "Pedido concluído (passagem não iniciada)", 0.0 },
    {"6.2.1", "Imprime mensagem erro", 0.0 },


    {"7.1"  , "Pedido cancelado", 0.0 },
    {"7.1.1", "Imprime mensagem sucesso", 0.0 },

    {"---", "_end_",0.0}
};


// Global variables for cidadao-student
Mensagem mensagem;                      // Variável que tem o pedido enviado do Cliente para o Servidor
int msgId;                              // Variável que tem o ID da Message Queue
char passagemIniciada = 0;          // Variável que indica que o Servidor já enviou sinal de início da passagem

/**
 * Wrapper for getMsg()
 **/
struct {
    int action;
    int msgId;
} _getMsg_data;

int getMsg() {
    _getMsg_data.msgId = -1;
    _getMsg_data.msgId = _student_getMsg();
    return _getMsg_data.msgId;
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

struct {
    int action;
    Passagem pedido;
    int msgId;
    int ret;
} _enviaPedido_data;

int enviaPedido( Passagem pedido, int msgId ) {
    _enviaPedido_data.pedido = pedido;
    _enviaPedido_data.msgId = msgId;
    _enviaPedido_data.ret = -1;
    _enviaPedido_data.ret = _student_enviaPedido(pedido,msgId);
    return _enviaPedido_data.ret;
}

struct {
    int action;
    int msgId;
    Mensagem ret;
} _recebeMensagem_data;

Mensagem recebeMensagem( int msgId ) {
    _recebeMensagem_data.msgId = msgId;
    Mensagem m = {};
    _recebeMensagem_data.ret = m;
    _recebeMensagem_data.ret = _student_recebeMensagem(msgId);
    return _recebeMensagem_data.ret;
}

void pedidoAck() {
    _student_pedidoAck();
}

void pedidoConcluido( Mensagem mensagem ) {
    _student_pedidoConcluido( mensagem );
}

void pedidoCancelado() {
    _student_pedidoCancelado();
}

/**
 * Evaluate C1
 **/
int eval_c1( ) {
    eval_info("Evaluating C1.1 - %s...", question_text(questions,"1.1"));
    eval_reset_stats();

    _eval_msgget_data.status = 0;
    _eval_msgget_data.action = 1; // Return dummy queue ID
    _eval_msgget_data.msgflg = -1;
    _eval_msgget_data.msqid = 0xD840;

    EVAL_CATCH( getMsg() );
    if ( 0 != _eval_env.stat ) {
        eval_error( "(C1) bad termination");
        printf(" _eval_env.stat = %d\n", _eval_env.stat);
    }

    if ( 1 != _eval_msgget_data.status ) {
        eval_error("(C1) mssget() should have been called exactly once");
    } else {
 
        if( IPC_KEY != _eval_msgget_data.key )
            eval_error( "(C1) invalid IPC key");

        if( 0 != _eval_msgget_data.msgflg )
            eval_error( "(C1) invalid msgflg");
        
        if ( 0xD840 != _eval_msgget_data.ret )
            eval_error( "(C1) invalid value returned");

        eval_check_successlog( "(C1)", "C1" );
    }

    eval_close_logs( "(C1)" );

    eval_info("Evaluating C1.2 - %s...", question_text(questions,"1.2"));

    _eval_msgget_data.status = 0;
    _eval_msgget_data.action = 2; // Return error
    _eval_msgget_data.msgflg = -1;
    _eval_msgget_data.msqid = 0xD840;

    EVAL_CATCH( getMsg() );
    if ( 1 != _eval_env.stat ) {
        eval_error( "(C1.1) should have been terminated by exit");
    } else {
        if ( -1 != _eval_exit_data.status )
            eval_error( "(C1.1) invalid exit status");
    }

    eval_check_errorlog( "C1", "(C1)" );

    eval_close_logs( "(C1)" );
    return eval_complete("(C1)");
}


/**
 * Evaluate C2
 **/
int eval_c2( ) {
    eval_info("Evaluating C2.1 - %s...", question_text(questions,"2.1"));
    eval_reset_stats();

    const int tipo_passagem = 1;
    const char matricula[] = "21-36-EN";
    const char lanco[] = "Lisboa-Porto";

    FILE* ft;
    ft = fopen(FILE_STDIN,"w");
    fprintf(ft,"%d\n", tipo_passagem );
    fprintf(ft,"%s\n", matricula );
    fprintf(ft,"%s\n", lanco );
    for(int i = 0; i < 5; i++ ) fprintf(ft,"\n" );
    fclose(ft);

    // Redirect standard I/O to files
    EVAL_CATCH_IO( getDadosPedidoUtilizador(), FILE_STDIN, FILE_STDOUT );

    if ( 0 != _eval_env.stat ) {
        eval_error( "(C2.1) bad termination");
    }

    // Remove I/O files
    unlink( FILE_STDIN );
    unlink( FILE_STDOUT );

    question_setgrade( questions,"2.1", 1.0f );

    if ( tipo_passagem != _getDadosPedidoUtilizador_data.p.tipo_passagem ) {
        eval_error( "(C2.1) invalid p.tipo_passagem" );
        question_setgrade( questions,"2.1", 0.0f );
    }

    if ( strcmp(matricula,_getDadosPedidoUtilizador_data.p.matricula ) ) {
        eval_error( "(C2.1) invalid p.matricula");
        question_setgrade( questions,"2.1", 0.0f );
    }

    if ( strcmp(lanco,_getDadosPedidoUtilizador_data.p.lanco ) ) {
        eval_error( "(C2.1) invalid p.lanco");
        question_setgrade( questions,"2.1", 0.0f );
    }
    
    if ( getpid() != _getDadosPedidoUtilizador_data.p.pid_cliente ) {
        eval_error( "(C2.1) invalid p.pid_cliente");
        question_setgrade( questions,"2.1", 0.0f );
    }

    if ( eval_check_successlog( "(C2)","(C2) Passagem do tipo" ) )
        question_setgrade( questions,"2.1.1", 1.0f );

    eval_close_logs( "(C2.1)" );

    // Test with bad tipo passagem
    eval_info("Evaluating C2.2 - %s...", question_text(questions,"2.2"));

    ft = fopen(FILE_STDIN,"w");
    fprintf(ft,"%d\n", 5 );
    fprintf(ft,"%s\n", matricula );
    fprintf(ft,"%s\n", lanco );
    for(int i = 0; i < 5; i++ ) fprintf(ft,"\n" );
    fclose(ft);

    // Redirect standard I/O to files
    EVAL_CATCH_IO( getDadosPedidoUtilizador(), FILE_STDIN, FILE_STDOUT );

    if ( 1 != _eval_env.stat ) {
        eval_error( "(C2) Should have finished with exit()");
    } else {
        if ( -1 != _eval_exit_data.status ) {
            eval_error("(C2.2) Invalid exit status");
        }
    }

    if ( _getDadosPedidoUtilizador_data.p.tipo_passagem > 0 ) {
        eval_error( "(C2) Valid dados utilizador from bad input (tipo_passagem)");
    } else {
        question_setgrade( questions,"2.2", 1.0f );
    }
    if( eval_check_errorlog( "C2", "(C2)" ) )
        question_setgrade( questions,"2.2.1", 1.0f );

    // Remove I/O files
    unlink( FILE_STDIN );
    unlink( FILE_STDOUT );


    // Test with bad tipo matricula
    eval_info("Evaluating C2.3 - %s...", question_text(questions,"2.3"));

    ft = fopen(FILE_STDIN,"w");
    fprintf(ft,"%d\n", tipo_passagem );
    fprintf(ft,"\n" );
    fprintf(ft,"%s\n", lanco );
    for(int i = 0; i < 5; i++ ) fprintf(ft,"\n" );
    fclose(ft);

    // Redirect standard I/O to files
    EVAL_CATCH_IO( getDadosPedidoUtilizador(), FILE_STDIN, FILE_STDOUT );

    if ( 1 != _eval_env.stat ) {
        eval_error( "(C2) Should have finished with exit()");
    } else {
        if ( -1 != _eval_exit_data.status ) {
            eval_error("(C2.3) Invalid exit status");
        }
    }

    if ( _getDadosPedidoUtilizador_data.p.tipo_passagem > 0 ) {
        eval_error( "(C2) Valid dados utilizador from bad input (matricula)");
    } else {
        question_setgrade( questions,"2.3", 1.0f );
    }
    if ( eval_check_errorlog( "C2", "(C2)" ) )
        question_setgrade( questions,"2.3.1", 1.0f );


    // Remove I/O files
    unlink( FILE_STDIN );
    unlink( FILE_STDOUT );

    // Test with bad lanco
    eval_info("Evaluating C2.4 - %s...", question_text(questions,"2.4"));

    ft = fopen(FILE_STDIN,"w");
    fprintf(ft,"%d\n", tipo_passagem );
    fprintf(ft,"%s\n", matricula );
    fprintf(ft,"\n" );
    for(int i = 0; i < 5; i++ ) fprintf(ft,"\n" );
    fclose(ft);

    // Redirect standard I/O to files
    EVAL_CATCH_IO( getDadosPedidoUtilizador(), FILE_STDIN, FILE_STDOUT );

    if ( 1 != _eval_env.stat ) {
        eval_error( "(C2) Should have finished with exit()");
    } else {
        if ( -1 != _eval_exit_data.status ) {
            eval_error("(C2.4) Invalid exit status");
        }
    }

    if ( _getDadosPedidoUtilizador_data.p.tipo_passagem > 0 ) {
        eval_error( "(C2) Valid dados utilizador from bad input (lanco)");
    } else {
        question_setgrade( questions,"2.4", 1.0f );
    }
    if ( eval_check_errorlog( "C2", "(C2)" ) )
        question_setgrade( questions,"2.4.1", 1.0f );

    // Remove I/O files
    unlink( FILE_STDIN );
    unlink( FILE_STDOUT );

    eval_close_logs( "(C2)" );
    return eval_complete("(C2)");
}


/**
 * Evaluate C3
 **/
int eval_c3( ) {
    eval_info("Evaluating C3.1 - %s...", question_text(questions,"3.1"));
    eval_reset_stats();

    _eval_msgsnd_data.status = 0;
    _eval_msgsnd_data.action = 1; // capture
    _eval_msgsnd_data.msgp = NULL;

    const Passagem p = {
        .tipo_passagem = 2,
        .matricula = "21-36-EN",
        .lanco = "Lisboa-Porto",
        .pid_cliente = 0xD840
    };

    EVAL_CATCH( enviaPedido( p, 0xD840 ) );

    if ( 0 != _eval_env.stat ) {
        eval_error( "(C3) bad termination");
    } else {
        if ( 1 != _eval_msgsnd_data.status ) {
            eval_error ("(C3) msgsnd() should have been called exactly once.");
        } else {

            if ( 0xD840 != _eval_msgsnd_data.msqid ) {
                eval_error("(C3) argumento msqid inválido no msgsnd()");
            }

            Mensagem *m;
   
            if ( _eval_msgsnd_data.msgsz != sizeof(m -> conteudo) ) {
                eval_error("(C3) argumento msgsz inválido no msgsnd()");
            }

            m = _eval_msgsnd_data.msgp;

            if ( m == NULL ) {
                eval_error("(C3) Invalid msgp in msgsnd()" );
            }


            if ( _eval_msgsnd_data.msgsz == sizeof(m -> conteudo) && m ) {
                if ( 1 != m ->tipoMensagem )
                    eval_error("(C3) .tipoMensagem inválido");
                
                if ( 1 != m -> conteudo.action )
                    eval_error("(C3) .conteudo.action inválido");

                if ( p.tipo_passagem != m -> conteudo.dados.pedido_cliente.tipo_passagem ) {
                    eval_error("(C3) Invalid .tipo_passagem found in file");
                    question_setgrade( questions,"4.1", 0.0f );
                }

                if ( strcmp(p.matricula, m -> conteudo.dados.pedido_cliente.matricula ) ) {
                    eval_error("(C3) Invalid .matricula found in file");
                    question_setgrade( questions,"4.1", 0.0f );
                }

                if ( strcmp(p.lanco, m -> conteudo.dados.pedido_cliente.lanco ) ) {
                    eval_error("(C3) Invalid .lanco found in file");
                    question_setgrade( questions,"4.1", 0.0f );
                }

                if ( p.pid_cliente != m -> conteudo.dados.pedido_cliente.pid_cliente ) {
                    eval_error("(C3) Invalid .pid_cliente found in file");
                    question_setgrade( questions,"4.1", 0.0f );
                }
            }

        }

        eval_check_successlog( "C3", "(C3) Enviei mensagem");
    }

    if ( _eval_msgsnd_data.msgp ) {
        free( _eval_msgsnd_data.msgp );
        _eval_msgsnd_data.msgp = NULL;
    }

    eval_info("Evaluating C3.2 - %s...", question_text(questions,"3.2"));

    _eval_msgsnd_data.status = 0;
    _eval_msgsnd_data.action = 2; // error
    _eval_msgsnd_data.msgp = NULL;

    EVAL_CATCH( enviaPedido( p, 0xD840 ) );

    if ( 1 != _eval_env.stat ) {
        eval_error( "(C3) should have ended with exit()");
    } else {
        if ( -1 != _eval_exit_data.status ) {
            eval_error( "(C3) exit code should be -1");
        }
    }

    eval_check_errorlog( "C3", "C3" );

    eval_close_logs( "(C3)" );
    return eval_complete("(C3)");
}

/**
 * Evaluate C4
 **/
int eval_c4( ) {
    eval_info("Evaluating C4.1 - %s...", question_text(questions,"4.1"));
    eval_reset_stats();

    const Passagem p = {
        .tipo_passagem = 2,
        .matricula = "21-36-EN",
        .lanco = "Lisboa-Porto",
        .pid_cliente = 0xD840
    };

    _eval_msgrcv_data.status = 0;
    _eval_msgrcv_data.action = 2; // inject

    _eval_msgrcv_data.msqid = -1;
    _eval_msgrcv_data.msgtyp = -1;
    _eval_msgrcv_data.msgflg = -1;


    Mensagem snd;
    snd.tipoMensagem = 1;
    snd.conteudo.action = 1;
    snd.conteudo.dados.pedido_cliente = p;

    _eval_msgrcv_data.msgp = &snd;
    _eval_msgrcv_data.msgsz = sizeof(snd.conteudo);

    EVAL_CATCH( recebeMensagem( 0xD840 ) );

    if ( 0 != _eval_env.stat ) {
        eval_error( "(C4) bad termination");
    } else {

        if ( 1 != _eval_msgrcv_data.status  ) {
            eval_error("(C4) msgrcv() should have been called exactly once.");
        } else {
            if ( 0xD840 != _eval_msgrcv_data.msqid ) {
                eval_error("(C4) Argumento msgid inválido no msgrcv()");
            }

            if ( sizeof(snd.conteudo) != _eval_msgrcv_data.msgsz ) {
                eval_error("(C4) Argumento msgsz inválido no msgrcv()");
            }

            if ( getpid() != _eval_msgrcv_data.msgtyp ) {
                eval_error("(C4) Argumento msgtyp inválido no msgrcv()");
            }

            Mensagem rcv = _recebeMensagem_data.ret;
            if ( ( rcv.tipoMensagem != snd.tipoMensagem ) ||
                 ( rcv.conteudo.action != snd.conteudo.action) ||
                 ( rcv.conteudo.dados.pedido_cliente.tipo_passagem != snd.conteudo.dados.pedido_cliente.tipo_passagem ) ||
                 ( rcv.conteudo.dados.pedido_cliente.pid_cliente != snd.conteudo.dados.pedido_cliente.pid_cliente ) ||
                 ( strncmp( rcv.conteudo.dados.pedido_cliente.matricula, snd.conteudo.dados.pedido_cliente.matricula , 9 ) ) ||
                 ( strncmp( rcv.conteudo.dados.pedido_cliente.lanco, snd.conteudo.dados.pedido_cliente.lanco, 50 ) )
            ) {
                eval_error("(C4) A mensagem devolvida não corresponde à mensagem enviada");
            }
        }
    }

    eval_check_successlog("C4", "(C4) Li mensagem do Servidor");

    eval_info("Evaluating C4.2 - %s...", question_text(questions,"4.2"));

    _eval_msgrcv_data.status = 0;
    _eval_msgrcv_data.action = 3; // error

    EVAL_CATCH( recebeMensagem( 0xD840 ) );

    if ( 1 != _eval_env.stat ) {
        eval_error( "(C4) should have ended with exit()");
    } else {
        if ( -1 != _eval_exit_data.status ) {
            eval_error( "(C4) exit code should be -1");
        }
    }

    eval_check_errorlog("C4", "C4");

    eval_close_logs( "(C4)" );
    return eval_complete("(C4)");
}

/**
 * Evaluate C5
 **/
int eval_c5( ) {
    eval_info("Evaluating C5.1 - %s...", question_text(questions,"5.1"));

    passagemIniciada = 0;
    EVAL_CATCH( _student_pedidoAck( ) );

    if ( 0 != _eval_env.stat ) {
        eval_error( "(C5) bad termination");
    } 
    
    if ( 1 != passagemIniciada ) {
        eval_error("(C5)", "passagemIniciada not set to TRUE");
    }

    eval_check_successlog("C5", "(C5) Passagem Iniciada");

    eval_close_logs( "(C5)" );
    return eval_complete("(C5)");
}

/**
 * Evaluate C6
 **/
int eval_c6( ) {
    eval_info("Evaluating C6.1 - %s...", question_text(questions,"6.1"));

    const Contadores c = {
        .contadorNormal = 2,
        .contadorViaVerde = 3,
        .contadorAnomalias = 4
    };

    Mensagem m;
    m.tipoMensagem = 1;
    m.conteudo.action = 3;
    m.conteudo.dados.contadores_servidor = c;

    passagemIniciada = 1;
    EVAL_CATCH( pedidoConcluido( m ) );

    if ( 1 != _eval_env.stat ) {
        eval_error( "(C6) Should have called exit()");
    } else {
        if ( 0 != _eval_exit_data.status ) {
            eval_error("(C6) Bad exit status");
        }
    }

    eval_check_successlog("C6", "(C6) Passagem Concluída com estatísticas: 2 3 4");

    eval_info("Evaluating C6.2 - %s...", question_text(questions,"6.2"));

    passagemIniciada = 0;
    EVAL_CATCH( pedidoConcluido( m ) );

    if ( 1 != _eval_env.stat ) {
        eval_error( "(C6) Should have called exit()");
    } else {
        if ( -1 != _eval_exit_data.status ) {
            eval_error("(C6) Bad exit status");
        }
    }

    eval_check_errorlog("C6", "(C6)" );

    eval_close_logs( "(C6)" );
    return eval_complete("(C6)");
}

/**
 * Evaluate C7
 **/
int eval_c7( ) {
    eval_info("Evaluating C7 - %s...", question_text(questions,"7.1"));


    EVAL_CATCH( pedidoCancelado( ) );

    if ( 1 != _eval_env.stat ) {
        eval_error( "(C7) Should have called exit()");
    } else {
        if ( -1 != _eval_exit_data.status ) {
            eval_error("(C7) Bad exit status");
        }
    }

    eval_check_successlog("C7", "(C7) Processo Não Concluído e Incompleto");

    eval_close_logs( "(C7)" );
    return eval_complete("(C7)");
}

int main(int argc, char *argv[]) {

    initlog( &_success_log );
    initlog( &_error_log );

    eval_info(" %s/cliente.c\n", TOSTRING( _EVAL ) );

    _eval_exit_data.action = 0;

    // abort test whenever pause() is called
    _eval_pause_data.action = 2; 

    int nerr = 0;

    nerr += eval_c1();
    nerr += eval_c2();
    nerr += eval_c3();
    nerr += eval_c4();
    nerr += eval_c5();
    nerr += eval_c6();
    nerr += eval_c7();

    eval_info("Finished." );
    _eval_stats.error = nerr;
    eval_complete("cliente");

    /* Print out full evaluation report */
    if ( argc >= 2 ) {
        if ( ! strcmp( argv[1], "-x" ) ) question_export(questions, "cliente" );
        if ( ! strcmp( argv[1], "-l" ) ) question_list(questions, "cliente" );
    }

}
