/*
 *	Author: Alim19
 *	Assignment: Lab 2 Task 4 (EC)
 *	Course: Secure Application Development
 *	Instructor: Dr. Phu Phung
 *	University of Dayton
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
	if (argc!=2) {
		printf("Usage %s <URL>\n", argv[0]);
		exit(1);
	}

   char *url= argv[1];
   char servername[255];
	char path[255];
   char *file;
   char slash = '/';
	printf("\nDEBUG:\n");

	if (strlen(servername) > 255 || strlen(path) > 255) {
		perror("-> Enter correct input please!\n");
		exit(1);
	}

   sscanf(url, "http://%[^/]%s", servername, path);
   file = strrchr(url, slash);
   sscanf(file, "/%s", file);
   if (strlen(file) > 100) {
      printf("File name too long!\n");
      exit(2);
   }

	printf("-> hostname: %s\n-> path: %s\n-> file: %s\n", servername, path, file);
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("-> Error in opening a socket\n");
		exit(2);
	}
	printf("-> Socket has opened\n");

	struct addrinfo hints, *serveraddr;
	memset(&hints, 0, sizeof hints);
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int dns_status = getaddrinfo(servername, "80", &hints, &serveraddr);
	if (dns_status != 0) {
		perror("-> Cannot get the server address\n");
		exit(3);
	}
	
	int connected = connect(sockfd, serveraddr->ai_addr, serveraddr->ai_addrlen);
	if (connected < 0) {
		perror("-> Error in connecting to the server");
		exit(4);
	}

	printf("-> Connected to the server: %s%s\n\n", servername, path);

	int BUFFERSIZE   = 1024 * 1024;
	char buffer[BUFFERSIZE];
	bzero(buffer, BUFFERSIZE);
	sprintf(buffer, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, servername);
	int byte_sent = send(sockfd, buffer, strlen(buffer), 0);

	bzero(buffer, BUFFERSIZE);
	int bytes_received = recv(sockfd, buffer, BUFFERSIZE, 0);

	if (bytes_received < 0) {
		perror("-> Error in reading");
		exit(4);
	}

   // All this is used to get the response code :^)
   char code[10];
   sscanf(buffer, "HTTP/1.1%[^\r\n]", code);

   // Invalid Input case
   char *valid;
   valid = strstr(code, "200");
   if (!valid) {
      printf("[%s ] Sorry, could not find file!\n", code);
      exit(5);
   }

  printf("-> Response Code:%s, getting file...\n", code);

/* Old way of getting content-length, I should've looked at Isidore earlier :^)
   char *ret;
   char totalBytes[10];
   ret = strstr(buffer, "Content-Length:");
   sscanf(ret, "Content-Length: %[^\r\n]", totalBytes);
   printf("-> Total Bytes: %s\n", totalBytes);  */

   // Parse data & open file with write perms
   char *data;
   //  char crln = '\r'; <-- Idea?
   data = strstr(buffer, "\r\n\r\n");
   FILE *fp;
   fp = fopen(file, "w");

   unsigned int headerBytes;
   unsigned int   dataBytes;
   unsigned int  totalBytes;

   // New way of calculating bytes
   dataBytes   = strlen(data);
   headerBytes = (strlen(buffer) - dataBytes);
   totalBytes  = dataBytes + headerBytes;
   printf("-> Total Bytes: %d\n", totalBytes);
   printf("-> Header Bytes: %d\n-> Data Bytes: %d\n", headerBytes, dataBytes);

   // If no file name, we assume its index.html
   if (fp == NULL) {
      fp = fopen("index.html", "w");
   }
   fwrite(data+4, dataBytes-4, 1, fp);

   printf("%d\n", bytes_received);
   while( (bytes_received = recv(sockfd, buffer, BUFFERSIZE,0)) !=0 ) {
      fwrite(buffer, bytes_received,1, fp);
      printf("...\n");
   }
   printf("after\n");

   fclose(fp);
	freeaddrinfo(serveraddr);
	close (sockfd);
	return 0;
}
