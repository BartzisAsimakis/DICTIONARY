/*ΟΝΟΜΑ: Μπάρτζης Ασημάκης
  Α.Μ.:  2022202100129

  ΕΡΓΑΣΙΑ 2η: "Λεξικό"
  Ερώτημα:    1ο
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Ορίζω σαν σταθερά τον αριθμό των εγγραφών στο δοθέν αρχείο ώστε το πρόγραμμα να μπορεί να δοκιμαστεί και σε αρχεία
//με διαφορετικό πλήθος εγγραφών*/
#define fullFile 183719

//Δήλωση δομής δεδομένων που θα αποτελούν τα στοιχεία του πίνακα κατακερματισμού.
typedef struct hashItem
{
    int key;
    char* value;
} Item;

//Δήλωση δομής δεδομένων του πίνακα κατακερματισμού.
typedef struct hashTable
{
    Item** allItems;
    int size;
} Table;

//----------------------------------------------- ΠΡΩΤΟΤΥΠΑ ΣΥΝΑΡΤΗΣΕΩΝ ---------------------------------------------------------------------

//Δήλωση συνάρτησης κατακερματισμού. Στα ορίσματα της συνάρτησης έχει προστεθεί και το μέγεθος του πίνακα κατακερματισμού
//αφού θα δίνεται από τον χρήστη και δεν είναι σταθερό στην άσκησή μας.
unsigned int strToHash(char* str, int dictionary);

//Συνάρτηση δημιουργίας στοιχείου του πίνακα κατακερματισμού.
Item* createItem(int key, char* value);

//Συνάρτηση δημιουργίας του πίνακα κατακερματισμού.
Table* createHtable(int size);


//Συνάρτηση εισαγωγής στοιχείων στον πίνακα κατακερματισμού.
void insertData(Table* table, int key, char* value, int userSize);

//Συνάρτηση απεδέσμευσης μνήμης για κάθε στοιχείο του πίνακα κατακερματισμού που έχει δημιουργηθεί.
void freeItem(Item* item);

//Συνάρτηση αποδέσμευσης μνήμης του πίνακα κατακερματισμού.
void freeTable(Table* table);

//Συνάρτηση που διαβάζει χαρακτήρες από τον χρήστη και δεσμεύει δυναμικά τη μνήμη
//για να αποθηκεύσει το κείμενο που εισάγει ο χρήστης.
char* loc();

//Συνάρτηση γραμμικής αναζήτησης αλφαριθμητικού στον πίνακα κατακερματισμού.
int linearSearch(Table* table, char* value, int userSize);

//Συνάρτηση ελέγχου εισαγωγής δεδομένων από τον χρήστη.
void checkData(int limit,int* num);


int main(void)
{
    FILE* fp;
    int i, arraySize,quont_strings;
    char* strF = NULL;
    char* searchValue = NULL;
    int cnt,ch,choice,result;

//Άνοιγμα δοθέντος αρχείου προς ανάγνωση των περιεχομένων του.
    fp = fopen("dictionary.txt", "r");
    if (fp == NULL)
    {
        printf("\nError file opening!!!\n");
        exit(0);
    }

//Έναρξη διεπαφής με τον χρήστη και εισαγωγή του επιθυμητού αριθμού αλφαριθμητικών του αρχείου.
    printf("\n\033[1;31m********************************************************************************************\033[0m\n");
    printf("\t\t\t\t\033[1;31m    DICTIONARY PROGRAM\033[0m");
    printf("\n\033[1;31m********************************************************************************************\033[0m\n");
    printf("\n\nPlease insert the number of the strings to be entered to the hash table: ");

//Έλεγχος και εισαγωγή επιλογής χρήστη.
    checkData(fullFile,&quont_strings);

    printf("\n\n\n\033[1;36m%-23s%35s","STRING","  HASH  FUNCTION VALUE\033[0m");
    printf("\n\033[1;36m============================================================================================\033[0m");
//Δημιουργία πίνακα κατακερματισμού.
    Table* ht = createHtable(quont_strings);

    arraySize = quont_strings;
    cnt=0;
//Δείκτης στην αρχή του αρχείου μας.
    fseek(fp,0,SEEK_SET);

//Ώσπου να εισαχθεί ο επιθυμητός αριθμός αλφαριθμητικών από το αρχείο.
    while (quont_strings > 0)
    {
	if(ftell(fp)!=0)
	{
           fseek(fp,2,SEEK_CUR);
	}

//Καταμέτρηση των χαρακτήρων του κάθε αλφαριθμητικού (μαζί με το '\0').
	while((ch=getc(fp))!='\n')
	{
		cnt++;
	}

//Επαναφορά του δείκτη στην αρχή του αλφαριθμητικού.
	fseek(fp,-cnt-1,SEEK_CUR);

//Δέσμευση ακριβώς της μνήμης που απαιτείται για κάθε αλφαριθμητικό στον δείκτη προς χαρακτήρα *strF.
	strF = (char*)malloc(sizeof(char) * cnt);
	if(strF==NULL)
	{
	     printf("\nError allocate memory!!!\n");
	     exit(0);
	}

//Αντιγραφή αλφαριθμητικού μέσω του *strF
        fgets(strF,cnt,fp);

//Εμφάνιση κάθε αλαφαριθμητικού που θα εισαχθεί στον πίνακα κατακερματισμού.
        printf("\n%-23s   ", strF);

//Εμφάνιση του αντίστοιχου κλειδιού που προέκυψε από την συνάρτηση κατακερματισμού.
        printf("%20u\n", strToHash(strF, arraySize));

//Εισαγωγή στοιχείου στον πίνακα κατακερματισμού.
        insertData(ht, strToHash(strF, arraySize), strF, arraySize);

        quont_strings--;
	cnt=0;
    }

//Εμφάνιση επιλογών χρήστη βάση εκφώνησης μέσω επαναληπτικής διαδικασίας.
//1.Αναζήτηση αλφαριθμητικού στον πίνακα κατακερματισμού.
//2.Έξοδος από το πρόγραμμα.
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
	    printf("\nEnter the string you want to search in the hash table: ");
	    searchValue = loc(searchValue);

	    result = linearSearch(ht, searchValue, arraySize);

//Εμφάνιση αποτελεσμάτων αναζήτησης.
   	    printf("\n\033[1;31m****************************************** RESULTS *****************************************\033[0m\n");

	    if (result !=-1 )
    	    {
                 printf("String: \033[1;31m%s\033[0m\nHash function result: \033[1;34m%25d\033[0m\nFinal position:\033[1;34m%32d\033[0m", searchValue,strToHash(ht->allItems[result]->value,arraySize), result);
    	    }
    	    else
    	    {
                 printf("String \033[1;31m%s\033[0m not found in the hash table.", searchValue);
    	    }
	    printf("\n\033[1;31m********************************************************************************************\033[0m\n");

	}

    }while(1);

//Αποδέσμευση της μνήμης που χρησιμοποιήθηκε.
    free(strF);
    freeTable(ht);
    fclose(fp);

    return 0;
}

//Συνάρτηση κατακερματισμού (από την εκφώνηση).
unsigned int strToHash(char* str, int dictionary)
{
    unsigned int hash = 0;
    while (*str)
    {
        hash = *str + 31 * hash;
        str++;
    }
    return hash % dictionary;
}

//Συνάρτηση δημιουργίας αντικειμένου των δεδομένων που περιέχονται
//σε κάθε κελί του πίνακα κατακερματισμού.
Item* createItem(int key,char* value)
{
    Item* item = (Item*)malloc(sizeof(Item));
    item->key = key;
    item->value = (char*)malloc(strlen(value) + 1);
    strcpy(item->value, value);
    return item;
}

//Συνάρτηση δημιουργίας αντικειμένου πίνακα κατακερματισμού.
Table* createHtable(int userSize)
{
    int i;
    Table* table = (Table*)malloc(sizeof(Table));
    table->size = userSize;
    table->allItems = (Item**)calloc(table->size, sizeof(Item*));
    for (i = 0; i < table->size; i++)
    {
        table->allItems[i] = NULL;
    }
    return table;
}


//Συνάρτηση εισαγωγής δεδόμενων στον πίνακα, μέσω γραμμικής διερεύνησης ανοικτής διευθυνσιοδότησης.
void insertData(Table* table, int key, char* value, int userSize)
{
    int hindex;
    Item* item = createItem(strToHash(value, userSize), value);

    hindex = key % userSize;


    while ((table->allItems[hindex] != NULL)&& (table->allItems[hindex]->key != -1))
    {
        hindex++;
        hindex %= userSize;
    }

    if ((table->allItems[hindex] == NULL)||(table->allItems[hindex]->key==-1))
    {
	table->allItems[hindex] = item;
    }
    else
    {
	hindex++;
    }
}

//Συνάρτηση αποδέσμευσης μνήμης που καταλαμβάνουν τα δεδομένα ενός στοιχείου του πίνακα κατακερματισμού
void freeItem(Item* item)
{
    free(item->value);
    free(item);
}


//Συνάρτηση αποδέσμευσης μνήμης του πίνακα κατακερματισμού.
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

//Συνάρτηση που διαβάζει χαρακτήρες από τον χρήστη και δεσμεύει δυναμικά τη μνήμη
//για να αποθηκεύσει το κείμενο που εισάγει ο χρήστης.
char *loc(char *str)
{
	int size=2;
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

//Συνάρτηση γραμμικής αναζήτησης.
int linearSearch(Table* table, char* value, int userSize)
{
    int hindex = strToHash(value, userSize);
    int cnt = 0;

    while (table->allItems[hindex] != NULL)
    {
        if (strcmp(table->allItems[hindex]->value,value) == 0)
        {
            return hindex;
        }

        hindex++;
        hindex %= userSize;

	cnt++;


	if(cnt == table->size+1)
	{
		return -1;
	}
    }

	    return -1;
}


//Συνάρτηση ελέγχου εισαγωγής δεδομένων από τον χρήστη.
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



