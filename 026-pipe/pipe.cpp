#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

using namespace std;

int main(){
	int fd[2];
	pid_t pid;

	int ret = pipe(fd);
	if(ret == -1){
		perror("pipe error\n");
	}

	int i;
	for(i=0 ; i<2 ; i++){
		pid = fork();
		if(pid == 0){
			break;
		}
	}

	if(i == 2){
		close(fd[0]);
		close(fd[1]);

		pid_t wpid;
		while((wpid = waitpid(0,NULL,WNOHANG)) != -1){
			if(wpid > 0){
				printf("waitpid is %d\n",wpid);
			}else{
				sleep(1);
				continue;
			}
		}
	}

	if(i == 0){
		close(fd[0]);
		write(fd[1],"hello\n",6);
	}

	if(i == 1){
		close(fd[1]);
		char buf[1024];
		int n = read(fd[0],buf,1024);
		write(STDOUT_FILENO,buf,n);
	}

	return 0;
}
