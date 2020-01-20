#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QTextEdit>
#include <QtWidgets>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QApplication>
#include <QSerialPort>
#include <QObject>

#include "C:\Users\tonip\Desktop\Koulu\Ohjelmoinnin sovellusprojekti\KoodiHarjotuksia\bankSimul\rfidserial.h"
#include "C:\Users\tonip\Desktop\Koulu\Ohjelmoinnin sovellusprojekti\KoodiHarjotuksia\bankSimul\pincodedll.h"
#include "C:\Users\tonip\Desktop\Koulu\Ohjelmoinnin sovellusprojekti\KoodiHarjotuksia\bankSimul\mysqldll.h"
#include "C:\Users\tonip\Desktop\Koulu\Ohjelmoinnin sovellusprojekti\KoodiHarjotuksia\bankSimul\lahjoitusdll.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getValuesFromModelToSaldo();
    void getValuesFromModelToTili();
    void haeTiedot();
    void mousePressEvent(QMouseEvent *e);
    void createConnection();
signals:
    void nappiaPainettu();
    void kirjautumisYritys();
    void koodiVaarin(QString);
private slots:
    void idleUlostuminen();
    void poisLahjoituksesta();
    void vastaanOtaLahjoitus(QString);
    void kirjauduSisaan();
    void pinKoodiLuettu(QString);
    void luettuRFIDTunnus(QString);
    void kirjauduUlos();
    void ajastin();
    void on_pushButtonKirjauduUlos_clicked();
    void on_pushButtonSaldo_clicked();
    void on_pushButtonPaavalikkoonSaldosta_clicked();
    void on_pushButtonPaavalikkoonTilitapahtumista_clicked();
    void on_pushButtonPaavalikkoonNostosta_clicked();
    void on_pushButtonTilitapahtumat_clicked();
    void on_pushButtonNosto_clicked();
    void on_pushButtonNum1_clicked();
    void on_pushButtonNum2_clicked();
    void on_pushButtonNum3_clicked();
    void on_pushButtonNum4_clicked();
    void on_pushButtonNum5_clicked();
    void on_pushButtonNum6_clicked();
    void on_pushButtonNum7_clicked();
    void on_pushButtonNum8_clicked();
    void on_pushButtonNum9_clicked();
    void on_pushButtonNum0_clicked();
    void on_pushButtonNumBack_clicked();
    void on_pushButtonNumPoistu_clicked();
    void on_pushButtonMuuSumma_clicked();
    void on_pushButtonLahjoitus_clicked();
    void on_pushButton10e_clicked();
    void on_pushButton20e_clicked();
    void on_pushButton40e_clicked();
    void on_pushButton50e_clicked();
    void on_pushButton60e_clicked();
    void on_pushButton80e_clicked();
    void on_pushButton90e_clicked();
    void on_pushButton100e_clicked();
    void on_pushButtonTESTINosto_clicked();
    void on_pushButtonNumOK_clicked();
    void on_pushButtonScrollDown_clicked();
    void on_pushButtonScrollUP_clicked();
    void on_pushButtonTiliNayta10_clicked();
    void on_pushButtonTiliNayta20_clicked();
    void on_pushButtonTiliNayta30_clicked();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *modelSaldo;
    QSqlTableModel *modelTili;
    QTimer *timer;
    RFIDSerial *serial;
    QSqlDatabase db;
    PinCodeDLL *koodiOlio;
    MySqlDll *mysliOlio;
    LahjoitusDLL *keniaOlio;

    QString RFIDTunnus;
    QString kayttajaNimi;
    QString tilinumero;
    QString TilinSaldo;
    QString saldo;
    QString lahjoituksenMaara;
    QTableView *oTable;
    int muuSummaPyoristus;
    double haluttuNostonMaara;
    bool muuSummaState;
    double uusiTilisaldo;
    QString muuSumma;
    QString kayttajanPinKoodi;
    int kortinTilanne;
    int idleTimer;
    int scrollTiliValue;
    bool RFIDState;
    int naytettavaMaaraTili;
    int scrollValue;
    bool lahjoitusState;
    QString naytettavaSumma;
};

#endif // MAINWINDOW_H
