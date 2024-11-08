#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#define PERMS 0644
#define MSG_KEY 0x123

struct my_msgbuf{
	long mtype;
	char mtext[200];
}

int main(void){
		
}
