# Sparse matrix
## by Denis Kozub
___
- template class of SparseVector  
- uses template class List to store a list of non-default elements  
- precision of floating point comparison set by epsilon parameter  
- default value can be set by default_value parameter  
- indexing operators [] and * for non-constant objects allocate memory for a default object (ex cout << v[0])  
- NO cycles through default elements

### Methods
~~~
template <class T> class SparseMatrix
~~~
T can be any type that has:
- friend const T operator+ (const T&, const T&)
- T& operator=  (const T&) const
- bool operator!= (const T&) const
- T ()
- T (const T&)

~~~
explicit SparseMatrix(size_t number_of_lines, size_t number_of_columns, T default_value, double epsilon = 0.0000001);
~~~
sets number of columns and lines in matrix, default value that fills the matrix and epsilon for floating type comparison

~~~
void set(size_t line_number, size_t col_number, T value);
~~~
sets value at double index

~~~
size_t num_rows() const;
~~~
return number of rows

~~~
size_t num_columns() const;
~~~
return number of columns

~~~
T get(size_t line_number, size_t col_number) const;
~~~
get value at double index
does not allocate memory

~~~
SparseVector<T>& operator[] (size_t line_number);
~~~
return address at double index
if value was not set, allocates memory and sets default value

~~~
SparseVector<T> operator[] (size_t line_number) const;
~~~
get value at double index
does not allocate memory

~~~
SparseMatrix<T>& operator+ (size_t line_number);
~~~
sets double index to return by operator*
by default operator* returns element [0][0]

~~~
SparseVector<T>& operator* ();
~~~
return address at double index
if value was not set, allocates memory and sets default value

~~~
SparseVector<T> operator* () const;
~~~
get value at double index
does not allocate memory

~~~
bool operator== (const SparseMatrix<T>& obj) const;
~~~
compare two matrixes
epsilon not accounted

~~~
bool operator!= (const SparseMatrix<T>& obj) const;
~~~
compare two matrixes
epsilon not accounted

~~~
friend const SparseMatrix<T> operator+ (const SparseMatrix<T>& obj1, const SparseMatrix<T>& obj2);
~~~
return sum of 2 matrixes

~~~
friend const SparseMatrix<T> operator* (const SparseMatrix<T>& obj1, const SparseMatrix<T>& obj2);
~~~
return mult of 2 matrixes

### Example
~~~
SparseMatrix<double> a(3, 4, 0), b(3, 4, 0);
a[2][2] = 4;
**b = 6;
SparseMatrix<double> c = a + b;
cout << c.get(0, 0) << endl;
cout << *(*(c + 2) + 2) << endl;
~~~

