#include "wordcount.h"

void merge(int l, int m, int r, veclist *list){
	int l1,l2,i;
	veclist *tmpList = (veclist*)malloc(sizeof(list));
	tmpList->array = malloc(sizeof(list->array));
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



void mergeSort(int l, int r, veclist *list){
	if(l<r){
		int m = (l+r)/2;
		mergeSort(l,m,list);
		mergeSort(m+1,r,list);
		merge(l,m,r,list);
	}
}
