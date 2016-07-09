#include "bitArray.h"

#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

// op = 1 => set bit at ndx
// op = 0 => clear bit at ndx
// op = -1 => toggle bit at ndx

BITARRAY_DEF(bit, 20);

#define sizeInBits(x) (sizeof(x) << 3)

int modifyBitArray(BITARRAY_MU_TYPE *bitArr, int len, int ndx, int op)
{
	if (ndx >= len)
		return -1;
	int elemNum = floor(ndx, sizeInBits(BITARRAY_MU_TYPE));
	int bitPos =  ndx % sizeInBits(BITARRAY_MU_TYPE);

	if (!op) //clear bit op = 0
		bitClear(bitArr[elemNum], bitPos);
	else if (op > 0) //set bit op = 1
		bitSet(bitArr[elemNum], bitPos);
	else //toggle bit op = -1
		bitToggle(bitArr[elemNum], bitPos);
	return 0;
}

int readBitArray(BITARRAY_MU_TYPE *bitArr, int len, int ndx)
{
	if (ndx >= len)
		return -1;

	int elemNum = floor(ndx, sizeInBits(BITARRAY_MU_TYPE));
	int bitPos = ndx % sizeInBits(BITARRAY_MU_TYPE);
	return bitCheck(bitArr[elemNum], bitPos);
}

bool isBitArrClearTillPos(BITARRAY_MU_TYPE *bitArr, int len, int bitPos)
{
	if (bitPos >= len)
		return false;
	int elemNum = floor(bitPos, sizeInBits(BITARRAY_MU_TYPE));
	int lastBitPos = bitPos % sizeInBits(BITARRAY_MU_TYPE);
	printf("Bit - %d\n", elemNum);

	int i;
	for (i = 0; i < elemNum; i++)
		if (bitArr[i])
			return false;
	for (i = 0; i < lastBitPos; i++)
		if (bitCheck(bitArr[elemNum], i))
			return false;

	return true;
}

bool isBitArrSetTillPos(BITARRAY_MU_TYPE *bitArr, int len, int bitPos)
{
	if (bitPos >= len)
		return false;
	int elemNum = floor(bitPos, sizeInBits(BITARRAY_MU_TYPE));
	int lastBitPos = bitPos % sizeInBits(BITARRAY_MU_TYPE);

	int i;
	BITARRAY_MU_TYPE max;
	memset((void *)&max, 0xFF, sizeof(BITARRAY_MU_TYPE));
	for (i = 0; i < elemNum; i++)
		if (bitArr[i] < max)
			return false;
	for (i = 0; i < lastBitPos; i++)
		if (!(bitCheck(bitArr[elemNum], i)))
			return false;
	return true;
}

bool clearBitArray(BITARRAY_MU_TYPE *bitArr, int len, int bitPos)
{
	if (bitPos > len)
		return false;

	int elemNum = floor(bitPos, sizeInBits(BITARRAY_MU_TYPE));
	int lastBitPos = bitPos % sizeInBits(BITARRAY_MU_TYPE);

	memset((void *)bitArr, 0, elemNum * sizeof(BITARRAY_MU_TYPE));
	int i;
	for (i = 0; i < lastBitPos; i++)
		bitClear(bitArr[elemNum], i);
	return true;
}

bool setBitArray(BITARRAY_MU_TYPE *bitArr, int len, int bitPos)
{
	if (bitPos > len)
		return false;
	int elemNum = floor(bitPos, sizeInBits(BITARRAY_MU_TYPE));
	int lastBitPos = bitPos % sizeInBits(BITARRAY_MU_TYPE);

	memset((void *)bitArr, 0xFF, elemNum * sizeof(BITARRAY_MU_TYPE));
	int i;
	for (i = 0; i < lastBitPos; i++)
		bitSet(bitArr[elemNum], i);
	return true;
}

//#define __MAIN__

#ifdef __MAIN__

void printBitArray()
{
	int i = 0;
	char str[50];
	str[0] = '\0';
	for (; i < 20; i++) {
		readBitArray(bitArr_bit, 20, i) ? strcat(str, "1") : strcat(str, "0");
	}
	printf("Bit Array is %.*s\n", 20, str);
}

int main()
{

	//modifyBitArray(bitArr_bit, 20, 10, 1);
	setBitArray(bitArr_bit, 20, 5);
	//clearBitArray(bitArr_bit, 20, 5);
	modifyBitArray(bitArr_bit, 20, 0, -1);
	modifyBitArray(bitArr_bit, 20, 0, -1);
	if (isBitArrSetTillPos(bitArr_bit, 20, 4))
		printf("Yes it is set\n");
	if (isBitArrClearTillPos(bitArr_bit, 20, 5))
		printf("Clear\n");
	printBitArray();

}

#endif
