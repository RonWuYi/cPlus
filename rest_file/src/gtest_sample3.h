//
// Created by hdc on 3/18/19.
//

#ifndef CPLUS_GTEST_SAMPLE3_H
#define CPLUS_GTEST_SAMPLE3_H

//#include </usr/lib/gcc/stddef.h>
//#include <template>
#include <stdio.h>
#include ""
#include <>
template <typename valueType>
void MyClass<valueType>::valueToMap(const std::string key, const valueType value) // error
^
{
_map[key] = std::to_string(value);
}

template <typename E>
class Queue;

template <typename E>
class QueueNode {
    friend class Queue<E>;
};

#endif //CPLUS_GTEST_SAMPLE3_H
