#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#include "my402list.h"
#include "cs402.h"
#include "my402sortlist.h"

typedef struct{
	char *printDate;
	char *printDesc;
	char *printAmount;
	char *printBalance;
}My402Output;

int printOutput(My402List *pList);
int formatEachField(My402SortElem *currentElem, My402Output *, long long*);
extern char *toCurrency(long long);
extern char *representNegatives(char *);

int printOutput(My402List *pList){

	int line_number = 0;
	My402Output output;
	memset(&output, '\0', sizeof(My402Output));
	My402Output *pOutput = &output; 
	
	long long *pBalance = (long long *)malloc(sizeof(long long));
	if(pBalance == NULL){
		perror("\nUnable to allocate memory");	
		return FALSE;
	}
	memset(pBalance, 0, sizeof(long long)) ;
	if(pBalance == NULL){
		fprintf(stderr, "\nUnable to allocate memory");	
	}

	My402ListElem *current = NULL;
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	printf("|%7cDate%6c|%1cDescription%14c|%9cAmount%1c|%8cBalance%1c|\n",32,32,32,32,32,32,32,32);
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	for(current=My402ListFirst(pList);
		current!=NULL;
		current=My402ListNext(pList, current)){
		
		if(formatEachField((My402SortElem *)current->obj, pOutput, pBalance) == FALSE){
			return FALSE;	
		}
		line_number++;	
		printf("|%1c%s%1c|%1c%s%1c|%1c%s%1c|%1c%s%1c|\n",32,pOutput->printDate,32,32,pOutput->printDesc,32,32,pOutput->printAmount,32,32,pOutput->printBalance,32);	      
		//printf("12345678901234567890123456789012345678901234567890123456789012345678901234567890\n");
		//printf("         10        20        30        40        50        60        70        80\n");	
		//printf("\n");
		
	}	
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	        //free the inside pointers
		if(pOutput->printDate != NULL){
                        free(pOutput->printDate);
                }
                if(pOutput->printDesc != NULL){
                        free(pOutput->printDesc);
                } 
                if(pOutput->printAmount != NULL){
                        free(pOutput->printAmount);
                }
                if(pOutput->printBalance != NULL){
                        free(pOutput->printBalance);
                } 

	return TRUE;	
}

int formatEachField(My402SortElem *currentElem, My402Output *pOutput, long long *pBalance){

	//printf("\nPrint begins...");	
	int sign = 1; //Tracking the amount sign
	int i=0;
	int j=0;
	int whiteSpaces = 0;
	int count = 0;
	int balanceSign = 1;	
	int isEndingSpace = FALSE;
	//store the balance that is printed
	char *printDesc = (char *)malloc(25*sizeof(char)); //+1 for terminating char
	if(printDesc == NULL){
		perror("\nUnable to allocate memory");	
		return FALSE;	
	}
	char *printDate = (char *)malloc(16*sizeof(char)); //+1 for terminating char
	if(printDate == NULL){
		perror("\nUnable to allocate memory");	
		return FALSE;	
	}	
	char *desc = malloc(sizeof(25));
	if(desc == NULL){
		perror("\nUnable to allocate memory");	
		return FALSE;	
	}	
	memset(desc,'\0',25);
	char *amt = malloc(sizeof(15));
	if(amt == NULL){
		perror("\nUnable to allocate memory");	
		return FALSE;	
	}	
	memset(amt,'\0',15);
	char *bal = malloc(sizeof(15));
	if(bal == NULL){
		perror("\nUnable to allocate memory");	
		return FALSE;	
	}	
	memset(bal,'\0',15);
	char *printAmount = (char *)malloc(15*sizeof(char)); //+1 for termiating char
	if(printAmount == NULL){
		perror("\nUnable to allocate memory");	
		return FALSE;	
	}
	char *printBalance = (char *)malloc(15*sizeof(char));
	if(printBalance == NULL){
		perror("\nUnable to allocate memory");	
		return FALSE;	
	}
	char *cTime = (char *)malloc(26*sizeof(char));
	if(cTime == NULL){
		perror("Unable to allocate memory");	
		return FALSE;	
	}
	memset(cTime, '\0', 26*sizeof(char));
	//FIXME: what if sizeof(unsigned int) = 2 bytes?	
	long long currentAmount = currentElem->transAmount; 
	//printf("\ninput amt:%lld\n", currentElem->transAmount);	
	//Porcess Amount
	if('-' == currentElem->transType){
		sign = -1;	
	}
	//Process Date
	cTime = ctime(&currentElem->transTime);
	while(cTime[i] != '\n'){
		
		if(! (i >= 10 && i<=18) ){
			printDate[j] = cTime[i];
			j++;
		}
		i++;
	}	
	printDate[15] = '\0';
	pOutput->printDate = printDate;
	/*
	if(cTime != NULL){
		free(cTime);	
	}
	*/
	//Processing descriptio
	strncpy(printDesc, currentElem->transDesc,24);	
	i=24;	
	while(printDesc[i] == '\0' && i>0){ //although not expecting i=0
		printDesc[i] = ' ';
		i--;	
	}	
	printDesc[24] = '\0';
	//counting the leading white spaces 
	whiteSpaces = 0;
	i=0;	
	while(printDesc[i] == ' '&& printDesc[i]!= '\0'){
		whiteSpaces++;	
		i++;
	}
	strncpy(desc, printDesc+whiteSpaces, strlen(printDesc+whiteSpaces));
	//aligning the desc
	i=strlen(printDesc)-whiteSpaces;
	//printf("\ni=%d whitespaces=%d",i,whiteSpaces);
	while(i<=23){
		desc[i++] = ' ';
	}	
	//printf("\ndesc:%s..",desc);
	strncpy(printDesc, desc, 24);
	printDesc[24] = '\0';
	pOutput->printDesc = printDesc;	
	/*
	if(desc != NULL){
		free(desc);	
	}
	*/	
	//Processing printAmount	
	strncpy(printAmount,toCurrency(currentElem->transAmount),14);
	i=0;
	count = 12-strlen(printAmount);
	if(sign == -1){
		count = count-1;	
	}
	while(count>=0){ //1 white space at the end, 
		isEndingSpace = TRUE;
		amt[i] = ' ';
		i++;	
		count--;
	}
	for(j=0;printAmount[j] != '\0'; j++,i++){
		//printf("\ninside amoun condition\n");	
		amt[i] = printAmount[j];
	}
	if(isEndingSpace == TRUE){
		amt[13] = ' ';	
	}
	amt[14] = '\0';
	strncpy(printAmount,amt,14);
	printAmount[14] = '\0';
	if(sign == -1){
		strncpy(printAmount, representNegatives(printAmount),14);
	}
	printAmount[14] = '\0';
	pOutput->printAmount = printAmount;
	/*
	if(amt != NULL){
		free(amt);	
	}
	*/	
	//Process Balance
	*pBalance = (long long)sign*(long long)currentAmount + *pBalance;
	//printf("\nBal:%lld",*pBalance);
	if(*pBalance < 0){
		balanceSign = -1;	
	}
	strncpy(printBalance,toCurrency((*pBalance)*balanceSign),14);
	
        //right alignment starts
	i=0;
	j=0;
	count = 12-strlen(printBalance);
	if(*pBalance <0 ){
		count = count-1;	
	}
	isEndingSpace = FALSE;
	while(count>=0){ //although not expecting i=0
		bal[i] = ' ';
		i++;	
		count--;
		isEndingSpace = TRUE;
	}
	j=0;
	while(printBalance[j] != '\0'){
		bal[i++] = printBalance[j++];
	}
	if(isEndingSpace == TRUE){
		bal[13] = ' ';	
	}
	bal[14] = '\0';	
	//printf("%s ", printBalance);
	strncpy(printBalance, bal, 14);
	printBalance[14] = '\0';	
	if(*pBalance < 0){
		strncpy(printBalance, representNegatives(printBalance),14);
	}
	printBalance[14] = '\0';	
	pOutput->printBalance = printBalance;
	/*
	if(bal != NULL){
		free(bal);	
	}
	*/	
	return TRUE;
}
