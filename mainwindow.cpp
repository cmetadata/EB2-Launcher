#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optionscontroller.cpp"
#include "pugixml.hpp"
#include "QDebug"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() ^ Qt::WindowMaximizeButtonHint);

    pugi::xml_document doc;
    QString appPath = qApp->applicationDirPath();
    doc.load_file(appPath.toLatin1() + "/EB2_launcher_config.xml");
    QString xmlVersion = doc.child("version").text().get();

    manager = new QNetworkAccessManager();

    QObject::connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply)
    {
        if (reply->error())
        {
            qDebug() << reply->errorString();
            return;
        }

        QString answer = reply->readAll();

        pugi::xml_document new_doc;
        new_doc.load_string(answer.toLatin1());

        QString onlineVersion = new_doc.child("version").text().get();

        if (onlineVersion != xmlVersion)
        {
            ui->lblUpdate->setText("Update Available!");
        }
        else
        {
            ui->lblUpdate->setText("");
        }

        qDebug() << answer;
    }
    );

    request.setUrl(QUrl("http://eb2-version-app.herokuapp.com/versions.xml"));
    manager->get(request);
    ui->lblVersion->setText("Version " + xmlVersion);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete manager;
}

void MainWindow::on_btnExit_clicked()
{
    QCoreApplication::quit();
}

void MainWindow::on_btnPlay_clicked()
{
    StartBat();
}

void MainWindow::on_btnOptions_clicked()
{
    EditOptions();
}
