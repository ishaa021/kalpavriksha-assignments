#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Block
{
    int start;
    int size;
    int id;
    int isFree;
};

int increaseCapacity(struct Block **blocks, int *capacity, int blockCount)
{
    if (blockCount < *capacity)
    {
        return 0;
    }

    int newCapacity = *capacity * 2;

    struct Block *temp = realloc(*blocks, newCapacity * sizeof(struct Block));

    if (temp == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    *blocks = temp;
    *capacity = newCapacity;

    return 0;
}

void insertBlock(struct Block *blocks, int *blockCount, int position, struct Block newBlock)
{
    for (int i = *blockCount; i > position; i--)
    {
        blocks[i] = blocks[i - 1];
    }

    blocks[position] = newBlock;
    (*blockCount)++;
}

void removeBlock(struct Block *blocks, int *blockCount, int position)
{
    for (int i = position; i < *blockCount - 1; i++)
    {
        blocks[i] = blocks[i + 1];
    }
    (*blockCount)--;
}

void mergeBlocks(struct Block *blocks, int *blockCount)
{
    int i = 0;

    while (i < *blockCount - 1)
    {
        if (blocks[i].isFree == 1 && blocks[i + 1].isFree == 1)
        {
            blocks[i].size = blocks[i].size + blocks[i + 1].size;
            removeBlock(blocks, blockCount, i + 1);
        }
        else
        {
            i++;
        }
    }
}

int findBlock(struct Block *blocks, int blockCount, int requestedSize, int strategy)
{
    int selected = -1;

    for (int i = 0; i < blockCount; i++)
    {
        if (blocks[i].isFree == 1 && blocks[i].size >= requestedSize)
        {
            if (strategy == 1)
            {
                return i;
            }
            else if (strategy == 2)
            {
                if (selected == -1 || blocks[i].size < blocks[selected].size)
                {
                    selected = i;
                }
            }
            else if (strategy == 3)
            {
                if (selected == -1 || blocks[i].size > blocks[selected].size)
                {
                    selected = i;
                }
            }
        }
    }

    return selected;
}

int initializePool(char **memoryPool, struct Block **blocks, int *poolSize, int *capacity, int *blockCount)
{
    printf("Enter memory pool size: ");
    scanf("%d", poolSize);

    while (*poolSize < 1 || *poolSize > 1000000)
    {
        printf("Invalid pool size.\n");
        printf("Enter new memory pool size: ");
        scanf("%d", poolSize);
    }

    *memoryPool = malloc(*poolSize);

    if (*memoryPool == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Memory pool of %d bytes created successfully.\n", *poolSize);

    *capacity = 2;
    *blocks = malloc(*capacity * sizeof(struct Block));

    if (*blocks == NULL)
    {
        printf("Metadata allocation failed.\n");
        free(*memoryPool);
        *memoryPool = NULL;
        return 1;
    }

    (*blocks)[0].start = 0;
    (*blocks)[0].size = *poolSize;
    (*blocks)[0].id = -1;
    (*blocks)[0].isFree = 1;

    *blockCount = 1;

    printf("\nMemory pool initialized successfully!\n");
    printf("Total pool size: %d bytes\n", *poolSize);
    printf("Total blocks: %d\n", *blockCount);
    printf("Metadata capacity: %d\n", *capacity);

    return 0;
}

void displayMemoryMap(struct Block *blocks, int blockCount, int poolSize)
{
    printf("\nMemory Map (%d bytes):\n", poolSize);

    for (int i = 0; i < blockCount; i++)
    {
        if (blocks[i].isFree == 1)
        {
            printf("[Free: %d bytes, Start: %d]\n",
                   blocks[i].size,
                   blocks[i].start);
        }
        else
        {
            printf("[Allocated #%d: %d bytes, Start: %d]\n",
                   blocks[i].id,
                   blocks[i].size,
                   blocks[i].start);
        }
    }
}

void changeStrategy(int *strategy)
{
    int choice;

    printf("\nSelect allocation strategy:\n");
    printf("1. First Fit\n");
    printf("2. Best Fit\n");
    printf("3. Worst Fit\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        *strategy = 1;
        printf("First Fit selected.\n");
    }
    else if (choice == 2)
    {
        *strategy = 2;
        printf("Best Fit selected.\n");
    }
    else if (choice == 3)
    {
        *strategy = 3;
        printf("Worst Fit selected.\n");
    }
    else
    {
        printf("Invalid strategy.\n");
    }
}

void allocateMemory(struct Block **blocks, int *blockCount, int *capacity, int poolSize, int *nextId, int strategy)
{
    int size;

    printf("Enter requested size: ");
    scanf("%d", &size);

    if (size <= 0 || size > poolSize)
    {
        printf("Invalid allocation size.\n");
        return;
    }

    int index = findBlock(*blocks, *blockCount, size, strategy);

    if (index == -1)
    {
        int totalFree = 0;

        for (int i = 0; i < *blockCount; i++)
        {
            if ((*blocks)[i].isFree == 1)
            {
                totalFree = totalFree + (*blocks)[i].size;
            }
        }

        printf("Allocation failed. No suitable free block.\n");

        if (totalFree >= size)
        {
            printf("External fragmentation detected.\n");
            printf("Total free memory: %d bytes\n", totalFree);
            printf("Individual free blocks:\n");

            for (int i = 0; i < *blockCount; i++)
            {
                if ((*blocks)[i].isFree == 1)
                {
                    printf("%d bytes\n", (*blocks)[i].size);
                }
            }
        }
        return;
    }

    if ((*blocks)[index].size > size)
    {
        if (increaseCapacity(blocks, capacity, *blockCount) == 1)
        {
            return;
        }
    }

    int id = *nextId;

    if ((*blocks)[index].size > size)
    {
        struct Block newBlock;

        newBlock.start = (*blocks)[index].start + size;
        newBlock.size = (*blocks)[index].size - size;
        newBlock.id = -1;
        newBlock.isFree = 1;

        insertBlock(*blocks, blockCount, index + 1, newBlock);
    }

    (*blocks)[index].size = size;
    (*blocks)[index].id = id;
    (*blocks)[index].isFree = 0;

    (*nextId)++;

    printf("\nAllocation successful!\n");
    printf("Allocation ID: %d\n", id);
    printf("Start: %d\n", (*blocks)[index].start);
    printf("Size: %d bytes\n", size);
}

void releaseMemory(struct Block *blocks, int *blockCount)
{
    int id;
    int found = 0;

    printf("Enter allocation ID: ");
    scanf("%d", &id);

    for (int i = 0; i < *blockCount; i++)
    {
        if (blocks[i].isFree == 0 && blocks[i].id == id)
        {
            blocks[i].isFree = 1;
            blocks[i].id = -1;

            mergeBlocks(blocks, blockCount);

            found = 1;

            printf("Memory released successfully.\n");

            break;
        }
    }

    if (found == 0)
    {
        printf("Invalid allocation ID.\n");
    }
}

void resizeMemory(struct Block **blocks, int *blockCount, int *capacity, char *memoryPool, int poolSize, int strategy)
{
    int id;
    int newSize;

    printf("Enter allocation ID: ");
    scanf("%d", &id);

    int index = -1;

    for (int i = 0; i < *blockCount; i++)
    {
        if ((*blocks)[i].isFree == 0 && (*blocks)[i].id == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Invalid allocation ID.\n");
        return;
    }

    printf("Enter new size: ");
    scanf("%d", &newSize);

    if (newSize <= 0 || newSize > poolSize)
    {
        printf("Invalid size.\n");
        return;
    }

    int oldSize = (*blocks)[index].size;

    if (newSize == oldSize)
    {
        printf("Size is already the same.\n");
        return;
    }

    if (newSize < oldSize)
    {
        if (increaseCapacity(blocks, capacity, *blockCount) == 1)
        {
            return;
        }

        struct Block newBlock;

        newBlock.start = (*blocks)[index].start + newSize;
        newBlock.size = oldSize - newSize;
        newBlock.id = -1;
        newBlock.isFree = 1;

        (*blocks)[index].size = newSize;

        insertBlock(*blocks, blockCount, index + 1, newBlock);
        mergeBlocks(*blocks, blockCount);
        printf("Allocation decreased successfully.\n");

        return;
    }

    int extra = newSize - oldSize;

    if (index + 1 < *blockCount && (*blocks)[index + 1].isFree == 1 && (*blocks)[index + 1].size >= extra)
    {
        (*blocks)[index].size = newSize;
        (*blocks)[index + 1].start = (*blocks)[index + 1].start + extra;
        (*blocks)[index + 1].size = (*blocks)[index + 1].size - extra;

        if ((*blocks)[index + 1].size == 0)
        {
            removeBlock(*blocks, blockCount, index + 1);
        }

        printf("Allocation expanded without moving.\n");

        return;
    }

    int newIndex = findBlock(*blocks, *blockCount, newSize, strategy);

    if (newIndex == -1)
    {
        printf("Resize failed. Original allocation unchanged.\n");
        return;
    }

    if ((*blocks)[newIndex].size > newSize)
    {
        if (increaseCapacity(blocks, capacity, *blockCount) == 1)
        {
            return;
        }
    }

    int oldStart = (*blocks)[index].start;
    int newStart = (*blocks)[newIndex].start;

    memmove(memoryPool + newStart, memoryPool + oldStart, oldSize);

    if ((*blocks)[newIndex].size > newSize)
    {
        struct Block freeBlock;

        freeBlock.start = newStart + newSize;
        freeBlock.size = (*blocks)[newIndex].size - newSize;
        freeBlock.id = -1;
        freeBlock.isFree = 1;

        (*blocks)[newIndex].size = newSize;
        (*blocks)[newIndex].id = id;
        (*blocks)[newIndex].isFree = 0;

        insertBlock(*blocks, blockCount, newIndex + 1, freeBlock);
    }
    else
    {
        (*blocks)[newIndex].id = id;
        (*blocks)[newIndex].isFree = 0;
    }

    for (int i = 0; i < *blockCount; i++)
    {
        if ((*blocks)[i].start == oldStart &&
            (*blocks)[i].id == id &&
            (*blocks)[i].isFree == 0)
        {
            (*blocks)[i].isFree = 1;
            (*blocks)[i].id = -1;

            break;
        }
    }

    mergeBlocks(*blocks, blockCount);

    printf("Allocation moved and resized successfully.\n");
}

void displayStatistics(struct Block *blocks, int blockCount, int poolSize)
{
    int allocated = 0;
    int freeMemory = 0;
    int allocatedBlocks = 0;
    int freeBlocks = 0;
    int largestFree = 0;

    for (int i = 0; i < blockCount; i++)
    {
        if (blocks[i].isFree == 1)
        {
            freeMemory = freeMemory + blocks[i].size;
            freeBlocks++;

            if (blocks[i].size > largestFree)
            {
                largestFree = blocks[i].size;
            }
        }
        else
        {
            allocated = allocated + blocks[i].size;
            allocatedBlocks++;
        }
    }

    printf("\n----- Memory Statistics -----\n");
    printf("Total memory: %d bytes\n", poolSize);
    printf("Allocated memory: %d bytes\n", allocated);
    printf("Free memory: %d bytes\n", freeMemory);
    printf("Allocated blocks: %d\n", allocatedBlocks);
    printf("Free blocks: %d\n", freeBlocks);
    printf("Largest free block: %d bytes\n", largestFree);
}

int main()
{
    char *memoryPool;
    struct Block *blocks;
    int poolSize;
    int capacity;
    int blockCount;
    int nextId = 1;
    int strategy = 1;
    int choice;

    if (initializePool(&memoryPool, &blocks, &poolSize, &capacity, &blockCount) == 1)
    {
        return 1;
    }

    changeStrategy(&strategy);

    do
    {
        printf("\n========== SMART MEMORY MANAGER ==========\n");

        printf("1. Allocate memory\n");
        printf("2. Release memory\n");
        printf("3. Resize allocation\n");
        printf("4. Display memory map\n");
        printf("5. Display memory statistics\n");
        printf("6. Change allocation strategy\n");
        printf("7. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                allocateMemory(&blocks, &blockCount, &capacity,
                               poolSize, &nextId, strategy);
                break;

            case 2:
                releaseMemory(blocks, &blockCount);
                break;

            case 3:
                resizeMemory(&blocks, &blockCount, &capacity,
                             memoryPool, poolSize, strategy);
                break;

            case 4:
                displayMemoryMap(blocks, blockCount, poolSize);
                break;

            case 5:
                displayStatistics(blocks, blockCount, poolSize);
                break;

            case 6:
                changeStrategy(&strategy);
                break;

            case 7:
                printf("Exiting Memory Manager.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 7);

    free(memoryPool);
    free(blocks);

    printf("Memory released successfully.\n");

    return 0;
}