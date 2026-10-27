#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int n;

    printf("Enter matrix size (2-10): ");
    scanf("%d", &n);

    srand(time(NULL));

    while (n < 2 || n > 10)
    {
        printf("Invalid matrix size. Please Enter matrix size (2-10): ");
        scanf("%d", &n);
    }

    int ***matrix = malloc(n * sizeof(int **));

    if (matrix == NULL)
    {
        free(matrix);
        printf("Memory allocation failed.");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        *(matrix + i) = malloc(n * sizeof(int *));

        if (*(matrix + i) == NULL)
        {
            for (int k = 0 ; k < n ; k++)
            {
            free(*(matrix + k));
            }
            free(matrix);
            printf("Memory allocation failed.");
            return 1;
        }

        for (int j = 0; j < n; j++)
        {
            *(*(matrix + i) + j) = malloc(sizeof(int));

            if (*(*(matrix + i) + j) == NULL)
            {
                for ( int l = 0 ; l < n ; l++)
                {
                    for ( int m = 0 ; m < n ; m++)
                    {
                    free(*(*(matrix + l) + m));
                    }
                    free(*(matrix + l));
                }
                free(matrix);
                printf("Memory allocation failed.");
                return 1;
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int num = rand() % 256;
            **(*(matrix + i) + j) = num;
        }
    }

    printf("\nOriginal Randomly Generated Matrix:\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", **(*(matrix + i) + j));
        }

        printf("\n");
    }

    // Transpose using the address

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            int *temp = *(*(matrix + i) + j);

            *(*(matrix + i) + j) = *(*(matrix + j) + i);

            *(*(matrix + j) + i) = temp;
        }
    }

    // Reverse using the address

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n / 2; j++)
        {
            int *temp = *(*(matrix + i) + j);

            *(*(matrix + i) + j) =
                *(*(matrix + i) + (n - 1 - j));

            *(*(matrix + i) + (n - 1 - j)) = temp;
        }
    }

    printf("\nMatrix after 90° Clockwise Rotation:\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", **(*(matrix + i) + j));
        }

        printf("\n");
    }

    int *previousRow = malloc(n * sizeof(int));
    int *currentRow = malloc(n * sizeof(int));
    int *nextRow = malloc(n * sizeof(int));

    if (previousRow == NULL || currentRow == NULL || nextRow == NULL)
    {
        printf("Memory allocation failed.\n");
        free(previousRow);
        free(currentRow);
        free(nextRow);

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                free(*(*(matrix + i) + j));
            }
            free(*(matrix + i));
        }

        free(matrix);
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        *(previousRow + i) = 0;
        *(currentRow + i) = **(*(matrix + 0) + i);
        *(nextRow + i) = **(*(matrix + 1) + i);
    }

    printf("\nMatrix after Applying 3×3 Smoothing Filter:\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
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

                if (j + 1 < n)
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

            if (j + 1 < n)
            {
                sum += *(currentRow + j + 1);
                count++;
            }

            if (i + 1 < n)
            {
                sum += *(nextRow + j);
                count++;

                if (j - 1 >= 0)
                {
                    sum += *(nextRow + j - 1);
                    count++;
                }

                if (j + 1 < n)
                {
                    sum += *(nextRow + j + 1);
                    count++;
                }
            }

            sum += *(currentRow + j);
            count++;

            int avg = sum / count;

            **(*(matrix + i) + j) = avg;

            printf("%d ", avg);
        }

        printf("\n");

        if (i + 1 < n)
        {
            for (int j = 0; j < n; j++)
            {
                *(previousRow + j) = *(currentRow + j);
                *(currentRow + j) = *(nextRow + j);
            }

            if (i + 2 < n)
            {
                for (int j = 0; j < n; j++)
                {
                    *(nextRow + j) = **(*(matrix + i + 2) + j);
                }
            }
        }
    }
    free(previousRow);
    free(currentRow);
    free(nextRow);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            free(*(*(matrix + i) + j));
        }
        free(*(matrix + i));
    }
    free(matrix);

    return 0;
}