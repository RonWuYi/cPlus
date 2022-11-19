#ifndef GT_H_

#define GT_H_

#include "example.h"

using namespace leet;

namespace gt
{
    template<typename T>
    class MyCircularQueue {
    public:
        MyCircularQueue() = delete;
        MyCircularQueue(int k) : m_size{0}, full_size{k} {
            t_arr = new T(k);
        };

        bool enQueue(T t) {
            if (m_size == full_size)
            {
                /* code */
                return false;
            }
            else{
                t_arr[++m_size] = t;
                return true;
            }
            

        }
        bool deQueue() {
            if (m_size == 0)
            {
                /* code */
                return false;
            }
            
            t_arr[m_size] = 0;
            return true;
            // if (m_size == full_size)
            // {
            //     /* code */
            // }
            
        }

        int Rear();
        bool isFull() {
            if (m_size < full_size)
            {
                /* code */
                return false;
            }
            return true;
        }

    private:
        int m_size;
        int full_size;
        // std::array<T, m_size> p;
        T t_arr[];
    };
   
    // MyCircularQueue::MyCircularQueue(int k) : m_size(k) {

    // }
} // namespace gt

#endif