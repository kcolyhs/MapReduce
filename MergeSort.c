#include "wordcount.h"
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>




void merge(int l, int m, int r, veclist *list){
	int l1,l2,i;
	veclist *tmpList = createVecList(list->length);//(veclist*)malloc(sizeof(list));
	//	tmpList->array = malloc(sizeof(list->array));
	for (l1=l,l2=m+1,i=l;l1<=m && l2<=r; i++){
		if(strcmp(list->array[l1].word,list->array[l2].word)<=0){
			tmpList->array[i]=list->array[l1++];

		}else{
			tmpList->array[i]=list->array[l2++];
		}
	}
	while(l1<=m){
		tmpList->array[i++]=list->array[l1++];
	}
	while(l2<=r){
		tmpList->array[i++]=list->array[l2++];
	}
	for(i=l; i<=r; i++){
		list->array[i]=tmpList->array[i];
	}
}

void mergeInt(int l, int m, int r, intvec_list *list){
	int l1,l2,i;
	intvec_list *tmpList = createIntVecList(list->length);

	for (l1=l,l2=m+1,i=l;l1<=m && l2<=r; i++){
		if(list->array[l1].value < list->array[l2].value){
			tmpList->array[i]=list->array[l1++];

		}else{
			tmpList->array[i]=list->array[l2++];
		}
	}
	while(l1<=m){
		tmpList->array[i++]=list->array[l1++];
	}
	while(l2<=r){
		tmpList->array[i++]=list->array[l2++];
	}
	for(i=l; i<=r; i++){
		list->array[i]=tmpList->array[i];
	}
}

void mergeProc(int l, int m, int r,int length){
	int l1,l2,i;
	char (*testing)[30];
	int shm_fd = shm_open("OS", O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, (length+1)*30);
	testing =mmap(0,(length+1)*30, PROT_READ | PROT_WRITE, MAP_SHARED , shm_fd, 0);

	int after_fd;
	after_fd=shm_open("after", O_CREAT | O_RDWR, 0666);
	ftruncate(after_fd, (length+1)*30);
	char (*after)[30];
	after = mmap(0,(length+1)*30, PROT_READ | PROT_WRITE, MAP_SHARED, after_fd, 0);


	int after_sort;
	after_sort=shm_open("aftersort", O_CREAT | O_RDWR, 0666);
	ftruncate(after_sort, (length+1)*30);
	char (*aftersort)[30];
	aftersort = mmap(0,(length+1)*30, PROT_READ | PROT_WRITE, MAP_SHARED, after_sort, 0);


	for (l1=l,l2=m+1,i=l; l1<=m && l2<=r; i++){
		if(strcmp(testing[l1],testing[l2])<=0){
			strcpy(after[i],testing[l1++]);
		}else{
			strcpy(after[i], testing[l2++]);
		}
	}
	while(l1<=m){
		strcpy(after[i++],testing[l1++]);
	}
	while(l2<=r){
		strcpy(after[i++],testing[l2++]);
	}
	for(i=l; i<=r; i++){
		strcpy(testing[i],after[i]);
		//printf("%s\n", aftersort[i]);
	}
	//printf("%s\n","in sort");
	//shm_unlink("OS");
	//shm_unlink("after");
	shmdt(testing);
	shmctl(shmget(shm_fd,(length+1) * 30,O_CREAT | O_RDWR),IPC_RMID,NULL);
	shmdt(after);
	shmctl(shmget(after_fd,(1+length)*30,O_CREAT | O_RDWR), IPC_RMID, NULL);
	//munmap(testing, (length+1) *30);
	munmap(after_sort, (length+1) *30);
}



void mergeIntProc(int l, int m, int r,int length){
	int shm_fd = shm_open("OS", O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, length*sizeof(int));

	int *testing;
	testing =mmap(0,length*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	int l1,l2,i;
	

	        int after_fd;
        after_fd=shm_open("after", O_CREAT | O_RDWR, 0666);
        ftruncate(after_fd, length*sizeof(int));
        int *after;
        after = mmap(0,length*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, after_fd, 0);



	for (l1=l,l2=m+1,i=l; l1<=m && l2<=r; i++){
		if(testing[l1]<=testing[l2]){
			after[i]=testing[l1++];
		}else{
			after[i]=testing[l2++];
		}
	}
	while(l1<=m){
		after[i++]=testing[l1++];
	}
	while(l2<=r){
		after[i++]=testing[l2++];
	}
	for(i=l; i<=r; i++){
		testing[i]=after[i];
	}
	munmap(testing, length*sizeof(int));
	munmap(after, length*sizeof(int));
}

void mergeSort(int l, int r, veclist *list){

	if(l<r){
		int m = (l+r)/2;
		mergeSort(l,m,list);
		mergeSort(m+1,r,list);
		merge(l,m,r,list);
	}
}


void mergeSortInt(int l, int r, intvec_list *list){

	if(l<r){
		int m = (l+r)/2;
		mergeSortInt(l,m,list);
		mergeSortInt(m+1,r,list);
		mergeInt(l,m,r,list);
	}
}


void mergeSortIntProc(int l, int r, int length){
	if(l<r){
		int m = (l+r)/2;
		mergeSortIntProc(l,m,length);
		mergeSortIntProc(m+1,r,length);
		mergeIntProc(l,m,r,length);
	}

}

void mergeSortProc(int l, int r, int length){
	if(l<r){
		int m = (l+r)/2;
		mergeSortProc(l,m,length);
		mergeSortProc(m+1,r,length);
		mergeProc(l,m,r,length);
	}
}
void* mergeThreaded(void* arg){
	veclist *list = (veclist*)arg;
	int l = 0;
	int r = list->length-1;
	mergeSort(l,r,list);

}
