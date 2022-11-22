#ifndef PROGRAM_INCLUDED
#define PROGRAM_INCLUDED

class Program
{
    public:
        Program()
        {
            
        }

        void on_toggled()
        {
            if(m_enabled = !m_enabled)
                enable();
            else
                disable();
        }

    private:
        bool m_enabled { false };

        void enable()
        {
            
        }

        void disable()
        {
            
        }
};

#endif
