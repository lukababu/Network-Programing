#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>		//write
#include<time.h>		//For random number seed

const int debug = 0;
char testHTML[] = "<html>\n"
"<title>\n"
"HTML Test Page for CPSC 441 Assignment 1\n"
"</title>\n"
"<body>\n"
"<h1>HTML Test Page </h1>\n"
"<p>\n"
"This is a very small, simple, and boring HTML test file.\n"
"</p>\n"
"<p>\n"
"Without your proxy, you should be able to view this page just fine.\n"
"</p>\n"
"<p>\n"
"With your proxy, this page should have one or more strange <b>spelling errors</b>,\n"
"but should still be formatted properly.\n"
"</p>\n"
"</body>\n"
"</html>\n";

char testGET[] = "GET http://pages.cpsc.ucalgary.ca/~carey/CPSC441/test1.html HTTP/1.1\n"
"Host: pages.cpsc.ucalgary.ca\n"
"User-Agent: Mozilla/5.0 (X11; Fedora; Linux x86_64; rv:62.0) Gecko/20100101 Firefox/62.0\n"
"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n"
"Accept-Language: en-US,en;q=0.5\n"
"Accept-Encoding: gzip, deflate\n"
"Connection: keep-alive\n"
"Upgrade-Insecure-Requests: 1\n"
"If-Modified-Since: Mon, 10 Sep 2018 00:30:45 GMT\n"
"If-None-Match: \"44244e0-189-5757977ce908e\"\n"
"Cache-Control: max-age=0\n";

char testResponse[] = "HTTP/1.1 200 OK\n"
"Date: Thu, 04 Oct 2018 02:45:12 GMT\n"
"Server: Apache/2.2.15 (Scientific Linux)\n"
"Last-Modified: Mon, 10 Sep 2018 00:30:45 GMT\n"
"ETag: \"44244e0-189-5757977ce908e\"\n"
"Accept-Ranges: bytes\n"
"Content-Length: 393\n"
"Connection: close\n"
"Content-Type: text/html; charset=UTF-8";

char *multi_tok(char *input, char *delimiter) {
    static char *string;
    if (input != NULL)
        string = input;

    if (string == NULL)
        return string;

    char *end = strstr(string, delimiter);
    if (end == NULL) {
        char *temp = string;
        string = NULL;
        return temp;
    }

    char *temp = string;

    *end = '\0';
    string = end + strlen(delimiter);
    return temp;
}

char rand_string(char *text, int position)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	srand((time_t)ts.tv_nsec);
	char charachter;

	const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int key = rand() % (int)(sizeof charset - 1);
	charachter = charset[key];
	//printf("here is: %c\n",charset[key]);
	if (debug != 0) printf("Random char is: %c\n", charachter);
	text[position] = charachter;
	return text;
}

int randInRange(int min, int max)
{
	struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
	srand((time_t)ts.tv_nsec);
	return (rand()%max) + min;
	//return min + (int)(rand() / (double)(RAND_MAX + 1) * (max - min + 1));
}
void findTxt(char htmlDoc[], int errorNum, int length) {
	const char ch = '\n';
	char buffer[30000];
	strcpy(buffer, htmlDoc);
	if (debug != 0) printf("Finding header\n");
	char *header = multi_tok(buffer, "\nContent-Type:");
	strcat(header, "\nContent-Type: text/plain; charset=UTF-8\n");
	if (debug != 0) printf("%s\nFinding body\n", header);

	char *body, *bodyHead;
	body = bodyHead = strchr(htmlDoc, ch);

	for(int i = 0; i < 10; i++) {
		bodyHead = strchr(body, ch);
		body = bodyHead + 1;
   }
	
	if (debug != 0) printf("%s,\nclearing\n", body);
	htmlDoc[0] = '\0';

	int bodyStart = 0;
	int bodyEnd = length;
	if (debug != 0) printf("Body parts found\n");

	int random_start;
	int random_occur = 0;
	char randomString;
	
	int j;
	do {
		if (debug != 0) printf("\n\nStarting random insertion\n\n");

		for (j = bodyStart; j < bodyEnd; j++) {
			if (errorNum == 0) break;

			if (debug != 0) printf("Random num is: %d Position is: %d\n", random_occur, j);
			if (body[j] == ch) {
				if (debug != 0) printf("Position: %d Char: %c\n", j, body[j]);
				j++;
			}
			// Place the error
			else {
				random_occur = randInRange(0,20);
				if (random_occur == 0) {
					if (debug != 0) printf("Proceeding to change char\n");
					rand_string(body, j);
					errorNum--;
					if (debug != 0) printf("Change of char at %d error count: %d\n", j, errorNum);
					j++;
				}
			}
			
		}
	} while (errorNum > 0);

	strcat(htmlDoc, header);
	strcat(htmlDoc, "\n\0");
	strcat(htmlDoc, body);
	
	if (debug != 0) printf("Product is:\n%s\nExiting findTxt\n", htmlDoc);
}

void findBody(char htmlDoc[], int errorNum, int length) {
	int bodyStart = -1;
	int bodyEnd, formatingCount;
	int j;
	int i = 0;
	char *body, *bodyHead;
	const char ch = '\n';
	char buffer[30000];
	body = bodyHead = strchr(htmlDoc, ch);
			
		
	strcpy(buffer, htmlDoc);
	if (debug != 0) printf("Finding header\n");
	char *header = multi_tok(buffer, "\nContent-Type:");
	strcat(header, "\nContent-Type: text/html; charset=UTF-8\n");
	if (debug != 0) printf("%s\nFinding body\n", header);


	do {
		if (htmlDoc[i] == '<') {
			if (debug != 0) printf("Position: %d Char: %c\n", i, htmlDoc[i]);
			i++;
			if (htmlDoc[i] == 'b') {
				if (debug != 0) printf("Position: %d Char: %c\n", i, htmlDoc[i]);
				i++;
				if (htmlDoc[i] == 'o') {
					if (debug != 0) printf("Position: %d Char: %c\n", i, htmlDoc[i]);
					i++;
					if (htmlDoc[i] == 'd') {
						if (debug != 0) printf("Position: %d Char: %c\n", i, htmlDoc[i]);
						i++;
						if (htmlDoc[i] == 'y') {
							if (debug != 0) printf("Position: %d Char: %c\n", i, htmlDoc[i]);
							i++;
							if (htmlDoc[i] == '>') {
								if (debug != 0) printf("GOAL! Position: %d Char: %c\n", i, htmlDoc[i]);
								i++;
								bodyStart = i;
							}
						}
					}
				}
			}

			if (htmlDoc[i] == '/') {
				if (debug != 0) printf("Position: %d Char: %c\n", i, htmlDoc[i]);
				i++;
				if (htmlDoc[i] == 'b') {
					if (debug != 0) printf("Position: %d Char: %c\n", i, htmlDoc[i]);
					i++;
					if (htmlDoc[i] == 'o') {
						if (debug != 0) printf("Position: %d Char: %c\n", i, htmlDoc[i]);
						i++;
						if (htmlDoc[i] == 'd') {
							if (debug != 0) printf("Position: %d Char: %c\n", i, htmlDoc[i]);
							i++;
							if (htmlDoc[i] == 'y') {
								if (debug != 0) printf("Position: %d Char: %c\n", i, htmlDoc[i]);
								i++;
								if (htmlDoc[i] == '>') {
									if (debug != 0) printf("GOAL! Position: %d Char: %c\n", i, htmlDoc[i]);
									i++;
									bodyEnd = i - 7;
								}
							}
						}
					}
				}
			}
		}

		i++;
	} while (htmlDoc[i] != '\0');

	if (debug != 0) printf("Body parts found\n");
	int random_start;
	int random_occur = 0;
	char randomString;
	formatingCount = 1;
	if (bodyStart != -1){
		do {
			if (debug != 0) printf("\n\nStarting random insertion\n\n");

			for (j = bodyStart; j < bodyEnd; j++) {
				if (errorNum == 0) break;

				if (htmlDoc[j] == '<') {
					if (debug != 0) printf("Position: %d Char: %c\n", j, htmlDoc[j]);

					while (htmlDoc[j] != '>') {
						j++;
					}
					if (debug != 0) printf("Position: %d Char: %c\n", j, htmlDoc[j]);
					j++;
					formatingCount++;
				}
				else {
					random_occur = randInRange(0,20); //rand() % 10;
					if (debug != 0) printf("Random num is: %d Position is: %d\n", random_occur, j);

					// Place the error
					if (random_occur == 0) {
						if (debug != 0) printf("Proceeding to change char\n");
						rand_string(htmlDoc, j);
						errorNum--;
						if (debug != 0) printf("Change of char at %d error count: %d\n", j, errorNum);
						j++;
					}
				}

			}
		} while (errorNum > 0);
	}
	else {
		for(int i = 0; i < 9; i++) {
			bodyHead = strchr(body, ch);
			body = bodyHead + 1;
		}
		
		if (debug != 0) printf("%s,\nclearing\n", body);
		htmlDoc[0] = '\0';

		bodyStart = 0;
		bodyEnd = length;
		if (debug != 0) printf("Body parts found\n");
		i = 0;
		do {
			if (debug != 0) printf("\n\nStarting random insertion\n\n");

			for (j = bodyStart; j < 1000; j++) {
				if (errorNum == 0) break;

				if (body[j] == '<') {
					printf("Starting loop: %d Char: %c\n", j, body[j]);

					while (body[j] != '>') {
						
						j++;
						printf("Breaking: %d Char: %c\n", j, body[j]);
					}
					j++;
					if (debug != 0) printf("Position: %d Char: %c\n", j, body[j]);

					
				}
				else {
					random_occur = randInRange(0,20); //rand() % 10;
					if (debug != 0) printf("Random num is: %d Position is: %d\n", random_occur, j);

					// Place the error
					if (random_occur == 0) {
						if (debug != 0) printf("Proceeding to change char\n");
						rand_string(body, j);
						errorNum--;
						if (debug != 0) printf("Change of char at %d error count: %d\n", j, errorNum);
						j++;
					}
				}
				
				

			}
		} while (errorNum > 0);
		
	}
	strcat(header, "\n");
	strcat(header, body);
	//strcat(htmlDoc, "\n");
	strcat(htmlDoc, header);
	if (debug != 0) printf("Final result:\n%s\n", htmlDoc);

	//puts(htmlDoc);
}

void modifyGET(char *getReq[]) {
	char *token = multi_tok(getReq, "If-Modified-Since");

	strcat(token, "\r\n");
	strcpy(getReq, token);
}

void getContentType(char *serverResponse[]) {
	char *token = multi_tok(serverResponse, "; charset=UTF-8");

	const char ch = '\n';
	char *start;
	char *end;

	start = end = strchr(token, ch);

	for(int i = 0; i < 8; i++) {
		end = strchr(start, '\n');
		start = end + 1;
   }

	strcpy(serverResponse, start);
}

int getContentLenght(char *serverResponse[]) {
	if (debug != 0) printf("Entering getContentLenght() method\n");
	char *token = multi_tok(serverResponse, "\nConnection:");

	const char ch = '\n';
	char *start;
	char *end;
	char *token2;
	char number[20];

  start = end = strchr(token, ch);

	 for(int i = 0; i < 6; i++) {
		  end = strchr(start, '\n');
			start = end + 1;
   }
	 token2 = strtok(start, " ");
	 token2 = strtok(NULL, " ");

	 strcpy(number, token2);

	if (debug != 0) printf("Exiting getContentLenght() method, the number is: %s\n", number);
	 return atoi(number);
}

//Extracts the URL from a given GET request
void getURL(const char *get_request, char *url)
{
	int len = strlen(get_request);
	if (len > 11)
		strcpy(url, (get_request+11));
	strtok(url, "/");
}
