#include <Preferences.h>
#include "Position.h"
#include "IToggleSensor.h"
#include <vector>

#define USER_ID_KEY "USER_ID"

class StateManager
{
    public:
        StateManager(Preferences& store) : 
            m_store(store), 
            m_sosToggled(false)
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

        inline bool isSOSMode() const
        {
            return m_sosToggled;
        }

        void toggleSOSMode()
        {
            enableSOSMode(!m_sosToggled);
        }

        void enableSOSMode(bool toggle)
        {
            m_sosToggled = toggle;

            for(const auto& toggleSensor : m_toggleSensors)
            {
                toggleSensor->enable(m_sosToggled);
            }
        }

        Position getPosition() const
        {
            return m_position;
        }

        void setPosition(Position position)
        {
            m_position.Longitude = position.Longitude;
            m_position.Latitude = position.Latitude;
        }

        void update()
        {
            for(const auto& toggleSensor : m_toggleSensors)
            {
                toggleSensor->update();
            }
        }

        void addToggleSensor(std::shared_ptr<IToggleSensor> toggleSensor)
        {
            m_toggleSensors.emplace_back(std::move(toggleSensor));
        }

    private:
        boolean m_sosToggled;
        String m_userId;
        Position m_position; 
        Preferences& m_store;
        std::vector<std::shared_ptr<IToggleSensor>> m_toggleSensors;
};
