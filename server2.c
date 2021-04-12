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
  char *request_mes = strtok(buffer, "/");
  request_mes = strtok(NULL, " ");
  char *full_name = malloc(sizeof(request_mes));
  strcpy(full_name, request_mes);
  char *file_name = strtok(request_mes, ".");
  char *file_type = strtok(NULL, " ");
  printf("%s\n",full_name);
  printf("%s\n",file_name);
  printf("%s\n",file_type);

  if(!(strcmp(file_type, "html"))){

    accept_mes = strcat(accept_mes, "text/html\n\n");
    char tempbuf[1024]; bzero(tempbuf, 1024);
    FILE *file = fopen(full_name, "r");

    if(file){
      while(fgets(tempbuf, sizeof(tempbuf), file) != NULL){
        strcat(accept_mes, tempbuf);
      }
      fclose(file);
      return 1;
    }
    else return 0;
  }

  else if(!(strcmp(file_type, "gif"))){

    accept_mes = strcat(accept_mes, "image/gif\n\n");
    FILE *file = fopen(full_name, "rb");

    if(file){
      fseek(file, 0, SEEK_END);
      int filelen = ftell(file);
      fseek(file, 0, SEEK_SET);

      char *tempbuf = (char *)malloc(strlen(accept_mes) + filelen);
      if (!tempbuf){
        fprintf(stderr, "Memory error!");
        fclose(file);
        return 0;
      }

      strcat(tempbuf, accept_mes);
      char *gifbuf = (char *)malloc(filelen + 1);
      int fileread = fread(gifbuf, filelen, 1, file);
      if(fileread > 0) memcpy(tempbuf+strlen(accept_mes), gifbuf, filelen);

      fclose(file);
      int n = write(newsockfd, tempbuf, strlen(accept_mes)+filelen);
      return 2;
    }
    else{
      return 0;
    }
  }

  else if(strcmp(file_type, "jpg") == 0 || strcmp(file_type, "jpeg") == 0){

    accept_mes = strcat(accept_mes, "image/jpeg\n\n");
    FILE *file = fopen(full_name, "rb");

    if(file){
      fseek(file, 0, SEEK_END);
      int filelen = ftell(file);
      fseek(file, 0, SEEK_SET);

      char *tempbuf = (char *)malloc(strlen(accept_mes) + filelen);
      if (!tempbuf){
        fprintf(stderr, "Memory error!");
        fclose(file);
        return 0;
      }

      strcat(tempbuf, accept_mes);
      char *jpgbuf = (char *)malloc(filelen + 1);
      int fileread = fread(jpgbuf, filelen, 1, file);
      if(fileread > 0) memcpy(tempbuf+strlen(accept_mes), jpgbuf, filelen);

      fclose(file);
      int n = write(newsockfd, tempbuf, strlen(accept_mes)+filelen);
      return 2;
    }
    else{
      return 0;
    }
  }
  
  else if(!(strcmp(file_type, "pdf"))){

    accept_mes = strcat(accept_mes, "application/pdf\n\n");
    FILE *file = fopen(full_name, "rb");

    if(file){
      fseek(file, 0, SEEK_END);
      int filelen = ftell(file);
      fseek(file, 0, SEEK_SET);

      char *tempbuf = (char *)malloc(strlen(accept_mes) + filelen);
      if (!tempbuf){
        fprintf(stderr, "Memory error!");
        fclose(file);
        return 0;
      }

      strcat(tempbuf, accept_mes);
      char *pdfbuf = (char *)malloc(filelen + 1);
      int fileread = fread(pdfbuf, filelen, 1, file);
      if(fileread > 0) memcpy(tempbuf+strlen(accept_mes), pdfbuf, filelen);

      fclose(file);
      int n = write(newsockfd, tempbuf, strlen(accept_mes)+filelen);
      return 2;
    }
    else{
      return 0;
    }
  }

  else if(!(strcmp(file_type, "mp3"))){
    accept_mes = strcat(accept_mes, "audio/mpeg\n\n");
    FILE *file = fopen(full_name, "rb");
    if(file){
      fseek(file, 0, SEEK_END);
      int filelen = ftell(file);
      fseek(file, 0, SEEK_SET);
      char *tempbuf = (char *)malloc(strlen(accept_mes) + filelen);
      if (!tempbuf){
        fprintf(stderr, "Memory error!");
        fclose(file);
        return 0;
      }
      strcat(tempbuf, accept_mes);
      while(1){
        char *mp3buf = (char *)malloc(filelen + 1);
        int fileread = fread(mp3buf, filelen, 1, file);
        if(fileread > 0) memcpy(tempbuf+strlen(accept_mes), mp3buf, filelen);
        if(fileread <= filelen) break;
      }
      fclose(file);
      int n = write(newsockfd, tempbuf, strlen(accept_mes)+filelen);
      return 2;
    }
    else{
      return 0;
    }
  }
  else{
    accept_mes = strcat(accept_mes, "text/html\n\n");
    char tempbuf[1024]; bzero(tempbuf, 1024);
    FILE *file = fopen("NotFound", "r");
    if(file){
      while(fgets(tempbuf, sizeof(tempbuf), file) != NULL){
        strcat(accept_mes, tempbuf);
      }
      fclose(file);
      return 0;
    }
  }
  return 0;
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
     while(1){
       char notFound[1024] = "HTTP/1.1 404 Not Found\nContent-type: text/html\n\n<html>\n\t<body>\n\t\t<h1>Not Found</h1>\n\t\t<p>The requested URL was not found on this server.</p>\n\t</body>\n</html>\n";
       char accept_mes[4096] = "HTTP/1.1 200 OK\nAccept-Ranges: bytes\nContent-type: ";
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

       int type_num = type_select(buffer, accept_mes, newsockfd);
       printf("type_num : %d\n", type_num);
       if(type_num == 1){
         printf("%s\n\n", accept_mes);
         n = write(newsockfd, accept_mes, sizeof(accept_mes));
         if(n < 0) error("ERROR writing to socket");
       }
       else if(type_num == 2){
         printf("%s\n\n", accept_mes);
       }
       else{
         printf("%s\n\n", notFound);
         n = write(newsockfd, notFound, sizeof(notFound));
       }

       close(newsockfd);
     }

     close(sockfd);
     return 0;
}
