#include <stdio.h>
#define hash_size 15
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
	struct RecordType *next; 
};

// Fill out this structure
struct HashType
{
	struct RecordType **data; 
	int size; 
};

// Compute the hash function
int hash(int x)
{
	return x % hash_size; 
}

void reverseIssue(struct RecordType **head)
{
	struct RecordType *previous = NULL; 
	struct RecordType *current = *head; 
	struct RecordType *nxt = NULL;

	while (current != NULL)
	{
		nxt = current->next; 
		current->next = previous; 
		previous = current; 
		current = nxt; 
	}

	*head = previous; 

}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    for (int i = 0; i < hashSz; i++)
    {
        if (pHashArray->data[i] != NULL)
        {
            printf("Index %d -> ", i);

            int count = 0;
            struct RecordType *records[100]; 
            struct RecordType *current = pHashArray->data[i];
            while (current != NULL)
            {
                records[count] = current;
                current = current->next;
                count++;
            }

            
            for (int j = count - 1; j >= 0; j--)
            {
                printf("%d %c %d", records[j]->id, records[j]->name, records[j]->order);
                if (j > 0)
                {
                    printf(" -> ");
                }
            }

            printf("\n");
        }
    }
}



int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation

	int hts = hash_size; 
	struct HashType hasht; 

	hasht.data = (struct RecordType**)malloc(sizeof(struct RecordType) * hts); 

	if (hasht.data == NULL)
	{
		exit(0);  
	}

	for (int i = 0; i < hts; i++)
	{
		hasht.data[i] = NULL; 
	}
	
	hasht.size = hts; 

	for(int i = 0; i < recordSz; ++i)
	{
		int idx = hash(pRecords[i].id); 
		struct RecordType* new = (struct RecordType*)malloc(sizeof(struct RecordType)); 
		if (new == NULL)
		{
			exit(0); 
		}
		*new = pRecords[i]; 
		new->next = hasht.data[idx]; 
		hasht.data[idx] = new; 
	}
	displayRecordsInHash(&hasht, hts);

	for (int j = 0; j < hasht.size; ++j)
	{
		struct RecordType *current = hasht.data[j]; 
		while (current != NULL)
		{
			struct RecordType *tmp = current; 
			current = current->next; 
			free(tmp); 
		}
		 
	}

	free(pRecords); 
	free(hasht.data); 
	 
}