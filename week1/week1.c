#include <stdio.h>
#include <string.h>
#include <ctype.h>

char stopWordList[100][100], PList[100][100], wordList[300][100];
int countList[300][100];
int stopwSize, Psize;

int checkWord(char *word, char List[][100], int size)
{

    for (int i = 0; i < size; i++)
    {

        if (strcasecmp(word, List[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}
int isWord(char *string)
{
    for (int i = 0; i < strlen(string); i++)
    {
        if (!isalpha(string[i]))
            return 0;
    }

    return 1;
}
void toLower(char word[])
{
    for (int i = 0; i < strlen(word); i++)
    {
        word[i] = tolower(word[i]);
    }
}
void readStopWord(const char *fileName)
{
    FILE *fptr;
    if ((fptr = fopen(fileName, "r")) == NULL)
    {
        printf("File khong ton tai\n");
        return;
    }

    while (!feof(fptr))
    {
        fscanf(fptr, "%s\n", stopWordList[stopwSize]);

        stopwSize++;
    }

    fclose(fptr);
}
void swapStr(char *x, char *y)
{
    char temp[100];
    strcpy(temp, x);
    strcpy(x, y);
    strcpy(y, temp);
}
void swapArr(int *x, int *y)
{
    for (int i = 0; i < 100; i++)
    {
        int temp = x[i];
        x[i] = y[i];
        y[i] = temp;
    }
}
void sortAlphabet(int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (strcmp(wordList[i], wordList[j]) < 0)
            {
                swapStr(wordList[i], wordList[j]);
                swapArr(countList[i], countList[j]);
            }
        }
    }
}
void readPword(const char *filename)
{
    FILE *fptr;
    if ((fptr = fopen(filename, "r")) == NULL)
    {
        printf("File khong ton tai\n");
        return;
    }
    while (!feof(fptr))
    {
        char line[300];
        fgets(line, 300, fptr);
        if (line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = '\0';
        }
        char *word = strtok(line, " ");
        char *word2 = ".";
        char *word3;
        while (word != NULL)
        {
            if (word2[strlen(word2) - 1] != '.' && isupper(word[0]))
            {
                word3 = strdup(word);
                if (word3[strlen(word3) - 1] == ',' || word3[strlen(word3) - 1] == '.')
                {
                    word3[strlen(word3) - 1] = '\0';
                }
                strcpy(PList[Psize], word3);
                Psize++;
            }
            word2 = strdup(word);
            word = strtok(NULL, " ");
        }
    }
}
void printList()
{
}

int main()
{
    FILE *fptr;
    if ((fptr = fopen("vanban.txt", "r")) == NULL)
    {
        printf("File khong ton tai\n");
        return 0;
    }
    int index = 0;
    readPword("vanban.txt");
    readStopWord("stopw.txt");
    int currentLine = 1;
    while (!feof(fptr))
    {
        char line[200];
        fgets(line, 200, fptr);
        if (line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = '\0';
        }
        char *word = strtok(line, " ,;?!+*<>()''");

        while (word != NULL)
        {

            char newWord[100];
            toLower(word);
            strcpy(newWord, word);
            if (newWord[strlen(newWord) - 1] == '.')
            {
                newWord[strlen(newWord) - 1] = '\0';
            }
            if (checkWord(newWord, stopWordList, stopwSize) == -1 &&
                isWord(newWord) && checkWord(newWord, PList, Psize) == -1)
            {
                if (checkWord(newWord, wordList, index) == -1)
                {
                    strcpy(wordList[index], newWord);
                    countList[index][0] = 1;
                    countList[index][1] = currentLine;
                    index++;
                }
                else
                {
                    int a = checkWord(newWord, wordList, index);
                    countList[a][0]++;
                    int b = countList[a][0];
                    countList[a][b] = currentLine;
                }
            }
            word = strtok(NULL, " ,;?!+*<>()");
        }
        currentLine++;
    }
    fclose(fptr);
    sortAlphabet(index);
    for (int i = 0; i < index; i++)
    {
        printf("%s: ", wordList[i]);
        for (int j = 0; j <= countList[i][0]; j++)
        {
            printf("%d ", countList[i][j]);
        }
        printf("\n");
    }
    return 0;
}
