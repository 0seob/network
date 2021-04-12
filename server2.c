/*
   A simple server in the internet domain using TCP
   Usage:./server port (E.g. ./server 10000 )
*/
#include <stdio.h>
#include <sys/types.h>   // definitions of a number of data types used in socket.h and netinet/in.h
#include <sys/socket.h>  // definitions of structures needed for sockets, e.g. sockaddr
#include <netinet/in.h>  // constants and structures needed for internet domain addresses, e.g. sockaddr_in
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int type_select(char *buffer, char *accept_mes, int newsockfd){
  char *request_mes = strtok(buffer, "/");                                      //buffer에 들어온 입력값을 strtok을 이용해 파싱
  char *file_name;                                                              //파일 이름과 타입을 저장할 변수 선언
  char *file_type;
  request_mes = strtok(NULL, " ");                                              //request_mes를 공백까지 파싱
  char *full_name = malloc(sizeof(request_mes));                                //request_mes를 full_name에 저장
  strcpy(full_name, request_mes);
  if(strchr(full_name, '.') != NULL){                                           //full_name에 .이 포함되어 있다면
    file_name = strtok(request_mes, ".");                                       //.을 기준으로 file_name과 file_type으로 나누어 저장
    file_type = strtok(NULL, " ");
    printf("%s\n",full_name);                                                   //확인하기 위하여 출력
    printf("%s\n",file_name);
    printf("%s\n",file_type);
  }
  else {
    printf("%s\n",full_name);                                                   //full_name에 .이 포함되어있지 않다면 출력하고 0을 반환
    return 0;
  }

  if(!(strcmp(file_type, "html"))){                                             //file_type이 html이면

    accept_mes = strcat(accept_mes, "text/html\n\n");                           //accept_mes에 text/html을 붙임
    char tempbuf[1024]; bzero(tempbuf, 1024);                                   //버퍼 선언 후 0으로 초기화
    FILE *file = fopen(full_name, "r");                                         //full_name 파일을 읽기전용으로 열기

    if(file){                                                                   //파일이 존재한다면
      while(fgets(tempbuf, sizeof(tempbuf), file) != NULL){                     //fgets함수를 통해 file에서 tempbuf의 크기만큼 읽기
        strcat(accept_mes, tempbuf);                                            //tempbuf를 accept_mes에 붙임
      }
      fclose(file);                                                             //파일을 닫고 1을 반환
      return 1;
    }
    else return 0;                                                              //파일이 없다면 0을 반환
  }

  else if(!(strcmp(file_type, "gif"))){                                         //file_type이 gif라면

    accept_mes = strcat(accept_mes, "image/gif\n\n");                           //accept_mes에 image/gif를 붙임
    FILE *file = fopen(full_name, "rb");                                        //파일을 바이너리파일 읽기전용으로 열기

    if(file){                                                                   //파일이 존재한다면
      fseek(file, 0, SEEK_END);                                                 //파일의 크기를 구하고
      int filelen = ftell(file);
      fseek(file, 0, SEEK_SET);

      char *tempbuf = (char *)malloc(strlen(accept_mes) + filelen);             //파일과 accept_mes를 담을 버퍼를 선언
      if (!tempbuf){                                                            //tempbuf가 0이면 오류
        fprintf(stderr, "Memory error!");
        fclose(file);
        return 0;
      }

      strcat(tempbuf, accept_mes);                                              //tempbuf에 accept_mes를 끼워넣고
      char *gifbuf = (char *)malloc(filelen + 1);                               //gifbuf를 file크기+1로 선언하여
      int fileread = fread(gifbuf, filelen, 1, file);                           //fread를 통해 읽어들임
      if(fileread > 0) memcpy(tempbuf+strlen(accept_mes), gifbuf, filelen);     //읽어들인 값이 0보다 크면 tempbuf에다가 gifbuf값을 저장

      fclose(file);                                                             //파일을 닫고
      write(newsockfd, tempbuf, strlen(accept_mes)+filelen);                    //소켓으로 tempbuf를 전송
      return 2;                                                                 //2를 반환
    }
    else{                                                                       //파일이 없다면 0을 반환
      return 0;
    }
  }

  else if(strcmp(file_type, "jpg") == 0 || strcmp(file_type, "jpeg") == 0){     //file_type이 jpg나 jpeg라면

    accept_mes = strcat(accept_mes, "image/jpeg\n\n");                          //accept_mes에 image/jpeg를 붙임
    FILE *file = fopen(full_name, "rb");                                        //파일을 바이너리파일 읽기전용으로 열기

    if(file){                                                                   //파일이 존재한다면
      fseek(file, 0, SEEK_END);                                                 //파일의 크기를 구하고
      int filelen = ftell(file);
      fseek(file, 0, SEEK_SET);

      char *tempbuf = (char *)malloc(strlen(accept_mes) + filelen);             //파일과 accept_mes를 담을 버퍼를 선언
      if (!tempbuf){                                                            //tempbuf가 0이면 오류
        fprintf(stderr, "Memory error!");
        fclose(file);
        return 0;
      }

      strcat(tempbuf, accept_mes);                                              //tempbuf에 accept_mes를 끼워넣고
      char *jpgbuf = (char *)malloc(filelen + 1);                               //gifbuf를 file크기+1로 선언하여
      int fileread = fread(jpgbuf, filelen, 1, file);                           //fread를 통해 읽어들임
      if(fileread > 0) memcpy(tempbuf+strlen(accept_mes), jpgbuf, filelen);     //읽어들인 값이 0보다 크면 tempbuf에다가 gifbuf값을 저장

      fclose(file);                                                             //파일을 닫고
      write(newsockfd, tempbuf, strlen(accept_mes)+filelen);                    //소켓으로 tempbuf를 전송
      return 2;                                                                 //2를 반환
    }
    else{                                                                       //파일이 없다면 0을 반환
      return 0;
    }
  }

  else if(!(strcmp(file_type, "pdf"))){                                         //file_type이 pdf라면

    accept_mes = strcat(accept_mes, "application/pdf\n\n");                     //accept_mes에 application/pdf를 붙임
    FILE *file = fopen(full_name, "rb");                                        //파일을 바이너리파일 읽기전용으로 열기

    if(file){                                                                   //파일이 존재한다면
      fseek(file, 0, SEEK_END);                                                 //파일의 크기를 구하고
      int filelen = ftell(file);
      fseek(file, 0, SEEK_SET);

      char *tempbuf = (char *)malloc(strlen(accept_mes) + filelen);             //파일과 accept_mes를 담을 버퍼를 선언
      if (!tempbuf){                                                            //tempbuf가 0이면 오류
        fprintf(stderr, "Memory error!");
        fclose(file);
        return 0;
      }

      strcat(tempbuf, accept_mes);                                              //tempbuf에 accept_mes를 끼워넣고
      char *pdfbuf = (char *)malloc(filelen + 1);                               //gifbuf를 file크기+1로 선언하여
      int fileread = fread(pdfbuf, filelen, 1, file);                           //fread를 통해 읽어들임
      if(fileread > 0) memcpy(tempbuf+strlen(accept_mes), pdfbuf, filelen);     //읽어들인 값이 0보다 크면 tempbuf에다가 gifbuf값을 저장

      fclose(file);                                                             //파일을 닫고
      write(newsockfd, tempbuf, strlen(accept_mes)+filelen);                    //소켓으로 tempbuf를 전송
      return 2;                                                                 //2를 반환
    }
    else{                                                                       //파일이 없다면 0을 반환
      return 0;
    }
  }
  else if(!(strcmp(file_type, "mp3"))){                                         //file_type이 mp3라면

    accept_mes = strcat(accept_mes, "audio/mpeg\n\n");                          //accept_mes에 audio/mpeg를 붙임
    FILE *file = fopen(full_name, "rb");                                        //파일을 바이너리파일 읽기전용으로 열기

    if(file){                                                                   //파일이 존재한다면
      fseek(file, 0, SEEK_END);                                                 //파일의 크기를 구하고
      int filelen = ftell(file);
      fseek(file, 0, SEEK_SET);

      char *tempbuf = (char *)malloc(strlen(accept_mes) + filelen);             //파일과 accept_mes를 담을 버퍼를 선언
      if (!tempbuf){                                                            //tempbuf가 0이면 오류
        fprintf(stderr, "Memory error!");
        fclose(file);
        return 0;
      }

      strcat(tempbuf, accept_mes);                                              //tempbuf에 accept_mes를 끼워넣고
      char *mp3buf = (char *)malloc(filelen + 1);                               //gifbuf를 file크기+1로 선언하여
      int fileread = fread(mp3buf, filelen, 1, file);                           //fread를 통해 읽어들임
      if(fileread > 0) memcpy(tempbuf+strlen(accept_mes), mp3buf, filelen);     //읽어들인 값이 0보다 크면 tempbuf에다가 gifbuf값을 저장

      fclose(file);                                                             //파일을 닫고
      write(newsockfd, tempbuf, strlen(accept_mes)+filelen);                    //소켓으로 tempbuf를 전송
      return 2;                                                                 //2를 반환
    }
    else{                                                                       //파일이 없다면 0을 반환
      return 0;
    }
  }

  else if(!(strcmp(file_type, "ico"))){                                         //file_type이 ico라면 3을 반환
    return 3;
  }

  else{                                                                         //나머지 type은 0을 반환
    return 0;
  }
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd; //descriptors rturn from socket and accept system calls
     int portno; // port number
     socklen_t clilen;

     char buffer[4096];

     /*sockaddr_in: Structure Containing an Internet Address*/
     struct sockaddr_in serv_addr, cli_addr;

     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     /*Create a new socket
       AF_INET: Address Domain is Internet
       SOCK_STREAM: Socket Type is STREAM Socket */
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]); //atoi converts from String to Integer
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY; //for the server the IP address is always the address that the server is running on
     serv_addr.sin_port = htons(portno); //convert from host to network byte order

     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) //Bind the socket to the server address
              error("ERROR on binding");

     listen(sockfd,5); // Listen for socket connections. Backlog queue (connections to wait) is 5
     while(1){                                                          //연결이 끊기지 않게 하기 위해 while로 반복
       char notFound[1024] = "HTTP/1.1 404 Not Found\nContent-type: text/html\n\n<html>\n\t<body>\n\t\t<h1>Not Found</h1>\n\t\t<p>The requested URL was not found on this server.</p>\n\t</body>\n</html>\n";
                                                                        //페이지가 없을때 보내는 message
       char accept_mes[4096] = "HTTP/1.1 200 OK\nAccept-Ranges: bytes\nContent-type: ";
                                                                        //페이지가 있을때 보내는 message
       clilen = sizeof(cli_addr);
       /*accept function:
         1) Block until a new connection is established
         2) the new socket descriptor will be used for subsequent communication with the newly connected client.
       */
       newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
       if (newsockfd < 0)
            error("ERROR on accept");

       bzero(buffer,4096);
       n = read(newsockfd,buffer,4095); //Read is a block function. It will read at most 255 bytes
       if (n < 0) error("ERROR reading from socket");
          printf("Here is the message: %s\n",buffer);

       int type_num = type_select(buffer, accept_mes, newsockfd);      //type_select함수를 통해 나온 결과를 type_num에 저장
       printf("type_num : %d\n", type_num);                            //type_num 출력
       if(type_num == 1){                                              //type_num이 1이면 (html)
         printf("%s\n\n", accept_mes);                                 //accept_mes를 출력하고
         n = write(newsockfd, accept_mes, sizeof(accept_mes));         //write를 이용하여 accept_mes를 소켓으로 전송
         if(n < 0) error("ERROR writing to socket");                   //에러처리
       }
       else if(type_num == 2){                                         //type_num이 2이면 (binaryfile)
         printf("%s\n\n", accept_mes);                                 //type_select함수에서 이미 소켓으로 전송했으므로 accept_mes만 출력
       }
       else if(type_num == 3){                                         //type_num이 3이면 (favicon.ico)
         continue;                                                     //이므로 반복문 다시 진행
       }
       else{                                                           //type_num이 0이면 notFound를 소켓에 전송
         printf("%s\n\n", notFound);                                   //notFound 출력
         n = write(newsockfd, notFound, sizeof(notFound));
       }

       close(newsockfd);
     }

     close(sockfd);
     return 0;
}
