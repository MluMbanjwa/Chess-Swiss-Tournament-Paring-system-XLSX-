#ifndef PARING_H
#define PARING_H

#include <QWidget>
#include "scoreselector.h"
#include <QTableWidget>
#include "playerlist.h"
#include <QVBoxLayout>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QMap>
#include<QHeaderView>
class Paring : public QWidget
{
    Q_OBJECT
private:
    int m_round=0;
    QList<ScoreSelector*>m_parings;
    QTableWidget *m_view;
    QVBoxLayout * m_layout;
    PlayerList* m_players;
    bool m_isfinished=false;
public:
    explicit Paring(QWidget *parent = nullptr,PlayerList* list=nullptr);
    void pair(int round);
    QStringList colorChoser(Player *one, Player *two);
    void addToView(ScoreSelector* s, int row);
    Game* game(int board);
    int round() const;
    void setRound(int newRound);
    Paring * clone();
    PlayerList *players() const;
    QList<ScoreSelector *> parings() const;
    bool isfinished() const;

public slots:
    void exportToExcel(int round, QString fileName);
    void printParings();
    void printInformation(QMap<QString, QString> info);
    void exportInformation(QString fileName, QMap<QString, QString> info);
};

#endif // PARING_H
