#include <stdio.h>
#include <memory.h>
#include <locale.h>
#include <Windows.h>

void PrintResult(unsigned char* result, int count)
{
	for (int i = 0; i < count; i++)
		printf("%d", result[i]);
	printf("\n");
}

int low_range(double* statistic, int range, char ch)
{
	return statistic[ch - 1] * range;
}

int high_range(double* statistic, int range, char ch)
{
	return statistic[ch] * range - 1;
}

int Code(double* statistic, char* name, unsigned char* result, int *countDigit )
{
	int count = 0;
	*result = 0;
	while (name[count] != NULL && name[count] != '\n')
	{
		statistic[name[count]]++;
		count++;
	}

	statistic[0] = 0;
	for (int i = 1; i < 255; i++)
		statistic[i] = statistic[i - 1] + (statistic[i] / ((double)count));

	int high = 99999;
	int low = 0;
	int underflow_bits = 0;
	printf("high\tlow\tOutputDigit\tunderflow_bits\n");

	for (int i = 0; i < count; i++)
	{
		int temp = high;
		high = low + high_range(statistic, high - low + 1, name[i]);
		low = low + low_range(statistic, temp - low + 1, name[i]);
		printf("%d\t%d\n", low, high);
		if ((int)(high / 10000) == (int)(low / 10000))
		{
			result[*countDigit] = high / 10000;
			(*countDigit)++;
			printf("%d\t%d\t", low, high);
			PrintResult(result, *countDigit);
			while (underflow_bits)
			{
				result[*countDigit] = 9;
				(*countDigit)++;
				printf("%d\t%d\t", low, high);
				PrintResult(result, *countDigit);
				--underflow_bits;
			}
			high -= (high / 10000) * 10000;
			high = high * 10 + 9;
			low -= (low / 10000) * 10000;
			low = low * 10;
			printf("%d\t%d\t", low, high);
			PrintResult(result, *countDigit);
		}
		else
		{
			while (((int)(high / 10000) - (int)(low / 10000)) == 1 &&
				(high / 1000) % 10 == 0 && (low / 1000) % 10 == 9)
			{
				high = (high % 1000) * 10 + (high / 10000) * 10000 + 9;
				low = (low % 1000) * 10 + (low / 10000) * 10000;
				underflow_bits++;
				printf("%d\t%d\t\t\t%d\n", low, high, underflow_bits);
			}
		}
	}

	int tempData = (high + low) / 2;
	result[*countDigit] = tempData / 10000;
	(*countDigit)++;
	result[*countDigit] = tempData / 1000 % 10;
	(*countDigit)++;
	result[*countDigit] = tempData / 100 % 10;
	(*countDigit)++;
	result[*countDigit] = tempData / 10 % 10;
	(*countDigit)++;
	result[*countDigit] = tempData % 10;
	(*countDigit)++;
	return count;
}

char GetDecodeChar(double* statistic, int code)
{
	int range = 100000;
	for (int i = 1; i < 255; i++)
	{
		if ((int)(statistic[i - 1] * range) <= code &&
			code < (int)(statistic[i] * range) &&
			(int)(statistic[i - 1] * range) != (int)(statistic[i] * range))
		{
			printf("%d\t%c\t%d - %d\n", code, (char)i, (int)(statistic[i - 1] * range), (int)(statistic[i] * range));
			return (char)i;
		}
	}
	return (char)' ';
}

void Decode(double* statistic, unsigned char* result, int countDigit, int countB)
{
	int data = 0, i;
	for (i = 0; i < 5; i++)
		data = data * 10 + result[i];

	while (data && countB)
	{
		unsigned char index = GetDecodeChar(statistic, data);
		data = data * (1.0 / (statistic[index] - statistic[index - 1]));
		data = data % 100000;
		countB--;
	}
}

int main()
{
	char word[32];
	fgets(word, 32, stdin);
	
	double* statistic = new double[255];
	memset(statistic, 0.0, sizeof(double) * 255);
	unsigned char *result = new unsigned char[255];
	memset(result, 0, sizeof(unsigned char) * 255);
	int count = 0;
	printf("\n--------------------------Code----------------------------------\n");
	int countB = Code(statistic, word, result, &count);
	printf("\n-------------------------Decode---------------------------------\n");
	Decode(statistic, result, count, countB);
}