#ifndef PRINTAREA_H
#define PRINTAREA_H

#include <iostream>

#include <QObject>
#include <QWidget>
#include <QString>
#include <QListWidget>
#include <QFile>
#include <QTextStream>
#include <QByteArray>

class printArea : public QListWidget {
    Q_OBJECT
public:
    const static int dataBuffer = 16384;//Buffer size, 16kB
    const static int hex = 16;//the 16 bytes we read per line.

    printArea(QWidget *parent = nullptr);

    QTextStream *in;

    void hexdump(QFile *inputFile);//Hexdumps the file, returns true if successful.
    void clear();
    //bool hexdump();//Hexdumps the file, returns true if successful.

};

#endif // PRINTAREA_H
