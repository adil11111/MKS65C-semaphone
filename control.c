# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/shm.h>
# include <sys/ipc.h>
# include <sys/types.h>
# include <sys/sem.h>
# include <fcntl.h>
# include <errno.h>
# include <unistd.h>

# define KEY 0xCA


union semun{
  int val; // used for SETVAL
  struct semid_ds *buf; // used for IPC_STAT and IPC_SET
  unsigned short *array; // used for SETALL
  struct seminfo *__buf; // buffer for IPC_INFO
};

int main(int argc, char const *argv[]) {

  const char * command;

  //if someone doesnt put smth in make setup args =
  if (!argv[1]){
    printf("Error: Arg is empty!\n");
    exit(0); //exits program
  }

  //setting up
  command = argv[1];

  int file;

  //CREATING!
  if (!strcmp(command, "-c")){
    int shmid = shmget(KEY, 4, 0644 | IPC_CREAT);

    //error handling
    if (shmid == -1){
      printf("ERROR: %s\n", strerror(errno));
    }


    file = open("./story.txt", O_CREAT | O_TRUNC, 0644);

    //error handling
    if (file == -1){
      printf("ERROR: %s\n", strerror(errno));
    }

    int semaphore = semget(KEY, 1, IPC_CREAT | 0644);

    //YOU CANNOT DO ANYTHING
    if (semaphore == -1){

      printf("ERROR: %s\n", strerror(errno));
      semaphore = semget(KEY, 1, 0);
      int v = semctl(semaphore, 0, GETVAL, 0);

    }
    //YOU CAN DO STUFF
    else {
      union semun data;
      data.val = 1;
      int r = semctl(semaphore, 0, SETVAL, data);


    }
  }
  // VIEWING!
  else if (!strcmp(command, "-v")){
    file = open("./story.txt", O_RDONLY);

    //error handling
    if (file == -1){
      printf("ERROR: %s\n", strerror(errno));
    }


    char text[1000];
    //reading the story
    read(file, text, 999);
    close(file);
    if (errno){
      //error handling
      printf("ERROR: %s\n", strerror(errno));
    }
    else{
      printf("Story:\n%s\n", text);
    }
  }

  //REMOVING
  else if (!strcmp(command, "-r")){
    int shmid = shmget(KEY, 4, 0644);

    //error handling
    if (shmid == -1){
      printf("Shared Memory Get Error: %s\n", strerror(errno));
    }

    shmctl(shmid, IPC_RMID, NULL);

    char text[1000];
    file = open("./story.txt", O_RDONLY);
    read(file, text, 999);
    close(file);
    if (errno){
      printf("ERROR: %s\n", strerror(errno));
    }
    else{
      printf("Deleted Story:\n%s\n", text);
    }

    if (remove("./story.txt")){
      printf("Error!\n");
    }

    int semaphore = semget(KEY, 1, IPC_CREAT | 0644);

    //error handling
    if (semaphore == -1){
      printf("ERROR: %s\n", strerror(errno));
    }

    union semun data;
    if (semctl(semaphore, 1, IPC_RMID, data) == -1){
      printf("ERROR: %s\n", strerror(errno));
    }
  }
  else {
    printf("Error: Valid input are -v, -r, or -c\n");
  }

  return 0;
}
