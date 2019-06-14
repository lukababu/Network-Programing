#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dictionary.h"

#define DEBUG 0
#define MAX_MESSAGE_LENGTH 100

int word_Count = 0;

/*
Method for sequantial hashing, takes message as an argument which is the sent message
*/
void storeSeq(char (*message)[MAX_MESSAGE_LENGTH]) {
  char *result;
  char *encodedMessage[MAX_MESSAGE_LENGTH];
  inithashtab();

  // Hashing out the message by storing it into a dictionary
  for (int i=0; i<MAX_MESSAGE_LENGTH; i++) {
    char str[12];
    sprintf(str, "%d", i+1); //Sequential hash, int converted to hex
    if (message[i] != NULL) install(message[i], str);
    else return;
  }

  if (DEBUG == 1) {
    displaytable();
    char *tempMessage;
    char hexNum[100];
    printf("The message in hash is: ");
    for (int i = 0; i < word_Count; i++) {
      if (message[i] != NULL) {
        int buff = atoi(get(message[i]));
        decimal_to_hexadecimal(buff, hexNum);   
        printf("0x%s ", hexNum);
      }
      else return;
    }
  }

  printf("\n");
}

/*
Method for decoding sequantial hash, takes encoded message as an argument and the hashes stored in an array
*/
void decodeSeq(char* message, char (*hashes)[MAX_MESSAGE_LENGTH]) {
  char *result;
  if (DEBUG == 1) displaytable();

  // Decode message
  printf("Decoding message \"%s\"...\nThe message is: ", message);

  result = changeWords (message,hashes[0],get_hash(hashes[0]+2));
  for(int i = 1; i < word_Count; i++) {
    result = changeWords (result,hashes[i],get_hash(hashes[i]+2));
  }
  printf ("%s\n\n", result);
  free (result);
}

/*
  Gets all the words in the string without punctuation
  */
void getWords(char* str, char (*wordArray)[MAX_MESSAGE_LENGTH]) {
  char * pch;
  word_Count = 0;

  if (DEBUG == 1) printf ("Splitting string \"%s\" into tokens:\n",str);
  pch = strtok (str," ,.-!?:");

  
  while (pch != NULL)
  {
    strcpy(wordArray[word_Count], pch);
    if (DEBUG == 1) {
      for (int i = 0; i < MAX_MESSAGE_LENGTH; i++)
        printf("Word %d: %s\n", i, wordArray[i]);
    }
    pch = strtok (NULL, " ,.-!?:");
    word_Count++;
  }
  for (int i = word_Count; i<=MAX_MESSAGE_LENGTH; i++) {
    wordArray[i][0] = '\0';
    //strcpy(wordArray[i], '\0');
  }
  //

  //return (*wordArray)[100];
}

/*
  replace the words in the message with hashes
  */
void changeMessage(char* message, char (*wordArray)[MAX_MESSAGE_LENGTH]) {
  char *result;

  printf("Encoding string: \"%s\"...\n", message);
  char hashWord[MAX_MESSAGE_LENGTH] = "0x";
  strcat(hashWord, get(wordArray[0]));
  result = changeWords (message,wordArray[0], hashWord);
  for(int i = 1; i < word_Count; i++) {
    hashWord[2] = '\0';
    strcat(hashWord, get(wordArray[i]));
    result = changeWords (result,wordArray[i],hashWord);
  }
  printf ("%s\n\n", result);
  free (result);
}

int main(){
  
  char input[MAX_MESSAGE_LENGTH] ="Luka IS Luka, Sometimes asd a !";
  char strEn[MAX_MESSAGE_LENGTH];
  strcpy(strEn, input);

  char wordsEn[100][100];
  char wordsDe[100][100];

  getWords(strEn, wordsEn);
  storeSeq(wordsEn);

  changeMessage(input, wordsEn);

  char output[] = "0x3 0x2 0x3 0x4, 0x5 0x6!";
  char strDe[MAX_MESSAGE_LENGTH];
  strcpy(strDe, output);

  getWords(strDe, wordsDe);
  decodeSeq(output, wordsDe);

  return 0;
}