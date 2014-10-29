#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#define SYST "Unix Type: L8"
int main(int argc, char* argv[]){
    int sockfd, clientfd, recv_size, send_size,portno=21;
    char recv_mesg[1024],send_mesg[1024]="220 (kaaFTP 0.5)\r\n",mesg[1024],
        command[16],parameter[1024],user[64],pass[64],pwd[512]="/home/";
    struct sockaddr_in serv_addr,cli_addr;
    socklen_t cli_len=sizeof(cli_addr);
    if((sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0){
        perror("ERROT on creating socket");
        return -1;
    }
    memset(&serv_addr,0,sizeof(serv_addr));
    if(argc>1){
        portno=atoi(argv[1]);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr =  INADDR_ANY;
    if (bind(sockfd, (struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0){
        perror("ERROR on binding");
        return -1;
    }
    if(listen(sockfd,5) < 0){
        perror("ERROR on listening");
        return -1;
    }
    if((clientfd= accept(sockfd,(struct sockaddr*) &cli_addr,&cli_len)) < 0){
        perror("ERROR on accepting client");
        return -1;
    }
    if((send_size = write(clientfd,send_mesg,strlen(send_mesg))) < 0){
        perror("ERROR on sending message");
    }
    while(1){
        if((recv_size=read(clientfd,recv_mesg,sizeof(recv_mesg)-1)) < 0){
            perror("ERROR on receiving message");
        }
        sscanf(recv_mesg,"%[^ ] %[^\r\n]",command,parameter);
        if(!strcmp(command,"USER")){
            strcpy(user,parameter);
            strcpy(send_mesg,"331 Please specify the password.\r\n");
        }
        else if(!strcmp(command,"PASS")){
            strcpy(pass,parameter);
            if(1){
                strcpy(send_mesg,"230 Login successful.\r\n");
                strcat(pwd,user);
            }
            printf("%s %s\n",user,pass);
        }
//        else if(!strcmp(command,"SYST")){
//            strcpy(send_mesg,SYST);
//        }
        else if(!strcmp(command,"PWD")){

        }
        if((send_size = write(clientfd,send_mesg,strlen(send_mesg))) < 0){
            perror("ERROR on sending message");
        }
    }
    return 0;
}
