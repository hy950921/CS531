#include <stdio.h>
#include <string.h>
#define TotalStr 10     //10 character strings
#define MaxLimit 65535  //2^16
/*
Yu Han G00849895
CS 531: Fundamentals of	Systems	Programming
Spring 2019	 Instructor: Hal Greenwald
Homework #1     02/25/2019
*/

int CharacterValidation(const char InputPar[]){
    /*
    This function takes a string as input parameter to validate
    if it contains any banned character and the function returns
    an integer called CheckVar. If the CheckVar is not 1 that
    means the string contains illegal character otherwise it is
    a valid input string.
    */
    int CheckVar = strlen(InputPar);    //assign CheckVar with value of the length of input string
    char BannedChar[8] = {'!', '@', '#', '$', '%', '^', '(', ')'};  //illegal character list
    for(int i = 0; i < strlen(InputPar); i++){   //iterate input string and illegal character list
        for(int j = 0; j < strlen(BannedChar); j++){
            if(InputPar[i] == BannedChar[j]){     //when a character from the input strings is the same as the character from the illegal
                CheckVar = i;                     //character list, let CheckVar equals the index of illegal character in the input string
            }
        }
    }
    return CheckVar;    //return the CheckVar
}

char SizeValidation(const char InputPar[]){
    /*
    This function takes a string as input parameter to validate
    if the length of the string is over 25 or it is an empty strings
    and the function returns a char called CheckChar, if it is 'O' that
    means the string is oversized and 'E' means the string is empty.
    */
    char CheckChar;
    if(strlen(InputPar) > 25){      //when the length is greater than 25
        CheckChar = 'O';
    }
    else if(strlen(InputPar) == 0){     //when the length equals 0
        CheckChar = 'E';
    }
    return CheckChar;
}

int DuplicateValidation(const char InputList[TotalStr][MaxLimit], const char InputPar[]){
    /*
    This function takes two parameters that are a 2D array with stored
    input strings and another string to check if this string is duplicate
    with those stored strings. The function returns a CheckVar, if it equals
    0 that means this string is duplicate otherwise it is not.
    */
    int CheckVar = 1;
    for(int i = 0; i < TotalStr; i++){          //iterate the 2D array with stored strings
        if(strcmp(InputList[i], InputPar) == 0){    //strcmp() compares two strings and returns 0 if they are the same
            CheckVar = 0;
        }
    }
    return CheckVar;
}

int TotalAscii(char InputList[TotalStr][MaxLimit]){
    /*
    This functions takes an array of strings as parameter to find the total
    asciivalue from strcmp() to other 9 strings and returns the total value.
    */
    int Result = 0;
    int CmpValue;
    int LoopVar = 1;
    for(int i = 0; i < (TotalStr-1); i++){
        CmpValue = strcmp(InputList[0], InputList[LoopVar]);    //ascii value as determined by strcmp()
        Result += CmpValue;
        LoopVar += 1;
    }
    return Result;
}

int FindIndex(const int SearchPar[TotalStr], int num){
    /*
    This function takes two parameters to search the num in an array of AsciiValue and returns the index
    */
    int index;
    for(int i = 0; i < TotalStr; i++){
        if(SearchPar[i] == num){
            index = i;
        }
    }
    return index;
}

void RemoveNewline(char InputPar[]){
    /*
    This function removes newline from fgets();
    */
    int InputLen;
    InputLen = strlen(InputPar);
    if(InputPar[InputLen-1] == '\n'){
        InputPar[InputLen-1] = 0;
    }
}

int main(void){
    char InputStr[MaxLimit], TempChar[MaxLimit], UserOrder[MaxLimit], LowestAscii[MaxLimit], HighestAscii[MaxLimit];
    char StrList[TotalStr][MaxLimit], TempList[TotalStr][MaxLimit];
    int AsciiList[TotalStr], IndexList[TotalStr], SortedList[TotalStr];
    int Swap, IllegaIndex, StrLength;
    int LoopVar = 1, NumStr = 1;
    char IllegalChar, BubbleOrder;
    printf("Enter 10 character strings:\n\n");
    while(NumStr <= TotalStr){
        printf("Enter string %d: ", NumStr);
        fgets(InputStr, sizeof(InputStr), stdin);
        RemoveNewline(InputStr);
        StrLength = strlen(InputStr);
        if(CharacterValidation(InputStr) != StrLength){     //call function to check if the input string contains illegal character
            IllegaIndex = CharacterValidation(InputStr);
            IllegalChar = InputStr[IllegaIndex];
            printf("Error: %c is an illegal character – please re-enter\n", IllegalChar);
        }
        else if(SizeValidation(InputStr) == 'O'){   //call function to chekc if the length of input string is more than 25
            printf("Error: Oversized string – please re-enter\n");
        }
        else if(SizeValidation(InputStr) == 'E'){   //call function to chekc if the input string is empty
            printf("Error: Empty string – please re-enter\n");
        }
        else if(DuplicateValidation(StrList, InputStr) != 1){   //call function to check if the string is duplicate with previous inputs
            printf("Error: duplicate string – please re-enter\n");
        }
        else{
            strcpy(StrList[NumStr-1], InputStr);        //store every input string to a 2D array
            strcpy(TempList[NumStr-1], InputStr);
            NumStr += 1;
        }
    }
    printf("\n");
    for(int i = 0; i < TotalStr; i++){      //store the totalascii values of 10 strings to the list
        AsciiList[i] = TotalAscii(TempList);
        SortedList[i] = TotalAscii(TempList);
        strcpy(TempChar, TempList[0]);          //swap two strings
        strcpy(TempList[0], TempList[LoopVar]);
        strcpy(TempList[LoopVar], TempChar);
        LoopVar += 1;
    }
    while(1){
        printf("Print character strings in Ascending or Descending order: ");   //Ascending or Descending order based on user specification
        fgets(UserOrder, sizeof(UserOrder), stdin);
        RemoveNewline(UserOrder);
        if(strcmp(UserOrder, "A") == 0){    //Ascending order
            BubbleOrder = 'A';
            printf("Ascending order:\n\n");
            break;
        }
        else if(strcmp(UserOrder, "D") == 0){   //Descending order
            BubbleOrder = 'D';
            printf("Descending order:\n\n");
            break;
        }
        else{
            printf("Invalid Order - please re-enter\n");    //Invalid orders
        }
    }
    for(int i = 0; i < (TotalStr-1); i++){
        for(int j = 0; j < (TotalStr-i-1); j++){
            if(SortedList[j] > SortedList[j+1] && BubbleOrder == 'A'){    //Bubble sort in Ascending
                Swap = SortedList[j];
                SortedList[j] = SortedList[j+1];
                SortedList[j+1] = Swap;
            }
            else if(SortedList[j] < SortedList[j+1] && BubbleOrder == 'D'){    //Bubble sort in Descending
                Swap = SortedList[j];
                SortedList[j] = SortedList[j+1];
                SortedList[j+1] = Swap;
            }
        }
    }
    for(int i = 0; i < TotalStr; i++){
        IndexList[i] = FindIndex(AsciiList, SortedList[i]);     //call function to find the index of each character
    }
    for(int i = 0; i < TotalStr; i++){
        printf("%s\n", StrList[IndexList[i]]);      //print character strings
    }
    if(BubbleOrder == 'A'){       //find the lowest and highest ascii value for Ascending order
        strcpy(LowestAscii, StrList[IndexList[0]]);
        strcpy(HighestAscii, StrList[IndexList[TotalStr-1]]);
    }
    else if(BubbleOrder == 'D'){      //find the lowest and highest ascii value for Descending order
        strcpy(LowestAscii, StrList[IndexList[TotalStr-1]]);
        strcpy(HighestAscii, StrList[IndexList[0]]);
    }
    printf("\n");
    printf("String with lowest ascii value: %s\n", LowestAscii);    //print the lowest and highest ascii values
    printf("String with highest ascii value: %s\n", HighestAscii);

    return 0;
}
