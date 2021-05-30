#include "printarea.h"

printArea::printArea(QWidget *parent) : QListWidget(parent) {
    setStyleSheet("background-color: white");

}

void printArea::hexdump(QFile *inputFile) {
    QByteArray data;
    in = new QTextStream(inputFile);
    QString hexDumpLine;
    size_t filePos;//Current position we are at in the file
    unsigned int output = 0;

    while (!inputFile->atEnd()) {

        filePos = inputFile->pos();//Get the current position in the file.
        inputFile->seek(filePos);//Go the position we are at ready for reading

        QString myText = in->read(dataBuffer);//Read 16 kBbytes
        data.append(myText);//Put myText into the byte array

        for (size_t i = 0; i < (size_t) data.size(); i+=hex){
            for (int j = 0; j < hex; j++) {
                output = data[(int)i + j] & 0xff;
                hexDumpLine = hexDumpLine + QString("%1").arg(output,2,hex,QLatin1Char('0')) + " ";
                if(j == 7) {hexDumpLine = hexDumpLine + " ";}
            }
            this->addItem(hexDumpLine);
            hexDumpLine.clear();//Empty the string
        }

        inputFile->flush();
        data.clear();//empty the byte array ready for the next read

    }
}

//Clears the print area of any lines.
void printArea::clear() {
    QListWidget::clear();
}
