#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QtDebug>

static info Info;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createConnection();
    modelSaldo =new  QSqlTableModel(this,db);
    modelTili =new QSqlTableModel(this,db);
    timer=new QTimer(this);
    serial=new RFIDSerial;
    koodiOlio =new PinCodeDLL;
    koodiOlio->dialogOlio->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    mysliOlio=new MySqlDll;
    keniaOlio =new LahjoitusDLL;
    keniaOlio->lahjoitusOlio->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    ui->tableViewSaldo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewSaldo->setFocusPolicy(Qt::NoFocus);
    ui->tableViewSaldo->setSelectionMode(QAbstractItemView::NoSelection);

    ui->tableViewTilitapahtumat->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewTilitapahtumat->setFocusPolicy(Qt::NoFocus);
    ui->tableViewTilitapahtumat->setSelectionMode(QAbstractItemView::NoSelection);

    ui->framePaavalikko->hide();
    ui->frameSaldo->hide();
    ui->frameTilitapahtumat->hide();
    ui->frameNosto->hide();
    ui->frameMuuSuma->hide();
    ui->labelLahjoitusKiitos->hide();
    ui->labelLahjoitusFailed->hide();
    ui->labelNOSTOFAILED->hide();
    ui->labelNOSTOOK->hide();
    ui->labelTunnusVaarin->hide();
    ui->labelYrityksiaJaljella_2->hide();
    ui->labelYrityksiaJaljella->hide();
    ui->labelIDLEKirjautuminen->hide();
    ui->labelKorttiLukittu->hide();

    muuSummaState=false;
    RFIDState=true;
    lahjoitusState=false;
    scrollTiliValue=0;

    connect(this,SIGNAL(nappiaPainettu()),this,SLOT(ajastin()));
    QObject::connect(serial,SIGNAL(RFIDTunnus(QString)),this,SLOT(luettuRFIDTunnus(QString)));
    QObject::connect(koodiOlio,SIGNAL(PinKoodi(QString)),this,SLOT(pinKoodiLuettu(QString)));
    QObject::connect(koodiOlio->dialogOlio,SIGNAL(peruutaPainettu()),this,SLOT(kirjauduUlos()));
    QObject::connect(keniaOlio->lahjoitusOlio,SIGNAL(okPainettu(QString)),this,SLOT(vastaanOtaLahjoitus(QString)));
    QObject::connect(keniaOlio->lahjoitusOlio,SIGNAL(peruutaPainettu()),this,SLOT(poisLahjoituksesta()));

}
void MainWindow::idleUlostuminen()
{
    ui->labelIDLEKirjautuminen->show();
}
void MainWindow::vastaanOtaLahjoitus(QString x)
{
    lahjoituksenMaara=x;
    if(Info.AccountInfo[2].toInt() >= lahjoituksenMaara)
    {
        mysliOlio->ExchangeMoney(&db, Info.AccountInfo[0].toInt(), "FI00001234001234", "Punainen risti", lahjoituksenMaara.toInt());
        Info = mysliOlio->GetAllInformation(&db, Info.CardInfo[2]);

        ui->labelLahjoitusKiitos->show();
        ui->labelLahjoitusFailed->hide();
        keniaOlio->ikkunaKiinni();
        lahjoitusState=false;
    }
    else
    {
        ui->labelLahjoitusKiitos->hide();
        ui->labelLahjoitusFailed->show();
        lahjoitusState=false;
        keniaOlio->ikkunaKiinni();
    }
}
void MainWindow::poisLahjoituksesta()
{
    lahjoitusState=false;
    keniaOlio->ikkunaKiinni();
}
void MainWindow::createConnection()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("mysli.oamk.fi");
    db.setUserName("m8leto00");
    db.setPassword("pesuharja");
    db.setDatabaseName("opisk_m8leto00");
    bool ok = db.open();

    if(ok)
    {
        db.exec("use opisk_m8leto00;");
    }
}
void MainWindow::luettuRFIDTunnus(QString x)
{
    if(RFIDState==true)
    {
            RFIDTunnus=x;
            Info = mysliOlio->GetAllInformation(&db, RFIDTunnus);
            if(Info.CardInfo[4] == "0")
            {
                //QString LockCardQuery = "update Kortit set Lukittu = 0 where Kortin_ID = " + Info.CardInfo[0];
                //QSqlQuery Result = mysliOlio->RunQuery(&db,LockCardQuery);

                kortinTilanne=0;
                Info = mysliOlio->GetAllInformation(&db, RFIDTunnus);
                koodiOlio->ikkunaTakas();
                ui->labelIDLEKirjautuminen->hide();
            }
            else
            {
                ui->labelKorttiLukittu->show();
                ui->labelIDLEKirjautuminen->hide();
            }
    }
}
void MainWindow::pinKoodiLuettu(QString x)
{
    kayttajanPinKoodi=x;
    koodiOlio->ikkunaPiiloon();
    kirjauduSisaan();
}
void MainWindow::ajastin()
{
    timer->start(40000);
    connect(timer, SIGNAL(timeout()),this,SLOT(kirjauduUlos()));
    connect(timer, SIGNAL(timeout()),this,SLOT(idleUlostuminen()));
}
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
    {
        emit nappiaPainettu();
    }
}
void MainWindow::kirjauduUlos()
{
    ui->frameNosto->hide();
    ui->frameSaldo->hide();
    ui->frameMuuSuma->hide();
    ui->frameTilitapahtumat->hide();
    ui->framePaavalikko->hide();
    ui->frameAloitus->show();
    ui->labelYrityksiaJaljella->hide();
    ui->labelYrityksiaJaljella_2->hide();
    ui->labelTunnusVaarin->hide();
    ui->labelIDLEKirjautuminen->show();
    ui->labelIDLEKirjautuminen->hide();
    ui->labelKorttiLukittu->hide();
    RFIDState=true;
    koodiOlio->ikkunaPiiloon();

}
void MainWindow::getValuesFromModelToSaldo()
{
    modelSaldo->setTable("Tilitapahtumat");
    int temp =mysliOlio->GetCount(&db, Info.AccountInfo[0], 5);
    qDebug()<<"Temp: "<< temp;
    modelSaldo->setFilter("Tili_ID = " + Info.AccountInfo[0] + " AND Tilitapahtuma_ID > " + QString::number(temp));
    modelSaldo->setSort(0,Qt::DescendingOrder);
    modelSaldo->select();
    ui->tableViewSaldo->setModel(modelSaldo);
    ui->tableViewSaldo->hideColumn(0);
    ui->tableViewSaldo->hideColumn(1);

}
void MainWindow::getValuesFromModelToTili()
{

    modelTili->setTable("Tilitapahtumat");
    int temp = mysliOlio->GetCount(&db, Info.AccountInfo[0], naytettavaMaaraTili);
    modelTili->setFilter("Tili_ID = " + Info.AccountInfo[0] + " AND Tilitapahtuma_ID > " + QString::number(temp));
    modelTili->setSort(0,Qt::DescendingOrder);
    modelTili->select();
    ui->tableViewTilitapahtumat->setModel(modelTili);
    ui->tableViewTilitapahtumat->hideColumn(0);
    ui->tableViewTilitapahtumat->hideColumn(1);

}
MainWindow::~MainWindow()
{
    delete ui;
    delete modelSaldo;
    delete modelTili;
    delete timer;
    delete serial;
    delete koodiOlio;
    delete mysliOlio;
    delete keniaOlio;
}
void MainWindow::haeTiedot()
{
    saldo.setNum(TilinSaldo.toInt());
    QString valuutta="€";
    saldo.append(valuutta);

}
void MainWindow::kirjauduSisaan()
{
        pinCodeResult Result = mysliOlio->TryPINCode(&db, RFIDTunnus, kayttajanPinKoodi);
        if(Result.PinResult)
        {
            kayttajaNimi= Info.CustomerInfo[1]; //haeNimi(&db,RFIDTunnus);
            TilinSaldo = Info.AccountInfo[2];
            tilinumero= Info.AccountInfo[3];
            ui->frameAloitus->hide();
            ui->framePaavalikko->show();
            RFIDState=false;
            haeTiedot();

            ui->labelPaavalikkoKayttajaNimi->setText(kayttajaNimi);
            ui->labelSaldoKayttajaNimi->setText(kayttajaNimi);
            ui->labelTilinSaldo->setText(saldo);
            ui->labelTilinumero->setText(tilinumero);
            ui->labelSaldoTillaRahaa->setText(saldo);
            kortinTilanne=0;
            ui->labelTunnusVaarin->hide();
            ui->labelYrityksiaJaljella->hide();
            ui->labelYrityksiaJaljella_2->hide();
            kayttajanPinKoodi="0";
            emit nappiaPainettu();
            ui->labelIDLEKirjautuminen->hide();
        }
        else
        {
            kortinTilanne++;
            int tilanne = 5-kortinTilanne;
            ui->labelTunnusVaarin->show();
            ui->labelYrityksiaJaljella_2->show();
            ui->labelYrityksiaJaljella->show();
            ui->labelYrityksiaJaljella->setNum(tilanne);
            ui->labelIDLEKirjautuminen->hide();
            emit nappiaPainettu();
            RFIDState=true;
            koodiOlio->dialogOlio->show();
        }
        if(kortinTilanne>5)
        {
            koodiOlio->ikkunaPiiloon();
            ui->labelKorttiLukittu->show();
            ui->labelIDLEKirjautuminen->hide();
            ui->labelYrityksiaJaljella->hide();
            ui->labelYrityksiaJaljella_2->hide();
            emit nappiaPainettu();
            QString LockCardQuery = "update Kortit set Lukittu = 1 where Kortin_ID = " + Info.CardInfo[0];
            QSqlQuery Result = mysliOlio->RunQuery(&db,LockCardQuery);
        }
}

void MainWindow::on_pushButtonSaldo_clicked()
{
    if(lahjoitusState==false)
    {
        Info = mysliOlio->GetAllInformation(&db, RFIDTunnus);
        ui->framePaavalikko->hide();
        ui->frameSaldo->show();
        haeTiedot();
        getValuesFromModelToSaldo();
        TilinSaldo = Info.AccountInfo[2];
        ui->labelTilinSaldo->setText(TilinSaldo.append("€"));
        ui->labelLahjoitusKiitos->hide();
        ui->labelLahjoitusFailed->hide();
        emit nappiaPainettu();
    }
}
void MainWindow::on_pushButtonTilitapahtumat_clicked()
{
    if(lahjoitusState==false)
    {
        naytettavaMaaraTili = 10;
        scrollValue=4;
        ui->framePaavalikko->hide();
        ui->frameTilitapahtumat->show();
        haeTiedot();
        getValuesFromModelToTili();
        emit nappiaPainettu();
        ui->labelNayta10->show();
        ui->labelNayta20->hide();
        ui->labelNayta30->hide();
        ui->labelLahjoitusKiitos->hide();
        ui->labelLahjoitusFailed->hide();
        scrollTiliValue=0;
    }
}
void MainWindow::on_pushButtonNosto_clicked()
{
    if(lahjoitusState==false)
    {
        haeTiedot();
        ui->framePaavalikko->hide();
        ui->frameNosto->show();
        ui->labelLahjoitusKiitos->hide();
        ui->labelLahjoitusFailed->hide();
        haluttuNostonMaara=0;
        emit nappiaPainettu();
    }
}
void MainWindow::on_pushButtonKirjauduUlos_clicked()
{
    if(lahjoitusState==false)
    {
        ui->framePaavalikko->hide();
        ui->frameAloitus->show();
        ui->labelLahjoitusKiitos->hide();
        ui->labelLahjoitusFailed->hide();
        kirjauduUlos();
        emit nappiaPainettu();
    }
}
void MainWindow::on_pushButtonPaavalikkoonSaldosta_clicked()
{
    ui->frameSaldo->hide();
    ui->framePaavalikko->show();
    emit nappiaPainettu();
}
void MainWindow::on_pushButtonPaavalikkoonTilitapahtumista_clicked()
{
    ui->frameTilitapahtumat->hide();
    ui->framePaavalikko->show();
    emit nappiaPainettu();
}
void MainWindow::on_pushButtonPaavalikkoonNostosta_clicked()
{
    if(muuSummaState==false)
    {
    ui->frameNosto->hide();
    ui->framePaavalikko->show();
    ui->labelNOSTOOK->hide();
    ui->labelNOSTOFAILED->hide();
    ui->labelTESTISumma->hide();
    emit nappiaPainettu();
    }
}
void MainWindow::on_pushButtonNum1_clicked()
{
    ui->lineEditHaluttuSumma->insert("1");
    emit nappiaPainettu();
}
void MainWindow::on_pushButtonNum2_clicked()
{
    ui->lineEditHaluttuSumma->insert("2");
    emit nappiaPainettu();
}
void MainWindow::on_pushButtonNum3_clicked()
{
    ui->lineEditHaluttuSumma->insert("3");
    emit nappiaPainettu();
}
void MainWindow::on_pushButtonNum4_clicked()
{
    ui->lineEditHaluttuSumma->insert("4");
    emit nappiaPainettu();
}
void MainWindow::on_pushButtonNum5_clicked()
{
    ui->lineEditHaluttuSumma->insert("5");
    emit nappiaPainettu();
}
void MainWindow::on_pushButtonNum6_clicked()
{
    ui->lineEditHaluttuSumma->insert("6");
    emit nappiaPainettu();
}
void MainWindow::on_pushButtonNum7_clicked()
{
    ui->lineEditHaluttuSumma->insert("7");
    emit nappiaPainettu();
}
void MainWindow::on_pushButtonNum8_clicked()
{
    ui->lineEditHaluttuSumma->insert("8");
    emit nappiaPainettu();
}
void MainWindow::on_pushButtonNum9_clicked()
{
    ui->lineEditHaluttuSumma->insert("9");
    emit nappiaPainettu();
}
void MainWindow::on_pushButtonNum0_clicked()
{
    ui->lineEditHaluttuSumma->insert("0");
    emit nappiaPainettu();
}
void MainWindow::on_pushButtonNumBack_clicked()
{
    ui->lineEditHaluttuSumma->backspace();
    emit nappiaPainettu();
}
void MainWindow::on_pushButtonNumPoistu_clicked()
{
    ui->frameMuuSuma->hide();
    muuSummaState=false;
    emit nappiaPainettu();
}
void MainWindow::on_pushButtonMuuSumma_clicked()
{
    ui->frameMuuSuma->show();
    muuSummaState=true;
    ui->labelNOSTOOK->hide();
    ui->labelNOSTOFAILED->hide();
    ui->labelTESTISumma->hide();
    emit nappiaPainettu();
}
void MainWindow::on_pushButtonLahjoitus_clicked()
{
    if(lahjoitusState==false)
    {
        keniaOlio->ikkunaAuki();
        lahjoitusState=true;
        ui->labelLahjoitusFailed->hide();
        ui->labelLahjoitusKiitos->hide();
        emit nappiaPainettu();
    }
}
void MainWindow::on_pushButton10e_clicked()
{
    if(muuSummaState==false)
    {
        haluttuNostonMaara=10;
        naytettavaSumma.setNum(haluttuNostonMaara);
        naytettavaSumma.append("€?");
        ui->labelTESTISumma->setText(naytettavaSumma);
        ui->labelNOSTOOK->hide();
        ui->labelNOSTOFAILED->hide();
        ui->labelTESTISumma->show();
        emit nappiaPainettu();
    }
}
void MainWindow::on_pushButton20e_clicked()
{
    if(muuSummaState==false)
    {
        haluttuNostonMaara=20;
        naytettavaSumma.setNum(haluttuNostonMaara);
        naytettavaSumma.append("€?");
        ui->labelTESTISumma->setText(naytettavaSumma);
        ui->labelNOSTOOK->hide();
        ui->labelNOSTOFAILED->hide();
        ui->labelTESTISumma->show();
        emit nappiaPainettu();
    }
}
void MainWindow::on_pushButton40e_clicked()
{
    if(muuSummaState==false)
    {
        haluttuNostonMaara=40;
        naytettavaSumma.setNum(haluttuNostonMaara);
        naytettavaSumma.append("€?");
        ui->labelTESTISumma->setText(naytettavaSumma);
        ui->labelNOSTOOK->hide();
        ui->labelNOSTOFAILED->hide();
        ui->labelTESTISumma->show();
        emit nappiaPainettu();
    }
}
void MainWindow::on_pushButton50e_clicked()
{
    if(muuSummaState==false)
    {
        haluttuNostonMaara=50;
        naytettavaSumma.setNum(haluttuNostonMaara);
        naytettavaSumma.append("€?");
        ui->labelTESTISumma->setText(naytettavaSumma);
        ui->labelNOSTOOK->hide();
        ui->labelNOSTOFAILED->hide();
        ui->labelTESTISumma->show();
        emit nappiaPainettu();
    }
}
void MainWindow::on_pushButton60e_clicked()
{
    if(muuSummaState==false)
    {
        haluttuNostonMaara=60;
        naytettavaSumma.setNum(haluttuNostonMaara);
        naytettavaSumma.append("€?");
        ui->labelTESTISumma->setText(naytettavaSumma);
        ui->labelNOSTOOK->hide();
        ui->labelNOSTOFAILED->hide();
        ui->labelTESTISumma->show();
        emit nappiaPainettu();
    }
}
void MainWindow::on_pushButton80e_clicked()
{
    if(muuSummaState==false)
    {
        haluttuNostonMaara=80;
        naytettavaSumma.setNum(haluttuNostonMaara);
        naytettavaSumma.append("€?");
        ui->labelTESTISumma->setText(naytettavaSumma);
        ui->labelNOSTOOK->hide();
        ui->labelNOSTOFAILED->hide();
        ui->labelTESTISumma->show();
        emit nappiaPainettu();
    }
}
void MainWindow::on_pushButton90e_clicked()
{
    if(muuSummaState==false)
    {
        haluttuNostonMaara=90;
        naytettavaSumma.setNum(haluttuNostonMaara);
        naytettavaSumma.append("€?");
        ui->labelTESTISumma->setText(naytettavaSumma);
        ui->labelNOSTOOK->hide();
        ui->labelNOSTOFAILED->hide();
        ui->labelTESTISumma->show();
        emit nappiaPainettu();
    }
}
void MainWindow::on_pushButton100e_clicked()
{
    if(muuSummaState==false)
    {
        haluttuNostonMaara=100;
        naytettavaSumma.setNum(haluttuNostonMaara);
        naytettavaSumma.append("€?");
        ui->labelTESTISumma->setText(naytettavaSumma);
        ui->labelNOSTOOK->hide();
        ui->labelNOSTOFAILED->hide();
        ui->labelTESTISumma->show();
        emit nappiaPainettu();
    }
}
void MainWindow::on_pushButtonTESTINosto_clicked()
{
    if(muuSummaState==false && haluttuNostonMaara>0)
    {

        if(Info.AccountInfo[2].toInt() >= haluttuNostonMaara)
        {
            ui->labelNOSTOFAILED->hide();
            ui->labelNOSTOOK->show();

            mysliOlio->Withdrawal(&db, Info.AccountInfo[3],"Withdrawal", QString::number(haluttuNostonMaara));
            Info = mysliOlio->GetAllInformation(&db, Info.CardInfo[2]);
            TilinSaldo = Info.AccountInfo[2];
            ui->labelSaldoTillaRahaa->setText(TilinSaldo.append("€"));
            haluttuNostonMaara=0;

            emit nappiaPainettu();
        }
        else
        {
            ui->labelNOSTOFAILED->show();
            ui->labelNOSTOOK->hide();
            emit nappiaPainettu();
        }
    }
}
void MainWindow::on_pushButtonNumOK_clicked()
{
    muuSummaPyoristus= ui->lineEditHaluttuSumma->text().toInt();
    haluttuNostonMaara= 10*((muuSummaPyoristus+5)/10);
    naytettavaSumma.setNum(haluttuNostonMaara);
    naytettavaSumma.append("€?");
    ui->labelTESTISumma->setText(naytettavaSumma);
    ui->frameMuuSuma->hide();
    ui->lineEditHaluttuSumma->setText("");
    muuSummaState=false;
    ui->labelTESTISumma->show();
    emit nappiaPainettu();
}

void MainWindow::on_pushButtonScrollDown_clicked()
{
    if(scrollTiliValue<=scrollValue)
    {
    scrollTiliValue+=1;
    ui->tableViewTilitapahtumat->verticalScrollBar()->setValue(scrollTiliValue);
    emit nappiaPainettu();
    }
}
void MainWindow::on_pushButtonScrollUP_clicked()
{
    if(scrollTiliValue>=1)
    {
    scrollTiliValue-=1;
    ui->tableViewTilitapahtumat->verticalScrollBar()->setValue(scrollTiliValue);
    emit nappiaPainettu();
    }
}
void MainWindow::on_pushButtonTiliNayta10_clicked()
{
    naytettavaMaaraTili=10;
    scrollValue=4;
    getValuesFromModelToTili();
    ui->labelNayta10->show();
    ui->labelNayta20->hide();
    ui->labelNayta30->hide();
}

void MainWindow::on_pushButtonTiliNayta20_clicked()
{
    naytettavaMaaraTili=20;
    scrollValue=14;
    getValuesFromModelToTili();
    ui->labelNayta10->hide();
    ui->labelNayta20->show();
    ui->labelNayta30->hide();
}

void MainWindow::on_pushButtonTiliNayta30_clicked()
{
    naytettavaMaaraTili=30;
    scrollValue=24;
    getValuesFromModelToTili();
    ui->labelNayta10->hide();
    ui->labelNayta20->hide();
    ui->labelNayta30->show();
}
