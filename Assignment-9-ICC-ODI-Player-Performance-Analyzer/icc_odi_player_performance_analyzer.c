#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h"

struct Player
{
    int playerId;
    char name[51];
    char teamName[51];
    int role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct Player *next;
};

struct Team
{
    int teamId;
    char name[51];
    int totalPlayers;
    int battingPlayers;
    float totalBattingStrikeRate;
    float averageBattingStrikeRate;
    struct Player *rolePlayers[3];
};

struct ResultNode
{
    struct Player *player;
    struct ResultNode *next;
};

struct Team teams[TOTAL_TEAMS];

float calculatePerformanceIndex(struct Player *player)
{
    float performanceIndex = 0;

    switch (player->role)
    {
        case 1:
            performanceIndex = (player->battingAverage * player->strikeRate) / 100;
            break;
        case 2:
            performanceIndex = (player->wickets * 2) + (100 - player->economyRate);
            break;
        case 3:
            performanceIndex = ((player->battingAverage * player->strikeRate) / 100) + (player->wickets * 2);
            break;
    }
    return performanceIndex;
}

void initializePlayers()
{
    for (int i = 0; i < TOTAL_PLAYERS; i++)
    {
        struct Player *newPlayer = malloc(sizeof(struct Player));
        if (newPlayer == NULL)
        {
            printf("Memory allocation failed!");
            return;
        }

        newPlayer->playerId = playersData[i].playerId;
        strcpy(newPlayer->name, playersData[i].name);
        newPlayer->role = playersData[i].role;

        int roleIndex = newPlayer->role - 1;

        if (roleIndex < 0 || roleIndex >= 3)
        {
            free(newPlayer);
            continue;
        }

        newPlayer->totalRuns = playersData[i].totalRuns;
        newPlayer->battingAverage = playersData[i].battingAverage;
        newPlayer->strikeRate = playersData[i].strikeRate;
        newPlayer->wickets = playersData[i].wickets;
        newPlayer->economyRate = playersData[i].economyRate;

        int teamIndex = playersData[i].teamId - 1;

        if (teamIndex < 0 || teamIndex >= TOTAL_TEAMS)
        {
            free(newPlayer);
            continue;
        }

        strcpy(newPlayer->teamName, teams[teamIndex].name);
        newPlayer->performanceIndex = calculatePerformanceIndex(newPlayer);
        newPlayer->next = NULL;

        struct Player **head = &teams[teamIndex].rolePlayers[roleIndex];

        if (*head == NULL || (*head)->performanceIndex < newPlayer->performanceIndex)
        {
            newPlayer->next = *head;
            *head = newPlayer;
        }
        else
        {
            struct Player *temp = *head;

            while (temp->next != NULL && temp->next->performanceIndex >= newPlayer->performanceIndex)
            {
                temp = temp->next;
            }

            newPlayer->next = temp->next;
            temp->next = newPlayer;
        }

        teams[teamIndex].totalPlayers++;

        if (newPlayer->role == 1 || newPlayer->role == 3)
        {
            teams[teamIndex].totalBattingStrikeRate += newPlayer->strikeRate;
            teams[teamIndex].battingPlayers++;
            teams[teamIndex].averageBattingStrikeRate = teams[teamIndex].totalBattingStrikeRate / teams[teamIndex].battingPlayers;
        }
    }
}

int searchTeam(int teamId)
{
    int low = 0;
    int high = TOTAL_TEAMS - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (teams[mid].teamId == teamId)
        {
            return mid;
        }
        else if (teams[mid].teamId < teamId)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    return -1;
}

void addPlayer()
{
    int teamId, teamIndex;
    int roleIndex;

    printf("\nEnter Team ID: ");
    scanf("%d", &teamId);

    teamIndex = searchTeam(teamId);

    if (teamIndex == -1)
    {
        printf("\nInvalid Team ID!\n");
        return;
    }

    if (teams[teamIndex].totalPlayers >= 50)
    {
        printf("\nTeam already has the maximum 50 players!\n");
        return;
    }

    struct Player *newPlayer = malloc(sizeof(struct Player));

    if (newPlayer == NULL)
    {
        printf("\nMemory allocation failed!\n");
        return;
    }

    printf("Enter Player ID: ");
    scanf("%d", &newPlayer->playerId);

    if (newPlayer->playerId < 1 || newPlayer->playerId > 1000)
    {
        printf("\nInvalid Player ID! ID must be between 1 and 1000.\n");
        free(newPlayer);
        return;
    }

    for (int i = 0; i < TOTAL_TEAMS; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            struct Player *temp = teams[i].rolePlayers[j];

            while (temp != NULL)
            {
                if (temp->playerId == newPlayer->playerId)
                {
                    printf("\nPlayer ID already exists!\n");
                    free(newPlayer);
                    return;
                }
                temp = temp->next;
            }
        }
    }

    getchar();

    printf("Enter Player Name: ");
    fgets(newPlayer->name, sizeof(newPlayer->name), stdin);
    newPlayer->name[strcspn(newPlayer->name, "\n")] = '\0';

    printf("Enter Player Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &newPlayer->role);

    if (newPlayer->role < 1 || newPlayer->role > 3)
    {
        printf("\nInvalid Role!\n");
        free(newPlayer);
        return;
    }

    printf("Enter Total Runs: ");
    scanf("%d", &newPlayer->totalRuns);

    printf("Enter Batting Average: ");
    scanf("%f", &newPlayer->battingAverage);

    printf("Enter Strike Rate: ");
    scanf("%f", &newPlayer->strikeRate);

    printf("Enter Wickets: ");
    scanf("%d", &newPlayer->wickets);

    printf("Enter Economy Rate: ");
    scanf("%f", &newPlayer->economyRate);

    strcpy(newPlayer->teamName, teams[teamIndex].name);
    newPlayer->performanceIndex = calculatePerformanceIndex(newPlayer);
    newPlayer->next = NULL;

    roleIndex = newPlayer->role - 1;

    struct Player **head = &teams[teamIndex].rolePlayers[roleIndex];

    if (*head == NULL || (*head)->performanceIndex < newPlayer->performanceIndex)
    {
        newPlayer->next = *head;
        *head = newPlayer;
    }
    else
    {
        struct Player *temp = *head;

        while (temp->next != NULL && temp->next->performanceIndex >= newPlayer->performanceIndex)
        {
            temp = temp->next;
        }
        newPlayer->next = temp->next;
        temp->next = newPlayer;
    }

    teams[teamIndex].totalPlayers++;

    if (newPlayer->role == 1 || newPlayer->role == 3)
    {
        teams[teamIndex].totalBattingStrikeRate += newPlayer->strikeRate;
        teams[teamIndex].battingPlayers++;
        teams[teamIndex].averageBattingStrikeRate = teams[teamIndex].totalBattingStrikeRate / teams[teamIndex].battingPlayers;
    }
    printf("\nPlayer added successfully to %s!\n", teams[teamIndex].name);
}

void displayTeamPlayers()
{
    int teamId, teamIndex;

    printf("\nEnter Team ID: ");
    scanf("%d", &teamId);

    teamIndex = searchTeam(teamId);

    if (teamIndex == -1)
    {
        printf("\nInvalid Team ID!\n");
        return;
    }

    printf("\nPlayers of Team %s:\n", teams[teamIndex].name);
    printf("\n==========================================================================\n");
    printf("%-8s %-22s %-15s %-10s %-10s %-10s %-10s %-10s %-15s\n", "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("==========================================================================\n");

    int totalDisplayed = 0;

    for (int i = 0; i < 3; i++)
    {
        struct Player *temp = teams[teamIndex].rolePlayers[i];

        while (temp != NULL)
        {
            char roleName[15];
            switch (temp->role)
            {
                case 1:
                    strcpy(roleName, "Batsman");
                    break;
                case 2:
                    strcpy(roleName, "Bowler");
                    break;
                case 3:
                    strcpy(roleName, "All-Rounder");
                    break;
            }
            printf("%-8d %-22s %-15s %-10d %-10.1f %-10.1f %-10d %-10.1f %-15.2f\n", temp->playerId, temp->name, roleName, temp->totalRuns, temp->battingAverage, temp->strikeRate, temp->wickets, temp->economyRate, temp->performanceIndex);
            temp = temp->next;
            totalDisplayed++;
        }
    }

    if (totalDisplayed == 0)
    {
        printf("\nNo players found in this team!\n");
    }
    printf("\n==========================================================================\n");
    printf("\nTotal Players: %d\n", teams[teamIndex].totalPlayers);
    printf("\nAverage Batting Strike Rate: %.2f\n", teams[teamIndex].averageBattingStrikeRate);
}

void displayTeamsByStrikeRate()
{
    struct Team temp;
    struct Team sortedTeams[TOTAL_TEAMS];

    for (int i = 0; i < TOTAL_TEAMS; i++)
    {
        sortedTeams[i] = teams[i];
    }

    for (int i = 0; i < TOTAL_TEAMS - 1; i++)
    {
        for (int j = 0; j < TOTAL_TEAMS - i - 1; j++)
        {
            if (sortedTeams[j].averageBattingStrikeRate <
                sortedTeams[j + 1].averageBattingStrikeRate)
            {
                temp = sortedTeams[j];
                sortedTeams[j] = sortedTeams[j + 1];
                sortedTeams[j + 1] = temp;
            }
        }
    }

    printf("\n========== TEAMS BY AVERAGE BATTING STRIKE RATE ==========\n");
    printf("\n%-8s %-18s %-15s %-15s\n", "ID", "Team Name", "Avg Bat SR", "Total Players");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < TOTAL_TEAMS; i++)
    {
        printf("%-8d %-18s %-15.1f %-15d\n", sortedTeams[i].teamId, sortedTeams[i].name, sortedTeams[i].averageBattingStrikeRate, sortedTeams[i].totalPlayers);
    }
}

void displayTopKPlayers()
{
    int teamId, teamIndex;
    int role, roleIndex;
    int k, count = 0;

    struct Player *temp;

    printf("\nEnter Team ID: ");
    scanf("%d", &teamId);

    teamIndex = searchTeam(teamId);

    if (teamIndex == -1)
    {
        printf("\nInvalid Team ID!\n");
        return;
    }

    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &role);

    if (role < 1 || role > 3)
    {
        printf("\nInvalid Role!\n");
        return;
    }

    printf("Enter number of players: ");
    scanf("%d", &k);

    if (k <= 0)
    {
        printf("\nInvalid number of players!\n");
        return;
    }

    roleIndex = role - 1;
    temp = teams[teamIndex].rolePlayers[roleIndex];

    if (temp == NULL)
    {
        printf("\nNo players found for this role!\n");
        return;
    }

    printf("\nTop %d ", k);

    switch (role)
    {
        case 1:
            printf("Batsmen");
            break;
        case 2:
            printf("Bowlers");
            break;
        case 3:
            printf("All-rounders");
            break;
    }

    printf(" of Team %s:\n", teams[teamIndex].name);
    printf("\n==========================================================================\n");
    printf("%-8s %-22s %-15s %-10s %-10s %-10s %-10s %-10s %-15s\n", "ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("==========================================================================\n");

    while (temp != NULL && count < k)
    {
        char roleName[15];

        switch (temp->role)
        {
            case 1:
                strcpy(roleName, "Batsman");
                break;
            case 2:
                strcpy(roleName, "Bowler");
                break;
            case 3:
                strcpy(roleName, "All-Rounder");
                break;
        }

        printf("%-8d %-22s %-15s %-10d %-10.1f %-10.1f %-10d %-10.1f %-15.2f\n", temp->playerId, temp->name, roleName, temp->totalRuns, temp->battingAverage, temp->strikeRate, temp->wickets, temp->economyRate, temp->performanceIndex);
        temp = temp->next;
        count++;
    }

    printf("\n==========================================================================\n");

    if (count < k)
    {
        printf("\nOnly %d players are available for this role.\n", count);
    }
}

struct ResultNode *mergeResultLists(struct ResultNode *first, struct ResultNode *second)
{
    struct ResultNode dummy;
    struct ResultNode *tail = &dummy;

    dummy.next = NULL;

    while (first != NULL && second != NULL)
    {
        if (first->player->performanceIndex >= second->player->performanceIndex)
        {
            tail->next = first;
            first = first->next;
        }
        else
        {
            tail->next = second;
            second = second->next;
        }

        tail = tail->next;
    }

    if (first != NULL)
    {
        tail->next = first;
    }
    else
    {
        tail->next = second;
    }

    return dummy.next;
}

void freeResultList(struct ResultNode *head)
{
    while (head != NULL)
    {
        struct ResultNode *temp = head;
        head = head->next;
        free(temp);
    }
}

void displayPlayersByRole()
{
    int role;

    struct ResultNode *listHeads[TOTAL_TEAMS];

    for (int i = 0; i < TOTAL_TEAMS; i++)
    {
        listHeads[i] = NULL;
    }

    printf("\nEnter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &role);

    if (role < 1 || role > 3)
    {
        printf("\nInvalid Role!\n");
        return;
    }

    int roleIndex = role - 1;

    for (int i = 0; i < TOTAL_TEAMS; i++)
    {
        struct Player *temp = teams[i].rolePlayers[roleIndex];
        struct ResultNode *lastNode = NULL;

        while (temp != NULL)
        {
            struct ResultNode *newNode = malloc(sizeof(struct ResultNode));

            if (newNode == NULL)
            {
                printf("\nMemory allocation failed!\n");

                for (int j = 0; j < TOTAL_TEAMS; j++)
                {
                    freeResultList(listHeads[j]);
                }

                return;
            }

            newNode->player = temp;
            newNode->next = NULL;

            if (listHeads[i] == NULL)
            {
                listHeads[i] = newNode;
            }
            else
            {
                lastNode->next = newNode;
            }

            lastNode = newNode;
            temp = temp->next;
        }
    }

    int listCount = TOTAL_TEAMS;

    while (listCount > 1)
    {
        int newCount = 0;
        for (int i = 0; i < listCount; i += 2)
        {
            if (i + 1 < listCount)
            {
                listHeads[newCount] = mergeResultLists(listHeads[i], listHeads[i + 1]);
            }
            else
            {
                listHeads[newCount] = listHeads[i];
            }

            newCount++;
        }
        listCount = newCount;
    }

    struct ResultNode *temp = listHeads[0];
    if (temp == NULL)
    {
        printf("\nNo players found for this role!\n");
        return;
    }
    printf("\nAll ");
    switch (role)
    {
        case 1:
            printf("Batsmen");
            break;
        case 2:
            printf("Bowlers");
            break;
        case 3:
            printf("All-rounders");
            break;
    }

    printf(" of all teams in descending order of Performance Index:\n");
    printf("%-8s %-22s %-18s %-15s %-10s %-10s %-10s %-10s %-10s %-15s\n", "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf. Index");
    printf("====================================================================\n");
    while (temp != NULL)
    {
        struct Player *player = temp->player;
        char roleName[15];
        switch (player->role)
        {
            case 1:
                strcpy(roleName, "Batsman");
                break;
            case 2:
                strcpy(roleName, "Bowler");
                break;
            case 3:
                strcpy(roleName, "All-Rounder");
                break;
        }

        printf("%-8d %-22s %-18s %-15s %-10d %-10.1f %-10.1f %-10d %-10.1f %-15.2f\n", player->playerId, player->name, player->teamName, roleName, player->totalRuns, player->battingAverage, player->strikeRate, player->wickets, player->economyRate, player->performanceIndex);
        temp = temp->next;
    }
    freeResultList(listHeads[0]);
}

void freePlayers()
{
    for (int i = 0; i < TOTAL_TEAMS; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            struct Player *temp = teams[i].rolePlayers[j];

            while (temp != NULL)
            {
                struct Player *next = temp->next;
                free(temp);
                temp = next;
            }
            teams[i].rolePlayers[j] = NULL;
        }
    }
}

int main()
{
    int choice;

    for (int i = 0; i < TOTAL_TEAMS; i++)
    {
        teams[i].teamId = i + 1;
        strcpy(teams[i].name, teamNames[i]);
        teams[i].totalPlayers = 0;
        teams[i].battingPlayers = 0;
        teams[i].averageBattingStrikeRate = 0.0;
        teams[i].totalBattingStrikeRate = 0.0;

        for (int j = 0; j < 3; j++)
        {
            teams[i].rolePlayers[j] = NULL;
        }
    }

    initializePlayers();

    do
    {
        printf("\n========== ICC ODI PLAYER PERFORMANCE ANALYZER ==========\n");
        printf("1. Add Player to Team\n");
        printf("2. Display All Players of a Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Team by Role\n");
        printf("5. Display All Players of a Specific Role Across Teams\n");
        printf("6. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addPlayer();
                break;

            case 2:
                displayTeamPlayers();
                break;

            case 3:
                displayTeamsByStrikeRate();
                break;

            case 4:
                displayTopKPlayers();
                break;

            case 5:
                displayPlayersByRole();
                break;

            case 6:
                printf("\nExiting the program!\n");
                freePlayers();
                break;

            default:
                printf("\nInvalid choice! Please enter 1-6.\n");
        }

    } while (choice != 6);
    return 0;
}