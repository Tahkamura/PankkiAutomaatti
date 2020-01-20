#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QObject>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
signals:
    void valmistaSaatana(QString);
    void peruutaPainettu();

private slots:
    void on_pushButtonNum1_clicked();
    void on_pushButtonNum2_clicked();
    void on_pushButtonNum3_clicked();
    void on_pushButtonNumOk_clicked();
    void on_pushButtonNum4_clicked();
    void on_pushButtonNum5_clicked();
    void on_pushButtonNum6_clicked();
    void on_pushButtonNum7_clicked();
    void on_pushButtonNum8_clicked();
    void on_pushButtonNum9_clicked();
    void on_pushButtonNum0_clicked();
    void on_pushButtonNumBack_clicked();
    void on_pushButtonNumClose_clicked();

private:
    Ui::Dialog *ui;
    QString pinKoodi;
};

#endif // DIALOG_H
