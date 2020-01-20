#ifndef LAHJOITUSDLL_H
#define LAHJOITUSDLL_H

#include "lahjoitusdll_global.h"
#include "lahjoitusdialog.h"
#include <QObject>

class LAHJOITUSDLLSHARED_EXPORT LahjoitusDLL : public QObject
{
Q_OBJECT
public:
    LahjoitusDLL();
    ~LahjoitusDLL();
    void LAHJOITUSDLLSHARED_EXPORT ikkunaAuki();
    void LAHJOITUSDLLSHARED_EXPORT ikkunaKiinni();
    void LAHJOITUSDLLSHARED_EXPORT execAuki();
    lahjoitusDialog *lahjoitusOlio;

signals:


private slots:

};

#endif // LAHJOITUSDLL_H
