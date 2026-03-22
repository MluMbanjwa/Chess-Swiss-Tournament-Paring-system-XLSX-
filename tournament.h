#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <QWidget>
#include "paring.h"
#include "treeviewer.h"
#include "inputs.h"

class Tournament : public QWidget
{
    Q_OBJECT
private:

    int m_round=1;
    QPushButton* m_btnNextround,*m_btnBack,* m_export,*m_print;
    QString m_name;
    QDate m_date;
    QString m_venue;
    QString m_Orgniser;
    Paring * m_paring;
    PlayerList* m_playerLists;
    TreeViewer* m_view;
    QVBoxLayout*m_layout;
    inputs *m_inputs;
    QLabel* m_lblRound;
    QList<Paring*>m_paring_Rounds;
    QList<QCheckBox*>m_selection;
    QVBoxLayout* m_layFileHandling;
    QGroupBox *m_groupselection;

public:
    explicit Tournament(QWidget *parent = nullptr);
    QMap<QString,QString>tournamentInformation();

public slots:
    void nextRound();
    void back();
    void startTournament();
    void exportFilehandler();
    void printFileHandler();

signals:
};

#endif // TOURNAMENT_H
