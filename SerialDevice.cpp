#include "SerialDevice.h"

//-----------------------------------------------------------------------------

SerialDevice::SerialDevice(QObject *parent) : QObject(parent)
{
   this->timeoutRead = 500;
   this->timeoutWrite = 500;
}

//-----------------------------------------------------------------------------

bool SerialDevice::open(const QString &portName)
{
   close();

   this->portName = portName;

   serial.setPortName(portName);
   serial.setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);
   serial.setFlowControl(QSerialPort::NoFlowControl);
   serial.setDataBits(QSerialPort::Data8);
   serial.setParity(QSerialPort::NoParity);
   serial.setStopBits(QSerialPort::OneStop);

   if ( serial.open(QIODevice::ReadWrite) ) {
      return true;
   }
   return false;
}

//-----------------------------------------------------------------------------

void SerialDevice::close()
{
   serial.close();
}

//-----------------------------------------------------------------------------

qint32 SerialDevice::peek(char *buffer, qint32 size)
{
   qint32 remaining = size;
   qint32 received;
   qint32 offset = 0;

   do
   {
      while ( serial.bytesAvailable() )
      {
         received = static_cast<qint32>(serial.peek(&buffer[offset], remaining));
         if ( received < 0 ) {
            return RetSerialFailure;
         }

         if ( received > 0 )
         {
            offset += received;
            remaining -= received;

            if ( offset == size ) {
               return size;
            }
         }
      }

      if ( serial.waitForReadyRead(timeoutRead) == false ) {
         return RetSerialTimeout;
      }
   } while ( remaining > 0 );

   return offset;
}

//-----------------------------------------------------------------------------

qint32 SerialDevice::read(char *buffer, qint32 size)
{
   qint32 remaining = size;
   qint32 received;
   qint32 offset = 0;

   do
   {
      while ( serial.bytesAvailable() )
      {
         received = static_cast<qint32>(serial.read(&buffer[offset], remaining));
         if ( received < 0 ) {
            return RetSerialFailure;
         }

         if ( received > 0 )
         {
            offset += received;
            remaining -= received;

            if ( offset == size ) {
               return size;
            }
         }
      }

      if ( serial.waitForReadyRead(timeoutRead) == false ) {
         return RetSerialTimeout;
      }
   } while ( remaining > 0 );

   return offset;
}

//-----------------------------------------------------------------------------

qint32 SerialDevice::write(const char *buffer, qint32 size)
{
   qint32 remaining = size;
   qint32 sended;
   qint32 offset = 0;

   do
   {
      sended = static_cast<qint32>(serial.write(&buffer[offset], remaining));
      if ( sended < 0 ) {
         return RetSerialFailure;
      }

      if ( sended > 0 )
      {
         if ( serial.waitForBytesWritten(timeoutWrite) )
         {
            serial.flush();

            offset += sended;
            remaining -= sended;

            if ( offset == size ) {
               return size;
            }
         }
      }
   } while ( remaining > 0 );

   return offset;
}

//-----------------------------------------------------------------------------
