#include <stdio.h>

char fileBuf[10000];
char *treesMat[100];

int visible[100][100][4];

enum
{
    L,
    R,
    T,
    B
};

int tokenize(char *s, char **tokens, int len)
{
    int i = 0;
    tokens[i++] = s;
    for (char *curr = s, *end = s + len; curr < end; curr++)
    {
        if (*curr == '\n')
        {
            *curr = '\0';
            curr++;
            tokens[i++] = curr;
        }
    }

    return tokens[1] - tokens[0] - 1;
}

struct heightsStackInd
{
    int stack[100];
    int front;
};

#define init(s) ((s).front = -1)
#define isempty(s) ((s).front == -1)
#define any(s) ((s).front != -1)
#define push(s, x) ((s).stack[++((s).front)] = (x))
#define pop(s) ((s).stack[(s).front--])
#define top(s) ((s).stack[(s).front])

int main()
{
    struct heightsStackInd s;
    FILE *fp = fopen("input.txt", "r");
    if (!fp)
    {
        fprintf(stderr, "Can't open: input.txt\n");
        return 1;
    }

    int charsRead = fread(fileBuf, 1, sizeof(fileBuf), fp);

    fclose(fp);

    int dim = tokenize(fileBuf, treesMat, charsRead);

    for (int i = 0; i < dim; i++)
    {
        // left
        init(s);
        int j = 0;
        while (j < dim)
        {
            if (isempty(s) || treesMat[i][top(s)] >= treesMat[i][j])
            {
                push(s, j);
                j++;
            }
            else
            {
                int k = pop(s);
                visible[i][k][L] = (isempty(s) ? k : k - top(s));
            }
        }

        while (any(s))
        {
            int k = pop(s);
            visible[i][k][L] = (isempty(s) ? k : k - top(s));
        }

        // right
        j = dim - 1;
        while (j >= 0)
        {
            if (isempty(s) || treesMat[i][top(s)] >= treesMat[i][j])
            {
                push(s, j);
                j--;
            }
            else
            {
                int k = pop(s);
                visible[i][k][R] = (isempty(s) ? dim - k -1: top(s) - k);
            }
        }

        while (any(s))
        {
            int k = pop(s);
            visible[i][k][R] = (isempty(s) ? dim - k -1: top(s) - k);
        }
    }

    for (int j = 0; j < dim; j++)
    {
        // top
        init(s);
        int i = 0;
        while (i < dim)
        {
            if (isempty(s) || treesMat[top(s)][j] >= treesMat[i][j])
            {
                push(s, i);
                i++;
            }
            else
            {
                int k = pop(s);
                visible[k][j][T] = (isempty(s) ? k : k - top(s));
            }
        }
        while (any(s))
        {
            int k = pop(s);
            visible[k][j][T] = (isempty(s) ? k : k - top(s));
        }

        //bottom
        i = dim - 1;
        while (i >= 0)
        {
            if (isempty(s) || treesMat[top(s)][j] >= treesMat[i][j])
            {
                push(s, i);
                i--;
            }
            else
            {
                int k = pop(s);
                visible[k][j][B] = (isempty(s) ? dim-k-1 : k - top(s));
            }
        }
        while (any(s))
        {
            int k = pop(s);
            visible[k][j][B] = (isempty(s) ? dim - k -1: top(s) - k);
        }
    }

    int maxScore = -1;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            int score = 1;
            for (int k = 0; k < 4; k++)
            {
                score *= visible[i][j][k];
            }
            if (score > maxScore)
                maxScore = score;
        }
    }

    printf("%d\n", maxScore);
}