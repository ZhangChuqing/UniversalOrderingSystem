#include <stdio.h>
#include <string.h>
#include "../Data/data.h"
#include "user.h"

int findUser(int id)
{
    int i;
    for (i = 0; i < userCount; i++)
        if (users[i].userID == id && users[i].active)
            return i;
    return -1;
}

int findUserByUsername(const char username[])
{
    int i;
    for (i = 0; i < userCount; i++)
        if (users[i].active && strcmp(users[i].username, username) == 0)
            return i;
    return -1;
}

int registerUser(void)
{
    User u;
    int i, maxID = 1000;

    if (userCount >= MAX_USERS)
    {
        puts("User list is full.");
        return -1;
    }

    for (i = 0; i < userCount; i++)
        if (users[i].userID > maxID)
            maxID = users[i].userID;

    printf("Choose username (no spaces): ");
    if (scanf(" %19s", u.username) != 1)
        return -1;

    if (findUserByUsername(u.username) >= 0)
    {
        puts("This username is already taken. Please register with another username.");
        return -1;
    }

    u.userID = maxID + 1;

    printf("Name: ");
    if (scanf(" %49[^\n]", u.name) != 1)
        return -1;
    printf("Telephone: ");
    if (scanf(" %19s", u.telephone) != 1)
        return -1;
    printf("Password: ");
    if (scanf(" %19s", u.password) != 1)
        return -1;

    u.balance = 0;
    u.active = 1;

    users[userCount] = u;
    userCount++;
    saveUsers();
    printf("Registered successfully. Your account number is %d.\n", u.userID);
    printf("You can now log in using username: %s\n", u.username);
    return u.userID;
}

int loginUser(void)
{
    int index;
    char username[20], password[20];

    printf("Username: ");
    if (scanf(" %19s", username) != 1)
        return -1;
    printf("Password: ");
    if (scanf(" %19s", password) != 1)
        return -1;

    index = findUserByUsername(username);
    if (index >= 0 && strcmp(users[index].password, password) == 0)
        return index;

    puts("Invalid username or password.");
    return -1;
}

void displayUser(int n)
{
    if (n >= 0 && n < userCount)
        printf("\nAccount: %d\nUsername: %s\nName: %s\nTelephone: %s\nBalance: $%.2f\n", users[n].userID, users[n].username, users[n].name, users[n].telephone, users[n].balance);
}

void displayUserList(void)
{
    int i;
    puts("\n--- Customers ---");
    for (i = 0; i < userCount; i++)
        if (users[i].active)
            printf("%d | %s | %s | %s | $%.2f\n", users[i].userID, users[i].username, users[i].name, users[i].telephone, users[i].balance);
}

void editUser(int n)
{
    if (n < 0 || n >= userCount || !users[n].active)
        return;
    printf("New name: ");
    scanf(" %49[^\n]", users[n].name);
    printf("New telephone: ");
    scanf(" %19s", users[n].telephone);
    saveUsers();
    puts("Customer updated.");
}

void deleteUser(int n)
{
    if (n < 0 || n >= userCount || !users[n].active)
        return;
    users[n].active = 0;
    saveUsers();
    puts("Customer deactivated.");
}
