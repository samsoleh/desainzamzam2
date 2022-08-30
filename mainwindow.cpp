#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1920, 1080);
    QMainWindow::showFullScreen();
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
         db.setHostName("localhost");
         db.setDatabaseName("jamazan");
         db.setUserName("eddy");
         db.setPassword("123456");

       if (!db.open())
       {
           qDebug() << "Can't Connect to DB !";

       }
       else {
            qDebug() << "Database connected !";
       }
  //  QPixmap bkgnd("assets/background.png");
  //  bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
 //   QPalette palette;
 //   palette.setBrush(QPalette::Background, bkgnd);
 //  this->setPalette(palette);

       QTimer *timerNamaMesjid = new QTimer(this);
       connect(timerNamaMesjid, &QTimer::timeout, this, &MainWindow::showNamaMesjid);
       timerNamaMesjid->start(1000);
       showNamaMesjid();

       QTimer *timerHadis = new QTimer(this);
       connect(timerHadis, &QTimer::timeout, this, &MainWindow::showHadis);
       timerHadis->start(5000);
       showHadis();

       QTimer *timerWaktu = new QTimer(this);
       connect(timerWaktu, &QTimer::timeout, this, &MainWindow::showWaktu);
       timerWaktu->start(1000);
       showWaktu();

       QTimer *timerJadwalSholat = new QTimer(this);
       connect(timerJadwalSholat, &QTimer::timeout, this, &MainWindow::showJadwalSholat);
      timerJadwalSholat->start(1000);
      showJadwalSholat();

      QTimer *timerAgenda = new QTimer(this);
      connect(timerAgenda, &QTimer::timeout, this, &MainWindow::showAgenda);
      timerAgenda->start(1000);
      showAgenda();

}

void MainWindow::showNamaMesjid()
{
    db.open();
    QSqlQuery query;
    query.prepare("SELECT a.*,b.kabkoNama from data_mesjid a LEFT JOIN kabkot b ON(a.kode_kabkota = b.kabkoKode) where id = 1 ");
    if(!query.exec())
    {
          qDebug() << "Query nama mesjid gagal";
    }
    else
    {
          while(query.next())
           {
                 labelMesjid = query.value(1).toString();
                 labelMesjid += "\n";
                 labelMesjid += query.value(2).toString();
                 labelMesjid += "\n";
                 labelMesjid += query.value(5).toString();
                 ui->labelNamaMesjid->setText(labelMesjid);
                 kodeKabkot = query.value(3).toString();
            }
    }


 }

void MainWindow::showHadis()
{
    QDate cd = QDate::currentDate();
    QString bulan = cd.toString("M");
    db.open();
    QSqlQuery queryHadis;
    //QString sqlHadis = "select * from data_hadis where bulan ="+bulan+" ORDER BY rand() limit 1 ";
    QString sqlHadis = "select * from data_hadis where aktif = 1 ORDER BY rand() limit 1 ";
   //qDebug() << sqlHadis;
    queryHadis.prepare(sqlHadis);
    if(!queryHadis.exec())
    {
          qDebug() << "Query hadis gagal";
    }
    else
    {
          while(queryHadis.next())
           {
                 labelHadis = queryHadis.value(1).toString();
                 labelHadis += "\n";
                 labelHadis += queryHadis.value(2).toString();

            }
           ui->labelNamaQuranHadis->setText(labelHadis);
    }


 }

void MainWindow::showWaktu()
{
    //waktu
    QTime time = QTime::currentTime();
    QString jam = time.toString("hh");
    QString menit = time.toString("mm");
    labelWaktu = jam+" : "+menit;
 //   labelWaktu += "\n";

    //tanggal
    QDate cd = QDate::currentDate();
    QString hari = cd.toString("dddd");

    QString tanggal = cd.toString("dd MMMM yyyy");

    QString haritanggal = hari + ",  " + tanggal;

    QString tgl_masehi = cd.toString("dd");
    QString bln_masehi = cd.toString("MM");
    QString thn_masehi = cd.toString("yyyy");
    QString tanggal_hijriyah;
    db.open();
    QSqlQuery queryTanggalHijriyah;
    QString sqlTanggalHijriyah = "select a.tanggal_hijriyah,  (select nama from bulan_hijriyah where id = a.bulan_hijriyah) as bulan_hijriyah, a.tahun_hijriyah from tanggal_hijriyah a where 1=1 ";
            sqlTanggalHijriyah += " and a.tanggal_masehi = '"+tgl_masehi+"' ";
             sqlTanggalHijriyah += " and a.bulan_masehi = '"+bln_masehi+"' ";
            sqlTanggalHijriyah += " and a.tahun_masehi = '"+thn_masehi+"' ";

   // qDebug() << sqlTanggalHijriyah;
    queryTanggalHijriyah.prepare(sqlTanggalHijriyah);
    if(!queryTanggalHijriyah.exec())
    {
          qDebug() << "Query tanggal hijriyah gagal";
    }
    else
    {
           while(queryTanggalHijriyah.next())
           {
                 tanggal_hijriyah = queryTanggalHijriyah.value(0).toString();
                 tanggal_hijriyah += " ";
                 tanggal_hijriyah += queryTanggalHijriyah.value(1).toString();
                 tanggal_hijriyah += " ";
                 tanggal_hijriyah += queryTanggalHijriyah.value(2).toString();

            }
    }

    ui->labelJamSekarang->setText(labelWaktu);
    ui->labelHari->setText(haritanggal);
    ui->labelTanggal->setText(tanggal_hijriyah);






}

void MainWindow::showJadwalSholat()
{
    int imsyakJam;
    int imsyakMenit;
    int subuhJam;
    int subuhMenit;
    int terbitJam;
    int terbitMenit;
    int dhuhurJam;
    int dhuhurMenit;
    int asharJam;
    int asharMenit;
    int maghribJam;
    int maghribMenit;
    int isyaJam;
    int isyaMenit;
    int jamSekarang;
    int menitSekarang;
    int detikSekarang;
    QString narasiMenujuSholat = "";
    QString labelBedaJam = "";
    QString labelBedaMenit = "";
    QString labelBedaDetik = "";

    //waktu
    QDateTime jd = QDateTime::currentDateTime();
    QString tglJadwal = jd.toString("dd");
    QString blnJadwal = jd.toString("MM");
    QString thnJadwal = jd.toString("yyyy");
    QString jamJadwal = jd.toString("HH");
    QString menitJadwal = jd.toString("mm");
    QString detikJadwal = jd.toString("ss");

    jamSekarang = jamJadwal.toInt();
    menitSekarang = menitJadwal.toInt();
    detikSekarang = detikJadwal.toInt();
    int bedaJamTeks = 0;
    int bedaMenitTeks = 0;
    int bedaDetik = 0;

    db.open();
    QSqlQuery queryJadwalSholat;
    //QString sqlJadwalSholat = "select a.* from jadwal_sholat a where a.kode_kabkot = (select kode_kabkot from data_mesjid where id = 1)";
    QString sqlJadwalSholat = "select a.* from jadwal_sholat a where 1=1 ";
            sqlJadwalSholat += " and a.kode_kabkot = '"+kodeKabkot+"' ";
             sqlJadwalSholat += " and trim(a.tgl) = '"+tglJadwal+"' ";
            sqlJadwalSholat += " and trim(a.bln) = '"+blnJadwal+"' ";
           sqlJadwalSholat += " and trim(a.thn) = '"+thnJadwal+"' ";
   // qDebug() << sqlJadwalSholat;
    queryJadwalSholat.prepare(sqlJadwalSholat);
    if(!queryJadwalSholat.exec())
    {
          qDebug() << "Query jadwal sholat gagal";
    }
    else
    {
           while(queryJadwalSholat.next())
           {
               //DATA IMSYAK
                 labelImsyak = queryJadwalSholat.value(6).toString();
                 labelImsyak += " : ";
                 labelImsyak += queryJadwalSholat.value(7).toString();
                 imsyakJam =  queryJadwalSholat.value(6).toString().toInt();
                 imsyakMenit = queryJadwalSholat.value(7).toString().toInt();

               //DATA SHUBUH
                 labelShubuh = queryJadwalSholat.value(8).toString();
                 labelShubuh += " : ";
                 labelShubuh += queryJadwalSholat.value(9).toString();
                 subuhJam  = queryJadwalSholat.value(8).toString().toInt();
                 subuhMenit = queryJadwalSholat.value(9).toString().toInt();

               //DATA TERBIT
                 labelTerbit = queryJadwalSholat.value(10).toString();
                 labelTerbit += " : ";
                 labelTerbit += queryJadwalSholat.value(11).toString();
                 terbitJam = queryJadwalSholat.value(10).toString().toInt();
                 terbitMenit = queryJadwalSholat.value(11).toString().toInt();

               //DATA DHUHUR
                 labelDhuhur = queryJadwalSholat.value(14).toString();
                 labelDhuhur += " : ";
                 labelDhuhur += queryJadwalSholat.value(15).toString();
                 dhuhurJam = queryJadwalSholat.value(14).toString().toInt();
                 dhuhurMenit = queryJadwalSholat.value(15).toString().toInt();

               //DATA ASHAR
                 labelAshar = queryJadwalSholat.value(16).toString();
                 labelAshar += " : ";
                 labelAshar += queryJadwalSholat.value(17).toString();
                 asharJam = queryJadwalSholat.value(16).toString().toInt();
                 asharMenit = queryJadwalSholat.value(17).toString().toInt();

               //DATA MAGHRIB
                   labelMaghrib = queryJadwalSholat.value(18).toString();
                   labelMaghrib += " : ";
                   labelMaghrib += queryJadwalSholat.value(19).toString();
                   maghribJam = queryJadwalSholat.value(18).toString().toInt();
                   maghribMenit = queryJadwalSholat.value(19).toString().toInt();

              //DATA ISYA
                   labelIsya = queryJadwalSholat.value(20).toString();
                   labelIsya += " : ";
                   labelIsya += queryJadwalSholat.value(21).toString();
                   isyaJam = queryJadwalSholat.value(20).toString().toInt();
                   isyaMenit = queryJadwalSholat.value(21).toString().toInt();

            }

       if(jamSekarang <= isyaJam)
           {
               if(jamSekarang == isyaJam && menitSekarang >= isyaMenit)
               {
                   narasiMenujuSholat = "Waktu Isya";
               }
               else
               {
                   int bedaJam = (isyaJam - jamSekarang)*60;
                   int bedaMenit = isyaMenit - menitSekarang;
                   int jmlBedaMenit = bedaJam + bedaMenit;

                    if((jmlBedaMenit % 60) == 0)
                    {
                        bedaJamTeks = std::floor(jmlBedaMenit/60)-1;
                        bedaMenitTeks = (60-1);
                        bedaDetik = 60-detikSekarang;
                    }
                    else
                    {
                       if(std::floor(jmlBedaMenit / 60) > 0)
                       {
                           bedaJamTeks = std::floor(jmlBedaMenit/60);
                       }
                       else
                       {
                           bedaJamTeks = 0;
                       }

                       bedaMenitTeks = ((jmlBedaMenit % 60)-1);
                       bedaDetik = 60-detikSekarang;
                    }

                   labelBedaJam.setNum(bedaJamTeks);
                   labelBedaMenit.setNum(bedaMenitTeks);
                   labelBedaDetik.setNum(bedaDetik);
                  narasiMenujuSholat = "Isya     -"+labelBedaJam+":"+labelBedaMenit+":"+labelBedaDetik;
               }
           }

       if(jamSekarang <= maghribJam)
           {
               if(jamSekarang == maghribJam && menitSekarang >= maghribMenit)
               {
                   narasiMenujuSholat = "Waktu Maghrib";
               }
               else
               {
                   int bedaJam = (maghribJam - jamSekarang)*60;
                   int bedaMenit = maghribMenit - menitSekarang;
                   int jmlBedaMenit = bedaJam + bedaMenit;
                    if((jmlBedaMenit % 60) == 0)
                    {
                        bedaJamTeks = std::floor(jmlBedaMenit/60)-1;
                        bedaMenitTeks = (60-1);
                        bedaDetik = 60-detikSekarang;
                    }
                    else
                    {
                       if(std::floor(jmlBedaMenit / 60) > 0)
                       {
                           bedaJamTeks = std::floor(jmlBedaMenit/60);
                       }
                       else
                       {
                           bedaJamTeks = 0;
                       }

                       bedaMenitTeks = ((jmlBedaMenit % 60)-1);
                       bedaDetik = 60-detikSekarang;
                    }

                   labelBedaJam.setNum(bedaJamTeks);
                   labelBedaMenit.setNum(bedaMenitTeks);
                   labelBedaDetik.setNum(bedaDetik);
                  narasiMenujuSholat = "Maghrib     -"+labelBedaJam+":"+labelBedaMenit+":"+labelBedaDetik;
               }
           }

       if(jamSekarang <= asharJam)
           {
               if(jamSekarang == asharJam && menitSekarang >= asharMenit)
               {
                   narasiMenujuSholat = "Waktu Ashar";
               }
               else
               {
                   int bedaJam = (asharJam - jamSekarang)*60;
                   int bedaMenit = asharMenit - menitSekarang;
                   int jmlBedaMenit = bedaJam + bedaMenit;

                    if((jmlBedaMenit % 60) == 0)
                    {
                        bedaJamTeks = std::floor(jmlBedaMenit/60)-1;
                        bedaMenitTeks = (60-1);
                        bedaDetik = 60-detikSekarang;
                    }
                    else
                    {
                       if(std::floor(jmlBedaMenit / 60) > 0)
                       {
                           bedaJamTeks = std::floor(jmlBedaMenit/60);
                       }
                       else
                       {
                           bedaJamTeks = 0;
                       }

                       bedaMenitTeks = ((jmlBedaMenit % 60)-1);
                       bedaDetik = 60-detikSekarang;
                    }

                   labelBedaJam.setNum(bedaJamTeks);
                   labelBedaMenit.setNum(bedaMenitTeks);
                   labelBedaDetik.setNum(bedaDetik);
                  narasiMenujuSholat = "Ashar     -"+labelBedaJam+":"+labelBedaMenit+":"+labelBedaDetik;
               }
           }

        if(jamSekarang <= dhuhurJam)
           {
               if(jamSekarang == dhuhurJam && menitSekarang >= dhuhurMenit)
               {
                   narasiMenujuSholat = "Waktu Dhuhur";
               }
               else
               {
                   int bedaJam = (dhuhurJam - jamSekarang)*60;
                   int bedaMenit = dhuhurMenit - menitSekarang;
                   int jmlBedaMenit = bedaJam + bedaMenit;

                    if((jmlBedaMenit % 60) == 0)
                    {
                        bedaJamTeks = std::floor(jmlBedaMenit/60)-1;
                        bedaMenitTeks = (60-1);
                        bedaDetik = 60-detikSekarang;
                    }
                    else
                    {
                       if(std::floor(jmlBedaMenit / 60) > 0)
                       {
                           bedaJamTeks = std::floor(jmlBedaMenit/60);
                       }
                       else
                       {
                           bedaJamTeks = 0;
                       }

                       bedaMenitTeks = ((jmlBedaMenit % 60)-1);
                       bedaDetik = 60-detikSekarang;
                    }

                   labelBedaJam.setNum(bedaJamTeks);
                   labelBedaMenit.setNum(bedaMenitTeks);
                   labelBedaDetik.setNum(bedaDetik);
                  narasiMenujuSholat = "Dhuhur     -"+labelBedaJam+":"+labelBedaMenit+":"+labelBedaDetik;
               }
           }

        if(jamSekarang <= terbitJam)
           {
               if(jamSekarang == terbitJam && menitSekarang >= terbitMenit)
               {
                   narasiMenujuSholat = "Waktu Terbit";
               }
               else
               {
                   int bedaJam = (terbitJam - jamSekarang)*60;
                   int bedaMenit = terbitMenit - menitSekarang;
                   int jmlBedaMenit = bedaJam + bedaMenit;

                    if((jmlBedaMenit % 60) == 0)
                    {
                        bedaJamTeks = std::floor(jmlBedaMenit/60)-1;
                        bedaMenitTeks = (60-1);
                        bedaDetik = 60-detikSekarang;
                    }
                    else
                    {
                       if(std::floor(jmlBedaMenit / 60) > 0)
                       {
                           bedaJamTeks = std::floor(jmlBedaMenit/60);
                       }
                       else
                       {
                           bedaJamTeks = 0;
                       }

                       bedaMenitTeks = ((jmlBedaMenit % 60)-1);
                       bedaDetik = 60-detikSekarang;
                    }

                   labelBedaJam.setNum(bedaJamTeks);
                   labelBedaMenit.setNum(bedaMenitTeks);
                   labelBedaDetik.setNum(bedaDetik);
                  narasiMenujuSholat = "Terbit     -"+labelBedaJam+":"+labelBedaMenit+":"+labelBedaDetik;
               }
           }

        if(jamSekarang <= subuhJam)
           {
               if(jamSekarang == subuhJam && menitSekarang >= subuhMenit)
               {
                   narasiMenujuSholat = "Waktu Shubuh";
               }
               else
               {
                   int bedaJam = (subuhJam - jamSekarang)*60;
                   int bedaMenit = subuhMenit - menitSekarang;
                   int jmlBedaMenit = bedaJam + bedaMenit;

                    if((jmlBedaMenit % 60) == 0)
                    {
                        bedaJamTeks = std::floor(jmlBedaMenit/60)-1;
                        bedaMenitTeks = (60-1);
                        bedaDetik = 60-detikSekarang;
                    }
                    else
                    {
                       if(std::floor(jmlBedaMenit / 60) > 0)
                       {
                           bedaJamTeks = std::floor(jmlBedaMenit/60);
                       }
                       else
                       {
                           bedaJamTeks = 0;
                       }

                       bedaMenitTeks = ((jmlBedaMenit % 60)-1);
                       bedaDetik = 60-detikSekarang;
                    }

                   labelBedaJam.setNum(bedaJamTeks);
                   labelBedaMenit.setNum(bedaMenitTeks);
                   labelBedaDetik.setNum(bedaDetik);
                  narasiMenujuSholat = "Shubuh     -"+labelBedaJam+":"+labelBedaMenit+":"+labelBedaDetik;
               }
           }

        if(jamSekarang <= imsyakJam)
        {
            if(jamSekarang == imsyakJam && menitSekarang >= imsyakMenit)
            {
                narasiMenujuSholat = "Waktu Imsyak";
            }
            else
            {
                int bedaJam = (imsyakJam - jamSekarang)*60;
                int bedaMenit = imsyakMenit - menitSekarang;
                int jmlBedaMenit = bedaJam + bedaMenit;

                 if((jmlBedaMenit % 60) == 0)
                 {
                     bedaJamTeks = std::floor(jmlBedaMenit/60)-1;
                     bedaMenitTeks = (60-1);
                     bedaDetik = 60-detikSekarang;
                 }
                 else
                 {
                    if(std::floor(jmlBedaMenit / 60) > 0)
                    {
                        bedaJamTeks = std::floor(jmlBedaMenit/60);
                    }
                    else
                    {
                        bedaJamTeks = 0;
                    }

                    bedaMenitTeks = ((jmlBedaMenit % 60)-1);
                    bedaDetik = 60-detikSekarang;
                 }

                labelBedaJam.setNum(bedaJamTeks);
                labelBedaMenit.setNum(bedaMenitTeks);
                labelBedaDetik.setNum(bedaDetik);
               narasiMenujuSholat = "Imsyak     -"+labelBedaJam+":"+labelBedaMenit+":"+labelBedaDetik;
            }
        }

    }
    ui->labelRincianJadwalImsyak->setText(labelImsyak);
    ui->labelRincianJadwalShubuh->setText(labelShubuh);
    ui->labelRincianJadwalSyuruk->setText(labelTerbit);
    ui->labelRincianJadwalDhuhur->setText(labelDhuhur);
    ui->labelRincianJadwalAshar->setText(labelAshar);
    ui->labelRincianJadwalMagrib->setText(labelMaghrib);
    ui->labelRincianJadwalIsya->setText(labelIsya);

    ui->labelTeksMenujuSholat->setText(narasiMenujuSholat);
}

void MainWindow::showAgenda()
{
    QString labelAgendaProses;
    QString spasiLabelAgenda;
    int jmlString = 0;
    db.open();
    QSqlQuery queryAgenda;
    QString sqlAgenda = "select * from data_agenda where aktif = 1";
    //qDebug() << sqlAgenda;
    queryAgenda.prepare(sqlAgenda);
    if(!queryAgenda.exec())
    {
          qDebug() << "Query agenda gagal";
    }
    else
    {
        for(int i = 0;i<=150;i++)
        {
            spasiLabelAgenda += ' ';
        }
          while(queryAgenda.next())
           {
                 labelAgenda = queryAgenda.value(1).toString().trimmed();
            }
           labelAgenda = spasiLabelAgenda + labelAgenda;
          jmlString = labelAgenda.size();
          for(int i = urutanAgenda; i<jmlString;i++)
          {
              labelAgendaProses +=  labelAgenda[i];
          }

    }
    ui->labelAgendaTeks->setText(labelAgendaProses);
    urutanAgenda++;

    if(urutanAgenda == jmlString)
    {
        urutanAgenda = 0;
    }
    QString stringUrutan;
    qDebug() << "urutan agenda:"+stringUrutan.setNum(urutanAgenda);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Escape)
    {
        // QCoreApplication::quit();
         qDebug() << "esc keyboard";
         delete ui;

    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

