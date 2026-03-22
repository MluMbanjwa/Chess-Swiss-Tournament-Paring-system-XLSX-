#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include "tournament.h"
class MainWindow : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout* m_layout;
    Tournament *m_tourn;

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();
};
#endif // MAINWINDOW_H
