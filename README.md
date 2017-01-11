# Template-for-Double-Ended-Queue-Container

Developed a macro to implement a template for double ended queue and its iterators in C++ without using any OO concepts, to work with any type of data.

Implemented following major functions:
1. push_back(): insert element at the back.
2. push_front(): insert element at the front.
3. pop_back(): remove element from the back.
4. pop_front(): remove element from the front.
5. at(): randomly access element at any given position.
6. front(): returns the element at the front.
7. back(): returns the element at the back.
8. begin(): gives an iterator pointing to the element at the front.
9. end(): gives an iterator pointing to one position after the last element (which is NULL).

The iterator provided the following functionalities,
1. inc(): move the iterator to the next element.
2. dec(): move the iterator to the previous element.
3. deref(): return the value, the iterator is pointing to.
