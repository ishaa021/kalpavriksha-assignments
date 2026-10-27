#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateMatrix(int ***matrix, int matrixSize)
{
    for (int row = 0; row < matrixSize; row++)
    {
        for (int column = 0; column < matrixSize; column++)
        {
            int randomValue = rand() % 256;
            **(*(matrix + row) + column) = randomValue;
        }
    }
}

void printMatrix(int ***matrix, int matrixSize)
{
    for (int row = 0; row < matrixSize; row++)
    {
        for (int column = 0; column < matrixSize; column++)
        {
            printf("%d ", **(*(matrix + row) + column));
        }
        printf("\n");
    }
}

void freeMatrix(int ***matrix, int matrixSize)
{
    if (matrix == NULL)
    {
        return;
    }

    for (int i = 0; i < matrixSize; i++)
    {
        if (*(matrix + i) != NULL)
        {
            for (int j = 0; j < matrixSize; j++)
            {
                free(*(*(matrix + i) + j));
            }

            free(*(matrix + i));
        }
    }

    free(matrix);
}

void rotateMatrix(int ***matrix, int matrixSize)
{
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = i + 1; j < matrixSize; j++)
        {
            int *temp = *(*(matrix + i) + j);

            *(*(matrix + i) + j) = *(*(matrix + j) + i);

            *(*(matrix + j) + i) = temp;
        }
    }

    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize / 2; j++)
        {
            int *temp = *(*(matrix + i) + j);

            *(*(matrix + i) + j) = *(*(matrix + i) + (matrixSize - 1 - j));

            *(*(matrix + i) + (matrixSize - 1 - j)) = temp;
        }
    }
}

void smoothMatrix(int ***matrix, int matrixSize)
{
    int *previousRow = malloc(matrixSize * sizeof(int));
    int *currentRow = malloc(matrixSize * sizeof(int));
    int *nextRow = malloc(matrixSize * sizeof(int));

    if (previousRow == NULL || currentRow == NULL || nextRow == NULL)
    {
        printf("Memory allocation failed.\n");
        free(previousRow);
        free(currentRow);
        free(nextRow);

        return;
    }

    for (int i = 0; i < matrixSize; i++)
    {
        *(previousRow + i) = 0;
        *(currentRow + i) = **(*(matrix + 0) + i);
        *(nextRow + i) = **(*(matrix + 1) + i);
    }  

    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            int sum = 0;
            int count = 0;

            if (i - 1 >= 0)
            {
                sum += *(previousRow + j);
                count++;

                if (j - 1 >= 0)
                {
                    sum += *(previousRow + j - 1);
                    count++;
                }

                if (j + 1 < matrixSize)
                {
                    sum += *(previousRow + j + 1);
                    count++;
                }
            }

            if (j - 1 >= 0)
            {
                sum += *(currentRow + j - 1);
                count++;
            }

            if (j + 1 < matrixSize)
            {
                sum += *(currentRow + j + 1);
                count++;
            }

            if (i + 1 < matrixSize)
            {
                sum += *(nextRow + j);
                count++;

                if (j - 1 >= 0)
                {
                    sum += *(nextRow + j - 1);
                    count++;
                }

                if (j + 1 < matrixSize)
                {
                    sum += *(nextRow + j + 1);
                    count++;
                }
            }

            sum += *(currentRow + j);
            count++;

            int average = sum / count;

            **(*(matrix + i) + j) = average;
        }

        if (i + 1 < matrixSize)
        {
            for (int j = 0; j < matrixSize; j++)
            {
                *(previousRow + j) = *(currentRow + j);
                *(currentRow + j) = *(nextRow + j);
            }

            if (i + 2 < matrixSize)
            {
                for (int j = 0; j < matrixSize; j++)
                {
                    *(nextRow + j) = **(*(matrix + i + 2) + j);
                }
            }
        }
    }

    free(previousRow);
    free(currentRow);
    free(nextRow);
    
}

int main()
{
    int matrixSize;

    printf("Enter matrix size (2-10): ");
    scanf("%d", &matrixSize);

    srand(time(NULL));

    while (matrixSize < 2 || matrixSize > 10)
    {
        printf("Invalid matrix size. Please Enter matrix size (2-10): ");
        scanf("%d", &matrixSize);
    }

    int ***matrix = malloc(matrixSize * sizeof(int **));

    if (matrix == NULL)
    {
        free(matrix);
        printf("Memory allocation failed.");
        return 1;
    }

    for (int i = 0; i < matrixSize; i++)
    {
        *(matrix + i) = NULL;
    }

    for (int i = 0; i < matrixSize; i++)
    {
        *(matrix + i) = malloc(matrixSize * sizeof(int *));

        if (*(matrix + i) == NULL)
        {
            printf("Memory allocation failed.");
            freeMatrix(matrix, matrixSize);
            return 1;
        }

        for (int j = 0; j < matrixSize; j++)
        {
            *(*(matrix + i) + j) = NULL;
        }

        for (int j = 0; j < matrixSize; j++)
        {
            *(*(matrix + i) + j) = malloc(sizeof(int));

            if (*(*(matrix + i) + j) == NULL)
            {
                printf("Memory allocation failed.");
                freeMatrix(matrix, matrixSize);
                return 1;
            }
        }
    }

    generateMatrix(matrix, matrixSize);

    printf("\nOriginal Randomly Generated Matrix:\n");
    printMatrix(matrix, matrixSize);

    rotateMatrix(matrix, matrixSize);

    printf("\nMatrix after 90° Clockwise Rotation:\n");
    printMatrix(matrix, matrixSize);
    
    smoothMatrix(matrix, matrixSize);
   
    printf("\nMatrix after Applying 3x3 Smoothing Filter:\n");
    printMatrix(matrix, matrixSize);

    freeMatrix(matrix, matrixSize);

    return 0;
}