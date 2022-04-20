/*
 * Author: Ali.m
 * Assignment: Lab 2 Task 4 (EC)
 * Course: Secure Application Development
 * Insutroctor: Dr. Phu Phung
 * University of Dayton
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
   if ( argc != 2 ) {
      printf("Usage %s <URL>\n", argv[0]);
      exit(1);
   }

   char *url = argv[1];
   char servername[255];
   char path[255];
   char *file;
   printf("\nDEBUG:\n");

   if ( strlen(servername) > 255 || strlen(path) > 255 ) {
      perror("-> Enter correct input please!\n");
      exit(1);
   }

   sscanf(url, "http://%[^/]%s", servername, path);
   file = strrchr(url, '/');
   sscanf(file, "/%s", file);
   if ( strlen(file) > 100 ) {
      printf("File name too long!\n");
      exit(2);
   }

   printf("-> hostname: %s\n-> path: %s\n-> file: %s\n", servername, path, file);
   int sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if ( sockfd < 0 ) {
      perror("-> Error in opening socket!\n");
      exit(3);
   }
   printf("-> Socket has opened!\n");

   struct addrinfo hints, *serveraddr;
   memset(&hints, 0, sizeof hints);
   hints.ai_family   = AF_INET;
   hints.ai_socktype = SOCK_STREAM;

   int dns_status = getaddrinfo(servername, "80", &hints, &serveraddr);
   if ( dns_status != 0 ) {
      perror("-> Cannot get server address!\n");
      exit(4);
   }

   int connected = connect(sockfd, serveraddr->ai_addr, serveraddr->ai_addrlen);
   if ( connected < 0 ) {
      perror("-> Error in connecting to the server!\n");
      exit(5);
   }

   printf("Connected to the server: %s%s\n\n", servername, path);

   int BUFFERSIZE = 1024 * 1024;
   char buffer[BUFFERSIZE];
   bzero(buffer, BUFFERSIZE);
   sprintf(buffer, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, servername);
   int byte_sent = send(sockfd, buffer, strlen(buffer), 0);

   bzero(buffer, BUFFERSIZE);

   int bytes_received;
   while (true) {
      bytes_received = recv(sockfd, buffer, BUFFERSIZE, 0);
      if ( bytes_received < 0 ) {
         perror("->Error in reading!\n");
         exit(6);
      } else if ( bytes_received == 0 ){
         printf("Done!\n");
         exit(7);
      }

      char code[10], *valid;
      sscanf(buffer, "HTTP/1.1%[^\r\n]", code);
      valid = strstr(code, "200");

      if ( !valid ) {
         printf("[%s ] Sorry, could not find file!\n", code);
         exit(8);
      }
      printf("-> Response Code:%s, getting file...\n", code);

      char *data;
      FILE *fp;
      data = strstr(buffer, "\r\n\r\n");
      fp = fopen(file, "w");
   
      unsigned int headerBytes;
      unsigned int   dataBytes;
      unsigned int  totalBytes;

      dataBytes   = strlen(data);
      headerBytes = (strlen(buffer) - dataBytes);
      totalBytes  = strlen(buffer);
      printf("-> Total Bytes: %d\n", totalBytes);
      printf("-> Header Bytes: %d\n-> Data Bytes: %d\n", headerBytes, dataBytes);

      if ( fp == NULL ) {
         fp = fopen("index.html", "w");
      }
      fwrite(data+4, dataBytes-4, 1, fp);

      bzero(buffer, BUFFERSIZE);
   }

//   fclose(fp);
   freeaddrinfo(serveraddr);
   close(sockfd);
   return 0;
}
