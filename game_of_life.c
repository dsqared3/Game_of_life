#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define M 25
#define N 80

void first_text_module(int ***main_field);
int **create_buffer();
void calculate(int **counting_neighbors, int ***main_field);
void equal(int **counting_neighbors, int ***main_field);
void check(int i, int j, int **counting_neighbors, int ***main_field);
int speed_reg(int speed, char input);
int end_game_status(int end_of_game, char input);
void output(int **main_field);
void easter_egg();
void enableNonBlockingMode();
int kbhit();

int main() {
    int speed = 200000;
    int generations = 0, end_of_game = 0;
    int **counting_neighbors = create_buffer();
    int **main_field = create_buffer();
    first_text_module(&counting_neighbors);
    (void)freopen("/dev/tty", "r", stdin);
    equal(counting_neighbors, &main_field);
    enableNonBlockingMode();

    while (generations != 2000 && end_of_game != 1) {
        generations += 1;
        if (kbhit()) {
            char input;
            if ((input = getchar()) != EOF) {
                end_of_game = end_game_status(end_of_game, input);
                speed = speed_reg(speed, input);
            }
        }
        equal(main_field, &counting_neighbors);
        calculate(counting_neighbors, &main_field);
        printf("                               Game of life\n");
        printf("Delay:%3d    Generations:%d    Press 'a' to high, 'z' to low speed, q to exit\n", speed,
               generations);
        output(main_field);
        usleep(speed);
        system("clear");
        int fflush(FILE * ostream);
    }
    printf("Симуляция жизни подошла к концу :)\n");
    easter_egg();
    free(counting_neighbors);
    free(main_field);
    return 0;
}

void enableNonBlockingMode() {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag &= ~(ICANON | ECHO);
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

int kbhit() {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
}

int end_game_status(int end_of_game, char input) {
    if (input == 'q') {
        end_of_game = 1;
    }
    return end_of_game;
}

int speed_reg(int speed, char input) {
    if (input == 'z') {
        if (speed < 600000) {
            speed += 50000;
        }
    }
    if (input == 'a') {
        if (speed > 50000) {
            speed -= 50000;
        }
    }
    return speed;
}

void equal(int **counting_neighbors, int ***main_field) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            (*main_field)[i][j] = counting_neighbors[i][j];
        }
    }
}

void calculate(int **counting_neighbors, int ***main_field) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            check(i, j, counting_neighbors, main_field);
        }
    }
}

void check(int i, int j, int **counting_neighbors, int ***main_field) {
    int life_count = 0;

    for (int k = i - 1; k < i + 2; k++) {
        for (int n = j - 1; n < j + 2; n++) {
            int xk = (k == -1) * (M - 1) + (k >= 0) * (k % M);
            int xn = (n == -1) * (N - 1) + (n >= 0) * (n % N);
            life_count = life_count + counting_neighbors[xk][xn];
        }
    }
    life_count = life_count - counting_neighbors[i][j];

    if (life_count == 3 && counting_neighbors[i][j] == 0) {
        (*main_field)[i][j] = 1;
    }
    if ((life_count < 2 || life_count > 3) && counting_neighbors[i][j] == 1) {
        (*main_field)[i][j] = 0;
    }
}

void output(int **main_field) {
    for (int i = 0; i < M + 2; i++) {
        for (int j = 0; j < N + 2; j++) {
            if (i == 0 || i == M + 1 || j == 0 || j == N + 1)
                printf("$");
            else
                printf("%c", main_field[i - 1][j - 1] == 0 ? ' ' : '@');
        }
        printf("\n");
    }
}

int **create_buffer() {
    int **arr = malloc((M * N * sizeof(int)) + (M * sizeof(int *)));
    if (!arr) {
        arr = NULL;
        free(arr);
    } else {
        int *ptr = (int *)(arr + M);
        for (int i = 0; i < M; i++) {
            arr[i] = ptr + N * i;
        }
    }
    return arr;
}

void first_text_module(int ***main_field) {
    printf("                   GAME OF LIFE\n       Print any key for random generated picture");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            (*main_field)[i][j] = rand() % 2;
        }
        printf("\n");
    }
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &(*main_field)[i][j]);
        }
        printf("\n");
    }
}

void easter_egg() {
    printf("            ******\n");
    printf("          **********\n");
    printf("         ************\n");
    printf("        **************\n");
    printf("        **************\n");
    printf("         ************\n");
    printf("          **********\n");
    printf("            ******\n");
}