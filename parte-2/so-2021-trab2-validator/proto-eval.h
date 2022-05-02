/**
 * Protoypes for the functions implemented by students and their wrappers
 **/

#if defined( _CLIENTE )

int getPidServidor();                   // C1: Função a ser implementada pelos alunos
int armaSinais();                       // C2: Função a ser implementada pelos alunos
Passagem getDadosPedidoUtilizador();    // C3: Função a ser implementada pelos alunos
int escrevePedido(Passagem);            // C4: Função a ser implementada pelos alunos
int configuraTemporizador();            // C5: Função a ser implementada pelos alunos
void trataSinalSIGUSR1(int);            // C6: Função a ser implementada pelos alunos
void trataSinalSIGTERM(int);            // C7: Função a ser implementada pelos alunos
void trataSinalSIGHUP(int);             // C8: Função a ser implementada pelos alunos
void trataSinalSIGINT(int);             // C9: Função a ser implementada pelos alunos
void trataSinalSIGALRM(int);            // C10: Função a ser implementada pelos alunos

int _student_getPidServidor();
int _student_armaSinais();
Passagem _student_getDadosPedidoUtilizador();
int _student_escrevePedido(Passagem);
int _student_configuraTemporizador();
void _student_trataSinalSIGUSR1(int);
void _student_trataSinalSIGTERM(int);
void _student_trataSinalSIGHUP(int);
void _student_trataSinalSIGINT(int);
void _student_trataSinalSIGALRM(int);

#elif defined( _SERVIDOR )

int init(Passagem*);                                // S1:   Função a ser implementada pelos alunos
int loadStats(Contadores*);                         // S2:   Função a ser implementada pelos alunos
int criaFicheiroServidor();                         // S3:   Função a ser implementada pelos alunos
int criaFifo();                                     // S4:   Função a ser implementada pelos alunos
int armaSinais();                                   // S5:   Função a ser implementada pelos alunos
Passagem lePedido();                                // S6:   Função a ser implementada pelos alunos
int validaPedido(Passagem);                         // S7:   Função a ser implementada pelos alunos
int reservaEntradaBD(Passagem*, Passagem);          // S8:   Função a ser implementada pelos alunos
int criaServidorDedicado(Passagem*, int);           // S9:   Função a ser implementada pelos alunos
void trataSinalSIGINT(int);                         // S10:  Função a ser implementada pelos alunos
void trataSinalSIGHUP(int, siginfo_t *, void *);    // S11:  Função a ser implementada pelos alunos
void trataSinalSIGCHLD(int);                        // S12:  Função a ser implementada pelos alunos
int sd_armaSinais();                                // SD13: Função a ser implementada pelos alunos
int sd_iniciaProcessamento(Passagem);               // SD14: Função a ser implementada pelos alunos
int sd_sleepRandomTime();                           // SD15: Função a ser implementada pelos alunos
int sd_terminaProcessamento(Passagem);              // SD16: Função a ser implementada pelos alunos
void sd_trataSinalSIGTERM(int);                     // SD17: Função a ser implementada pelos alunos


int _student_init(Passagem*);
int _student_loadStats(Contadores*);
int _student_criaFicheiroServidor();
int _student_criaFifo();
int _student_armaSinais();
Passagem _student_lePedido();
int _student_validaPedido(Passagem);
int _student_reservaEntradaBD(Passagem*, Passagem);
int _student_criaServidorDedicado(Passagem*, int);
void _student_trataSinalSIGINT(int);
void _student_trataSinalSIGHUP(int, siginfo_t *, void *);
void _student_trataSinalSIGCHLD(int);
int _student_sd_armaSinais();
int _student_sd_iniciaProcessamento(Passagem);
int _student_sd_sleepRandomTime();
int _student_sd_terminaProcessamento(Passagem);
void _student_sd_trataSinalSIGTERM(int);

#else

#error Either _CLIENTE or _SERVIDOR must be defined

#endif