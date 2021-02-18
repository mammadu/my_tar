#include "my_c_functions.h"

char* my_strdup(char* param_1)
{
    char* str_copy;
    int length = my_strlen(param_1);
    str_copy = (char*)malloc(length * sizeof(char));
    for(int i = 0; i < length; i++)
    {
        str_copy[i] = param_1[i];
    }
    return str_copy;
}

char* combine_strings(char* first_string, char* second_string)
{
    char* return_val = malloc((my_strlen(first_string) + my_strlen(second_string) + 1) * sizeof(char));
    int i = 0;
    while (first_string[i] != '\0')
    {
        return_val[i] = first_string[i];
        i++;
    }
    int j = 0;
    while (second_string[j] != '\0')
    {
        return_val[i] = second_string[j];
        j++;
        i++;
    }
    return_val[i] = '\0';
    return return_val;
}

int my_strlen(char* string)
{
    int i = 0;
    while(string[i] != '\0')
    {
        i++;
    }
    return i;
}

int my_recursive_pow(int param_1, int param_2)
{
    int return_val;
    if (param_2 == 0)
    {
        return_val = 1;
    }
    else
    {
        return_val = param_1 * my_recursive_pow(param_1, param_2 - 1);
    }
    return return_val;
}

int my_atoi(char* param_1)
{
    int return_val = 0; //This holds the return value
    int i = 0; //This will store the count of characters that are integers
    if (param_1[i] == '-') //This is to catch if the number is negative
    {
        i++;
    }
    if (param_1[i] <= 47 || param_1[i] >= 58) //This is to check if the current character is an integer, otherwise 
    {
        return return_val;
    }
    while (param_1[i] > 47 && param_1[i] < 58)
    {
        i++;
    }
    int index = i - 1; //This is used to iterate from the last integer character to the first integer character
    for (int j = 0; j < i; j++)
    {
        if (param_1[index] == '-')
        {
            return_val = return_val * -1;
        }
        else
        {
            return_val = return_val + (param_1[index] - 48) * my_recursive_pow(10, j);
            index--;
        }
    }
    return return_val;
}

void my_putstr(char* param_1)
{
    int i = 0;
    int length = my_strlen(param_1);
    while(param_1[i] != '\0')
    {
        write(1,&param_1[i],1);
        i++;
    }
}

char* reverse_string(char* param_1)
{
    char* return_val = my_strdup(param_1);
    int length = my_strlen(param_1); 
    for (int i = 0; i < (length / 2); i++)
    {
        char storage = param_1[i];
        return_val[i] = param_1[length - i - 1];
        return_val[length - i - 1] = storage;
    }
    return return_val;
}

char hex_to_char (int num, char base)
{
    char return_val;
    if (num == 15)
    {
        if (base == 'X')
        {
            return_val = 'F';
        }
        else if (base == 'x')
        {
            return_val = 'f';
        }
        
    }
    else if (num == 14)
    {
        if (base == 'X')
        {
            return_val = 'E';
        }
        else if (base == 'x')
        {
            return_val = 'e';
        }
    }
    else if (num == 13)
    {
        if (base == 'X')
        {
            return_val = 'D';
        }
        else if (base == 'x')
        {
            return_val = 'd';
        }
    }
    else if (num == 12)
    {
        if (base == 'X')
        {
            return_val = 'C';
        }
        else if (base == 'x')
        {
            return_val = 'c';
        }
    }
    else if (num == 11)
    {
        if (base == 'X')
        {
            return_val = 'B';
        }
        else if (base == 'x')
        {
            return_val = 'b';
        }
    }
    else if (num == 10)
    {
        if (base == 'X')
        {
            return_val = 'A';
        }
        else if (base == 'x')
        {
            return_val = 'a';
        }
    }
    else
    {
        return_val = num + '0';
    }
    return return_val;
}

char *num_to_str(int num, char base)
{
    int mod; //This is used to do operations in the specific base.
    int neg = 0; //this identifies if a number is negative. 1 means the number is negative, 0 means the number is non-negative.
    int remainder;
    char *str;
    int length = 12;
    int i = 0;
    if (base == 'd')
    {
        if (num < 0) //this is to catch negative numbers.
        {
            num = num * -1;
            neg = 1;
        }
        mod = 10;
        str = malloc(sizeof(char) * length); //The greatest value of int is 2,147,483,647. If that value is negative, then we'll need a string that is 11 characters long to hold it.   
        while (num >= mod)
        {
            remainder = num % mod;
            str[i] = remainder + '0';
            i++;
            num = (num - remainder) / mod;
        }
        str[i] = num + '0';
        if (neg == 1) //Adds a '-' to the string for negative numbers
        {
            i++;
            str[i] = '-';
        }
    }
    else if (base == 'o')
    {
        mod = 8;
        str = malloc(sizeof(char) * length); //The greatest value of int is 2,147,483,647. If that value is negative, then we'll need a string that is 11 characters long to hold it.      
        while (num >= mod)
        {
            remainder = num % mod;
            str[i] = remainder + '0';
            i++;
            num = (num - remainder) / mod;
        }
        str[i] = num + '0';
    }
    else if (base == 'u')
    {
        mod = 10;
        str = malloc(sizeof(char) * length);
        while (num >= mod)
        {
            remainder = num % mod;
            str[i] = remainder + '0';
            i++;
            num = (num - remainder) / mod;
        }
        str[i] = num + '0';
    }
    else if (base == 'x' || base == 'X')
    {
        mod = 16;
        str = malloc(sizeof(char) * length);
        while (num >= mod)
        {
            remainder = num % mod;
            str[i] = hex_to_char(remainder, base);
            i++;
            num = (num - remainder) / mod;
        }
        str[i] = hex_to_char(num, base);
    }
    str = reverse_string(str); //Reverses the string to provide a clean output.
    str = realloc(str, i + 1);
    return str;
}

int my_strcmp(char* param_1, char* param_2)
{
    int i = 0;
    while (param_1[i] != '\0' && param_2[i] != '\0')
    {
        int lex_diff = param_1[i] - param_2[i];
        if (lex_diff != 0)
        {
            return lex_diff;
        }
        i++;
    }
    return 0;
}