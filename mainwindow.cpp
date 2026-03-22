#include "mainwindow.h"
#include <qpushbutton.h>
#include <qwindowdefs.h>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{


    m_tourn=new Tournament(this);

    m_layout=new QHBoxLayout(this);
    m_layout->addWidget(m_tourn);
    setLayout(m_layout);
}

MainWindow::~MainWindow()
{

}

