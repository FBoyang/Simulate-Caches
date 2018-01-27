#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int Assoc_Transfer(char* input){
	if(strcmp(input,"direct")==0){
		return 1;
	}
	
	if(strncmp(input,"assoc",5)==0){
		if(strlen(input)==5){
			return 0;
		}
		if(strlen(input)>=7){
			int assoc = atoi(input+6);
			if(assoc==0){
				return 0;
			}
			else{
				return assoc;
			}
		}
	}

	return -1;
}

struct entry{
	int v;
	unsigned long long int tag;
	struct entry *next;
};

struct Queue{
	int size;
	struct entry* front;
	struct entry* rear;
};

void Enqueue(struct Queue** queue,unsigned long long int tag){
	struct entry* temp = (struct entry*)malloc(sizeof(struct entry));
	temp->tag = tag;
	//int g = strlen(tag);
	//printf("size is %d\n",g);
	//strcpy(temp->tag,tag);
	//temp->v = 1;
	temp->next =NULL;
	//queue = malloc(sizeof(struct Queue*));
	if(*queue==NULL){

		//struct Queue* Nqueue = malloc(sizeof(struct Queue));
		//Nqueue ->front =NULL;
		//(Nqueue->front) = temp;
	
		//Nqueue->rear = temp;
		*queue = malloc(sizeof(struct Queue));
		(*queue)->front = temp;
		(*queue)->rear = temp;
		(*queue)->size=1;	
		return;
	}
	(*queue)->rear->next = temp;
	(*queue)->rear = (*queue)->rear->next;
	(*queue)->size++;
	return;
}

void Dequeue(struct Queue** queue){
	struct entry* temp = (*queue)->front;
	if((*queue)->front==(*queue)->rear){
		(*queue)->front =NULL;
		(*queue)->rear = NULL;
		(*queue)->size=0;
		*queue=NULL;
		return;
	}
	
	else{
		(*queue)->front = (*queue)->front->next;
	
	(*queue)->size--;
	free(temp);
	}
	return;
}

		
	




struct Hash_table{
	int size;
	struct Queue **table;
};

struct Hash_table *ini_hash(int setnum){
	struct Hash_table *Ntable = malloc(sizeof(struct Hash_table));
	Ntable->size = setnum;
	if(setnum>0){
		Ntable -> table =(struct Queue**) malloc(sizeof(struct Queue*)*setnum);
		return Ntable;
	}
	else{
		return NULL;
	}
	


}

/*char* H_to_B(char* hex){
	char Hvalue;
	char Bvalue[5];
	char* Baddress = malloc(50);
	int i =2;
	int length = strlen(hex);
	while(i<length){
		Hvalue = hex[i];
		switch(Hvalue){
			case '0': strcpy(Bvalue ,"0000"); break;
			case '1': strcpy(Bvalue ,"0001"); break;
			case '2': strcpy(Bvalue ,"0010"); break;
			case '3': strcpy(Bvalue ,"0011"); break;
			case '4': strcpy(Bvalue ,"0100"); break;
			case '5': strcpy(Bvalue ,"0101"); break;
			case '6': strcpy(Bvalue ,"0110"); break;
			case '7': strcpy(Bvalue , "0111");break;
			case '8': strcpy(Bvalue ,"1000"); break;
			case '9': strcpy(Bvalue , "1001");break;
			case 'a': strcpy(Bvalue , "1010");break;
			case 'b': strcpy(Bvalue , "1011");break;
			case 'c': strcpy(Bvalue ,"1100"); break;
			case 'd': strcpy(Bvalue , "1101");break;
			case 'e': strcpy(Bvalue , "1110");break;
			case 'f': strcpy(Bvalue , "1111");break;
		}
		strcat(Baddress,Bvalue);
		i++;
	}
	strcat(Baddress,"\0");
	printf("original is %s\n",Baddress);
	return Baddress;
}
*/
struct Information{
	//char *tag;
	//int index;
	char type;
	int Mem_read;
	int Mem_write;
	int hit;
	int miss;
};


struct Information* transfer(struct Information* A,unsigned long long int hex_address,struct Hash_table *table, int Block_bit, 
int Block_size, int Set_bit, int Tag_bit,char* type,char cache, int assoc){

	unsigned long long int Tag;
	if(cache=='A'){
		A->type='A';
	}
	else{
		A->type = 'B';
	}
	if(Tag_bit<=0||Tag_bit>=48){
		return NULL;
	}

	//char* bin_address = H_to_B(hex_address);
	
	//define function H_to_B
	//memcpy(bin_address,H_to_B(hex_address),50);
	//bin_address = H_to_B(hex_address);
	
	/*int elength = strlen(bin_address);
	int count;
	char* N_address = malloc(50);
	for(count=0; count<48-elength;count++){
		N_address[count] = '0';
	}
	//get the full address in N_address
	strcat(N_address,bin_address);
	*/
	//printf("The current binary address is: %s\n", N_address);
	//printf("iteration time: %d\n", t);
	
	Tag = hex_address;
	if(cache=='A'){
		Tag = Tag>>(Block_bit+Set_bit);
//	printf("A Tag is %lld\n",Tag);

}
		else{//type is B
		Tag = Tag<<Set_bit;
		
		Tag = Tag>>(Set_bit+Block_bit);
//	printf("B tag is %lld\n",Tag);
	}
		
	unsigned long long int index = 0;
	//int Power = 0;
	unsigned long long int divisor =0;
	if(cache=='A'){
		unsigned long long int temp = hex_address;
		temp = temp>>Block_bit;
		divisor = pow(2,Set_bit);
		index = temp%divisor;
	//printf("A index is %lld\n", index);
	
}
		else{//type is B
		divisor = pow(2,Block_bit+Tag_bit);
		index = hex_address/divisor;
	//printf("B index is %lld\n", index);
	}
	//struct Queue *queue = (table->table)[index];
	if((table->table)[index]==NULL){
		if(strcmp(type,"R")==0){
			//table->table[index] = malloc(sizeof(struct Queue));
			Enqueue(&(table->table[index]),Tag);
			A->Mem_read++;
			A->miss++;
		}
		else{//type is W
		Enqueue(&(table->table[index]),Tag);
		A->Mem_read++;
		A->Mem_write++;
		A->miss++;
		}
	}

	else{
		
			struct entry *temp = (table->table)[index]->front;
			int i = 0;
			if(strcmp(type,"R")==0){
				while(i<(table->table)[index]->size){
			
			//	printf("%s vs %s\n",temp->tag,Tag);	
					if(temp->tag==Tag){
						//printf("read hit\n");
						A->hit++;
						i=0;
						break;
					}
					temp = temp->next;
					i++;
				}
				if(i==(table->table)[index]->size){//not hit
					//printf("size is %d\n",i);
					A->miss++;
					A->Mem_read++;
					if((table->table)[index]->size==assoc){
						Dequeue(&(table->table[index]));
						//printf("dequeue happens\n");
					}
						Enqueue(&(table->table[index]),Tag);
					
				
					
					
				}
				

			}
		 	else{
				i=0;
				while(i<(table->table)[index]->size){
					//int a= strlen(temp->tag);
					//int b = strlen(Tag);
					//printf("W %s vs %s\n", temp->tag,Tag);
					if(temp->tag==Tag){
						//printf("%s vs %s\n",temp->tag,Tag);	
						//printf("Write hit\n");
						A->Mem_write++;
						A->hit++;
						break;
					}
					temp = temp->next;
					i++;
				}
				//printf("i is %d\n",i);
				if(i==(table->table)[index]->size){//not hit
					//printf("Write miss\n");
					A->miss++;
					A->Mem_read++;
					A->Mem_write++;
					//new add
					if((table->table)[index]->size ==assoc){
						Dequeue(&(table->table[index]));
					}
					Enqueue(&(table->table[index]),Tag);
				
				}		
			}	
		
		}

	return A;
}
			
			
void Outprint(struct Information *infor){
	if(infor->type =='A'){
		printf("cache A\n");
	}
	else{
		printf("cache B\n");
	}
	
	printf("Memory reads: %d\n",infor->Mem_read);
	printf("Memory writes: %d\n",infor->Mem_write);
	
printf("Cache hits: %d\n",infor->hit);
	printf("Cache misses: %d\n",infor->miss);
}

	
	 


void free_mem(struct Hash_table** A){
	int count;
	struct Queue* temp;
	for(count=0;count<(*A)->size;count++){
		temp = (*A)->table[count];
		while(temp!=NULL){
			Dequeue(&temp);
		}
	}
	free((*A)->table);
	free(*A);
}
		
