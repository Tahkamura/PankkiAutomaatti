#ifndef LAHJOITUSDIALOG_H
#define LAHJOITUSDIALOG_H

#include <QDialog>

namespace Ui {
class lahjoitusDialog;
}

class lahjoitusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit lahjoitusDialog(QWidget *parent = nullptr);
    ~lahjoitusDialog();

private slots:
    void on_pushButton2_clicked();
    void on_pushButton5_clicked();
    void on_pushButton10_clicked();
    void on_pushButton20_clicked();
    void on_pushButton50_clicked();
    void on_pushButtonOK_clicked();
    void on_pushButtonPoistu_clicked();

signals:
    void okPainettu(QString);
    void peruutaPainettu();

private:
    Ui::lahjoitusDialog *ui;
    QString lahjoituksenMaara;
    QString naytettavaLahjoitus;
};

#endif // LAHJOITUSDIALOG_H
