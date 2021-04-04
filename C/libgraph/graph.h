//by Denis Kozub
//readme for full info

enum {
    ERR_OPEN_FILE = -1,
    ERR_CSV_DIFF_ROWS = -2,
    ERR_CSV_WRONG_VALUES = -3,
    ERR_CSV_WRONG_SEP = -4,
    ERR_CSV_WRONG_SHAPE = -5,
    ERR_CSV_EVEN_COL = -6,
    ERR_CSV_READ_DIGIT = -7,
    ERR_ALLOC = -8,
    ERR_EMPTY_GRAPH = -9,
    ERR_WRONG_PARAM = -10
};

typedef struct Gr {
    int **mat; //adjacency matrix
    int n; //dimension
} Graph;

//create graph
void init(Graph *g);

//check if empty
int is_empty_graph(const Graph *g);

//deinitialize graph
void free_graph(Graph *g);

//initialize graph from csv file
int init_file(Graph *g, const char *init_filename);

//print graph
void print_graph(const Graph *g);

//remove node
int remove_node(Graph *g, int d);

//remove_edge
int remove_edge(Graph *g, int from, int to);

//add_edge
int add_edge(Graph *g, int from, int to);

//add_node
int add_node(Graph *g);

//make csv file of current graph
int print_csv(const Graph *g, const char *output_filename);

//make dot file of current graph for vizualization
int print_dot(const Graph *g, const char *output_filename);

//check if graph is connected
int check_connected(const Graph *g);

//find path
int find_path(
        const Graph *g,
        int from,
        int to,
        int **path,
        int *path_length);
