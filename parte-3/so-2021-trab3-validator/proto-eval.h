/**
 * Protoypes for the functions implemented by students and their wrappers
 **/

#if defined( _CLIENTE )

/* Protótipos de funções */
int getMsg();                           // C1: Função a ser implementada pelos alunos
Passagem getDadosPedidoUtilizador();    // C2: Função a ser implementada pelos alunos
int enviaPedido( Passagem, int );       // C3: Função a ser implementada pelos alunos
Mensagem recebeMensagem();              // C4: Função a ser implementada pelos alunos
void pedidoAck();                       // C5: Função a ser implementada pelos alunos
void pedidoConcluido( Mensagem );       // C6: Função a ser implementada pelos alunos
void pedidoCancelado();                 // C7: Função a ser implementada pelos alunos

int _student_getMsg();
Passagem _student_getDadosPedidoUtilizador();
int _student_enviaPedido( Passagem, int );
Mensagem _student_recebeMensagem();
void _student_pedidoAck();
void _student_pedidoConcluido( Mensagem );
void _student_pedidoCancelado();

#elif defined( _SERVIDOR )

/* Protótipos de funções */
int shmGet();                                           // S1:   Função a ser implementada pelos alunos
int shmCreateAndInit();                                 // S2:   Função a ser implementada pelos alunos
int loadStats( Contadores* );                           // S2.3: Função a ser implementada pelos alunos
int createIPC();                                        // S3:   Função a ser implementada pelos alunos
Mensagem recebePedido();                                // S4:   Função a ser implementada pelos alunos
int criaServidorDedicado();                             // S5:   Função a ser implementada pelos alunos
void trataSinalSIGINT( int );                           // S6:   Função a ser implementada pelos alunos
int sd_armaSinais();                                    // SD7:  Função a ser implementada pelos alunos
int sd_validaPedido( Mensagem );                        // SD8:  Função a ser implementada pelos alunos
int sd_reservaEntradaBD( DadosServidor*, Mensagem );    // SD9:  Função a ser implementada pelos alunos
int apagaEntradaBD( DadosServidor*, int );           //       Função a ser implementada pelos alunos
int sd_iniciaProcessamento( Mensagem );                 // SD10: Função a ser implementada pelos alunos
int sd_sleepRandomTime();                               // SD11: Função a ser implementada pelos alunos
int sd_terminaProcessamento( Mensagem );                // SD12: Função a ser implementada pelos alunos
void sd_trataSinalSIGHUP( int );                        // SD13: Função a ser implementada pelos alunos
                                                        // SD14: Função a ser implementada pelos alunos


int _student_shmGet();                                       
int _student_shmCreateAndInit();                             
int _student_loadStats( Contadores* );                       
int _student_createIPC();                                    
Mensagem _student_recebePedido();                            
int _student_criaServidorDedicado();                         
void _student_trataSinalSIGINT( int );                       
int _student_sd_armaSinais();                                
int _student_sd_validaPedido( Mensagem );                    
int _student_sd_reservaEntradaBD( DadosServidor*, Mensagem );
int _student_apagaEntradaBD( DadosServidor*, int );       
int _student_sd_iniciaProcessamento( Mensagem );             
int _student_sd_sleepRandomTime();                           
int _student_sd_terminaProcessamento( Mensagem );            
void _student_sd_trataSinalSIGHUP( int );                    

#else

#error Either _CLIENTE or _SERVIDOR must be defined

#endif