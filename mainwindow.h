#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include<QPixmap>
//#include<QPalette>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QString>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QEvent>
#include <QKeyEvent>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
  //  bool eventFilter(QObject *parent, QEvent *event);
    void keyPressEvent(QKeyEvent* event);
    QSqlDatabase db;
    QString kodeKabkot;

private:
    Ui::MainWindow *ui;

    void showNamaMesjid();
       void showHadis();
       void showWaktu();
       void showAgenda();
       void showJadwalSholat();
       QString labelMesjid;
       QString labelHadis;
       QString labelWaktu;
       QString labelAgenda;
       QString labelImsyak;
       QString labelShubuh;
       QString labelTerbit;
       QString labelDhuha;
       QString labelDhuhur;
       QString labelAshar;
       QString labelMaghrib;
       QString labelIsya;
       int urutanAgenda = 0;
};
#endif // MAINWINDOW_H
