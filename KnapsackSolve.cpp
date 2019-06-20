#pragma warning(disable:4996)
#include <iostream>
#include <stdio.h>
#include<vector>

using namespace std;
int main(int argc, char* argv[])
{
	const char* filepath;
	if (argc > 1)
		filepath = argv[1];
	else
		filepath = "C:\\Users\\Flavio\\Documents\\PUC\\2019.1\\Análise de Algoritmos\\aa-trab3\\instances\\inst8.txt";

	FILE* file = freopen(filepath, "r", stdin);
	if (file == NULL)
	{
		cout << "Error! Failed to find file. Check path parameter used.";
	}

	int numItems, bagSize;
	cin >> numItems >> bagSize;

	vector<int> itemValues(numItems);
	vector<int> itemWeights(numItems);
	for (int i = 0; i < numItems; i++)
	{
		cin >> itemValues[i] >> itemWeights[i];
	}

	// states we have visited and know how to solve
	vector<vector<vector<int>>> memorizedStates(numItems + 1, vector<vector<int>>(bagSize + 1, vector<int>(11)));

	// item choices for each possible state
	vector<vector<vector<int>>> itemChoices(numItems + 1, vector<vector<int>>(bagSize + 1, vector<int>(11)));

	for (int i = 0; i <= numItems; i++)
	{
		for (int w = 0; w <= bagSize; w++)
		{
			for (int c = 0; c <= 10; c++)
			{
				if (i == 0 || w == 0 || c == 0)
				{
					memorizedStates[i][w][c] = 0;
					itemChoices[i][w][c] = 0;
				}
				else
				{
					if (itemWeights[i - 1] > w)
					{
						memorizedStates[i][w][c] = memorizedStates[i - 1][w][10];
						itemChoices[i][w][c] = itemChoices[i - 1][w][10];
					}
					else
					{
						// Choice: use or not use
						int useItem;
						if (c - 1 > 0)
							useItem = itemValues[i - 1] + memorizedStates[i][w - itemWeights[i - 1]][c - 1];
						else
							useItem = itemValues[i - 1] + memorizedStates[i - 1][w - itemWeights[i - 1]][10];

						int notUse = memorizedStates[i - 1][w][10];

						// We will use the best
						if (useItem >= notUse)
						{
							memorizedStates[i][w][c] = useItem;
							itemChoices[i][w][c] = i;
						}
						else
						{
							memorizedStates[i][w][c] = notUse;
							itemChoices[i][w][c] = itemChoices[i - 1][w][c];
						}
					}
				}
			}
		}
	}

	// Getting items used
	vector<int> itemsUsed(numItems);
	int chosenItem = numItems;
	int weightLeft = bagSize;
	int count = 10;

	while (chosenItem != 0)
	{
		int item; 

		if (count > 0)
			item = itemChoices[chosenItem][weightLeft][count];
		else
			item = itemChoices[chosenItem - 1][weightLeft][10];

		if (item == chosenItem)
		{
			count--;
		}
		else if (item == 0)
		{
			break;
		}
		else
		{
			chosenItem = item;
			count = 9;
		}

		weightLeft -= itemWeights[chosenItem - 1];

		itemsUsed[chosenItem - 1]++;
	}

	// Writing best value
	cout << "Melhor valor obtido na instancia: " << memorizedStates[numItems][bagSize][10] << endl;

	// Writing used items with counts
	for (int i = 0; i < numItems; i++)
	{
		cout << "Foram usados " << itemsUsed[i] << " do item numero " << i + 1 << endl;
	}

}