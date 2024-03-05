For the base assignment I did everything standardly.

For the bonus points I implemented "Display a prompt before each input line, such as [folder]>. Of course, in this case, ; and
\n should not be handled identically anymore."

I just added the following lines of code to the while loop in the main:
#if EXT_PROMPT
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("[%s]>", cwd);
#endif
