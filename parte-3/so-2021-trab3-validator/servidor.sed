s/int main(/void _student_main(/
s/^int shmId;                   /extern int shmId;                   /
s/^int msgId;                   /extern int msgId;                   /
s/^int semId;                   /extern int semId;                   /
s/^Mensagem mensagem;           /extern Mensagem mensagem;           /
s/^DadosServidor *dadosServidor;/extern DadosServidor *dadosServidor;/
s/^int indice_lista;            /extern int indice_lista;            /
s/int shmGet(/int _student_shmGet(/
s/int shmCreateAndInit(/int _student_shmCreateAndInit(/
s/int loadStats(/int _student_loadStats(/
s/int createIPC(/int _student_createIPC(/
s/Mensagem recebePedido(/Mensagem _student_recebePedido(/
s/int criaServidorDedicado(/int _student_criaServidorDedicado(/
s/void trataSinalSIGINT(/void _student_trataSinalSIGINT(/
s/int sd_armaSinais(/int _student_sd_armaSinais(/
s/int sd_validaPedido(/int _student_sd_validaPedido(/
s/int sd_reservaEntradaBD(/int _student_sd_reservaEntradaBD(/
s/int apagaEntradaBD(/int _student_apagaEntradaBD(/
s/int sd_iniciaProcessamento(/int _student_sd_iniciaProcessamento(/
s/int sd_sleepRandomTime(/int _student_sd_sleepRandomTime(/
s/int sd_terminaProcessamento(/int _student_sd_terminaProcessamento(/
s/void sd_trataSinalSIGHUP(/void _student_sd_trataSinalSIGHUP(/
