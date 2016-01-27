#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


pthread_mutex_t lock;
pthread_cond_t self;

typedef struct node{
	int temp;
	struct node * next;
}node_t;

void performLL(node_t * node);
node_t *createNode(int myTemp);
void forward(node_t **currentNode);
void insertEnd(node_t **head, node_t *addedNode);
void insertLL(int *id);
void temperature(int *id);

node_t * linkedList;
int NumOfLocs;
int *array = 0;
int completed;

int main (){
	linkedList = NULL;
	int *id;
	completed = 0;
	int min = 0;
	//int z = 0;
	printf("Number of sensor locations: ");
	scanf("%d", &NumOfLocs);
	array = malloc(sizeof(int)*NumOfLocs);
	int index;
 	for(index = 0; index < NumOfLocs; index++){
		array[index] = 0;
	}
	pthread_t sensors[NumOfLocs];
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&self, NULL);
	int count = 0;

	while (count < NumOfLocs){
		id = (int *)malloc(sizeof(int));
		*id = count;
		pthread_create(&(sensors[count]), NULL, (void *)temperature, id);
		count ++;
	}

	for(count = 0; count < NumOfLocs; count++){
		pthread_join(sensors[count], NULL);
	}
	printf("Done!\n");
	return;
}

node_t *createNode(int myTemp){
	node_t *p;
	p = (node_t *) malloc (sizeof(node_t));
	p->temp = myTemp;
	p->next = NULL;
	return p;
}

void forward(node_t **currentNode){
	if((*currentNode)->next != NULL){
		*currentNode=(*currentNode)->next;
		} 
	else {
		printf("Reached the end\n");
	}
}

void insertEnd(node_t **head, node_t *addedNode){
	node_t *p;
	if(*head == NULL) {
		*head = addedNode;
		return;
	}
	p = *head;
	while(p->next != NULL){
		p=p->next;
	}
	p->next = addedNode;
	return;
}

int findMin(int a[]){
	int min = a[0];
	int i;
	for (i = 1; i < NumOfLocs; i++){
		if(a[i] < min){
			min = a[i];
		}
	}
	return min;
} 

void performLL(node_t * node){
	node_t *tempNode;
	tempNode = node;
	int sum = 0;
	if (tempNode == NULL) {
		printf("list is empty\n"); 
		return;
	}
	else {
		while(tempNode->next != NULL){
			printf("Temp = %d, ", tempNode -> temp);
			sum = sum + (tempNode -> temp);
			forward(&tempNode);
		}
		printf("Temp = %d\n", tempNode -> temp);	
		sum = sum + (tempNode -> temp);
		printf("Average Temp = %f\n\n", (double)sum/(double)NumOfLocs);
		linkedList = NULL;
	return;
	}
}

void temperature(int *id){
	int count = 0;
	while(completed < 10){
		while(count < 10){
			pthread_mutex_lock(&lock);
			while(completed < array[*id]){
				printf("%d is waiting\n", *id);
				pthread_cond_wait(&self, &lock);
			}
			printf("%d is inserting\n", *id);
			insertLL(id);
			array[*id] = (array[*id] + 1);
			pthread_cond_broadcast(&self);
			pthread_mutex_unlock(&lock);
			if(findMin(array) > completed) {
				performLL(linkedList);
				completed ++;
			}
			count++;
		}
	}
	pthread_exit(NULL);
	return;
}

void insertLL(int *id){
	int temp;
	node_t *tempNode;
	temp = rand() % 100 - 30;
	tempNode = createNode(temp);
	insertEnd(&linkedList, tempNode);
	printf("sensor %d has temp %d\n", *id, temp);
	return;
}
