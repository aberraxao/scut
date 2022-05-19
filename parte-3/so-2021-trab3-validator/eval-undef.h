/**
 * Undefine the replacement macros defined in eval.h
 **/

#undef exit
#undef sleep

#undef signal
#undef kill
#undef fork
#undef signal
#undef sigaction
#undef pause
#undef alarm

// IPC Message queues
#undef msgget
#undef msgsnd
#undef msgrcv
#undef msgctl

// IPC semaphores
#undef semget
#undef semctl
#undef semop

// IPC shared memory
#undef shmget
#undef shmat
#undef shmdt
#undef shmctl

#undef mkfifo
#undef S_ISFIFO
#define S_ISFIFO(m)     (((m) & S_IFMT) == S_IFIFO) 

#undef remove
#undef unlink