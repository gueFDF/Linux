#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
    while (1)
    {
        char * str = readline("Myshell $ ");
        free(str);
    }
    return 0;
}