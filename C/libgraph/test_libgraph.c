//by Denis Kozub
//following function calls do not allow to get full coverage on tests
//specific errors will not be covered
//they are here to solve a task and demonstrate how to use libgraph

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

//task:
//we are given a graph in a csv file
//it represents cities (num > 4) and roads in a country
//road between city 5 and 2 has broken
//find out if it is still possible to get to capital city 4 from anywhere
//if not, repair the road and find path from city 1 to capital
//matematical language:
//if (the graph is not connected) then begin
//(add an edge between 5 and 2) end
//find path between 1 and 4
//in any case demonstrate other functions
int
main(int argc, char *argv[])
{
    //create graph
    Graph g;
    init(&g);

    //initialize graph from csv file
    int result;
    if ((argc > 1) && ((result = init_file(&g, argv[1])) < 0)) {
        printf("\ninit_file exited with code: %d\n", result);
        free_graph(&g);
        return result;
    }

    //print graph
    print_graph(&g);

    //check if graph is connected
    if ((result = check_connected(&g)) < 0) {
        printf("\ncheck_connected exited with code: %d\n", result);
        free_graph(&g);
        return result;
    }
    if (result) {
        printf("connected\n----------\n");
    }
    else {
        printf("not connected\n----------\n");
        //add_edge
        printf("adding edge from 5 to 2:\n");
        int result1;
        if ((result1 = add_edge(&g, 5, 2)) < 0) {
            printf("\nadd_edge exited with code: %d\n", result1);
            free_graph(&g);
            return result1;
        }
        print_graph(&g);
    }

    //find path
    int *path = NULL, path_length = 0;
    int from = 1, to = 4;
    if ((result = find_path(&g, from, to, &path, &path_length)) < 0) {
        printf("\nfind_path exited with code: %d\n", result);
        free_graph(&g);
        return result;
    }
    if (path == NULL) {
        printf("no path from %d to %d found\n----------\n", from, to);
    }
    else {
        printf("path from %d to %d found:\n", from, to);
        printf("%d", path[0]);
        for (int i = 1; i < path_length; i++) {
            printf(" -> %d", path[i]);
        }
        printf("\n----------\n");
    }
    free(path);

    printf("demonstrating other functions:\n----------\n");
    //remove node
    printf("remove node 3:\n");
    if ((result = remove_node(&g, 3)) < 0) {
        printf("\nremove_node exited with code: %d\n", result);
        free_graph(&g);
        return result;
    }

    print_graph(&g);

    //add_node
    printf("add node:\n");
    if ((result = add_node(&g)) < 0) {
        printf("\nadd_node exited with code: %d\n", result);
        free_graph(&g);
        return result;
    }

    print_graph(&g);

    //remove_edge
    printf("removing edge from 2 to 4:\n");
    if ((result = remove_edge(&g, 2, 4)) < 0) {
        printf("\nremove_edge exited with code: %d\n", result);
        free_graph(&g);
        return result;
    }

    print_graph(&g);

    //make csv file of current graph
    printf("making csv file...done\n----------\n");
    const char *output_filename_csv = "output.csv";
    if ((result = print_csv(&g, output_filename_csv)) < 0) {
        printf("\nprint_csv exited with code: %d\n", result);
        free_graph(&g);
        return result;
    }

    //make dot file of current graph for vizualization
    printf("making dot file...done\n----------\n");
    const char *output_filename_dot = "output.dot";
    if ((result = print_dot(&g, output_filename_dot)) < 0) {
        printf("\nprint_dot exited with code: %d\n", result);
        free_graph(&g);
        return result;
    }

    //deinitialize graph
    free_graph(&g);
    return 0;
}
