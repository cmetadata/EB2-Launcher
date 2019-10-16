#include <QApplication>
#include <QProcess>
#include <QRegularExpression>
#include <QFile>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <optionscontroller.h>

void StartBat()
{
    QString appPath = qApp->applicationDirPath();
    QProcess process;
    process.startDetached("cmd.exe", QStringList() << "/c" << appPath + "/launch.bat");
}


void EditOptions()
{
    QString appPath = qApp->applicationDirPath();

    QFile file (appPath + "/ebii.cfg");
    QFile result(appPath + "/ebii2.cfg");


    if(file.open(QIODevice::ReadOnly) && result.open(QIODevice::WriteOnly))
    {
        QByteArray ba;
        QDataStream ds(&ba, QIODevice::ReadWrite|QIODevice::Text);

        QTextStream in(&file);


        while (!in.atEnd())
        {
            QString line = ProcessLine(in.readLine());

            ds << line.toStdString().data();
        }


        ba.replace('\0', ' '); // null char
        ba.replace('\1', ' '); // start of heading
        ba.replace('\2', ' '); // start of text
        ba.replace('\3', ' '); // end of text
        ba.replace('\4', ' '); // end of transmission
        ba.replace('\5', ' '); // enquiry
        ba.replace('\6', ' '); // acknowledge


        ba.replace('\a', ' '); // audible bell
        ba.replace('\b', ' '); // backspace
        ba.replace('\t', ' '); // tab control
        ba.replace('\v', ' '); // vertical tab
        ba.replace('\f', ' '); // form feed

        ba.replace('\x0e', ' '); // shift out
        ba.replace('\x0f', ' '); // shift in
        ba.replace('\x10', ' '); // data link escape
        ba.replace('\x11', ' '); // device control 1
        ba.replace('\x12', ' '); // device control 2
        ba.replace('\x13', ' '); // device control 3
        ba.replace('\x14', ' '); // device control 4
        ba.replace('\x15', ' '); // negative acknowledge (something wrong)
        ba.replace('\x16', ' '); // synchronous idle
        ba.replace('\x17', ' '); // end of transmission block
        ba.replace('\x18', ' '); // cancel
        ba.replace('\x19', ' '); // end of medium
        ba.replace('\x1a', ' '); // substitute
        ba.replace('\x1b', ' '); // escape
        ba.replace('\x1c', ' '); // file seperator
        ba.replace('\x1d', ' '); // group seperator
        ba.replace('\x1e', ' '); // record seperator
        ba.replace('\x1f', ' '); // unit seperator


        ba.replace('(', ' '); // unit seperator
        ba.replace('\"', ' '); // unit seperator

        ba.replace('\'', ' ');
        ba.replace('$', ' ');
        ba.replace('!', ' ');
        ba.replace('#', ' ');
        ba.replace('@', ' ');
        ba.replace('^', ' ');
        ba.replace('-', ' ');

        qDebug() << ba;

        result.write(ba);
        result.flush();
    }

}

QString ProcessLine(QString line)
{
    QRegularExpression re;

    re.setPattern("borderless_window = 0");
    QRegularExpressionMatch match = re.match(line);
    bool hasMatch = match.hasMatch(); // true

    if (hasMatch)
    {
        line = "borderless_window = 1";
    }

    line += "\n";

    return line;
}
