#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QDebug>
#include <QFile>
#include <QNetworkReply>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QTime>
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkAccessManager *manager_2 = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
    connect(manager_2, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished_2(QNetworkReply*)));
//    manager->get(QNetworkRequest(QUrl("http://example.com/")));
      manager->get(QNetworkRequest(QUrl("http://kurs4today.ru/USD")));
      manager_2->get(QNetworkRequest(QUrl("https://pogoda.yandex.ru/omsk")));
}


void MainWindow::replyFinished (QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
        {
            qDebug() << "ERROR!";
            qDebug() << reply->errorString();
        }
        else
        {   qDebug()<<"OK";
//            qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
//            qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
//            qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
//            qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//            qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
            //qDebug() << reply->readAll();

            QString answer = QString::fromUtf8(reply->readAll());

            qDebug()<<"answer"<<answer;

            int j = 0;
            QString S;
            QTime time = QTime::currentTime();
            QDate date = QDate::currentDate();
             while ((j = answer.indexOf("valcalc_kurs   =", j)) != -1) {
                 qDebug() << "Found <musor> tag at index position" << j;
                for (int i = 17;i <24 ;i++)
                 { S = S + answer[j+i];}
                 qDebug()<<S;  // 70.3067
                 ++j;
                 break;
             }

            ui->textBrowser->append(tr("Курс доллара $ на %1,%2 равен:<font color=red>%3</font> rub").arg(time.toString()).arg(date.toString()).arg(S));

            j = 0;
            QString str = date.toString("dd.MM.yyyy");
            QString res;
            qDebug()<<str;
            while ((j = answer.indexOf(str, j)) != -1) {
                qDebug() << "Found <musor> tag at index position" << j;
               for (int i = 40;i <47 ;i++)
                { res = res + answer[i+j];}
                qDebug()<<res<<"HE";  // 70.3067
                ++j;
                break;
            }
            ui->textBrowser->append("\n");
            ui->textBrowser->append(tr("Курс доллара $ на %1 равен: <font color=red>%2</font> rub").arg(str).arg(res));


            QWebPage page;
            page.mainFrame()->setHtml(answer);
            QWebElement parse = page.mainFrame()->documentElement();  // main body
            QWebElement result = parse.findFirst("Россия");

            //qDebug()<<result.toPlainText();
            qDebug()<<parse.toPlainText();
            qDebug()<<result.toPlainText();


            QFile file("D:/downloader.txt");
            file.resize(0);
            if(file.open(QFile::Append))
            {
                file.write(reply->readAll());
            }
        }
        reply->deleteLater();


}

void MainWindow::replyFinished_2(QNetworkReply *reply)
{
    QString answer = QString::fromUtf8(reply->readAll());
    QTime times = QTime::currentTime();
    QString time = times.toString("hh:mm");
    QDate date = QDate::currentDate();
    int j = 0;
    QString tempr;
    while ((j = answer.indexOf("weather__thermometer_type_now", j)) != -1) {
        qDebug() << "Found <weather> tag at index position" << j;
       for (int i = 31;i <34 ;i++)
        { tempr = tempr + answer[i+j];}
        qDebug()<<tempr<<"TEMPERATURA";  // -3
        ++j;
        break;
    }

    ui->textBrowser->append("\n");
    ui->textBrowser->append(tr("Температура на %1,%2 равна:<font color=red>%3</font> °C").arg(time).arg(date.toString()).arg(tempr));
    ui->textBrowser->append("\n");

}

MainWindow::~MainWindow()
{
    delete ui;
}
