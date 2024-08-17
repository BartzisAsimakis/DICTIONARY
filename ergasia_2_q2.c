
/*ΟΝΟΜΑ: Μπάρτζης Ασημάκης
  Α.Μ.:  2022202100129

  ΕΡΓΑΣΙΑ 2η: "Λεξικό αναγραμματισμών"
  Ερώτημα:    2ο
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define fullFile 183719

//Δήλωση δομής λίστας
typedef struct listChain
{
	char* anagram;
	struct listChain* next;
}Chain;


//Δήλωση της δομής των δεδομένων του πίνακα κατακερματισμού.
typedef struct node
{
	char* sortedString;
	Chain* list;
}Item;

//Πίνακας κατακερματισμού.
typedef struct hashTable
{
	Item** allItems;
	int size;
	int full;
}Table;

//                 ΠΡΩΤΟΤΥΠΑ ΣΥΝΑΡΤΗΣΕΩΝ
// **********************************************************************

//Συνάρτηση δημιουργίας των δεδομένων  θέσης πίνακα.
Item* createItem(char* valueS);

//Συνάρτηση δημιουργίας αντικειμένου πίνακα κατακερματισμού.
Table* createTable();

//Συνάρτηση κατακερματισμού.
unsigned int strToHash(char* str);

//Συνάρτηση ταξινόμησης αλφαριθμητικού.
char* sortString(char* str);

//Συνάρτηση αποδέσμευσης της μνήμης της λίστας.
void freeChain(Chain* head);

//Συνάρτηση αποδέσμευσης μνήμης μίας θέσης του πίνακα κατακερματισμού.
void freeItem(Item* item);

//Συνάρτηση αποδέσμευσης μνήμης πίνακα κατακερματισμού.
void freeTable(Table* table);

//Συνάρτηση συγχώνευσης πινάκων
void merge(int arr[],int left,int mid,int right);

//Συνάρτηση ταξινόμησης πίνακα ακεραίων στοιχείων.
void merge_Sort(int arr[],int left,int right);

//Συνάρτηση γραμμικής αναζήτησης.
int linearSearch(Table* table,char* sorted,int key);

//Συνάρτηση εισαγωγής των αλφαριθμητικών στον πίνακα κατακερματισμού, βάση της εκφώνησης.
void insertData(Table* table,int key,char* sorted,char* unSorted,int* anagr);

//Συνάρτηση εισαγωγής αναγραμματισμού στο τέλος της λίστας.
void insertToChain(Chain** head,char* str);

//Συνάρτηση εκτύπωσης της λίστας αναγραμματισμών.
void printAnagrams(Chain* node);

//Συνάρτηση αναζήτησης αλφαριθμητικού βάση της εκφώνησης.
int stringSearching(Table* table,char* toFind);

//Συνάρτηση ελέγχου εισαχθέντων δεδομένων από τον χρήστη.
void checkData(int limit,int* num);

//Συνάρτηση εισαγωγής δεδομένων από τον χρήστη, με παράλληλη δέσμευση
//της απαραίτητης μνήμης βάση του μεγέθους του αλφαριθμητικού.
char *loc(char *str);

//Συνάρτηση ταξινόμησης πίνακα ακέραιων στοιχείων.
void quickSort(int arr[], int low, int high);

//Συνάρτηση αντιμετάθεσης των τιμών δύο δεικτών σε ακέραιο.
void swap(int* x,int* y);

//Συνάρτηση διαμέρισης πίνακα ακεραίων στοιχείων.
int partition(int arr[], int low, int high);

//Συνάρτηση ταξινόμησης αλφαριθμητικού.
void selectionSort(char* A,int n);


//ΚΥΡΙΩΣ ΠΡΟΓΡΑΜΜΑ

int main (void)
{

    FILE* fp;
    Chain* head = NULL;
    int i;
    unsigned int hashKey;
    char* sorted = NULL;
    char* anagram = NULL;
    char* searchValue = NULL;
    char* temp = NULL;
    int cnt,ch,choice,result,anagrams = 0;
    int arraySize,stringsToInsert;

//Άνοιγμα δοθέντος αρχείου προς ανάγνωση των περιεχομένων του.
    fp = fopen("dictionary.txt", "r");
    if (fp == NULL)
    {
        printf("\nError file opening!!!\n");
        exit(0);
    }

    printf("\n\033[1;31m********************************************************************************************\033[0m\n");
    printf("\t\t\t\t\033[1;31mANAGRAMS PROGRAM\033[0m");
    printf("\n\033[1;31m********************************************************************************************\033[0m\n");

    Table* ht = createTable(fullFile);

    arraySize = fullFile;
    stringsToInsert = fullFile;
    cnt=0;

    //Δείκτης στην αρχή του αρχείου μας.
    fseek(fp,0,SEEK_SET);

    //Ώσπου να εισαχθεί ο επιθυμητός αριθμός αλφαριθμητικών από το αρχείο.
    while (stringsToInsert > 0)
    {
		if(ftell(fp)!=0)
		{
	           fseek(fp,2,SEEK_CUR);
		}

		//Καταμέτρηση των χαρακτήρων του κάθε αλφαριθμητικού (μαζί με το '\0').
		while((ch=getc(fp))!='\n')
		{
			if(ch == EOF)
			{
			    break;
			}
			cnt++;
		}

		//Επαναφορά του δείκτη στην αρχή του αλφαριθμητικού.
		fseek(fp,-cnt-1,SEEK_CUR);

		//Δέσμευση ακριβώς της μνήμης που απαιτείται για κάθε αλφαριθμητικό στον δείκτη προς χαρακτήρα *strF.
		sorted = (char*)malloc(sizeof(char) * cnt);
		if(sorted == NULL)
		{
		     	printf("\nError allocate memory!!!\n");
		    	exit(0);
		}

	        fscanf(fp,"%s",sorted);
        	anagram = strdup(sorted);
                sorted = sortString(sorted);
		//selectionSort(sorted,strlen(sorted));

        	hashKey = strToHash(sorted);
	        insertData(ht,hashKey,sorted,anagram,&anagrams);

        	stringsToInsert--;

		cnt=0;

    }

    //ΤΕΛΟΣ ΕΙΣΑΓΩΓΗΣ ΑΛΦΑΡΙΘΜΗΤΙΚΩΝ ΑΠΟ ΤΟ ΑΡΧΕΙΟ






    //ΜΕΝΟΥ ΑΝΑΖΗΤΗΣΗΣ ΑΛΦΑΡΙΘΜΗΤΙΚΟΥ Ή ΕΞΟΔΟΥ ΑΠΟ ΤΟ ΠΡΟΓΡΑΜΜΑ
// ************************************************************************

    do
    {
	printf("\n\n1.Search for a String.\n2.Exit\n\n");
	printf("Enter your choice: ");
	checkData(2,&choice);

	if(choice == 2)
	{
		printf("\n\033[1;31m************************************* END OF ΤΗΕ PROGRAM ***********************************\033[0m\n");
		exit(0);
	}
	else
        {
		printf("\nEnter a string: ");
		searchValue = loc(searchValue);
		temp = strdup(searchValue);

		temp = sortString(temp);
		printf("\n%d\n",strToHash(temp));
                //selectionSort(searchValue,strlen(searchValue));
		result = stringSearching(ht,temp);
		printf("\n%d\n",result);

		//Εμφάνιση αποτελεσμάτων αναζήτησης.
   	    	printf("\n\033[1;31m**************************************** RESULTS *****************************************\033[0m\n");

	    if (result !=-1 )
    	    {
                 printf("String: \033[1;31m%s\033[0m\nHash function result: \033[1;34m%25d\033[0m\nFinal position:\033[1;34m%32d\033[0m",temp,strToHash(ht->allItems[result]->sortedString), result);
		 printf("\nAnagrams list:%27s"," ");
		 printAnagrams(ht->allItems[result]->list);
    	    }
    	    else
    	    {
                 printf("String \033[1;31m%s\033[0m not found in the hash table.",searchValue);
    	    }
	    printf("\n\033[1;31m******************************************************************************************\033[0m\n");

	}

	free(temp);
	free(searchValue);
    }while(choice!=2);

    //Αποδέσμευση μνήμης και λήξη προγράμματος.
    free(sorted);
    freeChain(head);
    free(anagram);
    freeTable(ht);
    fclose(fp);

  return 0;
}

//Συνάρτηση δημιουργίας των δεδομένων  θέσης πίνακα.
Item* createItem(char* valueS)
{
    Item* item = (Item*)malloc(sizeof(Item));
    item->list = (Chain*)malloc(sizeof(Chain));
    item->list = NULL;
    item->sortedString = (char*)malloc(strlen(valueS) + 1);
    strcpy(item->sortedString, valueS);

    return item;
}

//Συνάρτηση δημιουργίας αντικειμένου πίνακα κατακερματισμού.
Table* createTable(int size)
{
	int i;
	Table* table = (Table*)malloc(sizeof(Table));
	table->size = fullFile;
	table->full = 0;
	table->allItems = (Item**)calloc(table->size,sizeof(Item*));
	for(i=0; i<table->size; i++)
	{
		table->allItems[i] = NULL;
	}
	return table;
}

//Συνάρτηση κατακερματισμού.
unsigned int strToHash(char* str)
{
	unsigned int hash = 0;
    while (*str)
    {
        hash = *str + 31 * hash;
        str++;
    }
    return hash % fullFile;
}

//Συνάρτηση ταξινόμησης αλφαριθμητικού.
char* sortString(char* str)
{
	int i;
	int arr[strlen(str)];
	char strChar[strlen(str)];

	for(i=0; i<=strlen(str); i++)
	{
		strChar[i] = str[i];
		arr[i] = strChar[i];
	}

	//Μπορείτε να δοκιμάσετε το πρόγραμμα και με την quickSort.
	merge_Sort(arr,0,strlen(str)-1);
        //quickSort(arr,0,strlen(str)-1);

	for(i=0; i<strlen(str); i++)
	{
		strChar[i] = (char)arr[i];
	}


	str = strdup(strChar);

	if(strlen(strChar)%8==1)
	{
		str[strlen(str)-1] = '\0';
	}

	return str;

}

//Συνάρτηση αποδέσμευσης της μνήμης της λίστας.
void freeChain(Chain* head)
{
	  Chain* tmp;
   while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }
}

//Συνάρτηση αποδέσμευσης μνήμης μίας θέσης του πίνακα κατακερματισμού.
void freeItem(Item* item)
{
	free(item->sortedString);
	freeChain(item->list);
	free(item);
}

//Συνάρτηση αποδέσμευσης μνήμης πίνακα κατακερματισμού.
void freeTable(Table* table)
{
	int i;
    for (i = 0; i < table->size; i++)
    {
        Item* item = table->allItems[i];

        if (item != NULL)
        {
            freeItem(item);
        }
    }
    free(table->allItems);
    free(table);
}

//Συνάρτηση γραμμικής αναζήτησης.
int linearSearch(Table* table,char* sorted,int key)
{
	int hindex;
	int cnt =0;
	//Item* item = createItem(*head,sorted);

	hindex=key%fullFile;

	while((table->allItems[hindex]!= NULL)&&(strcmp(table->allItems[hindex]->sortedString,sorted)!=0))
	{
		hindex++;
		hindex %= fullFile;
		cnt++;
		if(cnt == fullFile+1)
		{
			return -1;
			//break;
		}
		return hindex;

	}
	return -1;
}

//Συνάρτηση εισαγωγής των αλφαριθμητικών στον πίνακα κατακερματισμού, βάση της εκφώνησης.
void insertData(Table* table,int key,char* sorted,char* unSorted,int* anagr)
{
	int i;
	int hindex = key;
	int flag = 0;

	Item* item;

	while(1)
	{
		if(table->allItems[key] == NULL)
		{
			item = createItem(sorted);

			table->allItems[key] = item;
			table->full++;

			insertToChain(&table->allItems[key]->list,unSorted);
			// *anagr = *anagr+1;

			printf("\nSORTED: %-20s\t\tANAGRAM: %-10s\n",table->allItems[key]->sortedString,table->allItems[key]->list->anagram);

			break;
		}
		if(table->allItems[key] != NULL)
		{
			if(strcmp(table->allItems[key]->sortedString,sorted)==0)
			{
				insertToChain(&table->allItems[key]->list,unSorted);
				*anagr = *anagr+1;
				printf("\nSORTED: %-20s\t\tANAGRAM: %-10s\n",table->allItems[key]->sortedString,table->allItems[key]->list->anagram);
				//free(item);
				break;
			}
			else
			{

				i = (key + 1) % table->size;

            			while ((i != key && table->allItems[i] != NULL)&&(strcmp(table->allItems[i]->sortedString, sorted) != 0))

				{
                			i = (i + 1) % table->size;
            			}

                                key = i;

			}
		}

	}
}

//Συνάρτηση εισαγωγής αναγραμματισμού στο τέλος της λίστας.
void insertToChain(Chain** head, char* str)
{
    Chain* newNode = (Chain*)malloc(sizeof(Chain));
    newNode->anagram = strdup(str);
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    Chain* last = *head;
    while (last->next != NULL) {
        last = last->next;
    }

    last->next = newNode;

    return;
}

//Συνάρτηση εκτύπωσης της λίστας αναγραμματισμών.
void printAnagrams(Chain* node)
{
	if(node->next==NULL)
	{
		printf("\033[1;92mNULL\033[0m\n");
	}
	else
	{
		printf("\033[1;92m%s->\033[0m",node->anagram);
		printAnagrams(node->next);
	}
}

//Συνάρτηση αναζήτησης αλφαριθμητικού βάση της εκφώνησης.

int stringSearching(Table* table,char* toFind)
{
	int flag = 0;
	char* sortToFind = strdup(toFind);
	int i,key;

//		sortToFind = sortString(sortToFind);

		key = strToHash(sortToFind);

		if(strcmp(sortToFind,table->allItems[key]->sortedString) == 0)
		{
			flag = 1;
			return key;
		}

		i = (key + 1) % table->size;

		while (i != key && table->allItems[i] != NULL)
		{
	                if (strcmp(table->allItems[i]->sortedString,sortToFind) == 0)
			{
                 		     return i;
               		}
               		     i = (i + 1) % table->size;

	        }

		return -1;
}

/*int stringSearching(Table* table,char* toFind)
{
	char* sortToFind = strdup(toFind);
	int i,key;

	key = strToHash(sortToFind);
	i = (key + 1) % table->size;




        do
	{
                if (strcmp(table->allItems[i]->sortedString,sortToFind) == 0)
		{
                     return i;
              	}

                i = (i + 1) % table->size;
		if(i==key-1)
		{
                     break;

		}
	}while (i != key); //&& table->allItems[i] != NULL);

	return -1;
}
*/

//Συνάρτηση εισαγωγής δεδομένων από τον χρήστη, με παράλληλη δέσμευση
//της απαραίτητης μνήμης βάση του μεγέθους του αλφαριθμητικού.
char *loc(char *str)
{
	int size = 10;
	int len=0;

	str=(char*) malloc(size*sizeof(char));
	if(str==NULL)
	{
             printf("\nMemory allocation failed!!!\n");
	     exit(-1);
	}

	while((str[len]=getchar())!='\n'&&str[len]!=EOF)
	{
	     ++len;

	     if(len>=size)
	     {
		size++;
		str=(char*)realloc(str,size);
	     }
	}

	str[len]='\0';

	if(str[len]=='\n')
	{
           str[len]='\0';
	}
	return str;
}


//Συνάρτηση ελέγχου εισαχθέντων δεδομένων από τον χρήστη.
void checkData(int limit,int* num)
{
    char* check = NULL;
    int input;

    do
    {
	check = loc(check);

        if ((atoi(check) > 0) && (atoi(check) <= limit) && (strcmp(check,"\n")!=0))
        {
            break;
        }
	else
        {
            printf("Wrong input!!!\nTry again..:");
        }
    }while ((atoi(check) <= 0) || (atoi(check) > limit)||(strcmp(check,"\n")==0));


    *num = atoi(check);

     free(check);
}

//Συνάρτηση ταξινόμησης αλφαριθμητικού.
void selectionSort(char* A,int n)
{
	int pos;
	int min;
	int i;
	int j;

	for(i=0; i<n; i++)
	{
	   pos=i;
	   min=A[pos];
	   for(j=i+1; j<n; j++)
	   {
              if(A[j]<min)
              {
		 pos=j;
		 min=A[j];
	      }
	   }
	   A[pos]=A[i];
	   A[i]=min;
	}
}

//Συνάρτηση αντιμετάθεσης των τιμών 90δύο δεικτών σε ακέραιο.
void swap(int* x,int* y)
{
 int temp;
 temp = *x;
 *x = *y;
 *y = temp;
}

//Αναδρομική συνάρτηση ταξινόμησης πίνακα ακεραίων στοιχείων.
void merge_Sort(int arr[], int left, int right)
{
    if (left < right)
    {

        int mid = left + (right - left) / 2;

        merge_Sort(arr, left, mid);
        merge_Sort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}


//Συνάρτηση συγχώνευσης πινάκων
void merge(int arr[], int left, int mid, int right)
{
    int i, j, k;
    int size1 = mid - left + 1;
    int size2 = right - mid;

    // Δημιουργία προσωρινού πίνακα
    int Left[size1], Right[size2];

    for (i = 0; i < size1; i++)
        Left[i] = arr[left + i];

    for (j = 0; j < size2; j++)
        Right[j] = arr[mid + 1 + j];

    // Διαδικασία συγχώνευσης πίνακα.
    i = 0;
    j = 0;
    k = left;
    while (i < size1 && j < size2)
    {
        if (Left[i] <= Right[j])
        {
            arr[k] = Left[i];
            i++;
        }
        else
        {
            arr[k] = Right[j];
            j++;
        }
        k++;
    }


    while (i < size1)
    {
        arr[k] = Left[i];
        i++;
        k++;
    }

    while (j < size2)
    {
        arr[k] = Right[j];
        j++;
        k++;
    }
}

//Συνάρτηση ταξινόμησης πίνακα ακεραίων στοιχείων
void quickSort(int arr[], int low, int high)
{
    int pi;

    if (low < high)
    {
        pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

//Συνάρτηση διαμέρησης πίνακα ακεραίων στοιχείων.
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    int j;

    for (j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
    		swap(&arr[i], &arr[j]);
         }
    }

    swap(&arr[i + 1], &arr[high]);

    return (i + 1);
}


