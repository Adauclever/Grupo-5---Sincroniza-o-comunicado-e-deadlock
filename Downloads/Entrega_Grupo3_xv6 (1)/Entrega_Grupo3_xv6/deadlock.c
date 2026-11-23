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
    // Processo Filho
    close(p1[1]); // Fecha escrita de p1 (apenas le)
    close(p2[0]); // Fecha leitura de p2 (apenas escreve)

    printf("Filho (PID %d): tentando ler de p1...\n", getpid());
    read(p1[0], &buf, 1); // Bloqueia esperando o pai escrever em p1

    printf("Filho: leu '%c', escrevendo em p2...\n", buf);
    write(p2[1], "F", 1);

    close(p1[0]);
    close(p2[1]);
    exit(0);
  } else {
    // Processo Pai
    close(p1[0]); // Fecha leitura de p1 (apenas escreve)
    close(p2[1]); // Fecha escrita de p2 (apenas le)

    printf("Pai (PID %d): tentando ler de p2...\n", getpid());
    read(p2[0], &buf, 1); // Bloqueia esperando o filho escrever em p2

    printf("Pai: leu '%c', escrevendo em p1...\n", buf);
    write(p1[1], "P", 1);

    close(p1[1]);
    close(p2[0]);
    wait(0);
    printf("Pai: finalizado sem deadlock.\n");
    exit(0);
  }
}
