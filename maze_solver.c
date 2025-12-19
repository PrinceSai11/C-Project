#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>   // for timing

#define MAX 16   // allow up to 15x15 maze

char maze[MAX][MAX];
int rows, cols;
int found = 0;
int steps = 0;   // step counter

typedef struct { int x, y; } Point;
Point start, end;

int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

// DFS solver
int dfs(int x, int y) {
    if (x == end.x && y == end.y) { 
        found = 1; 
        return 1; 
    }
    if (maze[x][y] != 'S' && maze[x][y] != 'E') {
        maze[x][y] = '*';
        steps++;   // count step when marking path
    }

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i], ny = y + dy[i];
        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols &&
            (maze[nx][ny] == '.' || maze[nx][ny] == 'E')) {
            if (dfs(nx, ny)) return 1;
        }
    }
    // backtrack
    if (maze[x][y] != 'S' && maze[x][y] != 'E') {
        maze[x][y] = '.';
        steps--;   // undo step count when backtracking
    }
    return 0;
}

// Print maze
void printMaze() {
    for (int i = 0; i < rows; i++) {
        printf("%s\n", maze[i]);
    }
}

// Save maze to file
void saveMaze(const char* outFile) {
    FILE *fp = fopen(outFile, "w");
    if (!fp) { 
        printf("Error writing file\n"); 
        return; 
    }
    for (int i = 0; i < rows; i++) {
        fprintf(fp, "%s\n", maze[i]);
    }
    fclose(fp);
    printf("Solution saved to %s\n", outFile);
}

// Load maze
int loadMaze(const char* filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { 
        printf("Error opening file: %s\n", filename); 
        return 0; 
    }

    rows = 0;
    while (rows < MAX && fgets(maze[rows], MAX, fp)) {
        maze[rows][strcspn(maze[rows], "\r\n")] = '\0';
        rows++;
    }
    fclose(fp);

    if (rows == 0) { 
        printf("File empty\n"); 
        return 0; 
    }
    cols = (int)strlen(maze[0]);
    if (rows >= MAX || cols >= MAX) { 
        printf("Maze too large (max 15x15)\n"); 
        return 0; 
    }

    int sCount = 0, eCount = 0;
    for (int i = 0; i < rows; i++) {
        if ((int)strlen(maze[i]) != cols) { 
            printf("Rows have unequal length\n"); 
            return 0; 
        }
        for (int j = 0; j < cols; j++) {
            char c = maze[i][j];
            if (c == 'S') { start.x = i; start.y = j; sCount++; }
            if (c == 'E') { end.x = i; end.y = j; eCount++; }
            if (c != 'X' && c != '.' && c != 'S' && c != 'E') {
                printf("Invalid character '%c' in maze\n", c);
                return 0;
            }
        }
    }
    if (sCount != 1 || eCount != 1) { 
        printf("Need exactly one S and one E\n"); 
        return 0; 
    }
    return 1;
}

int main() {
    char filename[100];
    printf("Enter maze filename: ");
    if (scanf("%99s", filename) != 1) return 0;

    if (!loadMaze(filename)) return 0;

    printf("Original Maze:\n");
    printMaze();

    // start timing
    clock_t startTime = clock();

    if (dfs(start.x, start.y)) {
        clock_t endTime = clock();
        double timeTaken = (double)(endTime - startTime) / CLOCKS_PER_SEC;

        printf("\nSolved Maze (path marked with *):\n");
        printMaze();

        // print statistics
        printf("\n--- Statistics ---\n");
        printf("Maze size: %dx%d\n", rows, cols);
        printf("Path length: %d steps\n", steps);
        printf("Time taken: %.4f seconds\n", timeTaken);

        // ask user if they want to save solution
        printf("\nDo you want to save solution to file? (y/n): ");
        char choice;
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            char outFile[100];
            printf("Enter output filename: ");
            scanf("%99s", outFile);
            saveMaze(outFile);
        }
    } else {
        printf("No path exists\n");
    }
    return 0;
}