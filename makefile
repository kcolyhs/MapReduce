mapreduce: MapReduce.c MergeSort.c intsort.h wordcount.h MapReduce.h
	gcc MapReduce.c -o mapred -g -lpthread -lrt
clean:
	rm mapreduce
