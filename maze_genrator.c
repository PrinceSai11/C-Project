#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 13
char maze[MAX][MAX];

int main() {
    int m, n;
    printf("Enter maze size (rows cols, both < 13): ");
    scanf("%d %d", &m, &n);

    if (m >= MAX || n >= MAX || m < 3 || n < 3) {
        printf("Invalid size! Must be between 3 and 12.\n");
        return 0;
    }

    srand(time(NULL));

    // Fill walls + null terminate
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            maze[i][j] = 'X';
        }
        maze[i][n] = '\0';
    }

    // Guaranteed path with random direction preference
    int x = 1, y = 1;
    maze[x][y] = 'S';

    while (x < m-2 || y < n-2) {
        if ((rand() % 2 == 0 && y < n-2) || x == m-2) {
            y++;
        } else {
            x++;
        }
        maze[x][y] = '.';
    }
    maze[x][y] = 'E';

    // Randomly carve other interior paths (ignore boundary & S/E)
    for (int i = 1; i < m-1; i++) {
        for (int j = 1; j < n-1; j++) {
            if (maze[i][j] == 'X') {
                maze[i][j] = (rand() % 4 == 0) ? 'X' : '.';
            }
        }
    }

    // Print maze
    printf("\nGenerated Solvable Maze:\n");
    for (int i = 0; i < m; i++) {
        printf("%s\n", maze[i]);
    }

    // Optionally save
    printf("\nSave maze to file? (y/n): ");
    char choice;
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        char filename[100];
        printf("Enter filename: ");
        scanf("%99s", filename);
        FILE *fp = fopen(filename, "w");
        if (!fp) { printf("Error writing file\n"); return 0; }
        for (int i = 0; i < m; i++)
            fprintf(fp, "%s\n", maze[i]);
        fclose(fp);
        printf("Maze saved to %s\n", filename);
    }

    return 0;
}
