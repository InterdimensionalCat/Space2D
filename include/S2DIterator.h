#pragma once

#ifndef S2D_ITR_DEF
#define S2D_ITR_DEF(typ) \
struct Iterator {\
T& operator*() const {\
    return *val[index];\
}\
T& operator*() {\
    return *val[index];\
}\
bool operator==(const Iterator& other) {\
    return index == other.index && val == other.val;\
}\
bool operator!=(const Iterator& other) {\
    return !(*this == other);\
}\
Iterator& operator++() {\
    index++;\
    return *this;\
}\
private:\
    size_t index;\
    typ*   val;\
};\
const Iterator begin() const {\
    return Iterator(0, this);\
}\
const Iterator end() const {\
    return Iterator(numVals(), this);\
}\
Iterator begin() {\
    return Iterator(0, this);\
}\
Iterator end() {\
    return Iterator(numVals(), this);\
}
#endif