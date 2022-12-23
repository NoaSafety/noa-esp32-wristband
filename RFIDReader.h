#ifndef RFIDREADER_INCLUDED
#define RFIDREADER_INCLUDED

#include <SPI.h>
#include <MFRC522.h>
#include "NfcAdapter.h"

class RFIDReader
{
    public:
        RFIDReader(StateManager& state, int sck, int miso, int mosi, int sda, int rst) :
            m_state(state),
            m_sck(sck),
            m_miso(miso),
            m_mosi(mosi),
            m_sda(sda),
            m_rst(rst),
            m_mfrc522(sda, rst),
            m_nfc(&m_mfrc522)
        {
          
        }

        void takeSPILead()
        {
            SPI.end();
            SPI.begin(m_sck, m_miso, m_mosi);
            m_mfrc522.PCD_Init();
            m_nfc.begin();   
        }

        bool begin()
        {
            takeSPILead();
            return m_mfrc522.PCD_ReadRegister(MFRC522::PCD_Register::VersionReg) == 0x92;
        }

        void checkRFID()
        {
            takeSPILead();
            if (m_nfc.tagPresent())
            {
                Serial.println("Reading NFC tag\n");
                NfcTag tag = m_nfc.read();
                NdefRecord record = tag.getNdefMessage().getRecord(0);
                String userId = NFCDataToString(record.getPayload(), record.getPayloadLength());
                Serial.println("[RFID] " + userId);
                m_state.setUserId(userId);
            }
        }

    private:
        int m_sck;
        int m_miso;
        int m_mosi;
        int m_sda;
        int m_rst;
        MFRC522 m_mfrc522;
        NfcAdapter m_nfc;
        StateManager& m_state;

        String NFCDataToString(const byte * data, const long numBytes)
        {
            int32_t szPos;
            String s = "";
            
            for (szPos=0; szPos < numBytes; szPos++)
            {
                s = s + (char)data[szPos];
            }

            return s;
      };

};

#endif
