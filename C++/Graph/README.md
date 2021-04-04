# Class iterable square matrix for graph storing
by Kozub Denis
___
### General
1) Multigraph  
2) Loops allowed  
3) Directed graph  
Note: a loop increases diagonal element by 1

### Project structure
1) Iterator - abstract class for matrix edge (non-zero) iterating  
2) IterableSquareMatrix - polymorphic class setting interface for matrix classes:
* IterableDenseMatrix - dense storing
* IterableSparseMatrix - sparse storing

### class Iterator
~~~
virtual pair<size_t, size_t> operator* () const = 0;
~~~
Returns a pair of a vertex and number of connections
~~~
virtual Iterator& operator++ () = 0;
~~~
Increasing an iterator
~~~
virtual bool operator== (const Iterator&) = 0;
~~~
~~~
virtual bool operator!= (const Iterator&) = 0;
~~~

##### Example - iterating a column
~~~
Iterator* begin = m->iterate_columns(col), * end = m->end_columns(col);
for (; begin != nullptr && *begin != *end; ++(*begin))
    cout << (**begin).first << ": " << (**begin).second << endl;
delete begin; delete end;
~~~

### class IterableSquareMatrix
~~~
virtual Iterator* iterate_rows(size_t) const = 0;
~~~
Returns a const begin iterator for iterating a row
~~~
virtual Iterator* iterate_columns(size_t) const = 0;
~~~
Returns a const begin iterator for iterating a column
~~~
virtual Iterator* end_rows(size_t) const = 0;
~~~
Returns a const end iterator for iterating a row
~~~
virtual Iterator* end_columns(size_t) const = 0;
~~~
Returns a const end iterator for iterating a column
~~~
size_t size() const;
~~~
Returns size of a graph
~~~
size_t max_inc_edges() const;
~~~
Returns a vertex which has most incoming edges
~~~
size_t max_outc_edges() const;
~~~
Returns a vertex which has most outcoming edges
~~~
size_t strong_components_number() const;
~~~
Returns a number of strong connected components

### class IterableDenseMatrix
~~~
IterableDenseMatrix(const vector<vector<size_t>>&, size_t);
~~~

##### Example - constructing a dense matrix
~~~
IterableDenseMatrix a = { { {0, 2, 0, 0, 0},
                            {1, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0},
                            {0, 0, 1, 1, 0},
                            {0, 0, 1, 0, 0} }, 5 };
~~~

### class IterableSparseMatrix
~~~
IterableSparseMatrix(const map<size_t, map<size_t, size_t>>&, size_t);
~~~

##### Example - constructing a sparse matrix
~~~
IterableSparseMatrix b = { { {0, { {1, 2} }},
                             {1, { {0, 1} }},
                             {3, { {2, 1}, {3, 1} }},
                             {4, { {2, 1} }}, }, 5 };
~~~
Note: matrix b equals matrix a
