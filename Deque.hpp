#ifndef DEQUE_H
#define DEQUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <assert.h>
#include <unistd.h>

#define INITIAL_LENGTH 2

#define Deque_DEFINE(t)\
typedef struct Deque_##t Deque_##t;\
typedef struct Deque_##t##_Iterator Deque_##t##_Iterator;\
\
struct Deque_##t {\
    t *arr;\
    int front_index;\
    int back_index;\
    int curr_length;\
    int max_length;\
    char type_name[sizeof("Deque_"#t)];\
    \
    void (*resize)(Deque_##t *);\
    \
    void (*push_front)(Deque_##t *, t);\
    void (*push_back)(Deque_##t *, t);\
    \
    void (*pop_front)(Deque_##t *);\
    void (*pop_back)(Deque_##t *);\
    \
    t& (*front)(const Deque_##t *);\
    t& (*back)(const Deque_##t *);\
    t& (*at)(const Deque_##t *,const int);\
    \
    Deque_##t##_Iterator (*begin)(const Deque_##t *);\
    Deque_##t##_Iterator (*end)(const Deque_##t *);\
    \
    size_t (*size)(const Deque_##t *);\
    bool (*empty)(const Deque_##t *);\
    bool (*less)(const t &,const t &);\
    \
    void (*clear)(Deque_##t *);\
    void (*dtor)(Deque_##t *);\
};\
\
struct Deque_##t##_Iterator {\
    t *Iterator_arr;\
    int index;\
    int begin_index;\
    int capacity;\
    \
    t& (*deref)(Deque_##t##_Iterator *);\
    void (*inc)(Deque_##t##_Iterator *);\
    void (*dec)(Deque_##t##_Iterator *);\
};\
\
\
t &deref(Deque_##t##_Iterator *it){\
    return it->Iterator_arr[(it->index+it->begin_index)%it->capacity];\
}\
\
void inc(Deque_##t##_Iterator *it){\
    it->index++;\
}\
\
void dec(Deque_##t##_Iterator *it){\
    it->index--;\
}\
\
void resize(Deque_##t *deq){\
    if(deq->curr_length==(deq->max_length-1)){\
        t *temp = (t *)malloc(2*sizeof(t)*(deq->max_length));\
        int f_index=deq->front_index;\
        for(int i=0;i<=deq->curr_length;i++){\
            f_index=(deq->front_index+i)%deq->max_length;\
            temp[i]=deq->arr[f_index];\
        }\
        if(deq->arr!=NULL)\
            free(deq->arr);\
        deq->arr=temp;\
        deq->front_index=0;\
        deq->back_index=deq->curr_length-1;\
        deq->max_length=2*deq->max_length;\
    }\
}\
\
void push_front(Deque_##t *deq, t data){\
    resize(deq);\
    if(deq->curr_length!=0)\
        deq->front_index =(deq->front_index - 1 + deq->max_length)%deq->max_length;\
    deq->arr[deq->front_index]=data;\
    ++deq->curr_length;\
}\
\
void push_back(Deque_##t *deq, t data){\
    resize(deq);\
    if(deq->curr_length!=0)\
        deq->back_index = (deq->back_index + 1)%deq->max_length;\
    deq->arr[deq->back_index]=data;\
    ++deq->curr_length;\
}\
\
void pop_front(Deque_##t *deq){\
    if(deq->curr_length!=0){\
        deq->front_index = (deq->front_index + 1)%deq->max_length;\
        --deq->curr_length;\
    }\
    return;\
}\
\
void pop_back(Deque_##t *deq){\
    if(deq->curr_length!=0){\
        deq->back_index =(deq->back_index - 1 + deq->max_length)%deq->max_length;\
        --deq->curr_length;\
    }\
    return;\
}\
\
size_t size(const Deque_##t *deq){\
    return deq->curr_length;\
}\
\
bool empty(const Deque_##t *deq){\
    if(deq->curr_length==0)\
        return true;\
    else\
        return false;\
}\
\
t& front(const Deque_##t *deq){\
    return deq->arr[deq->front_index];\
}\
\
t& back(const Deque_##t *deq){\
    return deq->arr[deq->back_index];\
}\
\
t& at(const Deque_##t *deq, int index){\
    return deq->arr[(deq->front_index+index)%deq->max_length];\
}\
\
void clear(Deque_##t *deq){\
    if(deq->curr_length!=0)\
        deq->curr_length=0;\
}\
\
void dtor(Deque_##t *deq){\
    if(deq->arr!=NULL)\
        free(deq->arr);\
}\
\
Deque_##t##_Iterator begin(const Deque_##t *deq){\
    Deque_##t##_Iterator temp;\
    temp.Iterator_arr=deq->arr;\
    temp.index=0;\
    temp.begin_index=deq->front_index;\
    temp.capacity=deq->max_length;\
    temp.deref=deref;\
    temp.dec=dec;\
    temp.inc=inc;\
    return temp;\
}\
\
Deque_##t##_Iterator end(const Deque_##t *deq){\
    Deque_##t##_Iterator temp;\
    temp.Iterator_arr=deq->arr;\
    temp.index=deq->curr_length;\
    temp.begin_index=deq->front_index;\
    temp.capacity=deq->max_length;\
    temp.deref=deref;\
    temp.dec=dec;\
    temp.inc=inc;\
    return temp;\
}\
\
\
void Deque_##t##_ctor(Deque_##t *deq, bool (* less)(const t &,const t &)) {\
    deq->less= less;\
    \
    t *temp=(t *)malloc(sizeof(t)*INITIAL_LENGTH);\
    deq->arr=temp;\
    deq->front_index=0;\
    deq->back_index=0;\
    deq->curr_length=0;\
    deq->max_length=INITIAL_LENGTH;\
    strcpy(deq->type_name, "Deque_"#t);\
    \
    deq->push_front=push_front;\
    deq->push_back=push_back;\
    deq->pop_front=pop_front;\
    deq->pop_back=pop_back;\
    deq->front=front;\
    deq->back=back;\
    deq->at=at;\
    deq->begin=begin;\
    deq->end=end;\
    deq->size=size;\
    deq->empty=empty;\
    deq->clear=clear;\
    deq->resize=resize;\
    deq->dtor=dtor;\
}\
\
bool Deque_##t##_equal(Deque_##t &deq1, Deque_##t &deq2){\
    if(strcmp(deq1.type_name,deq2.type_name)==0 && (deq1.curr_length==deq2.curr_length)){\
        for(int i=0; i<deq1.curr_length;i++){\
            if((deq1.less(deq1.arr[(deq1.front_index+i)%deq1.max_length],deq2.arr[(deq2.front_index+i)%deq2.max_length])==true) ||\
               (deq1.less(deq2.arr[(deq2.front_index+i)%deq2.max_length],deq1.arr[(deq1.front_index+i)%deq1.max_length])==true))\
                return false;\
        }\
        return true;\
    }\
    return false;\
}\
\
bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator it1,Deque_##t##_Iterator it2){\
    if((it1.Iterator_arr==it2.Iterator_arr) && (it1.index==it2.index)){\
        return true;\
    }\
    return false;\
}
#endif
