#ifndef MYSQLDLL_H
#define MYSQLDLL_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QApplication>

#include "mysqldll_global.h"

typedef int bool32;

struct pinCodeResult
{
   bool32 PinResult;
   int AccountID;
   int CardID;
   QString CorrectPIN;
};
struct info
{
    QString AccountInfo[4];
    QString CustomerInfo[2];
    QString CardInfo[5];
    int MaxInvoiceID;
};

class MYSQLDLLSHARED_EXPORT MySqlDll
{

public:
    MySqlDll();
    ~MySqlDll();
    bool32 MYSQLDLLSHARED_EXPORT IsValid(QSqlQuery *Query);
    QSqlQuery MYSQLDLLSHARED_EXPORT RunQuery(QSqlDatabase *DB, QString QueryString);
    QString MYSQLDLLSHARED_EXPORT haeNimi(QSqlDatabase *DB,QString RFID);
    bool32 MYSQLDLLSHARED_EXPORT createExchangeInvoice(QSqlDatabase *DB, QString SourceAccountID, QString DestinationAccount, QString ExchangeAmount);
    pinCodeResult MYSQLDLLSHARED_EXPORT TryPINCode(QSqlDatabase *DB, QString RFID, QString PIN);
    bool32 MYSQLDLLSHARED_EXPORT AddUserAccount(QSqlDatabase *DB, QString Name);
    QSqlQuery MYSQLDLLSHARED_EXPORT Withdrawal(QSqlDatabase *DB, QString SourceAccountName, QString Text, QString Amount);
    QSqlQuery MYSQLDLLSHARED_EXPORT ExchangeMoney(QSqlDatabase *DB, QString SourceAccountID, QString DestinationAccount, QString DestinationAccountName, QString ExchangeAmount);
    QSqlQuery MYSQLDLLSHARED_EXPORT ExchangeMoney(QSqlDatabase *DB, int SourceAccountID, QString DestinationAccountname, QString InvoiceText, int ExchangeAmount);
    info MYSQLDLLSHARED_EXPORT GetAllInformation(QSqlDatabase *DB, QString RFID);
    QSqlQuery MYSQLDLLSHARED_EXPORT lockCard(QSqlDatabase *DB, QString Card_ID);
    int MYSQLDLLSHARED_EXPORT GetCount(QSqlDatabase *DB,QString AccountID, int count);

};

#endif // MYSQLDLL_H
