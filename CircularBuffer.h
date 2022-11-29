#ifndef CIRCULARBUFFER_INCLUDED
#define CIRCULARBUFFER_INCLUDED

#include <cassert>
#include <array>

template<typename T, size_t S> class CircularBuffer
{
    public:
        CircularBuffer() : 
            m_head(0), 
            m_tail(0)
        {
            
        }

        void push(T item)
        {
            if(isFull())
                pop();

            m_array[m_tail] = item;
            m_tail = (m_tail + 1) % S;
        }

        T pop()
        {
            assert(!isEmpty());

            T item = m_array[m_head];
            T empty;
            m_array[m_head] = m_emptyItem;

            m_head = (m_head + 1) % S;

            return item;
        }

        void clear()
        {
            while(!isEmpty())
            {
                pop();
            }
        }

        inline T front() const
        {
            return m_array[m_head];
        }

        inline bool isEmpty() const
        {
            return m_head == m_tail;
        }

        inline bool isFull() const 
        {
            return m_head != m_tail && m_tail == (m_head - 1) % S;
        }

        size_t size()
        {
            if(m_tail >= m_head)
            {
                return m_tail - m_head;
            }

            return S - m_head - m_tail;
        }

        T get(int index)
        {
            return m_array[(m_head + index) % S];
        }
    
    private:
        std::array<T, S> m_array;
        size_t m_head;
        size_t m_tail;
        T m_emptyItem;
};

#endif
