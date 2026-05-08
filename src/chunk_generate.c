#include "world/chunk.h"
#include "world/map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef char visited_t[MAP_SIZE][MAP_SIZE];
typedef local_pos_t queue_t[MAP_SIZE * MAP_SIZE];

static int in_map_bounds(int x, int y) {
    return x >= 0 && x < MAP_SIZE && y >= 0 && y < MAP_SIZE;
}

static int count_wall_neighbors(map_t m, int x, int y) {
    int count = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            if (!in_map_bounds(nx, ny)) {
                count++;  // Treat out-of-bounds as wall
            } else if (m[ny][nx] == TILE_WALL) {
                count++;
            }
        }
    }

    return count;
}

static void random_fill(map_t m, double wall_chance) {
    if (!m) return;

    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            if (x == 0 || y == 0 || x == MAX_LOC || y == MAX_LOC) {
                m[y][x] = TILE_WALL;
            } else {
                double r = (double)rand() / (double)RAND_MAX;
                m[y][x] = (r < wall_chance) ? TILE_WALL : TILE_FLOOR;
            }
        }
    }
}

static void smooth_once(map_t m) {
    if (!m) return;

    map_t next;

    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            int neighbors = count_wall_neighbors(m, x, y);

            if (neighbors > 4) {
                next[y][x] = TILE_WALL;
            } else if (neighbors < 4) {
                next[y][x] = TILE_FLOOR;
            } else {
                next[y][x] = m[y][x];
            }
        }
    }

    memcpy(m, next, sizeof(map_t));
}

static int flood_fill(map_t m, visited_t visited, queue_t queue, local_pos_t *cells, int sx, int sy) {    
    int front = 0, back = 0;
    int count = 0;

    queue[back++] = (local_pos_t){ sx, sy };
    visited[sy][sx] = 1;

    while (front < back) {
        local_pos_t p = queue[front++];
        cells[count++] = p;

        const int dirs[4][2] = {
            { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }
        };

        for (int i = 0; i < 4; i++) {
            int nx = p.x + dirs[i][0];
            int ny = p.y + dirs[i][1];

            if (in_map_bounds(nx, ny) && !visited[ny][nx] && m[ny][nx] == TILE_FLOOR) {
                visited[ny][nx] = 1;
                queue[back++] = (local_pos_t){ nx, ny };
            }
        }
    }

    return count;
}

/// @return SUCCESS: 0; FAIL: 1
static int keep_largest_cave(map_t m, visited_t visited, queue_t queue) {
    memset(visited, 0, sizeof(visited_t));

    local_pos_t *best_cells = malloc(sizeof(local_pos_t) * MAP_SIZE * MAP_SIZE);
    local_pos_t *tmp_cells  = malloc(sizeof(local_pos_t) * MAP_SIZE * MAP_SIZE);

    if (!best_cells || !tmp_cells) {
        return 1;
    }

    int best_count = 0;

    for (int y = 1; y < MAP_SIZE - 1; y++) {
        for (int x = 1; x < MAP_SIZE - 1; x++) {
            if (m[y][x] == TILE_FLOOR && !visited[y][x]) {
                int count = flood_fill(m, visited, queue, tmp_cells, x, y);

                if (count > best_count) {
                    best_count = count;
                    memcpy(best_cells, tmp_cells, sizeof(local_pos_t) * count);
                }
            }
        }
    }

    // Turn everything into walls first
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            m[y][x] = TILE_WALL;
        }
    }

    // Restore only the largest connected cave
    for (int i = 0; i < best_count; i++) {
        m[best_cells[i].y][best_cells[i].x] = TILE_FLOOR;
    }

    free(best_cells);
    free(tmp_cells);

    return 0;
}

/// @return SUCCESS: 0; FAIL: 1
static int generate_cave(map_t m) {
    random_fill(m, 0.45);

    visited_t visited;
    queue_t queue;

    for (int i = 0; i < 5; i++) {
        smooth_once(m);
    }

    return keep_largest_cave(m, visited, queue);
}

/**
 * @brief Bot's version of smooth-cave
 * @param dest Chunk to generate to
 * @param pos Position in chunk-units
 */
chunk_t* chunk_generate(chunk_t* dest, chunk_pos_t pos) {
    if (!dest) return NULL;

    dest->pos = pos;
    memset(dest->map, 0, sizeof(map_t));

    int res = generate_cave(dest->map);

    //* line through all chunks to see it if transitions work
    const int line_y = 20;
    for (int i = 0; i < MAP_SIZE; i++) {
        dest->map[line_y - 1][i] = TILE_FLOOR;
        dest->map[i][line_y - 1] = TILE_FLOOR;
    }

    return res == 0 ? dest : NULL;
}

/* //* rect rooms generation
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_SIZE 80
#define MAP_SIZE 25

#define TILE_WALL 1
#define TILE_FLOOR 0

#define MAX_ROOMS 32

typedef struct {
    int x, y, w, h;
} Room;

static int grid[MAP_SIZE][MAP_SIZE];
static Room rooms[MAX_ROOMS];
static int room_count = 0;

static int in_map_bounds(int x, int y) {
    return x > 0 && x < MAP_SIZE-1 && y > 0 && y < MAP_SIZE-1;
}

static void fill_walls(void) {
    for (int y = 0; y < MAP_SIZE; y++)
        for (int x = 0; x < MAP_SIZE; x++)
            grid[y][x] = TILE_WALL;
}

static int rooms_overlap(Room a, Room b) {
    return (a.x <= b.x + b.w &&
            a.x + a.w >= b.x &&
            a.y <= b.y + b.h &&
            a.y + a.h >= b.y);
}

static void carve_room(Room r) {
    for (int y = r.y; y < r.y + r.h; y++) {
        for (int x = r.x; x < r.x + r.w; x++) {
            if (in_map_bounds(x, y)) {
                grid[y][x] = TILE_FLOOR;
            }
        }
    }
}

static void add_rooms(void) {
    room_count = 0;

    for (int i = 0; i < MAX_ROOMS; i++) {
        Room r;

        r.w = 4 + rand() % 8;
        r.h = 3 + rand() % 6;
        r.x = rand() % (MAP_SIZE - r.w - 2) + 1;
        r.y = rand() % (MAP_SIZE - r.h - 2) + 1;

        int failed = 0;

        for (int j = 0; j < room_count; j++) {
            if (rooms_overlap(r, rooms[j])) {
                failed = 1;
                break;
            }
        }

        if (!failed) {
            rooms[room_count++] = r;
            carve_room(r);
        }
    }
}

static void carve_h_tunnel(int x1, int x2, int y) {
    int min = x1 < x2 ? x1 : x2;
    int max = x1 > x2 ? x1 : x2;

    for (int x = min; x <= max; x++) {
        if (in_map_bounds(x, y)) grid[y][x] = TILE_FLOOR;
    }
}

static void carve_v_tunnel(int y1, int y2, int x) {
    int min = y1 < y2 ? y1 : y2;
    int max = y1 > y2 ? y1 : y2;

    for (int y = min; y <= max; y++) {
        if (in_map_bounds(x, y)) grid[y][x] = TILE_FLOOR;
    }
}

static void connect_rooms(void) {
    for (int i = 1; i < room_count; i++) {
        int prev_cx = rooms[i-1].x + rooms[i-1].w / 2;
        int prev_cy = rooms[i-1].y + rooms[i-1].h / 2;

        int cx = rooms[i].x + rooms[i].w / 2;
        int cy = rooms[i].y + rooms[i].h / 2;

        if (rand() % 2) {
            carve_h_tunnel(prev_cx, cx, prev_cy);
            carve_v_tunnel(prev_cy, cy, cx);
        } else {
            carve_v_tunnel(prev_cy, cy, prev_cx);
            carve_h_tunnel(prev_cx, cx, cy);
        }
    }
}

static void print_map(void) {
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            putchar(grid[y][x] ? '#' : '.');
        }
        putchar('\n');
    }
}

int main(void) {
    srand((unsigned int)time(NULL));

    fill_walls();
    add_rooms();
    connect_rooms();

    print_map();
    return 0;
}
    */