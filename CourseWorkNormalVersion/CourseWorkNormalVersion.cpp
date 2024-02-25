#include <stdio.h>
#include <memory.h>

#include <Windows.h>

int low_range(double* statistic, char ch)
{
	double sum = 0.0;
	for (int i = 0; i < (int)ch; i++)
		sum += statistic[i];
	return sum;
}

int high_range(double* statistic, char ch)
{
	double sum = 0.0;
	for (int i = 0; i <= (int)ch; i++)
		sum += statistic[i];
	return sum;
}

void UpdateStat(double* statistic, double range)
{
	for (int i = 0; i < 255; i++)
		statistic[i] = statistic[i] / range;
}

void CreateStatisticModel(double* statistic, char* name)
{

	int count = 0;
	while (name[count] != NULL && name[count] != '\n')
	{
		statistic[name[count]]++;
		count++;
	}

	for (int i = 0; i < 255; i++)
		statistic[i] = statistic[i] / ((double)count);

	//--------------------------------------------------

	int high = 100000;
	int low = 0;
	for (int i = 0; i < count; i++)
	{
		UpdateStat(statistic, (high - low) / high);
		high = low + high_range(statistic, name[i]);
		low = low + low_range(statistic, name[i]);
	}
}

int main()
{
	char word[32];
	fgets(word, 32, stdin);
	rewind(stdin);
	double* statistic = new double[255];
	memset(statistic, 0.0, sizeof(double) * 255);
	CreateStatisticModel(statistic, word);
}