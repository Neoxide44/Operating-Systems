#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "scanner.h"
#include "shell.h"

int main(int argc, char *argv[])
{
    char *inputLine;
    List tokenList;
    List startNode;

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    while (true)
    {
// Bonus 1
#if EXT_PROMPT
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("[%s]>", cwd);
#endif
        inputLine = readInputLine();
        // exit on EOF
        if (inputLine == NULL)
        {
            return 0;
        }
        tokenList = getTokenList(inputLine);

        free(inputLine);

        // Save the start node so we can free it
        startNode = tokenList;

        // Parse the token list
        bool parsedSuccessfully = startParser(&tokenList);
        // If the parser is false
        if (!parsedSuccessfully)
        {
            freeTokenList(startNode);
            printf("Error: invalid syntax!\n");
        }
        freeTokenList(startNode);
    }

    return 0;
}