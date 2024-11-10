#include <sys/msg.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* 
Filename: send 
*/
#define MSGKEY 0xDEAD
#define PERMS 0644
struct msgbuf{
    long mtype;
    char mtext[1024];
};
struct msgObj{
    int msqid;
};
void * send(void * _data);
void * receive(void* _data);
int main(int argc, char * argv[]){
   struct msgbuf buf;
   int msqid;
   int len;
   key_t key;
   system("touch msgq.txt");
   if ((msqid = msgget(MSGKEY, PERMS | IPC_CREAT)) == -1) {
      perror("msgget");
      exit(1);
   };
   struct msgObj data;
   data.msqid = msqid;
    pthread_t tid;
pthread_create(&tid, NULL,  receive, &data);
   send(&data);

   if(msgctl(msqid, IPC_RMID, NULL) == -1) {
      perror("msgctl");
      exit(1);
   }
    printf("message queue: done sending messages.\n");
    return 0;
};

void * send(void * _data){
    struct msgObj * data = _data;
    struct msgbuf buf;
    buf.mtype = 1;
    int len = 0;
   while(fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL) {
      len = strlen(buf.mtext);
      /* remove newline at end, if it exists */
      if (buf.mtext[len-1] == '\n') buf.mtext[len-1] = '\0';
      if (msgsnd(data->msqid, &buf, len+1, 0) == -1) /* +1 for '\0' */
        perror("msgsnd fail hehe!");
      if (msgsnd(data->msqid, &buf, len+1, 0) == -1) /* +1 for '\0' */
        perror("msgsnd fail hehe!");
    };
   return NULL;
};
void * receive(void* _data){
    struct msgObj * data = _data;
    struct msgbuf buf;
    buf.mtype = 1;
    int toend;
    while(1){
      if (msgrcv(data->msqid, &buf, sizeof(buf.mtext), 0, 0) == -1) {
         perror("msgrcv");
         exit(1);
      }
      printf("Received: \"%s\"\n", buf.mtext);
      toend = strcmp(buf.mtext,"end");
      if (toend == 0) break;
    };
    return NULL;
};

