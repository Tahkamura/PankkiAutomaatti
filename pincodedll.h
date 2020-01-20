#ifndef PINCODEDLL_H
#define PINCODEDLL_H
#include <QObject>
#include "pincodedll_global.h"
#include "dialog.h"

class PINCODEDLLSHARED_EXPORT PinCodeDLL :public QObject
{
Q_OBJECT
public:
    PinCodeDLL();
    ~PinCodeDLL();
    void PINCODEDLLSHARED_EXPORT ikkunaAukiTaiTapahtuuHirveita();
    void PINCODEDLLSHARED_EXPORT ikkunaTakas();
    void PINCODEDLLSHARED_EXPORT eksekki();
    void PINCODEDLLSHARED_EXPORT ikkunaPiiloon();
    Dialog *dialogOlio;


signals:
    void PINCODEDLLSHARED_EXPORT PinKoodi(QString);
private slots:
        void haeSeVitunPinKoodi(QString);
private:

    QString pinKoodi;
};

#endif // PINCODEDLL_H
