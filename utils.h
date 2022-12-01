char *stringFull(char *string, char *buff)
{
    if(string[0] == '\0'){
        string = buff;
    }

    return string;
}

char **parceString(char *data, char *delimiter)
{
    long sizeTable = fileSize();

    char **arrayTableValue = malloc(sizeTable * 128);

    char *ptr = strtok(data, delimiter);

    int i = 0;

	while (ptr != NULL)
	{
        arrayTableValue[i] = malloc(sizeof(ptr));
        arrayTableValue[i++] = ptr;

		ptr = strtok(NULL, delimiter);
	}

    return arrayTableValue;
}

const wchar_t *GetWC(const char *str)
{
    wchar_t *ws = malloc(strlen(str) * sizeof(char*));
    swprintf(ws, strlen(str) * sizeof(char*), L"%hs", str);
    return ws;
}

char *copyString(char *string)
{
    size_t destination_size = strlen(string) * sizeof(char*);
    char *newString = malloc(destination_size);

    strncpy(newString, string, destination_size);
    newString[destination_size - 1] = '\0';

    return newString;
}

void updateItemsIndex(char **array, int count)
{
    for(int i = 0; i < count; i++)
    {
        char **arrayElements = parceString(copyString(array[i]), FILE_STRING_DELIMITER);

        int resultSize = strlen(array[i]) * sizeof(char*) + 2;
        char *result = malloc(resultSize);

        snprintf(result, resultSize, "%d | %s | %s | %s", i + 1, arrayElements[1], arrayElements[2], arrayElements[3]);

        array[i] = result;
    }
}

void openLink(char *link)
{
    char startCommand[] = "start";

    size_t sizeCommand = strlen(link) * sizeof(char*) + sizeof(startCommand) + 1;
    char *command = malloc(sizeCommand);

    snprintf(command, sizeCommand, "%s %s", startCommand, link);

    system(command);
}

void deleteItemFromArray(char **array, unsigned int index)
{
    size_t count = countLines();
    size_t i;

    for(i = index; i < count - 1; i++)
    {
        array[i] = array[i + 1];
    }

    array[i] = NULL;
}

void appendArray(char **array, char *string, size_t size)
{
    int arrayNewLine = countLines();
    array[arrayNewLine] = malloc(size);
    array[arrayNewLine] = string;
}

void setValueInArray(char **array, char *string, size_t size, size_t index)
{
    array[index] = malloc(size);
    array[index] = string;
}
