#include "JGraphicsScene.h"

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(new JGraphicsScene(this));
}

MainWindow::~MainWindow() { delete ui; }

#include "moc_MainWindow.cpp"
