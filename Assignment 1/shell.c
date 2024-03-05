#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "scanner.h"

/**
 * Global variables for the exit code and the starting node of the list
 * We declared them as global variables so we don;t have to pass them in every function
 * We need both in parseBuiltIn, exitCode to print the correct status
 * And startNode to free the token list before calling exit()
 */
int exitCode = 0;
List startNode;

/**
 * The function acceptToken checks whether the current token matches a target identifier,
 * and goes to the next token if this is the case.
 * @param lp List pointer to the start of the tokenlist.
 * @param ident target identifier
 * @return a bool denoting whether the current token matches the target identifier.
 */
bool acceptToken(List *lp, char *ident)
{
    if ((*lp != NULL) && (strcmp(((*lp)->t), ident) == 0))
    {
        *lp = (*lp)->next;
        return true;
    }
    return false;
}

/**
 * The function parseExecutable parses an executable.
 * For now the fucntion only checks if the executable is null
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the executable was parsed successfully.
 */
bool parseExecutable(List *lp)
{
    if ((*lp)->t != NULL)
    {
        return false;
    }

    return true;
}

/**
 * Checks whether the input string \param s is an operator.
 * @param s input string.
 * @return a bool denoting whether the current string is an operator.
 */
bool isOperator(char *s)
{
    // NULL-terminated array makes it easy to expand this array later
    // without changing the code at other places.
    char *operators[] = {
        "&",
        "&&",
        "||",
        ";",
        "<",
        ">",
        "|",
        NULL};

    for (int i = 0; operators[i] != NULL; i++)
    {
        if (strcmp(s, operators[i]) == 0)
            return true;
    }
    return false;
}

/**
 * The function parseOptions parses options.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the options were parsed successfully.
 */
bool parseOptions(List *lp, char *options[])
{
    // Check if the first node is null
    if (*lp == NULL)
    {
        return false;
    }
    int count = 0;
    // Loop until the end of the token list or another operator is encountered
    // Add all options encoutered into the options array (array of strings)
    while (*lp != NULL && !isOperator((*lp)->t))
    {
        options[count] = (char *)malloc((strlen((*lp)->t) + 1) * sizeof(char));
        strcpy(options[count], (*lp)->t);
        (*lp) = (*lp)->next;
        count++;
    }
    // Since execvp() second arguments needs to ne a null terminated array
    // add that in last position
    options[count] = NULL;
    return true;
}

/**
 * The function parseRedirections parses a command according to the grammar:
 *
 * <command>        ::= <executable> <options>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the command was parsed successfully.
 */
bool parseCommand(List *lp)
{
    int count = 0;
    List dummy = *lp;
    // Get the length of the options array
    while (dummy != NULL && !isOperator(dummy->t))
    {
        dummy = dummy->next;
        count++;
    }
    char *options[count + 1];

    // Parse the executable
    parseExecutable(lp);
    // Parse the options and add them to the options array (array of strings to technically matrix)
    if (!parseOptions(lp, options))
    {
        return false;
    }

    int status;
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        // child process
        execvp(options[0], options);
        // Free memory
        freeTokenList(startNode);
        for (int i = 0; i < count; i++)
        {
            free(options[i]);
        }
        // Error command not found
        exit(127);
    }
    else if (pid > 0)
    {
        // parent process waits for the child process
        wait(&status);
    }

    // Check the exit code of the child process
    //  Print appropriate response
    if (WIFEXITED(status))
    {
        exitCode = WEXITSTATUS(status);
        if (exitCode == 127)
            printf("Error: command not found!\n");
    }
    // Free memory
    for (int i = 0; i < count; i++)
    {
        free(options[i]);
    }
    return true;
}

/**
 * The function parsePipeline parses a pipeline according to the grammar:
 *
 * <pipeline>           ::= <command> "|" <pipeline>
 *                       | <command>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the pipeline was parsed successfully.
 */
bool parsePipeline(List *lp)
{
    if (!parseCommand(lp))
    {
        return false;
    }

    if (acceptToken(lp, "|"))
    {
        return parsePipeline(lp);
    }

    return true;
}

/**
 * The function parseFileName parses a filename.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the filename was parsed successfully.
 */
bool parseFileName(List *lp)
{
    // TODO: Process the file name appropriately
    char *fileName = (*lp)->t;
    return true;
}

/**
 * The function parseRedirections parses redirections according to the grammar:
 *
 * <redirections>       ::= <pipeline> <redirections>
 *                       |  <builtin> <options>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the redirections were parsed successfully.
 */
bool parseRedirections(List *lp)
{
    if (isEmpty(*lp))
    {
        return true;
    }

    if (acceptToken(lp, "<"))
    {
        if (!parseFileName(lp))
            return false;
        if (acceptToken(lp, ">"))
            return parseFileName(lp);
        else
            return true;
    }
    else if (acceptToken(lp, ">"))
    {
        if (!parseFileName(lp))
            return false;
        if (acceptToken(lp, "<"))
            return parseFileName(lp);
        else
            return true;
    }

    return true;
}

/**
 * The function parseBuiltIn parses a builtin.
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the builtin was parsed successfully.
 */
bool parseBuiltIn(List *lp)
{
    //
    // TODO: Implement the logic for these builtins, and extend with
    // more builtins down the line
    //

    // NULL-terminated array makes it easy to expand this array later
    // without changing the code at other places.
    char *builtIns[] = {
        "exit",
        "status",
        "true",
        "false",
        NULL};
    int i;
    for (i = 0; builtIns[i] != NULL; i++)
    {
        if (acceptToken(lp, builtIns[i]))
        {
            // Depending on the index of the builtIn in the array execute that builtIn
            // exit == 0, status == 1, true == 2, false == 3
            // true and false just change the exitCode since that is checked in the
            if (i == 0)
            {
                freeTokenList(startNode);
                exit(0);
            }
            else if (i == 1)
            {
                printf("The most recent exit code is: %d\n", exitCode);
                return true;
            }
            else if (i == 2)
            {
                exitCode = 0;
                return true;
            }
            else if (i == 3)
            {
                exitCode = 1;
                return true;
            }
        }
    }

    return false;
}

/**
 * The function parseChain parses a chain according to the grammar:
 *
 * <chain>              ::= <pipeline> <redirections>
 *                       |  <builtin> <options>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the chain was parsed successfully.
 */
bool parseChain(List *lp)
{
    if (parseBuiltIn(lp))
    {
        // None of the builtins have options so far
        // So for now this just returns true
        // Instead of parsing the options
        return true;
    }
    if (parsePipeline(lp))
    {
        return parseRedirections(lp);
    }
    return false;
}
/**
 * Function that just blankly parses over list nodes
 * Until an operator is encoutered, practically "skips" next command
 * @param lp List pointer to the start of the tokenlist.
 */
void parseUntilOperator(List *lp)
{
    while (*lp != NULL && !isOperator((*lp)->t))
    {
        (*lp) = (*lp)->next;
    }
    return;
}

/**
 * The function parseInputLine parses an inputline according to the grammar:
 *
 * <inputline>      ::= <chain> & <inputline>
 *                   | <chain> && <inputline>
 *                   | <chain> || <inputline>
 *                   | <chain> ; <inputline>
 *                   | <chain>
 *                   | <empty>
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the inputline was parsed successfully.
 */
bool parseInputLine(List *lp)
{
    if (isEmpty(*lp))
    {
        return true;
    }

    if (!parseChain(lp))
    {
        return false;
    }

    if (acceptToken(lp, "&") || acceptToken(lp, "&&"))
    {
        // If the previous operation was successful do this one
        // If not skip the next command
        if (exitCode == 0)
            return parseInputLine(lp);
        else
            parseUntilOperator(lp);
    }
    if (acceptToken(lp, "||"))
    {
        // If the previous operation was anything but successful do this one
        // If not skip the next command
        if (exitCode != 0)
            return parseInputLine(lp);
        else
            parseUntilOperator(lp);
    }
    // ';' always executes the next command
    if (acceptToken(lp, ";"))
    {
        return parseInputLine(lp);
    }

    return true;
}

/**
 * The function that starts the parser
 * Sets the starting node
 *
 * @param lp List pointer to the start of the tokenlist.
 * @return a bool denoting whether the inputline was parsed successfully.
 */
bool startParser(List *lp)
{
    startNode = *lp;
    return parseInputLine(lp);
}