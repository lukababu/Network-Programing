/*
    Dictionary Data Structure
    Author: 	Sourav Datta
    Source:     http://www.sourcecodesworld.com/source/show.asp?ScriptID=1188

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>


typedef struct _node{
  char *name;
  char *desc;
  struct _node *next;
}node;

#define HASHSIZE 101
#define DEBUG 0
static node* hashtab[HASHSIZE];

void inithashtab(){
  int i;
  for(i=0;i<HASHSIZE;i++)
    hashtab[i]=NULL;
}

unsigned int hash(char *s){
  unsigned int h=0;
  for(;*s;s++)
    h=*s+h*71;
  return h%HASHSIZE;
}

node* lookup(char *n){
  unsigned int hi=hash(n);
  node* np=hashtab[hi];
  for(;np!=NULL;np=np->next){
    if(!strcmp(np->name,n))
      return np;
  }

  return NULL;
}

char* m_strdup(char *o){
  int l=strlen(o)+1;
  char *ns=(char*)malloc(l*sizeof(char));
  strcpy(ns,o);
  if(ns==NULL)
    return NULL;
  else
    return ns;
}

char* get(char* name){
  node* n=lookup(name);
  if(n==NULL)
    return NULL;
  else
    return n->desc;
}

node* lookup_hash(char *n){
  //unsigned int hi=hash(n);
  int i = 0;
  node* np;
  if (DEBUG == 1) printf("\n");
  for (int i = 0; i <= HASHSIZE; i++) {
    np=hashtab[i];
    //printf("Searching for: %s, Currently at %d, Found %s and %s\n",n, i, np->name, np->desc);
    
    if (np != NULL) {
        if (strcmp(n, np->desc)) {
            if (DEBUG == 1) printf("Searching for: %s, Currently at %d, Found %s and %s\n",n, i, np->name, np->desc);
        }
        else {
            if (DEBUG == 1) printf("Returning: %s, Currently at %d, Found %s and %s\n",n, i, np->name, np->desc);
            return np;
        }
    }
  }
  
  //return np;
}

char* get_hash(char* desc){
  node* n=lookup_hash(desc);
  if(n==NULL)
    return NULL;
  else
    return n->name;
}

int install(char* name,char* desc){
  unsigned int hi;
  node* np;
  if((np=lookup(name))==NULL){
    hi=hash(name);
    np=(node*)malloc(sizeof(node));
    if(np==NULL)
      return 0;
    np->name=m_strdup(name);
    if(np->name==NULL) return 0;
    np->next=hashtab[hi];
    hashtab[hi]=np;
  }
  else
    free(np->desc);
  np->desc=m_strdup(desc);
  if(np->desc==NULL) return 0;

  return 1;
}

/*  displays the hashtable in (key.value) pairs */

void displaytable(){
  int i;
  node *t;
  for(i=0;i<HASHSIZE;i++){
    if(hashtab[i]==NULL)
      printf("()");
    else{
      t=hashtab[i];
      printf("(");
      for(;t!=NULL;t=t->next)
	printf("(%s.%s) ",t->name,t->desc);
      printf(")");
    }
  }
}

void cleanup(){
  int i;
  node *np,*t;
  for(i=0;i<HASHSIZE;i++){
    if(hashtab[i]!=NULL){
      np=hashtab[i];
      while(np!=NULL){
	t=np->next;
	free(np->name);
	free(np->desc);
	free(np);
	np=t;
      }
    }
  }
}

int hexadecimal_to_decimal(int x)
{
      int decimal_number, remainder, count = 0;
      while(x > 0)
      {
            remainder = x % 10;
            decimal_number = decimal_number + remainder * pow(16, count);
            x = x / 10;
            count++;
      }
      return decimal_number;
}

char* decimal_to_hexadecimal(long int decimalNumber, char* hexadecimalNumber) {
    long int remainder,quotient;
	int i=1,j,temp;
    char tempHex[100];
    hexadecimalNumber[0] = '\0';

	quotient = decimalNumber;
	while(quotient!=0) {
		temp = quotient % 16;
		//To convert integer into character
		if( temp < 10)
		           temp =temp + 48; else
		         temp = temp + 55;
		tempHex[i++]= temp;
		quotient = quotient / 16;
	}
	//printf("Equivalent hexadecimal value of decimal number %d: ", i);
    
	for (j = i-1; j > 0; j--) {
	      //printf("%c",tempHex[j]);
          *hexadecimalNumber++ = tempHex[j];
    }

	return hexadecimalNumber;
}

char *changeWords (char *sentence, char *find, char *replace)
{
    char *dest = malloc (strlen(sentence)-strlen(find)+strlen(replace)+1);
    char *ptr;

    strcpy (dest, sentence);

    ptr = strstr (dest, find);
    if (ptr)
    {
        memmove (ptr+strlen(replace), ptr+strlen(find), strlen(ptr+strlen(find))+1);
        strncpy (ptr, replace, strlen(replace));
    }

    return dest;
}