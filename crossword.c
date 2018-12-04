#include <stdio.h>
#include <stdlib.h>

/* struct to hold the puzzle data and its length and width for access */
struct puzzle
{
    int length;
    int width;
    char **content;
};

struct puzzle *initialize_game_from_file()
{
    FILE *fp;
    char path[256];
    char puzzle_letters[80*80];
    char comma_or_newline;
    int length = 0;
    int width = 0;
    int count = 0;

    /* get the path from the user */
    printf("Enter the path: ");
    scanf(" %s", path);
    getchar();

    fp = fopen(path, "r");
    if (fp == NULL)
    {
        perror("Error in opening file");
        return NULL;
    }
    /* read from the file until the file is empty */
    do
    {
        /* store the letters into a 1d array for now */
        puzzle_letters[count] = fgetc(fp);
        /* input validation */
        if(puzzle_letters[count] < 'A' || puzzle_letters[count] > 'Z')
        {
            perror("File contains invalid characters");
            return NULL;
        }
        /* debug printf("%c", puzzle_letters[count]); */
        if (length < 1)
            width++;
        /* read the comma or newline */
        comma_or_newline = fgetc(fp);
        /* break when end of file */
        if (feof(fp))
        {
            length++;
            break;
        }
        /* validation */
        if (!(comma_or_newline == '\n' || comma_or_newline == ','))
        {
            perror("File contains invalid characters");
            return NULL;
        }
        if (comma_or_newline == '\n')
            length++;

        count++;
    } while (1);
    /* debug printf("Length: %d, Width: %d", length, width); */

    /* allocate memory based on length, width */
    char **content = calloc(length, sizeof(char *));
    for (int i = 0; i < length; i++)
    {
        content[i] = calloc(width, sizeof(char));
    }
    /* initialize array */
    int k = 0; /* count for puzzle_letters */
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < width; j++, k++)
        {
            content[i][j] = puzzle_letters[k];
        }
    }
    /* create crossword struct with the puzzle_letters */
    struct puzzle *p = malloc(sizeof(struct puzzle));
    p->length = length;
    p->width = width;
    p->content = content;

    return p;
}

void print_puzzle(struct puzzle *p)
{
    for (int i = 0; i < p->length; i++)
    {
        for (int j = 0; j < p->width; j++)
        {
            printf("%c ", p->content[i][j]);
        }
        printf("\n");
    }
}

/*  check_types 
    1: left to right
    2: right to left
    3: top to bottom
    4: bottom to top
    5: top left to bottom right
    6: top right to bottom left
    7: bottom left to top right
    8: bottom right to top left
*/
int check(struct puzzle *p, int i, int j, char *word, int check_type)
{
    int word_count = 1;
    do
    {   
        /* bound checking */
        if(i < 0 || j < 0 || i >= p->length || j >= p->width)
            return 0;

        /* debug printf("word: %c, content: %c\n", word[word_count], p->content[i][j]); */
        if (!word[word_count])
            return 1; /* word found */

        if (!p->content[i][j])
            break; /* word not found */

        if (p->content[i][j] != word[word_count])
            break; /* word not found */

        word_count++;
        if (check_type == 1)
        {
            j++;
        }
        if (check_type == 2)
        {
            j--;
        }
        if (check_type == 3)
        {
            i++;
        }
        if (check_type == 4)
        {
            i--;
        }
        if (check_type == 5)
        {
            i++;
            j++;
        }
        if (check_type == 6)
        {
            i++;
            j--;
        }
        if (check_type == 7)
        {
            i--;
            j++;
        }
        if (check_type == 8)
        {
            i--;
            j--;
        }
    } while (1);
    /* word was not found */
    return 0;
}

int find_word(struct puzzle *p, char *word)
{
    for (int i = 0; i < p->length; i++)
    {
        for (int j = 0; j < p->width; j++)
        {
            if (p->content[i][j] == word[0])
            {
                /* left to right */
                if (check(p, i, j + 1, word, 1))
                {
                    return 1;
                }
                /* right to left */
                if (check(p, i, j - 1, word, 2))
                {
                    return 1;
                }
                /* top to bottom */
                if (check(p, i + 1, j, word, 3))
                {
                    return 1;
                }
                /* bottom to top */
                if (check(p, i - 1, j, word, 4))
                {
                    return 1;
                }
                /* top left to bottom right */
                if (check(p, i + 1, j + 1, word, 5))
                {
                    return 1;
                }
                /* top right to bottom left */
                if (check(p, i + 1, j - 1, word, 6))
                {
                    return 1;
                }
                /* bottom left to top right */
                if (check(p, i - 1, j + 1, word, 7))
                {
                    return 1;
                }
                /* bottom right to top left */
                if (check(p, i - 1, j - 1, word, 8))
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int main()
{
    int input = 'a';
    struct puzzle *p;
    while (input != 'd')
    {
        printf("\na. Enter file location (full path)\n");
        printf("b. Display the crossword\n");
        printf("c. Find a word\n");
        printf("d. Exit\n");

        scanf(" %c", &input);
        getchar();

        if (input == 'a')
        {
            p = initialize_game_from_file();
        }
        else if (input == 'b')
        {
            print_puzzle(p);
        }
        else if (input == 'c')
        {   
            printf("Enter a word to search for (CAPITALIZED): ");
            char word[256];
            scanf(" %s", word);

            int found = find_word(p, word);

            if (found)
                printf("%s was found!\n", word);
            else
                printf("%s was not found!\n", word);
        }
    }
    free(p->content);
    free(p);
    return 0;
}