#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QObject>
#include <QSerialPort>

#define RetSerialFailure -1
#define RetSerialTimeout -2

//-----------------------------------------------------------------------------

class SerialDevice : public QObject
{
   Q_OBJECT

public:
   explicit SerialDevice(QObject *parent = nullptr);

   bool open(const QString &portName);
   void close();

   void setTimeoutRead(qint32 timeoutRead) { this->timeoutRead = timeoutRead; }
   void setTimeoutWrite(qint32 timeoutWrite) { this->timeoutWrite = timeoutWrite; }

   qint32 peek(char *buffer, qint32 size);
   qint32 read(char *buffer, qint32 size);
   qint32 write(const char *buffer, qint32 size);

private:
   QSerialPort serial;

   QString portName;

   qint32 timeoutRead;
   qint32 timeoutWrite;

};

//-----------------------------------------------------------------------------

#endif // SERIALDEVICE_H
