#include <stdio.h>

struct query {
  int collectable_count;
  int exit_count;
};

char *map[32] = {
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "00000000000000000000001000000000",
    "0000000000000000000001c000000000",
    "11111111111111111111111000000000",
    "000000000000000000000c10000001e1",
    "00000000000000000000001000000100",
};

char visited[32][32];

// clang-format off

enum direction {
    LEFT  = 0,
    RIGHT = 1,
    UP    = 2,
    DOWN  = 3
};

char direction_offset[4][2] = {
    [LEFT]  = { 0, -1},
    [RIGHT] = { 0,  1},
    [UP]    = {-1,  0},
    [DOWN]  = { 1,  0}
};

// clang-format on

int is_in_bound(int y_max, int x_max, int y, int x) {
    return (
           0 <= y && y < y_max
        && 0 <= x && x < x_max
    );
}

struct query query_map(char (*visited)[32], char **map, int y, int x) {
    struct query q = {0, 0};

    if (!is_in_bound(32, 32, y, x) || visited[y][x] || map[y][x] == '1') {
        return (struct query){0, 0};
    }

    q.collectable_count += map[y][x] == 'c';
    q.exit_count += map[y][x] == 'e';

    visited[y][x] = 1;

    for (int i = 0; i < 4; i++) {
        char *offset = direction_offset[i];
        struct query temp = query_map(visited, map, y + offset[0], x + offset[1]);
        q.collectable_count += temp.collectable_count;
        q.exit_count += temp.exit_count;
    }

    return q;
}

int main(void) {
    struct query q;

    q = query_map(visited, map, 0, 0);

    printf("visible exit        count: %d\n", q.exit_count);
    printf("visible collectable count: %d\n", q.collectable_count);
}