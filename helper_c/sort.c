/*
 * sort.c
 *
 *  Created on: 05-Apr-2016
 *      Author: abhiarora
 */

#include <stdio.h>
#include <stdint.h>

inline void swap(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void swap(int *a, int *b);


int bubbleSort(int *arr, unsigned int len)
{
	unsigned i,j;
	for (i=1; i<len; i++) {
		for (j=1; j<=len-i; j++) {
			if (arr[j] < arr[j-1])
				swap(arr+j, arr+j-1);
		}
	}

	return 0;
}

int insertSort(int *arr, unsigned len)
{
	int i,j;

	for (i=0; i<len; i++) {
		int current = arr[i];
		for (j=i-1; j>=0 && (current < arr[j]); j--)
			arr[j+1] = arr[j];
		if ((i-1) != j)
			arr[j+1] = current;
	}
	return 0;
}

int selectionSort(int *arr, unsigned len)
{
	int i,j,ndx;

	for (i=len-1; i>=0; i--) {
		for (j=1, ndx=0; j<=i; j++)
			if (arr[ndx] < arr[j])
				ndx = j;
		if (i == ndx)
			continue;
		swap(arr+i, arr+ndx);
	}
	return 0;
}


int shellSort(int *arr, unsigned len)
{
	int gap = len/2;
	int i,j,k;
	while (gap > 0) {
		for (i=0; i<gap; i++) {
			for (j=i; j<len; j+=gap) {
				int current = arr[j];
				for (k=j-gap; k>=i && ((current < arr[k])); k-= gap)
					arr[k+gap] = arr[k];
				arr[k+gap] = current;
			}
		}
		gap /= 2;
	}
	return 0;
}

void merge_sort(int *arr, unsigned start, unsigned end)
{
	int len = end - start;
	if (len <= 1)
		return;
	unsigned half=(start+end) / 2;

	merge_sort(arr, start, half);
	merge_sort(arr, half, end);


	int i,j,current;
	int li_len=half-start;
	int li[li_len];

	for (i=0, j=start; i<li_len && j<half; i++, j++)
		li[i]=arr[j];

	for (i=0, current=start, j=half; i<li_len && j<end; current++) {
		if (li[i] > arr[j])
			arr[current] = arr[j++];
		else
			arr[current] = li[i++];
	}

	for (; i<(half-start); i++)
		arr[current++] = li[i];
	for (;j<end; j++)
		arr[current++]=arr[j];

	return;
}

int mergeSort(int *arr, unsigned len)
{
	merge_sort(arr, 0, len);
	return 0;
}

unsigned pivotValue(int *arr, unsigned start, unsigned end)
{
	return start;
}

void quickSortHelper(int *arr, unsigned start, unsigned end)
{
	if (start >= end)
		return;
	unsigned pivot = pivotValue(arr, start, end);
	unsigned left=start+1;
	unsigned right=end-1;

	REPEAT:
	while (arr[left] <= arr[pivot] && right >= left)
		left++;
	while (arr[right] >= arr[pivot] && right >= left)
		right--;
	if (left > right)
		goto DONE;
	swap(arr+right, arr+left);
	goto REPEAT;

	DONE: swap(arr+right,arr+pivot);

	quickSortHelper(arr, start, right);
	quickSortHelper(arr, right+1, end);
	return;
}

int quickSort(int *arr, unsigned len)
{
	quickSortHelper(arr, 0, len);
	return 0;
}


#define __MAIN__

#ifdef __MAIN__
int main()
{
	int arr[] = {5,8,3,2,1,4,9,0,20, -1, -2, -10, 100};
	printf("Size - %d\n", sizeof(arr)/sizeof(arr[0]));
	quickSort(arr, sizeof(arr)/sizeof(arr[0]));
	int i;
	printf("Hello World\n");
	for (i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		printf("%d, ", arr[i]);

	scanf("%d", &i);

	return 0;
}



#endif
