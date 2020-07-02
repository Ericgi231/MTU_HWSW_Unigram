//Eric Grant
//ewgrant
//take string and return probability of each word in a language model

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

// We assue all words are no more than 63 characters long.
// This requires 64 characters in any array that holds a word since we need 1 for null.
// Instead of sprinkling the magic number 64 everywhere in your code, you should instead
// use the following constant:
#define WORD_ARRAY_SIZE 64

// Represents a single entry in the hash table
typedef struct
{
    char key[WORD_ARRAY_SIZE];    // Uppercase and stripped unique word
    int count;                    // How often we saw this word in the training file
} Entry;

// Function prototypes, this allows the function implementations to appear in any order in the file
// You will be implementing these three functions.
unsigned int hashString(char* str);
double calcUnigramLogProb(unsigned int count, unsigned long total);
void stripUpper(char* str);
Entry* initTable(int size);
void uninitTable(Entry* table);

// Function prototypes for the test functions we have provided
void testHashString();
void testCalcUnigramLogProb();
void testStripUpper();
void testInitUninitTable();

// Helper functions 

//return power of base
int power(int base, int exp){
   int result = 1;
   for (int i = 0; i < exp; i++)
   {
      result = result * base;
   }
   return result;
}

//delete char from string safely
void deleteChar(char *str, int i) {
   int size = strlen(str);

   for (; i < size - 1 ; i++)
   {
      str[i] = str[i+1];
   }

   str[i] = '\0';
}

// Hash a null-terminated string to an unsigned int.
// Implement as in Java's hashCode for String s, calculates this equation:
//   s[0]*31^(n-1) + s[1]*31^(n-2) + ... + s[n-1]  (^ denotes exponentiation)
//
// You can follow the Java source code in this discussion: http://tinyurl.com/pqg2al5
//
// NOTE: While the above equation uses exponentiation, it is quite expensive and
// unnecessary to call the pow() function for each character. Also you will not get
// the same answer as us for long strings. You can do the calculation using only
// multiplication and addition. HINT: Can you reorder the calculation?
unsigned int hashString(char* str)
{
   //get size of string
   int size = strlen(str);

   //hash
   int hash = 0;
   for (int i = 0; i < size; i++)
   {
      hash += str[i] * power(31, size-(i+1));
   }
   
   return hash;
}

// Calculate log probability of a word occurring count times in total words of data.
// Result is in log base 10.
double calcUnigramLogProb(unsigned int count, unsigned long total)
{
   double result = log10(count) - log10(total);
   return result;
}

// Strip the null-terminated string str of characters that aren't letters or apostrophe.
// Converts all lowercase letters to uppercase.
// This operates in place on the string str.
void stripUpper(char* str)
{
   //get size of string
   int size = strlen(str);

   //strip bad characters and convert lower to upper
   for (int i = 0; i < size; i++)
   {
      if (str[i] > 96 && str[i] < 123) //if lowercase letter, set to upper
      {
         str[i] = str[i] - 32;
      } 
      else if (!((str[i] > 64 && str[i] < 91) || str[i] == 39))//if not valid character, remove
      {
         deleteChar(str, i);
         i--;
         size--;
      } 
   }
}

// Allocate a hash table of the given size and initializes all entries.
// Returns the pointer to the dynamically allocated array of Entry structures.
// Returns NULL on failure (e.g. memory allocation failed or size not positive).
Entry* initTable(int size)
{
   //abort if invalid
   if (size < 1)
   {
      return NULL;
   }
   
   //create table
   Entry* entries = malloc(sizeof(Entry) * size);

   if(entries == NULL){
      return NULL;
   }

   Entry blank;
   blank.count = 0;
   strcpy(blank.key, "\0");

   //fill with empty data
   for (int i = 0; i < size; i++)
   {
      entries[i] = blank;
   }
   
   return entries;
}

// Deallocate the memory allocated to the hash table
void uninitTable(Entry* table)
{
   free(table);
   table = NULL;
}

// Test function for the hashString function.
// Do NOT change this function.
void testHashString()
{
   char tests[][WORD_ARRAY_SIZE] = {"BAD", "DAB", "GOODFELLOWS", "WRITERSHIP", "a", "A", "abcdefghijklmnopqrstuvwxyz", "1234567890!@#$%^&*()", ""};

   int i = 0;
   while (strlen(tests[i]) > 0)
   {
      printf("\"%s\" -> %u\n", tests[i], hashString(tests[i]));
      i++;
   }
}

// Test function for the calcUnigramLogProb function.
// Do NOT change this function.
void testCalcUnigramLogProb()
{
   const int SIZE = 8;
   unsigned int  counts[] = { 5 ,  50 ,   1,    1,   1234,   0 , 100 , 1};
   unsigned long totals[] = {10 , 100 , 100, 2000, 567890, 123 , 100 , 1};
   
   for (int i = 0; i < SIZE; i++)
   {
      printf("%5u %7lu -> %9.6f\n", counts[i], totals[i], calcUnigramLogProb(counts[i], totals[i]));
   }
}

// Test function for the stripUpper function.
// Do NOT change this function.
void testStripUpper()
{
   char tests[][WORD_ARRAY_SIZE] = {"bad", "BAD", "BaD", "CAN'T", "well-done", "!L00K-", "1234567890", ""};
   
   int i = 0;
   while (strlen(tests[i]) > 0)
   {
      printf("\"%s\" -> ", tests[i]);
      stripUpper(tests[i]);
      printf("\"%s\" (len %zu)\n", tests[i], strlen(tests[i]));
      i++;
   }
}

// Test function for the initTable and unitTable function.
// Do NOT change this function.
void testInitUninitTable()
{
    Entry* table = initTable(0);
    if (table != NULL)
    {
        printf("initTable(0) should return NULL!\n");
        return;
    }
    
    const int SIZE = 10000;
    table = initTable(SIZE);
    for (int i = 0; i < SIZE; i++)
    {
        if ((strlen(table[i].key) > 0) || (table[i].count != 0))
        {
            printf("initTable should initialize table entries!\n");
            return;
        }
    }
    
    uninitTable(table);
    printf("initTable and uninitTable completed\n");
}

int main(int argc, char** argv)
{
   // If no command line input we print out a help message and also run test functions
   if (argc <= 1)
   {
      printf("Usage: Unigram <hash table size> [debug]\n\n");
      testHashString();
      printf("\n");
      testCalcUnigramLogProb();
      printf("\n");
      testStripUpper();
      printf("\n");
      testInitUninitTable();
      return 0;
   }

   //fail if invalid parameter
   if (atoi(argv[1]) < 0)
   {
      printf("Table size must be positive!\n");
      return 0;
   }

   //check optional debug flag
   int debug = 0;
   if(argc >= 3 && atoi(argv[2]) != 0){
      debug = 1;
   }
   
   //declare vars
   int size = atoi(argv[1]);
   unsigned int hash = 0;
   int count = 0;
   int uniqueWords = 0;
   char str[WORD_ARRAY_SIZE];
   str[0] = '\0';
   Entry* hashTable = initTable(atoi(argv[1]));

   //take string input from files and store to hash table
   while (scanf("%63s", str) == 1)
   {  
      //strip and upper string, skip if string becomes nothing
      stripUpper(str);
      if (strcmp(str,"\0"))
      {
         if (uniqueWords >= size)
         {
            printf("ERROR: hash table is full!\n\n");
            return 0;
         }

         count++;
         if(debug){
            printf("%s -> hash ", str);
         }

         //find point in hash table using linear probing
         hash = hashString(str);
         if(debug){
            printf("%u, bucket ", hash);
         }
         hash = hash % size;
         if(debug){
            printf("%u, stored at ", hash);
         }
         while (strcmp(str, hashTable[hash].key) && strcmp("\0", hashTable[hash].key))
         {
            hash = (hash + 1) % size;
         }

         if(debug){
            printf("%u, count ", hash);
         }

         //update unique word count if new word
         if (hashTable[hash].count == 0)
         {
            uniqueWords++;
         }

         //update table
         strcpy(hashTable[hash].key, str);
         hashTable[hash].count++;
         if(debug){
            printf("%d\n", hashTable[hash].count);
         }
      }
   }
   
   //print data
   for (int i = 0; i < size; i++)
   {
      if (strcmp(hashTable[i].key, "\0"))
      {
         printf("%.6f %s\n", calcUnigramLogProb(hashTable[i].count, count), hashTable[i].key);  
      }
   }

   //clean
   uninitTable(hashTable);

   printf("\n");
   return 0;
}
