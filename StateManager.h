#include <Preferences.h>
#include "Position.h"

#define USER_ID_KEY "USER_ID"

class StateManager
{
    public:
        StateManager(Preferences& store) : m_store(store)
        {
          m_position = { .Latitude = 50.62021924615207, .Longitude = 5.582367411365839}; 
        }

        void loadData() 
        {
           m_userId = m_store.getString(USER_ID_KEY, "00000-00000");
        }

        String& getUserId()
        {
            return m_userId;
        }

        void setUserId(String userId)
        {
            m_store.putString(USER_ID_KEY, userId);
            m_userId = userId;
        }

        boolean isSOSMode()
        {
            return m_sosToggled;
        }

        void toggleSOSMode()
        {
            m_sosToggled = !m_sosToggled;
        }

        Position getPosition()
        {
            return m_position;
        }

        void setPosition(Position position)
        {
            m_position.Longitude = position.Longitude;
            m_position.Latitude = position.Latitude;
        }

    private:
        boolean m_sosToggled;
        String m_userId;
        Position m_position; 
        Preferences& m_store;

};
