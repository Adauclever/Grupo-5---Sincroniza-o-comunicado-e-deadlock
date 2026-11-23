#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2]; // Canal Pai -> Filho
  int p2[2]; // Canal Filho -> Pai
  char buf;

  if(pipe(p1) < 0 || pipe(p2) < 0){
    printf("Erro ao criar pipes\n");
    exit(1);
  }

  int pid = fork();
  if(pid < 0){
    printf("Erro no fork\n");
    exit(1);
  }

  if(pid == 0){
    // Processo Filho (mantem leitura primeiro, depois escrita)
    close(p1[1]);
    close(p2[0]);

    printf("Filho (PID %d): aguardando dado em p1...\n", getpid());
    read(p1[0], &buf, 1);

    printf("Filho (PID %d): recebeu '%c', respondendo em p2...\n", getpid(), buf);
    write(p2[1], "F", 1);

    close(p1[0]);
    close(p2[1]);
    exit(0);
  } else {
    // Processo Pai (ordem invertida: escreve antes de ler)
    close(p1[0]);
    close(p2[1]);

    printf("Pai (PID %d): escrevendo 'P' em p1...\n", getpid());
    write(p1[1], "P", 1);

    printf("Pai (PID %d): aguardando resposta em p2...\n", getpid());
    read(p2[0], &buf, 1);

    printf("Pai (PID %d): recebeu '%c' de p2!\n", getpid(), buf);

    close(p1[1]);
    close(p2[0]);
    wait(0);
    printf("Pai: finalizado com sucesso!\n");
    exit(0);
  }
}
