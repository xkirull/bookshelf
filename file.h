#define FILE_NAME "books.txt"
#define BUF_SIZE 65536
#define FILE_TABLE_SIZE 4
#define FILE_BASE_DELIMITER "\n"
#define FILE_STRING_DELIMITER " | "

void initFile()
{
    fclose(fopen(FILE_NAME, "ab+"));
}

int countLines()
{
    FILE *fp;
    int count = 0;
    char c;
  
    fp = fopen(FILE_NAME, "r");
 
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n')
            count = count + 1;
 
    fclose(fp);
 
    return count;
}

long fileSize()
{
    FILE *file = fopen(FILE_NAME, "r");

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    fclose(file);

    return size;
}

char *readFile() 
{
    char *fileContent = NULL;

    FILE *file = fopen(FILE_NAME, "r");

    long size = fileSize();

    fileContent = malloc(size);
    fread(fileContent, 1, size, file);

    fclose(file);

    return fileContent;
}

void writeFile(char **content, int count)
{
    fclose(fopen(FILE_NAME, "w"));

    FILE *file = fopen(FILE_NAME, "a");

    for (int i = 0; i < count; i++)
    {
        fprintf(file, content[i]);
        fprintf(file, "\n");
    }

    fclose(file);
}
