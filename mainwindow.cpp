#include "mainwindow.h"

#include <iostream>

//Function Prototypes
size_t roundUp(float input);//Simple round up function so i dont need to use cmath for 1 function

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), printingArea(new printArea(this)) {
    definedFileLength = 0;//No file length has been defined.

    //Create the size of the new printing area with the size of the
    setCentralWidget(printingArea);

    createActions();//create the actions we want;
    createMenus();//Put them in the menus

    createAddressBar();

    initialiseMemBar();
    createMemAddressBar();

    initialiseStringBar();
    createStringBar();

    setWindowTitle("Hex Dumper");
    resize(defaultSizeX, defaultSizeY);//Make the Window a default size.

}

//functions for opening, saving and creating new files.
void MainWindow::open() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"),
           QDir::currentPath(), tr(""));//we can open any file type we want as we are simply printing raw binary data.
    currentFile = new QFile(filename);
    //Open the file in the Dialog box and set the filename to the opened file's name,
    //This also includes the directories.
    currentFile->setFileName(filename);//set the current filename to the dialog filename
    if (!currentFile->open(QIODevice::ReadOnly)) {//open the file the open function returns false if it fails to open
        QTextStream cout(stderr);//it it fails output to the std error stream stating such
        std::cout << "File Failed to open" << "\n";
    } else {//if opening is a success
        printingArea->clear();//empty the print area if anything is in it.
        printingArea->hexdump(currentFile);//hexdump the file to the printArea widget

        definedFileLength = 1;//The file length has been defined.
        fileLength = currentFile->size();//update the fileLength variable in the class with the filesize
        createMemAddressBar();//create the memory address bar now we know how many memory addresses to print

        createStringBar();//Update the string bar with the read file

    }
}
//Not sure about these functions.
void MainWindow::save() {
    QTextStream cout(stdout);
    std::cout << "open test\n";
}

void MainWindow::newFile() {
    QTextStream cout(stdout);
    std::cout << "open test\n";
}

void MainWindow::createActions() {
    //Open File Action
    openFileAction = new QAction("&Open File", this);
    openFileAction->setShortcut(tr("CTRL+O"));
    connect(openFileAction, &QAction::triggered, this, &MainWindow::open);
//Repeat of the previous action but changed for the desired functions.

    //Unused actions maybe added later

    //saveFileAction = new QAction("Save &File", this);
    //saveFileAction->setShortcut(tr("CTRL+S"));
    //connect(saveFileAction, &QAction::triggered, this, &MainWindow::save);

    //newFileAction = new QAction(tr("&New File"), this);
    //newFileAction->setShortcut(tr("CTRL+N"));
    //connect(newFileAction, &QAction::triggered, this, &MainWindow::newFile);

}

void MainWindow::createMenus() {

    fileMenu = new QMenu("&File", this);
    //fileMenu->addAction(newFileAction);
    fileMenu->addAction(openFileAction);

    //fileMenu->addAction(saveFileAction);

    menuBar()->addMenu(fileMenu);//Make the menu.

}

//Creates the top Dock Widget
void MainWindow::createAddressBar() {

    QDockWidget *byteAddressBar = new QDockWidget(this);//make a new dock widget
    QListWidget *addressList = new QListWidget(byteAddressBar);//make a new list widget
    QString topText;//string to store the text
    topText.clear();//initalise the string
    //add corresponding text
    topText.append(" address   ");
    for (int i = 0; i< hex; i++) {
        topText = topText + QString("%1").arg(i,2,hex,QLatin1Char('0')) + " ";
        if(i==7) {topText = topText + " ";}
    }
//set the features of the dock widget
    byteAddressBar->setFeatures(QDockWidget::NoDockWidgetFeatures);
    byteAddressBar->setAllowedAreas(Qt::TopDockWidgetArea);
    byteAddressBar->setFixedHeight(18);//set the height of the top bar to approx fit 1 char
    byteAddressBar->setTitleBarWidget(new QWidget());//Remove the title bar
//add the text to the list widget
    addressList->addItem(topText);
    //add the list to the dock
    byteAddressBar->setWidget(addressList);
//add the dock to the main window
    addDockWidget(Qt::TopDockWidgetArea, byteAddressBar);

}

//Creates the Memory Address Bar for the currently opened file
void MainWindow::createMemAddressBar() {

    memAddressList->clear();//Clear the old QListWidget to make way for the new list widget
    //Note this deletes all entries in the QListWidget.

    sideMemAddressBar->setVisible(true);//Now a file has been opened show this dock

    if (definedFileLength) {
        for (size_t i = 0; i < roundUp((float)fileLength/hex); i++) {
            value = QString("%1").arg(hex*i, 8, hex,QLatin1Char('0'));
            memAddressList->addItem(value);
        }
    }
    sideMemAddressBar->setWidget(memAddressList);

    addDockWidget(Qt::LeftDockWidgetArea, sideMemAddressBar);

}

//Simple Roundup function so i dont need to include cmath just to use 1 function, decrease program size.
//Remember to typecast inputs to floats if your dividing integers!!
size_t roundUp(float input) {
  int inputInt = input;//Remove the floating point parts.
  if (inputInt == input) {
    return inputInt;
  } else {
    return inputInt + 1;
  }
}

//Initialises the memory address bar and keeps it hidden unless its needed.
void MainWindow::initialiseMemBar() {

    sideMemAddressBar->setVisible(false);//Hide this dock until we open a file
    sideMemAddressBar->setFeatures(QDockWidget::NoDockWidgetFeatures);
    sideMemAddressBar->setAllowedAreas(Qt::LeftDockWidgetArea);
    sideMemAddressBar->setFixedWidth(80);
    sideMemAddressBar->setTitleBarWidget(new QWidget());
}


void MainWindow::createStringBar() {
    QString currentRow;
    QString toPrint;
    QByteArray convertChars;
    QByteArray byte;
    char dataIn;//the char where we store the finished conversion to be added to the list
// initialse strings and byte arrays
    byte.clear();
    toPrint.clear();

    stringList->clear();//Clear the old QListWidget to make way for the new list widget
    //Note this deletes all entries in the QListWidget.

    stringBar->setVisible(true);//Now a file has been opened show this dock

    if (definedFileLength) {
        for (size_t i = 0; i < roundUp((float)fileLength/hex); i++) {
            currentRow = printingArea->item(i)->text();//get the text at the current row
            currentRow = currentRow.remove(QChar(' '),Qt::CaseInsensitive);//Remove the spaces from the string
            convertChars = currentRow.toUtf8();//convert the string into ascii (supports unicode characters)
            for (int j = 0; j < 2*hex; j+=2) {
                byte.append(convertChars[j]).append(convertChars[j+1]);//add to characters to the string
                dataIn = byte.toInt(nullptr,hex);//convert the 2 chars into an int using base 16

                if (isprint(dataIn)) {
                          toPrint.append(dataIn);//If its a printable character append it to the string
                        } else {
                          toPrint.append('.');//if it isnt a printable character append a '.' to the string
                        }
                byte.clear();//clear the array ready for the next 2 values in the 16 byte line
            }
            stringList->addItem(toPrint);//Add the characters to the list
            toPrint.clear();//empty the string ready for the next 16 bytes.
        }
    }

    stringBar->setWidget(stringList);
    addDockWidget(Qt::RightDockWidgetArea, stringBar);

}
//set initial properties of the dock
void MainWindow::initialiseStringBar() {

    stringBar->setVisible(false);//Hide this dock until we open a file
    stringBar->setFeatures(QDockWidget::NoDockWidgetFeatures);
    stringBar->setAllowedAreas(Qt::RightDockWidgetArea);
    stringBar->setFixedWidth(140);
    stringBar->setTitleBarWidget(new QWidget());
}
