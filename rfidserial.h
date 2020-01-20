#ifndef RFIDSERIAL_H
#define RFIDSERIAL_H

#include "rfidserial_global.h"
#include <QObject>
#include <QSerialPort>


class RFIDSERIALSHARED_EXPORT RFIDSerial : public QObject
{
    Q_OBJECT
public:
    RFIDSerial();

signals:
    void RFIDSERIALSHARED_EXPORT RFIDTunnus(QString);
public slots:
    void readData();
private:
    QSerialPort *serial;
    QString RFID;
};

#endif // RFIDSERIAL_H
