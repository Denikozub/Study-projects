#include <iostream>
#include <stdexcept>
#include "matrix.hpp"
#include "dense.hpp"
#include "sparse.hpp"
#include "iterator.hpp"

using std::cout;
using std::endl;

int main()
{
    try {
        IterableDenseMatrix a = { { {0, 2, 0, 0, 0},
                                    {1, 0, 0, 0, 0},
                                    {0, 0, 0, 0, 0},
                                    {0, 0, 1, 1, 0},
                                    {0, 0, 1, 0, 0} }, 5 };

        IterableSparseMatrix b = { { {0, { {1, 2} }},
                                     {1, { {0, 1} }},
                                     {3, { {2, 1}, {3, 1} }},
                                     {4, { {2, 1} }}, }, 5 };

        IterableDenseMatrix c = { { {0, 1, 0, 0, 2},
                                    {0, 0, 0, 1, 0},
                                    {0, 0, 2, 0, 0},
                                    {1, 0, 0, 0, 0},
                                    {0, 1, 1, 0, 0} }, 5 };

        IterableSparseMatrix d = { { {0, { {1, 1}, {4, 2} }},
                                     {1, { {3, 1} }},
                                     {2, { {2, 2} }},
                                     {3, { {0, 1} }},
                                     {4, { {1, 1}, {2, 1} }}, }, 5 };

        vector<IterableSquareMatrix*> matrixes = { &a, &b, &c, &d };
        Iterator* begin, * end;

        int matrix_num = 1;
        for (auto m : matrixes) {
            cout << "MATRIX #" << matrix_num << endl << endl;
            cout << "________size________" << endl;
            cout << m->size() << endl;

            vector<size_t> columns = { 2, 4 };
            for (auto col : columns) {
                cout << "________column_" << col << "________" << endl;
                begin = m->iterate_columns(col); end = m->end_columns(col);
                for (; begin != nullptr && *begin != *end; ++(*begin))
                    cout << (**begin).first << ": " << (**begin).second << endl;
                delete begin; delete end;
            }

            vector<size_t> rows = { 2, 3 };
            for (auto row : columns) {
                cout << "________row_" << row << "________" << endl;
                begin = m->iterate_rows(row); end = m->end_rows(row);
                for (; begin != nullptr && *begin != *end; ++(*begin))
                    cout << (**begin).first << ": " << (**begin).second << endl;
                delete begin; delete end;
            }
            
            cout << "________max_inc_edges________" << endl;
            cout << m->max_inc_edges() << endl;

            cout << "________max_outc_edges________" << endl;
            cout << m->max_outc_edges() << endl;

            cout << "________components________" << endl;
            cout << m->strong_components_number() << endl;

            cout << endl << endl;
            ++matrix_num;
        }
    }
    catch (std::exception& e) {
        std::cerr << typeid(e).name() << ": " << e.what() << std::endl;
    }

    return 0;
}
