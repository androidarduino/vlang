int main()
{
    /* This is a multi-line comment.
     * It should be ignored by the lexer. */
    int a = 10;
    float b = 20.5f;
    char c = 'X';
    const char *message = "Hello, World!";

    if (a < b)
    {
        printf("%s\n", message);
    }
    else
    {
        for (int i = 0; i < a; ++i)
        {
            printf("Iteration %d\n", i);
        }
    }

    return 0;
}
