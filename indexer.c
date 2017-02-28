#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HASH_SIZE 50

struct linklistA{
	int total;
	int file_no;
	struct linklistA *next;
};

struct Node{
	char *word;
	struct Node *next;
	struct Node *prev;
	struct linklistA *A_content;
}*head=NULL;

struct Hash{
	struct Node *begin;
	int c;
}hash_table[HASH_SIZE];

int gen_key(char *token){
	return (strlen(token)%100);
}

void hashnode(struct Node *m,int key){
	if(hash_table[key].c==0){
		hash_table[key].begin=m;
		hash_table[key].c++;
	}
	else
		hash_table[key].c++;
}

struct linklistA* check(int pageno,struct linklistA *n){
	while(n!=NULL){
		if(n->file_no == pageno){
			return n;
		}
		n=n->next;
	}
	return NULL;
}

void create_linklistA(int pageno,struct Node *m){
	struct linklistA *n=malloc(sizeof(struct linklistA));
	n=m->A_content;
	if(n==NULL){
		struct linklistA* temp=malloc(sizeof(struct linklistA));
		temp->file_no=pageno;
		temp->total=1;
		temp->next=NULL;
		m->A_content=temp;
	}
	else{
		n=check(pageno,n);
		if(n==NULL){
			n=m->A_content;
			while(n->next!=NULL)
				n=n->next;
			struct linklistA *temp=malloc(sizeof(struct linklistA));
			temp->file_no=pageno;
			temp->total=1;
			temp->next=NULL;
			n->next=temp;
		
	}
		else
			m->A_content->total++;
	}
}

void load_into_linklist(char *token,int pageno,int count){
	int key=gen_key(token),flag=0;
	count=1;
	struct Node *m=malloc(sizeof(struct Node));
	m->word=token;
	if(head==NULL){
		head=m;
		m->next=NULL;
		m->prev=NULL;
		create_linklistA(pageno,m);
		hashnode(m,key);
	}
	else{
		if(hash_table[key].c==0){
			head->prev=m;
			m->next=head;
			m->prev=NULL;
			create_linklistA(pageno,m);
			head=m;
		}
		else{
			struct Node *n=hash_table[key].begin;
			int total=hash_table[key].c,i;
			for(i=0;i<hash_table[key].c;i++){
				if(strcmp(n->word,token)==0){
					create_linklistA(pageno,n);
					flag=1;
					break;
				}
				if(total!= 1)
					n=n->next;
				total--;
			}
			if(flag==0){
				if(n->next==NULL){
					n->next=m;
					m->prev=n;
					m->next=NULL;
					create_linklistA(pageno,m);
				}
				else{
					n->next->prev=m;
					m->next=n->next;
					m->prev=n;
					n->next=m;
					create_linklistA(pageno,m);
				}
			}
		}
		if(flag!=1)
			hashnode(m,key);
	}
}

void print(struct Node *head){
	struct Node *temp=head;
	while(temp!=NULL){
		printf("Word : %s\n",temp->word);
		struct linklistA *temp1=temp->A_content;
		while(temp1!=NULL){
			printf("File : %d || Total : %d\n", temp1->file_no,temp1->total);
			temp1=temp1->next;
		}
		temp=temp->next;
		printf("\n");
	}
	printf("\n");
}

char* extractTitle(char *html,char *word){
	char *res = malloc(100*sizeof(char));
	memset(res,0,(100*sizeof(char)));
	int i=0,c=0;
	while(*html!=EOF){
		char *str1=html;
		char *str2=word;
		while(*html!='\0' && *str2!='\0' && *html==*str2){
			html++;
			str2++;
		}
		if(!*str2){
			while(html[i]!='<' || html[i+1]!='/'){
				res[i]=html[i]; i++; c++;
			}
			return res;
		}
		html=str1+1;
	}
	return NULL;
}

void write_to_a_file(struct Node *head){
	FILE *fptr;
	fptr=fopen("/home/lakshay/haha/abc.txt","w");
	if(fptr==NULL){
		printf("Error");
		exit(1);
	}
	struct Node *temp=head;
	while(temp!=NULL){
		fprintf(fptr, "%s\n" , temp->word);
		struct linklistA *temp1=temp->A_content;
		while(temp1!=NULL){
			fprintf(fptr,"----->  File :%d || Total :%d\n", temp1->file_no,temp1->total);
			temp1=temp1->next;
		}
		temp=temp->next;
	}
	fclose(fptr);
}

/*
The read_from_file function reads all the words and pageno from a file named abc.txt
and stores it into the hash_table and makes a linked list of words..


void read_from_file(){
	FILE *fptr;
	fptr=fopen("/home/lakshay/haha/abc.txt","r");
	if(fptr==NULL){
		printf("Error");
		exit(1);
	}
	else{
		char page[3];
		int pageno,total;
		char line[1000];
		char *token;
		while(fgets(line,sizeof line,fptr)!=NULL){
			int i=0;
			if(line[i]!='-' && line[i]!='E' && line[i+1]!='N' && line[i+2]!='D'){
				token=line;
				printf("%s", token);
			}
			else if(line[i]=='-'){
				int j=0,k=0;
				while(line[j]!=':')
					j++;
				while(line[j]!=' '){
					page[k]=line[j+1];
					j++; k++;
				}
				pageno=atoi(page);
				j=j+1; k=0;
				printf("%d  ",pageno);
				while(line[j]!=':')
					j++;
				while(line[j]!=' '){
					page[k]=line[j+1];
					j++; k++;
				}
				total=atoi(page);
				printf("%d\n" ,total);
			}
			//load_into_linklist(token,pageno,total);
		}
		fclose(fptr);
	}
}
*/

int main(){
	struct Node *linklist_head;
	FILE *fl;
	int page=0;
	while(page<25){
		char name[100];
		//printf("%d\n", page);
		long long int file_size,last_char_pos=0;
		sprintf(name,"/home/lakshay/haha/file%d.txt",page);
		fl=fopen(name,"r");
		fseek(fl, 0, SEEK_END);
		file_size = ftell(fl);
		fseek(fl, 0, SEEK_SET);
		//printf("\n%lld\n",file_size);
		char *html;
		html = (char*) malloc (file_size+100);
		memset(html,0,(file_size + 100));

		do{
			html[last_char_pos]=fgetc(fl);
			last_char_pos++;	
		}while(html[last_char_pos-1]!=EOF);
		html[last_char_pos-1]='\0';

		char *res=extractTitle(html,"<title>");
		//printf("%s", res);
		if(res!=NULL){
			const char delimit[5]=" -,/";
			char *token;
			token=strtok(res,delimit);
			while(token!=NULL){
				//printf("token is : %s\n", token);
				//printf("page is : %d\n",page);
				load_into_linklist(token,page,1);
				token=strtok(NULL, delimit);	
			}
		}
		free(html);
		page++;
	}
	//print(head);
	write_to_a_file(head);
	//read_from_file();
}
