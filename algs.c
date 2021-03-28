#include <stdio.h>

void ToPascal(int counter)
{
	if(counter == 0)
	{
		printf("[0,0]");
	}
	else
	{
		for(int i = 0; i < counter + 1; i++)
		{
			printf("[%d,%d]", counter, i);
		}
	}
}

void Expand(int binArray[], int arrayLength)
{
	int counter = 0;
	for(int i = 0; i < arrayLength; i++)
	{
		if(binArray[i] == 0)
		{
			counter++;
		}
		else
		{
			printf("%d", counter);
			ToPascal(counter);
			putchar('\n');
			counter++;
		}
	}
	
}

int main()
{
	int binArray[] = {1,0,0,1,0,1,0,0,0,1};
	int arrayLength = sizeof(binArray)/sizeof(binArray[0]);
	Expand(binArray, arrayLength);
	return 0;
}
