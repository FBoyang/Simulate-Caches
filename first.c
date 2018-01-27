#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"first.h"

int main(int argc, char** argv){
	
	if(argc!=5){
		//printf("%d\n",argc);
		printf("input error\n");
			return 0;
	}
	


	//check error later
	int cache_size = atoi(argv[1]);
	
	
	
	
	int assoc_type;
	assoc_type=Assoc_Transfer(argv[2]);
	if(assoc_type==-1){
		printf("input error\n");
		return 0;
	}
	/*
 * 	assoc_type counts for the num of blocks inside of the set 
 * 	if input is assoc, then assoc_type = 0 because only one set;
 * 	if input is assoc:n, then assoc_type is n
 * 	if input is direct , then assoc_type is 1  
 * 	*/

 	int block_size = atoi(argv[3]);
	
	int setnum=0;
	
	if(assoc_type!=0)
	{
	setnum = cache_size/(assoc_type*block_size);
	}
	else{//here setnum is block number when assoc_type is 0
	setnum = cache_size/(block_size); 
	}

	if(assoc_type==0){//assoc_type=0
		assoc_type = setnum;
		setnum=1;
	}
	//note: need to check wether it is a number of 2 to the power
	int block_bit = (int)(log(block_size)/log(2));
	int set_bit = (int)(log(setnum)/log(2));
	int tag_bit  = 48-block_bit-set_bit;
	/*printf("assoc_type is: %d\n",assoc_type);
	printf("setnum is: %d\n",setnum);
	printf("block_size is: %d\n",block_size);
	printf("block_num is: %d\n", block_bit);
	printf("cache_size is: %d\n",cache_size);
*/
	/*need to create hashtable and corresponding function in header file */
	struct Hash_table* table_A  = ini_hash(setnum);
	struct Hash_table* table_B = ini_hash(setnum);
	struct Information* Infor_A=malloc(sizeof(struct Information));
	struct Information* Infor_B=malloc(sizeof(struct Information));
	//Information Infor_B;
	FILE *Fptr;
	Fptr = fopen(argv[4],"r");
	
	char PC[30];
	char type[2];
	unsigned long long int  Prime_addr;
//	int counter = 0;
	while(!feof(Fptr)){
		fscanf(Fptr,"%s %s %llx\n",PC, type, &Prime_addr);
		//printf("%lld", Prime_addr);
		if(strcmp(PC,"#eof")==0)
			break;
		char A = 'A';
		char B = 'B';	
		//printf("type is: %s\n",type);
		//information set stores tag value, index value, Memory reads, Memory write, Cache hits, Cache miss
		 transfer(Infor_A,Prime_addr,table_A,block_bit,block_size,set_bit,tag_bit,type,A,assoc_type);
		//Infor_B = B_transfer(Prime_addr,table_B,block_bit,block_size,set_bit,tag_bit,type);
		 transfer(Infor_B,Prime_addr,table_B,block_bit,block_size,set_bit,tag_bit,type,B,assoc_type);
	}
	
	Outprint(Infor_A);
	Outprint(Infor_B);	
	free_mem(&table_A);
	free_mem(&table_B);		
/*	*transfer hex into binary
 * 	variable hex; variable bin*/

	return 0;
}
	
