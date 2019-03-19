//
// Created by hdc on 3/18/19.
//

#ifndef CPLUS_MAX_H
#define CPLUS_MAX_H

template <typename T>

inline T const& Max (T const& a, T const& b)
{
    return a < b? b:a;
}

#endif //CPLUS_MAX_H
