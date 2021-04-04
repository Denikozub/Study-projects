#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void
init(Graph *g)
{
    g->mat = NULL;
    g->n = 0;
}

int
is_empty_graph(const Graph *g)
{
    if ((g->n <= 0) || (g->mat == NULL)) {
        return ERR_EMPTY_GRAPH;
    }
    for (int i = 0; i < g->n; i++) {
        if ((g->mat)[i] == NULL) {
            return ERR_EMPTY_GRAPH;
        }
    }
    return 0;
}

static int
check_symbols(int m, int c)
{
    if ((m % 2 == 1) && (c != '1') && (c != '0')) {
        return ERR_CSV_WRONG_VALUES;
    }
    if ((m % 2 == 0) && (c != ' ') && (c != ',') && (c != ';')) {
        return ERR_CSV_WRONG_SEP;
    }
    return 0;
}

static int
matrix_size(FILE *f)
{
    fseek(f, 0L, SEEK_SET);
    int num_str = 0, temp = 0, num_stb = 0;
    int c, result;
    while ((c = fgetc(f)) != EOF) {
        if (c != '\n') {
            ++temp;
            if ((result = check_symbols(temp, c)) < 0) {
                return result;
            }
        }
        if ((c == '\n') && (temp != 0)) {
            if (temp % 2 == 0) {
                return ERR_CSV_EVEN_COL;
            }
            if (!num_str) {
                num_stb = temp;
            }
            if (num_stb != temp) {
                return ERR_CSV_DIFF_ROWS;
            }
            temp = 0;
            ++num_str;
        }
    }
    if ((num_stb + 1) / 2 != num_str) {
        return ERR_CSV_WRONG_SHAPE;
    }
    return (num_stb + 1) / 2;
}

static int
read_next_digit(FILE *f)
{
    int c;
    while ((c = fgetc(f)) != EOF) {
        if (c == '1') {
            return 1;
        }
        if (c == '0') {
            return 0;
        }
    }
    return ERR_CSV_READ_DIGIT;
}

void
free_graph(Graph *g)
{
    if (is_empty_graph(g)) {
        return;
    }
    for (int i = 0; i < g->n; i++) {
        free((g->mat)[i]);
        (g->mat)[i] = NULL;
    }
    free(g->mat);
    g->mat = NULL;
    g->n = 0;
}

static int
allocate_n(Graph *g)
{
    if (g->n == 0) {
        free_graph(g);
        return 0;
    }
    g->mat = (int**)malloc(g->n * sizeof(int*));
    if (!(g->mat)) {
        return ERR_ALLOC;
    }
    for (int i = 0; i < g->n; i++) {
        (g->mat)[i] = (int*)malloc(g->n * sizeof(int));
        if (!((g->mat)[i])) {
            return ERR_ALLOC;
        }
    }
    return 0;
}

int
init_file(Graph *g, const char *init_filename)
{
    free_graph(g);
    FILE *f;
    if ((f = fopen(init_filename, "r")) == NULL) {
        return ERR_OPEN_FILE;
    }
    int result;
    if ((result = matrix_size(f)) < 0) {
        fclose(f);
        return result;
    }
    g->n = result;
    if ((result = allocate_n(g)) < 0) {
        fclose(f);
        return result;
    }
    int c;
    fseek(f, 0L, SEEK_SET);
    for (int i = 0; i < g->n; ++i) {
        for (int j = 0; j < g->n; ++j) {
            if ((c = read_next_digit(f)) >= 0) {
                (g->mat)[i][j] = c;
            }
            else {
                fclose(f);
                return ERR_CSV_READ_DIGIT;
            }
        }
    }
    fclose(f);
    return 0;
}

void
print_graph(const Graph *g)
{
    if (is_empty_graph(g)) {
        return;
    }
    for (int i = 0; i < g->n; ++i) {
        for (int j = 0; j < g->n; ++j) {
            printf("%d", (g->mat)[i][j]);
        }
        printf("\n");
    }
    printf("----------\n");
}

static int
reallocate_n(Graph *g)
{
    --(g->n);
    free((g->mat)[g->n]);
    (g->mat)[g->n] = NULL;
    for (int i = 0; i < g->n; i++) {
        (g->mat)[i] = realloc((g->mat)[i], g->n * sizeof(int));
        if (!((g->mat)[i])) {
            return ERR_ALLOC;
        }
    }
    return 0;
}

int
remove_node(Graph *g, int d)
{
    if (is_empty_graph(g)) {
        return ERR_EMPTY_GRAPH;
    }
    if ((d > g->n) || (d < 1)) {
        return ERR_WRONG_PARAM;
    }
    if (g->n == 1) {
        free_graph(g);
        return 0;
    }
    --d;
    for (int i = 0; i < (g->n - 1); i++) {
        for (int j = 0; j < (g->n - 1); j++) {
            if ((j >= d) && (i < d)) {
                (g->mat)[i][j] = (g->mat)[i][j + 1];
            }
            if ((j < d) && (i >= d)) {
                (g->mat)[i][j] = (g->mat)[i + 1][j];
            }
            if ((j >= d) && (i >= d)) {
                (g->mat)[i][j] = (g->mat)[i + 1][j + 1];
            }
        }
    }
    int result;
    if ((result = reallocate_n(g)) < 0) {
        return result;
    }
    return 0;
}

int
remove_edge(Graph *g, int from, int to)
{
    if (is_empty_graph(g)) {
        return ERR_EMPTY_GRAPH;
    }
    if ((from > g->n) || (to > g->n) || (from < 1) || (to < 1)) {
        return ERR_WRONG_PARAM;
    }
    --from; --to;
    (g->mat)[from][to] = 0;
    return 0;
}

int
add_edge(Graph *g, int from, int to)
{
    if (is_empty_graph(g)) {
        return ERR_EMPTY_GRAPH;
    }
    if ((from > g->n) || (to > g->n) || (from < 1) || (to < 1)) {
        return ERR_WRONG_PARAM;
    }
    --from; --to;
    (g->mat)[from][to] = 1;
    return 0;
}

int
add_node(Graph *g)
{
    ++(g->n);
    g->mat = realloc(g->mat, g->n * sizeof(int*));
    if (!(g->mat)) {
        return ERR_ALLOC;
    }
    (g->mat)[g->n - 1] = malloc(g->n * sizeof(int));
    for (int i = 0; i < (g->n - 1); i++) {
        (g->mat)[i] = realloc((g->mat)[i], g->n * sizeof(int));
        if (!((g->mat)[i])) {
            return ERR_ALLOC;
        }
    }
    for (int i = 0; i < g->n; i++) {
        (g->mat)[i][g->n - 1] = 0;
        (g->mat)[g->n - 1][i] = 0;
    }
    return 0;
}

int
print_csv(const Graph *g, const char *output_filename)
{
    if(is_empty_graph(g)) {
        return ERR_EMPTY_GRAPH;
    }
    FILE *f;
    if ((f = fopen(output_filename, "w")) == NULL) {
        return ERR_OPEN_FILE;
    }
    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->n; j++) {
            if (g->mat[i][j] == 0) {
                fputc('0', f);
            }
            else {
                fputc('1', f);
            }
            if (j != (g->n - 1)) {
                fputc(',', f);
            }
            else {
                fputc('\n', f);
            }
        }
    }
    fclose(f);
    return 0;
}

int
print_dot(const Graph *g, const char *output_filename)
{
    if(is_empty_graph(g)) {
        return ERR_EMPTY_GRAPH;
    }
    FILE *f;
    if ((f = fopen(output_filename, "w")) == NULL) {
        return ERR_OPEN_FILE;
    }
    fprintf(f, "digraph G {\n");
    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->n; j++) {
            if (g->mat[i][j] == 1) {
                fprintf(f, "    %d -> %d\n", i + 1, j + 1);
            }
        }
    }
    fprintf(f, "}\n");
    fclose(f);
    return 0;
}

static void
fill_nodes(const Graph *g, int **nodes, int start)
{
    (*nodes)[start] = 1;
    for (int i = 0; i < g->n; i++) {
        if (((g->mat)[start][i] == 1) && ((*nodes)[i] == 0)) {
            fill_nodes(g, nodes, i);
        }
    }
    return;
}

int
check_connected(const Graph *g)
{
    if (is_empty_graph(g)) {
        return ERR_EMPTY_GRAPH;
    }
    int *nodes;
    nodes = malloc(g->n * sizeof(int));
    if (!nodes) {
        return ERR_ALLOC;
    }
    for (int i = 0; i < g->n; i++) {
        nodes[i] = 0;
    }

    fill_nodes(g, &nodes, 0);
    for (int i = 0; i < g->n; i++) {
        if (nodes[i] == 0) {
            free(nodes);
            return 0;
        }
    }
    free(nodes);
    return 1;
}

static void
set_path(const Graph *g, int **nodes, int from, int to, 
        int **path, int *path_len)
{
    if (from == to) {
        *path = malloc(++(*path_len) * sizeof(int));
        (*path)[0] = from + 1;
        return;
    }
    (*nodes)[from] = 1;
    for (int i = 0; i < g->n; i++) {
        if (((g->mat)[i][from] == 1) && ((*nodes)[i] == 0) && (!(*path))){
            set_path(g, nodes, i, to, path, path_len);
        }
    }
    if (*path) {
        *path = realloc((*path), ++(*path_len) * sizeof(int));
        (*path)[*path_len - 1] = from + 1;
        return;
    }
    return;
}

int
find_path(
        const Graph *g,
        int from,
        int to,
        int **path,
        int *path_length)
{
    if (is_empty_graph(g)) {
        return ERR_EMPTY_GRAPH;
    }
    if ((from > g->n) || (to > g->n) || (from < 1) || (to < 1)) {
        return ERR_WRONG_PARAM;
    }
    --from; --to;
    if (*path != NULL) {
        *path = NULL;
    }
    if (*path_length != 0) {
        *path_length = 0;
    }
    int *nodes;
    nodes = malloc(g->n * sizeof(int));
    if (!nodes) {
        return ERR_ALLOC;
    }
    for (int i = 0; i < g->n; i++) {
        nodes[i] = 0;
    }
    set_path(g, &nodes, to, from, path, path_length);
    free(nodes);
    return 0;
}
