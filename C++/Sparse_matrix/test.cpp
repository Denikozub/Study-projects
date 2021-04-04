#include <iostream>
#include <stdexcept>
#include "sparse_matrix.hpp"
using namespace std;

int main()
{
    try {
        SparseMatrix<double> m1(2, 3, 1), m2(12, 20, 0), m4(10, 9, 3);
        const SparseMatrix<double> m3(5, 6, 4);
        cout << "_____num____" << endl;
        cout << m1.num_columns() << endl;
        cout << m1.num_rows() << endl;
        cout << "_____num const____" << endl;
        cout << m3.num_columns() << endl;
        cout << m3.num_rows() << endl;
        cout << "_____get____" << endl;
        cout << m1.get(1, 2) << endl;
        //cout << m1.get(2, 3) << endl;
        cout << m3.get(0, 0) << endl;
        //cout << m3.get(5, 6) << endl;
        cout << "_____= const____" << endl;
        m1 = m3;
        cout << m1.num_columns() << endl;
        cout << m1.num_rows() << endl;
        cout << m1.get(0, 0) << endl;
        cout << "_____=____" << endl;
        m1 = m2;
        cout << m1.num_columns() << endl;
        cout << m1.num_rows() << endl;
        cout << m1.get(0, 0) << endl;
        cout << "_____indexing____" << endl;
        m2[0][1] = 1;
        m2[0][2] = 2;
        m2[0][3] = 3;
        **m2 = 0;
        *(2 + *m2 + 2) = 4;
        *(*(0 + m2 + 0) + 5) = 5;
        m4 = m2;
        cout << *(*m4 + 1) << endl;
        cout << m4[0][0] << endl;
        cout << **m4 << endl;
        cout << *(*(m4 + 0) + 2) << endl;
        cout << m4[0][4] << endl;
        cout << m4[0][5] << endl;
        cout << "_____indexing const____" << endl;
        const SparseMatrix<double> m5(m2);
	cout << *(*(0 + m5 + 0) + 4) << endl;
	cout << *(1 + *m5 + 1) << endl;
	cout << **m5 << endl;
	cout << m5[0][3] << endl;
        cout << "____set_____" << endl;
        m2.set(5, 6, 54);
        cout << m2[5][6] << endl;
        cout << m4[5][6] << endl;
        m4.set(4, 4, 17);
        cout << m4.get(4, 4) << endl;
        cout << "_____==____" << endl;
        //cout << m4[23][0];
        //cout << m4[4][28];
        cout << (m1 == m2) << endl;
        m1 = m2;
        cout << (m1 == m2) << endl;
        m2 = m3;
        cout << (m1 == m2) << endl;
        cout << "_____== const____" << endl;
        cout << (m3 == m2) << endl;
        m2 = SparseMatrix<double>(5, 6, 4);
        cout << (m3 == m2) << endl;
        cout << (m3 == SparseMatrix<double>(5, 6, 4)) << endl;
        cout << "_____sum____" << endl;
        SparseMatrix<double> a(3, 4, 1), b(3, 4, 2);
        a[2][2] = 44;
        b[0][0] = 66;
        SparseMatrix<double> c(a + b);
        cout << c.get(0, 0) << endl;
        cout << c.get(0, 1) << endl;
        cout << c.get(1, 3) << endl;
        cout << c.get(1, 2) << endl;
        cout << c.get(2, 2) << endl;
        cout << c.get(2, 3) << endl;
        cout << "_____mult 1____" << endl;
        SparseMatrix<double> d(1, 2, 0), e(2, 2, 0);
        d[0][0] = 1;
        d[0][1] = 5;
        e[0][0] = 2;
        e[0][1] = 3;
        e[1][0] = 0;
        e[1][1] = 4;
        SparseMatrix<double> f(d * e);
        cout << f.get(0, 0) << endl;
        cout << f.get(0, 1) << endl;
        cout << "_____mult 2____" << endl;
        SparseMatrix<double> g(4, 4, 0), h(4, 4, 0);
        g[2][1] = 5;
        g[0][3] = -1;
        h[1][1] = 3.3;
        SparseMatrix<double> j = g * h;
        cout << j.get(0, 0) << endl;
        cout << j.get(2, 1) << endl;
        cout << j.get(1, 1) << endl;
    }
    catch (exception& e) {
        cerr << typeid(e).name() << ": " << e.what() << endl;
    }
    return 0;
}
