#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define MAX_NAME 50
#define MAX_INPUT 8192
#define MAX_DEPTH 100

struct FreeBlock
{
    int index;
    struct FreeBlock *next;
    struct FreeBlock *prev;
};

struct FileNode
{
    char name[MAX_NAME + 1];
    int isDirectory;
    int size;
    int *blockPointers;
    int blockCount;
    struct FileNode *children;
    struct FileNode *parent;
    struct FileNode *next;
};

char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];

struct FreeBlock *head = NULL;
struct FreeBlock *lastNode = NULL;
struct FileNode *root = NULL;
struct FileNode *cwd = NULL;

int usedBlocks = 0;

struct FileNode *createNode(char name[], int isDirectory)
{
    struct FileNode *newNode = malloc(sizeof(struct FileNode));
    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    strcpy(newNode->name, name);
    newNode->isDirectory = isDirectory;
    newNode->size = 0;
    newNode->blockPointers = NULL;
    newNode->blockCount = 0;
    newNode->children = NULL;
    newNode->parent = NULL;
    newNode->next = NULL;
    return newNode;
}

void initializeFreeList()
{
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        struct FreeBlock *newNode = malloc(sizeof(struct FreeBlock));
        if (newNode == NULL)
        {
            printf("Memory allocation failed!\n");
            struct FreeBlock *temp = head;
            while (temp != NULL)
            {
                struct FreeBlock *nextNode = temp->next;
                free(temp);
                temp = nextNode;
            }
            head = NULL;
            lastNode = NULL;
            exit(1);
        }

        newNode->index = i;
        newNode->next = NULL;
        newNode->prev = lastNode;

        if (head == NULL)
        {
            head = newNode;
        }
        else
        {
            lastNode->next = newNode;
        }
        lastNode = newNode;
    }
}

void initializeVFS()
{
    initializeFreeList();
    root = createNode("/", 1);
    if (root == NULL)
    {
        exit(1);
    }
    cwd = root;
    printf("Virtual File System initialized successfully!\n");
}

void insertChild(struct FileNode *parent, struct FileNode *child)
{
    child->parent = parent;

    if (parent->children == NULL)
    {
        parent->children = child;
        child->next = child;
        return;
    }

    struct FileNode *temp = parent->children;
    while (temp->next != parent->children)
    {
        temp = temp->next;
    }
    temp->next = child;
    child->next = parent->children;
}

struct FileNode *findChild(struct FileNode *parent, char name[])
{
    if (parent == NULL || parent->children == NULL)
    {
        return NULL;
    }

    struct FileNode *temp = parent->children;
    do
    {
        if (strcmp(temp->name, name) == 0)
        {
            return temp;
        }
        temp = temp->next;
    } while (temp != parent->children);
    return NULL;
}

void removeChild(struct FileNode *parent, struct FileNode *target)
{
    if (parent == NULL || target == NULL || parent->children == NULL)
    {
        return;
    }

    struct FileNode *temp = parent->children;
    struct FileNode *previous = NULL;
    do
    {
        if (temp == target)
        {
            break;
        }
        previous = temp;
        temp = temp->next;

    } while (temp != parent->children);

    if (temp != target)
    {
        return;
    }
    if (target->next == target)
    {
        parent->children = NULL;
    }
    else
    {
        previous->next = target->next;

        if (parent->children == target)
        {
            parent->children = target->next;
        }
    }

    target->next = NULL;
    target->parent = NULL;
}

void createEntry(char name[], int isDirectory)
{
    if (strlen(name) == 0 || strlen(name) > MAX_NAME || strchr(name, '/') != NULL || strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
    {
        printf("Invalid name!\n");
        return;
    }

    if (findChild(cwd, name) != NULL)
    {
        printf("Name already exists!\n");
        return;
    }

    struct FileNode *newNode = createNode(name, isDirectory);
    if (newNode == NULL)
    {
        return;
    }

    insertChild(cwd, newNode);
    if (isDirectory == 1)
    {
        printf("Directory created successfully!\n");
    }
    else
    {
        printf("File created successfully!\n");
    }
}

void listDirectory()
{
    if (cwd->children == NULL)
    {
        printf("Directory is empty.\n");
        return;
    }

    struct FileNode *temp = cwd->children;
    printf("\nContents:\n");
    do
    {
        if (temp->isDirectory == 1)
        {
            printf("[DIR]  %s\n", temp->name);
        }
        else
        {
            printf("[FILE] %s  (%d bytes)\n", temp->name, temp->size);
        }
        temp = temp->next;
    } while (temp != cwd->children);
}

void changeDirectory(char name[])
{
    if (strcmp(name, "/") == 0)
    {
        cwd = root;
        return;
    }

    if (strcmp(name, "..") == 0)
    {
        if (cwd->parent != NULL)
        {
            cwd = cwd->parent;
        }

        return;
    }

    struct FileNode *target = findChild(cwd, name);
    if (target == NULL)
    {
        printf("Directory not found!\n");
        return;
    }
    if (target->isDirectory == 0)
    {
        printf("Cannot enter a file!\n");
        return;
    }
    cwd = target;
}

int allocateBlock()
{
    if (head == NULL)
    {
        return -1;
    }

    struct FreeBlock *temp = head;
    int blockIndex = temp->index;
    head = head->next;

    if (head != NULL)
    {
        head->prev = NULL;
    }
    else
    {
        lastNode = NULL;
    }

    free(temp);
    usedBlocks++;
    return blockIndex;
}

void freeBlock(int index)
{
    struct FreeBlock *newNode = malloc(sizeof(struct FreeBlock));

    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    newNode->index = index;
    newNode->next = NULL;
    newNode->prev = lastNode;

    if (head == NULL)
    {
        head = newNode;
    }
    else
    {
        lastNode->next = newNode;
    }

    lastNode = newNode;
    usedBlocks--;
}

void writeFile(char name[], char content[])
{
    struct FileNode *file = findChild(cwd, name);
    if (file == NULL)
    {
        printf("File not found!\n");
        return;
    }
    if (file->isDirectory == 1)
    {
        printf("Cannot write into a directory!\n");
        return;
    }
    int newSize = strlen(content);
    int requiredBlocks = (newSize + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int availableBlocks = NUM_BLOCKS - usedBlocks + file->blockCount;
    if (requiredBlocks > availableBlocks)
    {
        printf("Not enough disk space!\n");
        return;
    }

    int *newPointers = NULL;
    if (requiredBlocks > 0)
    {
        newPointers = malloc(requiredBlocks * sizeof(int));
        if (newPointers == NULL)
        {
            printf("Memory allocation failed!\n");
            return;
        }
    }

    for (int i = 0; i < file->blockCount; i++)
    {
        freeBlock(file->blockPointers[i]);
    }

    free(file->blockPointers);

    file->blockPointers = NULL;
    file->blockCount = 0;

    for (int i = 0; i < requiredBlocks; i++)
    {
        newPointers[i] = allocateBlock();
    }

    for (int i = 0; i < newSize; i++)
    {
        int blockNumber = i / BLOCK_SIZE;
        int offset = i % BLOCK_SIZE;

        int diskIndex = newPointers[blockNumber];

        virtualDisk[diskIndex][offset] = (char)content[i];
    }

    file->blockPointers = newPointers;
    file->blockCount = requiredBlocks;
    file->size = newSize;

    printf("File written successfully!\n");
}

void readFile(char name[])
{
    struct FileNode *file = findChild(cwd, name);

    if (file == NULL)
    {
        printf("File not found!\n");
        return;
    }

    if (file->isDirectory == 1)
    {
        printf("Cannot read a directory!\n");
        return;
    }

    printf("\nFile content:\n");

    for (int i = 0; i < file->size; i++)
    {
        int blockNumber = i / BLOCK_SIZE;
        int offset = i % BLOCK_SIZE;

        int diskIndex = file->blockPointers[blockNumber];

        putchar(virtualDisk[diskIndex][offset]);
    }

    printf("\n");
}

void deleteFile(char name[])
{
    struct FileNode *file = findChild(cwd, name);

    if (file == NULL)
    {
        printf("File not found!\n");
        return;
    }

    if (file->isDirectory == 1)
    {
        printf("Use rmdir to remove a directory!\n");
        return;
    }

    for (int i = 0; i < file->blockCount; i++)
    {
        freeBlock(file->blockPointers[i]);
    }

    free(file->blockPointers);
    removeChild(cwd, file);
    free(file);
    printf("File deleted successfully!\n");
}

void removeDirectory(char name[])
{
    struct FileNode *directory = findChild(cwd, name);

    if (directory == NULL)
    {
        printf("Directory not found!\n");
        return;
    }
    if (directory->isDirectory == 0)
    {
        printf("This is a file, not a directory!\n");
        return;
    }
    if (directory->children != NULL)
    {
        printf("Directory is not empty!\n");
        return;
    }
    removeChild(cwd, directory);
    free(directory);
    printf("Directory removed successfully!\n");
}

void displayDiskUsage()
{
    int freeBlocks = NUM_BLOCKS - usedBlocks;

    printf("\nDisk Information:\n");
    printf("Total Blocks : %d\n", NUM_BLOCKS);
    printf("Used Blocks  : %d\n", usedBlocks);
    printf("Free Blocks  : %d\n", freeBlocks);
    printf("Block Size   : %d bytes\n", BLOCK_SIZE);
    printf("Total Space  : %d bytes\n", NUM_BLOCKS * BLOCK_SIZE);
    printf("Used Space   : %d bytes\n", usedBlocks * BLOCK_SIZE);
    printf("Free Space   : %d bytes\n", freeBlocks * BLOCK_SIZE);
}

void cleanupDirectory(struct FileNode *directory)
{
    if (directory == NULL || directory->children == NULL)
    {
        return;
    }
    struct FileNode *first = directory->children;
    struct FileNode *current = first;
    do
    {
        struct FileNode *nextNode = current->next;
        cleanupDirectory(current);
        free(current->blockPointers);
        free(current);
        current = nextNode;
    } while (current != first);
    directory->children = NULL;
}

void cleanupVFS()
{
    cleanupDirectory(root);
    free(root);
    root = NULL;
    cwd = NULL;
    struct FreeBlock *temp = head;
    while (temp != NULL)
    {
        struct FreeBlock *nextNode = temp->next;
        free(temp);
        temp = nextNode;
    }
    head = NULL;
    lastNode = NULL;
    printf("Memory released successfully!\n");
}

void processCommand(char input[])
{
    char *command = strtok(input, " \t");
    if (command == NULL)
    {
        return;
    }
    if (strcmp(command, "ls") == 0)
    {
        listDirectory();
    }
    else if (strcmp(command, "pwd") == 0)
    {
        struct FileNode *path[MAX_DEPTH];
        int count = 0;

        struct FileNode *temp = cwd;

        while (temp != NULL && temp != root)
        {
            if (count == MAX_DEPTH)
            {
                printf("Maximum directory depth reached!\n");
                return ;
            }

            path[count++] = temp;
            temp = temp->parent;
        }

        printf("/");

        for (int i = count - 1; i >= 0; i--)
        {
            printf("%s", path[i]->name);
            if (i != 0)
            {
                printf("/");
            }
        }
        printf("\n");
    }
    else if (strcmp(command, "df") == 0)
    {
        displayDiskUsage();
    }
    else
    {
        char *name = strtok(NULL, " \t");

        if (name == NULL)
        {
            printf("Please provide a name!\n");
            return;
        }
        if (strcmp(command, "mkdir") == 0)
        {
            createEntry(name, 1);
        }
        else if (strcmp(command, "create") == 0)
        {
            createEntry(name, 0);
        }
        else if (strcmp(command, "cd") == 0)
        {
            changeDirectory(name);
        }
        else if (strcmp(command, "read") == 0)
        {
            readFile(name);
        }
        else if (strcmp(command, "delete") == 0)
        {
            deleteFile(name);
        }
        else if (strcmp(command, "rmdir") == 0)
        {
            removeDirectory(name);
        }
        else if (strcmp(command, "write") == 0)
        {
            char *content = strtok(NULL, "");
            if (content == NULL)
            {
                content = "";
            }
            writeFile(name, content);
        }
        else
        {
            printf("Unknown command!\n");
        }
    }
}

int main()
{
    initializeVFS();

    char input[MAX_INPUT];

    while (1)
    {
        printf("\n");

        struct FileNode *path[MAX_DEPTH];
        int count = 0;

        struct FileNode *temp = cwd;

        while (temp != NULL && temp != root)
        {
            if (count == MAX_DEPTH)
            {
                printf("Maximum directory depth reached!\n");
                return 1;
            }
            
            path[count++] = temp;
            temp = temp->parent;
        }

        printf("/");

        for (int i = count - 1; i >= 0; i--)
        {
            printf("%s", path[i]->name);

            if (i != 0)
            {
                printf("/");
            }
        }

        printf(" > ");

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        processCommand(input);
    }

    cleanupVFS();

    printf("Exiting Virtual File System...\n");

    return 0;
}