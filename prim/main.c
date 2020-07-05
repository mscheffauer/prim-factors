#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>

#define MAX_PRIMES 3000		//system can store up to 3000 primes
#define MAX_FACTORS 200		//maximum number of factors and exponents
#define MAX_OUTPUTLENGTH 300 //maximal string output length

//array of primes
static uint32_t arraysrcprim[MAX_PRIMES] = {3};


/******************check the input string if valid***************************/
long checkInputs(int argc, char*argv[])
{
	int base;
    char *endptr;
	char *str;
    long val;

	str = argv[1];
    base = (argc > 2) ? atoi(argv[2]) : 10;

	errno = 0;    /* To distinguish success/failure after call */
    val = strtol(str, &endptr, base);

   /* Check for various possible errors */

	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
            || (errno != 0 && val == 0)) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }

	if (endptr == str) {
        fprintf(stderr, "invalid argument\n");
        exit(EXIT_FAILURE);
    }


	if (*endptr != '\0')  {      /* no valid argument found */
        fprintf(stderr, "invalid argument\n");
        exit(EXIT_FAILURE);
	}else{
	     /* If we got here, strtol() successfully parsed a number */
		return val;   
	}
	exit(EXIT_SUCCESS);
	
}


/******************search all primes that are smaller or equal to a given number *************************/
/******************and store them in an array outside						*************************/

int findPrimesBefore(uint32_t endnumber, uint32_t srcarray[],uint32_t* len){

	uint32_t num=3;
	uint32_t cnt=0;
	bool condition=false;
		
	do {
					
		num = num +2;

		if (cnt == 0){
			if ((num % srcarray[cnt]) != 0){
				cnt++;
				srcarray[cnt] = num;	
			}
		}else{		
			for (uint16_t i=0; i<=cnt ;i++ ){
				if ((num % srcarray[i]) != 0){			
					condition= true;
				}else{
					condition= false;
					break;
				}
			}
		
			if (condition == true){
				if (cnt < (MAX_PRIMES-1)){
					cnt++;
					srcarray[cnt] = num;	
				}else{
					return -1;
				}
								
			}
			
		}

	} while ((num < endnumber));
	
		
	*len = cnt;
	//list of prime numbers is full
	//endnumber itself is a prime number
	if (num == endnumber){
		return 2;		
	}else{
		return 0;		
	}
	
	return -1;

		
}


/******************print out the factors and exponents arrays given	*************************/
void printFactorsAndExponents(uint32_t exponents[],uint32_t factors[],uint8_t cnt){
	
		char buf[16]=""; //big enough to hold numbers
		char tmpstring[MAX_OUTPUTLENGTH]="";
		
		//loop through count
		for (uint8_t i=0; i<cnt;i++ ){
			//if zero copy in string first
			if (i == 0){
				sprintf(buf,"%d",factors[i]);
				strcpy(tmpstring,buf);
				//handle exponents
				if (exponents[i] > 1){
					strcat(tmpstring,"^");
					sprintf(buf,"%d",exponents[i]);
					strcat(tmpstring,buf);
				}
			//else append * and continue
			}else{
				strcat(tmpstring," * ");
				sprintf(buf,"%d",factors[i]);
				strcat(tmpstring,buf);
				//handle exponents
				if (exponents[i] > 1){
					strcat(tmpstring,"^");
					sprintf(buf,"%d",exponents[i]);
					strcat(tmpstring,buf);
				}
				
			}
		}	
		
		printf("%s\n",tmpstring);
		
}

/******************main***************************/
int main(int argc, char* argv[]){

	uint32_t resultlen=0;
	uint32_t exponents[MAX_FACTORS]={0};
	uint32_t factors[MAX_FACTORS]={0};
	uint32_t givenNumber=0;	


	//invalid number of args
	if ((argc!=2)){
		printf("invalid number of arguments\n");
		return 0;
		
	}else{
		
		if (checkInputs(argc,argv) > 0){
			givenNumber = checkInputs(argc,argv);
			//handle invalid inputs
			if (givenNumber == 0){
				printf("bad argument\n");
				return 0;
			}		
		}else{
			printf("bad argument\n");
			return 0;
			
		}	
		
	}
		
	//handle 1 as input
	if(givenNumber == 1){
		factors[0] = 1;
		exponents[0] = 1;
			
		printFactorsAndExponents(exponents,factors,1);
	}else{
			
				//find all primes before the givennumber
		if (findPrimesBefore(givenNumber,arraysrcprim,&resultlen) == 0){
			uint32_t cnt=0;
			bool doFlag=false;
			uint8_t cntFact=0;
			bool twomodedone=false;
	
			//number has to be expressed using primes and is itself not a prime, all primes before the number were found
			//check number 2 first
			do {
				if ((givenNumber % 2) == 0){
								
					if (cntFact < MAX_FACTORS-1){
						factors[cntFact] = 2;
						givenNumber /= 2;
						exponents[cntFact]++;					
						doFlag = true;
					}else{
						return -1;
					}
		
				}else{
					twomodedone = true ;
					
					if (doFlag == true){
						doFlag = false;	
						cntFact++;
					}
					
					
				}
			} while (twomodedone == false );
			
			//check rest of prime numbers
	
			
			do {
				if ((givenNumber % arraysrcprim[cnt]) == 0){
								
					if (cntFact < MAX_FACTORS-1){
						factors[cntFact] = arraysrcprim[cnt];
						givenNumber /= arraysrcprim[cnt];
						exponents[cntFact]++;
						doFlag = true;
						
							
					}else{
						return -1;
					}
		
				}else{
					cnt++;
					if (doFlag == true){
						doFlag = false;	
						cntFact++;
					}
				}
			} while (cnt<resultlen);
		
			
			printFactorsAndExponents(exponents,factors,cntFact);
			
	
			
			return 0;
		
		
		}else if(findPrimesBefore(givenNumber,arraysrcprim,&resultlen) == 2){
			//number given is already a prime so just return it.		
			factors[0] = givenNumber;
			exponents[0] = 1;
			
			printFactorsAndExponents(exponents,factors,1);
			return 0;
			
		}
			
	}
	
		
	return -1;
}