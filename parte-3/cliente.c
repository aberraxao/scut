/******************************************************************************
 ** ISCTE-IUL: Trabalho prático 2 de Sistemas Operativos
 **
 ** Aluno: Nº: 103678 Nome: Diana Andreia de Oliveira Amaro
 ** Nome do Módulo: cliente.c v1
 ** Descrição/Explicação do Módulo: 
 **
 **
 ******************************************************************************/
#include "common.h"
#include "utils.h"
// #define DEBUG_MODE FALSE             // To disable debug messages, uncomment this line

/* Variáveis globais */
Mensagem mensagem;                      // Variável que tem o pedido enviado do Cliente para o Servidor
int msgId;                              // Variável que tem o ID da Message Queue
char passagemIniciada = FALSE;          // Variável que indica que o Servidor já enviou sinal de início da passagem

/* Protótipos de funções */
int getMsg();                           // C1: Função a ser implementada pelos alunos
Passagem getDadosPedidoUtilizador();    // C2: Função a ser implementada pelos alunos
int enviaPedido(Passagem, int);       // C3: Função a ser implementada pelos alunos
Mensagem recebeMensagem(int);              // C4: Função a ser implementada pelos alunos
void pedidoAck();                       // C5: Função a ser implementada pelos alunos
void pedidoConcluido(Mensagem);       // C6: Função a ser implementada pelos alunos
void pedidoCancelado();                 // C7: Função a ser implementada pelos alunos

/**
 * Main: Processamento do processo Cliente
 * 
 * @return int Exit value
 */
int main() {    // Os alunos em princípio não deverão alterar esta função
    // C1
    msgId = getMsg();
    // C2
    Passagem pedido = getDadosPedidoUtilizador();
    // C3
    enviaPedido(pedido, msgId);
    // Aguarda processamento por parte do Servidor
    while (TRUE) {
        debug("Aguarda processamento por parte do Servidor");
        // C4
        mensagem = recebeMensagem(msgId);
        switch (mensagem.conteudo.action) {
            // C5
            case ACTION_PEDIDO_ACK:
                pedidoAck();
                break;
                // C6
            case ACTION_PEDIDO_CONCLUIDO:
                pedidoConcluido(mensagem);
                break;
                // C7
            case ACTION_PEDIDO_CANCELADO:
                pedidoCancelado();
        }
    }
}

/**
 *  O módulo Cliente é responsável pelo pedido das passagens. Este módulo será utilizado para solicitar a passagem das viaturas pelas portagens.
 *  Após identificação da viatura, é realizado o pedido da respetiva passagem, ficando este módulo a aguardar até que o processamento esteja concluído.
 *  Assim, definem-se as seguintes tarefas a desenvolver:
 */

/**
 * C1   Tenta abrir uma fila de mensagens (message queue) IPC que tem a KEY IPC_KEY definida em common.h
 *      (alterar esta KEY para ter o valor do nº do aluno, como indicado nas aulas).
 *      Deve assumir que a fila de mensagens já foi criada.
 *      Em caso de erro, dá error C1 "<Problema>" e termina o Cliente com exit status -1. Caso contrário dá success C1 "<msg id>";
 *
 * @return int a msgID IPC desta Message Queue
 */
int getMsg() {
    debug("C1 <");

    // TODO: Remove at the end of the project
    // int id = msgget( IPC_KEY, IPC_CREAT | 0666 );
    // exit_on_error(id, "Erro no msgget");

    int msgId = msgget(IPC_KEY, 0);
    if (msgId < 0) {
        error("C1", "Erro ao obter a mensagem");
        exit(-1);
    } else
        success("C1", "<%d>", msgId);

    debug("C1 >");
    return msgId;
}

/**
 * C2   Pede ao Condutor (utilizador) que preencha os dados referentes à passagem da viatura (Matrícula e Lanço),
 *      criando um elemento do tipo Passagem com essas informações, e preenchendo o valor pid_cliente com o PID do seu próprio processo Cliente.
 *      Em caso de ocorrer qualquer erro, dá error C2 "<Problema>", e termina o processo Cliente;
 *      caso contrário dá success C2 "Passagem do tipo <Normal | Via Verde> solicitado pela viatura com matrícula <matricula> para o Lanço <lanco> e com PID <pid_cliente>";
 *
 * @return Passagem Elemento com os dados preenchidos. Se tipo_passagem = -1, significa que o elemento é inválido
 */
Passagem getDadosPedidoUtilizador() {
    debug("C2 <");
    Passagem p;
    // Por omissão, retorna valor inválido
    p.tipo_passagem = TIPO_PASSAGEM_INVALIDO;

    // Preenche os dados da "Passagem" com os dados fornecidos pelo Cliente
    printf("Preencha o seguinte formulário:\n");
    printf("Tipo de Passagem: 1-Normal, 2-Via Verde:\n");
    char tmp_tipo_passagem[2];
    my_gets(tmp_tipo_passagem, 2);
    p.tipo_passagem = atoi(tmp_tipo_passagem);
    printf("Matrícula:\n");
    my_gets(p.matricula, 9);
    printf("Lanço:\n");
    my_gets(p.lanco, 51);
    // Busca o processo do cliente
    p.pid_cliente = getpid();

    // Efetua validações: Tipo de passagem
    char *tipo_passagem;
    if (p.tipo_passagem == TIPO_PASSAGEM_NORMAL) {
        tipo_passagem = "Normal";
    } else if (p.tipo_passagem == TIPO_PASSAGEM_VIAVERDE) {
        tipo_passagem = "Via Verde";
    } else {
        error("C2", "O Tipo de passagem não é válido");
        exit(-1);
    }
    // Efetua validações: Matrícula
    if (p.matricula[0] == '\0') {
        error("C2", "Matrícula vazia.");
        exit(-1);
    }
    // Efetua validações: Lanço
    if (p.lanco[0] == '\0') {
        error("C2", "Lanço vazio.");
        exit(-1);
    }

    success("C2", "Passagem do tipo %s solicitado pela viatura com matrícula %s para o Lanço %s e com PID %d",
            tipo_passagem, p.matricula, p.lanco, p.pid_cliente);

    debug("C2 >");
    return p;
}

/**
 * C3   Envia as informações do elemento Passagem numa mensagem com o tipo de mensagem 1 e action 1 – Pedido para a message queue.
 *      Em caso de erro na escrita, dá error C3 "<Problema>", e termina o processo Cliente com exit code -1.
 *      Caso contrário, dá success C3 "Enviei mensagem";
 *      --> Preenche a variável global mensagem.
 *
 * @param pedido Passagem a ser enviada
 * @param msgId a msgID IPC desta Message Queue
 * @return int Sucesso
 */
int enviaPedido(Passagem pedido, int msgId) {
    debug("C3 <");

    // Define a mensagem
    Mensagem m;
    m.tipoMensagem = 1;
    m.conteudo.action = 1;
    m.conteudo.dados.pedido_cliente = pedido;

    // Verifica o msgId
    if (msgId < 0) {
        error("C3", "msgId inválido");
        exit(-1);
    }

    // Envia a mensagem
    int status = msgsnd(msgId, &m, sizeof(m.conteudo), 0);
    if (status < 0) {
        error("C3", "Erro ao enviar a mensagem");
        exit(-1);
    } else
        success("C3", "Enviei mensagem");

    debug("C3 >");
    return 0;
}

/**
 * C4   Lê da message queue com msgId uma mensagem cujo tipo de mensagem é igual ao PID deste processo Cliente, 
 *      mensagem essa que poderá vir do Servidor Dedicado. Se houver algum erro dá error C4 "<Problema>"
 *      e termina o Cliente com exit code -1. Caso contrário, dá success C4 "Li mensagem do Servidor".
 *
 * @param msgId a msgID IPC desta Message Queue
 * @return Mensagem a mensagem lida
 */
Mensagem recebeMensagem(int msgId) {
    debug("C4 <");
    Mensagem m;

    // Verifica o msgId
    if (msgId < 0) {
        error("C4", "msgId inválido");
        exit(-1);
    }

    // Lê a mensagem
    int status = msgrcv(msgId, &m, sizeof(m.conteudo), getpid(), 0);
    if (status < 0) {
        error("C4", "Erro ao ler a mensagem");
        exit(-1);
    } else
        success("C4", "Li mensagem do Servidor");

    debug("C4 >");
    return m;
}

/**
 * C5   Se a mensagem que chegou em C4 veio com action 2 – Pedido ACK,
 *      serve para o Servidor Dedicado indicar que o processamento da passagem foi iniciado.
 *      Se o Cliente receber essa mensagem, dá success C5 "Passagem Iniciada", assinala que o processamento iniciou,
 *      e retorna para aguardar a conclusão do processamento do lado do Servidor Dedicado;
 */
void pedidoAck() {
    debug("C5 <");

    passagemIniciada = TRUE;
    success("C5", "Passagem Iniciada");

    debug("C5 >");
}

/**
 * C6   Se a mensagem que chegou em C4 veio com action 3 – Pedido Concluído,
 *      serve para o Servidor Dedicado indicar que o processamento da passagem foi concluído.
 *      Se o Cliente receber essa mensagem, que inclui os contadores de estatística atualizados,
*      dá success C6 "Passagem Concluída com estatísticas: <contador normal> <contador via verde> <contador anomalias>",
 *      e termina o processo Cliente com exit code 0.
 *      ATENÇÂO: Deverá previamente validar que anteriormente este Cliente já tinha recebido a mensagem com action 2 – Pedido ACK (ver C5),
 *               indicando que o processamento do lado do Servidor Dedicado teve início,
 *               caso contrário, em vez de sucesso, dá error C6 e termina o processo Cliente com exit code -1;
 *
 * @param mensagem Mensagem recebida do Servidor Dedicado
 */
void pedidoConcluido(Mensagem mensagem) {
    debug("C6 <");

    if (passagemIniciada == TRUE) {
        success("C6", "Passagem Concluída com estatísticas: %d %d %d",
                mensagem.conteudo.dados.contadores_servidor.contadorNormal,
                mensagem.conteudo.dados.contadores_servidor.contadorViaVerde,
                mensagem.conteudo.dados.contadores_servidor.contadorAnomalias);
        exit(0);
    } else {
        error("C6", "Error, passagem não iniciada");
        exit(-1);
    }
    debug("C6 >");
}

/**
 * C7   Se a mensagem que chegou em C4 veio com action 4 – Pedido Cancelado,
 *      serve para o Servidor Dedicado indicar que o processamento a passagem não foi concluído.
 *      Se o Cliente receber esse sinal, dá success C7 "Processo Não Concluído e Incompleto",
 *      e termina o processo Cliente com exit code -1.
 */
void pedidoCancelado() {
    debug("C7 <");

    success("C7", "Processo Não Concluído e Incompleto");
    exit(-1);

    debug("C7 >");
}