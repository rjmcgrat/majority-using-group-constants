#include <stdlib.h>
int counts[2];
typedef int bool;
enum { false, true };
int referenceArray[4];

struct node
{
	int result;
	int arrayIndex;
};


void resolveSameOrOppositeResult(int thirdQueryResult)
{
	if (thirdQueryResult == 4)
		counts[0] += 2;
	else if (thirdQueryResult == 0)
		counts[1] += 2;
}


bool resolveRemaining3Same(int * result2Array, int num2Count, int * opposite)
{
	int remainder = num2Count % 2;
	int num3Same2Group = num2Count / 2;
	for (int i = 0; i < num3Same2Group; ++i)
	{
		int j = 2 * i;
		int elementArray[4];
		elementArray[0] = result2Array[j] + 2;
		elementArray[1] = result2Array[j] + 3;
		elementArray[2] = result2Array[j + 1] + 2;
		elementArray[3] = result2Array[j + 1] + 3;
		int queryResult = QCOUNT(1, elementArray);
		if (queryResult != 0 && queryResult != 2 && queryResult != 4)
			return false;
		if (queryResult == 4)
		{
			elementArray[0] = referenceArray[0];
			elementArray[1] = referenceArray[1];
			int secondQueryResult = QCOUNT(1, elementArray);
			if (secondQueryResult == 0)
				counts[1] += 4;
			else if (secondQueryResult == 4)
				counts[0] += 4;
		}
	}
	if (remainder > 0)
	{
		int elementArray[4];
		elementArray[0] = result2Array[num2Count - remainder];
		elementArray[1] = result2Array[num2Count - remainder] + 1;
		if (!(handleRemainder(2, elementArray, 1, opposite)))
			return false;
		elementArray[0] = result2Array[num2Count - remainder] + 2;
		elementArray[1] = result2Array[num2Count - remainder] + 3;
		return handleRemainder(2, elementArray, 1, opposite);
	}
	return true;
}

bool handleThreeSame(struct node * Result2Array, int num2Count, int * opposite)
{
	int * toComputeLater = malloc(num2Count * sizeof(int));
	int numElement = 0;
	for (int i = 0; i < num2Count; ++i)
	{
		int elementArray[4];
		elementArray[0] = referenceArray[0];
		elementArray[1] = referenceArray[1];
		elementArray[2] = Result2Array[i].arrayIndex;
		elementArray[3] = Result2Array[i].arrayIndex + 1;
		int queryResult = QCOUNT(1, elementArray);
		if (queryResult != 0 && queryResult != 2 && queryResult != 4)
		{
			free(toComputeLater);
			return false;
		}
		if (queryResult == 4)
			counts[0] += 2;
		else if (queryResult == 0)
			counts[1] += 2;
		else if (queryResult == 2)
		{
			toComputeLater[numElement] = Result2Array[i].arrayIndex;
			++numElement;
		}
	}
	bool result = true;
	if (numElement > 0)
		result = resolveRemaining3Same(toComputeLater, numElement, opposite);
	free(toComputeLater);
	return result;
}

bool handleAllSame(struct node * Result4Array, int num4Count, int * opposite)
{
	int num4CountGroup4 = num4Count / 4;
	int num4CountRemainder = num4Count % 4;
	for (int i = 0; i < num4CountGroup4; ++i)
	{
		int j = i * 4;
		int elementArray[4];
		elementArray[0] = Result4Array[j].arrayIndex;
		elementArray[1] = Result4Array[j + 1].arrayIndex;
		elementArray[2] = Result4Array[j + 2].arrayIndex;
		elementArray[3] = Result4Array[j + 3].arrayIndex;
		int queryResult = QCOUNT(1, elementArray);
		if (queryResult != 0 && queryResult != 2 && queryResult != 4)
			return false;
		if (queryResult == 4)
		{
			elementArray[0] = referenceArray[0];
			elementArray[1] = referenceArray[1];
			int secondresult = QCOUNT(1, elementArray);
			if (secondresult == 4)
				counts[0] += 16;
			else if (secondresult == 0)
			{
				opposite[0] = elementArray[3];
				opposite[1] = elementArray[2];
				counts[1] += 16;
			}
		}
		else if (queryResult == 2)
		{
			int last1 = elementArray[2];
			int last2 = elementArray[3];
			elementArray[2] = referenceArray[0];
			elementArray[3] = referenceArray[1];
			int secondQueryResult = QCOUNT(1, elementArray);
			if (secondQueryResult != 0 && queryResult != 2 && queryResult != 4)
				return false;
			if (secondQueryResult == 4)
				counts[0] += 8;
			else if (secondQueryResult == 2)
			{
				elementArray[0] = last1;
				elementArray[1] = last2;
				elementArray[2] = referenceArray[0];
				elementArray[3] = referenceArray[1];
				int thirdQueryResult = QCOUNT(1, elementArray);
				if (thirdQueryResult != 0 && queryResult != 2 && queryResult != 4)
					return false;
				if (thirdQueryResult == 0)
					counts[1] += 8;
				else if (thirdQueryResult == 4)
					counts[0] += 8;
				else
					return false;
			}
			else if (secondQueryResult == 0)
				counts[1] += 8;
		}
		else if (queryResult != 0)
			return false;
	}
	if (num4CountRemainder > 0)
	{
		int elementArray[4];
		for (int i = 0; i < num4CountRemainder; ++i)
			elementArray[i] = Result4Array[(num4Count - num4CountRemainder) + i].arrayIndex;
		return handleRemainder(num4CountRemainder, elementArray, 4, opposite);
	}
	return true;
}



bool handleRemainder(int numRemainder, int * elementArray, int weight, int * toFindOppositeIndex)
{
	if (numRemainder > 0)
	{
		for (int i = 3; i >= numRemainder; --i)
			elementArray[i] = referenceArray[3 - i];

		int queryResult = QCOUNT(1, elementArray);
		if (queryResult != 4 && toFindOppositeIndex[0] == -1)
			for (int i = 0; i < 3; ++i)
				toFindOppositeIndex[i] = elementArray[i];
		if (numRemainder == 1)
		{

			if (queryResult == 4)
				counts[0] += (1 * weight);
			else if (queryResult == 2)
				counts[1] += (1 * weight);
			else
				return false;
		}
		else if (numRemainder == 2)
		{
			if (queryResult == 0)
				counts[1] += (2 * weight);
			else if (queryResult == 4)
				counts[0] += (2 * weight);
			else if (queryResult != 2)
				return false;
		}
		else if (numRemainder == 3)
		{
			if (queryResult == 4)
				counts[0] += (3 * weight);
			else if (queryResult == 2)
			{
				int lastElement = elementArray[2];
				elementArray[2] = referenceArray[0];
				elementArray[3] = referenceArray[1];
				int secondQueryResult = QCOUNT(1, elementArray);
				elementArray[2] = lastElement;
				elementArray[0] = referenceArray[0];
				int thirdQueryResult = QCOUNT(1, elementArray);
				if (secondQueryResult == 0 && thirdQueryResult == 0)
					counts[1] += (3 * weight);
				else if (secondQueryResult == 4 && thirdQueryResult == 4)
					counts[0] += (3 * weight);
				else if ((secondQueryResult == 2 && thirdQueryResult == 4) || (secondQueryResult == 4 && thirdQueryResult == 2))
				{
					counts[0] += (2 * weight);
					counts[1] += (1 * weight);
				}
				else if ((secondQueryResult == 2 && thirdQueryResult == 0) || (secondQueryResult == 0 && thirdQueryResult == 2))
				{
					counts[1] += (2 * weight);
					counts[0] += (1 * weight);
				}
				else if (secondQueryResult == 2 && thirdQueryResult == 2)
				{
					counts[0] += (2 * weight);
					counts[1] += (1 * weight);
				}
				else
					return false;
			}
			else if (queryResult == 0)
			{
				counts[1] += (2 * weight);
				counts[0] += (1 * weight);
			}
			else
				return false;
		}
		else
			return false;
	}
	return true;
}

bool getInitialQueryResult(int numberOfItems, int * Result2Count, int * Result4Count, struct node * Result2Array, struct node * Result4Array, int * oppositeIndices)
{
	int elementArray[4];
	for (int i = 0; i < numberOfItems; ++i)
	{
		for (int j = 1; j <= 4; ++j)
			elementArray[j - 1] = (i * 4) + j + 10;
		int queryResult = QCOUNT(1, elementArray);
		if (queryResult != 0 && queryResult != 2 && queryResult != 4)
			return false;
		if (queryResult == 4)
		{
			Result4Array[(*Result4Count)].arrayIndex = (i * 4) + 11;
			Result4Array[(*Result4Count)].result = queryResult;
			++(*Result4Count);
		}
		else if (queryResult == 2)
		{
			Result2Array[(*Result2Count)].arrayIndex = (i * 4) + 11;
			Result2Array[(*Result2Count)].result = queryResult;
			++(*Result2Count);
		}

		if (queryResult == 0 || queryResult == 2)
		{
			if (oppositeIndices[0] <= -1)
			{
				oppositeIndices[0] = (i * 4) + 11;
				oppositeIndices[1] = (i * 4) + 12;
				oppositeIndices[2] = (i * 4) + 13;
				oppositeIndices[3] = (i * 4) + 14;
			}
		}
	}
	return true;
}


bool findReferencePoint(int * remainingElements)
{
	int exploreArray[4] = { 1,2,3 };
	int arrayToCheck[10] = { 1,2,3,4,5,6,7,8,9,10 };
	int QueryResult2Count = 0;
	int QueryResult2Explore[4] = { -1,-1,-1,-1 };
	int QueryResult0Count = 0;
	int QueryResult0Explore[4] = { -1,-1,-1,-1 };

	for (int i = 4; i <= 10; ++i)
	{
		if (QueryResult2Count >= 4)
		{
			for (int j = 0; j < 4; ++j)
				referenceArray[j] = QueryResult2Explore[j];
			break;
		}
		if (QueryResult0Count >= 4)
		{
			for (int j = 0; j < 4; ++j)
				referenceArray[j] = QueryResult0Explore[j];
			break;
		}
		exploreArray[3] = i;
		int queryResult = QCOUNT(1, exploreArray);
		if (queryResult != 0 && queryResult != 2 && queryResult != 4)
			return false;
		if (queryResult == 4)
		{
			for (int j = 0; j < 3; ++j)
				referenceArray[j] = exploreArray[j];
			referenceArray[3] = i;
			break;
		}
		else if (queryResult == 2)
		{
			fflush(stdout);
			QueryResult2Explore[QueryResult2Count] = i;
			++QueryResult2Count;
		}
		else if (queryResult == 0)
		{
			fflush(stdout);
			QueryResult0Explore[QueryResult0Count] = i;
			++QueryResult0Count;
		}
		else
			return false;
	}
	if (QueryResult2Count >= 4)
	{
		for (int j = 0; j < 4; ++j)
			referenceArray[j] = QueryResult2Explore[j];
	}
	if (QueryResult0Count >= 4)
	{
		for (int j = 0; j < 4; ++j)
			referenceArray[j] = QueryResult0Explore[j];
	}
	int count = 0;
	for (int i = 0; i< 10; ++i)
	{
		int numCount = 0;
		for (int j = 0; j < 4; ++j)
			if (referenceArray[j] != arrayToCheck[i])
				++numCount;
		if (numCount >= 4)
		{
			remainingElements[count] = arrayToCheck[i];
			++count;
		}
	}
	counts[0] += 4;
	return true;
}

bool handle6Remaining(int * remainingElement, int * oppositeIndices)
{
	int elementArray[4];
	for (int i = 0; i < 3; ++i)
	{
		elementArray[0] = remainingElement[(i * 2)];
		elementArray[1] = remainingElement[(i * 2) + 1];
		if (!(handleRemainder(2, elementArray, 1, oppositeIndices)))
			return false;
	}
	return true;
}

bool result(int * oppositeIndices)
{
	if (counts[0] == counts[1])
		return 0;
	if (counts[0] > counts[1])
		return referenceArray[0];
	if (counts[0] < counts[1])
	{
		int elementArray[4];
		elementArray[0] = referenceArray[0];
		elementArray[1] = referenceArray[1];
		elementArray[2] = referenceArray[2];
		for (int i = 0; i < 4; ++i)
		{
			elementArray[3] = oppositeIndices[i];
			if (QCOUNT(1,elementArray) == 2)
				return oppositeIndices[i];
		}
	}
	
}

int mysub(int n)
{
	if (n < 10 || n > 10000)
	{
		printf("Array Size is out of range. Must be from 10 to 10000");
		fflush(stdout);
		return -1;
	}

	//-----Setup initiailization
	int numberOfItems = (n - 10) / 4;
	int numRemainder = (n - 10) % 4;
	counts[0] = 0;
	counts[1] = 0;
	int Result4Count = 0;
	int Result2Count = 0;
	int oppositeIndices[4];
	int remainingElements[6];
	for (int i = 0; i < 4; ++i)
		oppositeIndices[i] = -1;
	for (int i = 0; i < 4; ++i)
		referenceArray[i] = 0;
	struct node * Result4Array = malloc(numberOfItems * sizeof(struct node));
	struct node * Result2Array = malloc(numberOfItems * sizeof(struct node));

	//---------Run The Query Algorithm to Figure Out The Majority
	bool statusCode[5];
	statusCode[0] = findReferencePoint(remainingElements);
	statusCode[1] = handle6Remaining(remainingElements, oppositeIndices);
	statusCode[2] = getInitialQueryResult(numberOfItems, &Result2Count, &Result4Count, Result2Array, Result4Array, oppositeIndices);
	statusCode[3] = handleThreeSame(Result2Array, Result2Count, oppositeIndices);
	statusCode[4] = handleAllSame(Result4Array, Result4Count, oppositeIndices);
	for (int i = 0; i < 5; ++i)
		if (statusCode[i] == false)
			return -1;
	
	//---------Check for any remainder elements that need to be handles before giving back the result.
	if (n > 10 && numRemainder != 0)
	{
		int remaimderArray[4];
		for (int i = 0; i < numRemainder; ++i)
			remaimderArray[i] = (n - numRemainder) + i + 1;
		for (int i = 3; i >= numRemainder; --i)
			remaimderArray[i] = referenceArray[i];
		handleRemainder(numRemainder, remaimderArray, 1, oppositeIndices);
	}

	free(Result4Array);
	free(Result2Array);

	return result(oppositeIndices);
}