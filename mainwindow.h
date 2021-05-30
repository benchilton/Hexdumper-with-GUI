#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QString>
#include <QListWidget>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QTextStream>
#include <QFont>

#include "printarea.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void open();
    void save();
    void newFile();

private:

    QSize windowSize = this->size();

    //const static size_t kBytes = 1024
    const static unsigned int defaultSizeX = 590;//default size the hexdump mainwindow.
    const static unsigned int defaultSizeY = 600;//default size the hexdump mainwindow.
    const static short hex = 16;//Used for formatting the dumper
    size_t fileLength; //length the currently open file.
    bool definedFileLength;

    printArea * printingArea;//The widget where the hex will be dumped.

    QDockWidget *byteAddressBar;//The dock widget that has 00 to 0f showing the 16 bytes

    //Data relating to the Mem Address Dock
    QDockWidget *sideMemAddressBar= new QDockWidget(this);
    QListWidget *memAddressList = new QListWidget(sideMemAddressBar);

    QDockWidget *stringBar= new QDockWidget(this);
    QListWidget *stringList = new QListWidget(sideMemAddressBar);

    QString value; // where the value of the address is stored.

    void createMenus();//Create the sile -> new etc menues.
    void createActions();
    void createAddressBar();
    void createMemAddressBar();
    void initialiseMemBar();
    void createStringBar();//Creates the dock which has the hexdump as a string with printable ascii characters
    // or a '.' if the character cant be printed.
    void initialiseStringBar();//Initialises the string docks properties

    //Current Menu types
    QMenu *fileMenu;

    //Actions for the menus:
    QAction *openFileAction;
    //Unused but kept incase making / saving files is added later
    //QAction *saveFileAction;
    //QAction *newFileAction;

    QFile *currentFile;
    QString filename;

};
#endif // MAINWINDOW_H
