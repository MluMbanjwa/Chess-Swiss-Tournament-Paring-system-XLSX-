#ifndef SCORESELECTOR_H
#define SCORESELECTOR_H

#include <QObject>
#include <QWidget>
#include "game.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QMessageBox>
#include <QButtonGroup>
class ScoreSelector: public QWidget
{
    Q_OBJECT
    private:
    Game * m_game;
    QRadioButton *m_white;
    QRadioButton *m_black;
    QRadioButton *m_draw;
    QGroupBox* m_group;
    QHBoxLayout* groupBoxLayout;
    bool m_isChecked=false;

public:
    ScoreSelector(QWidget *parent, QString whiteId, QString blackId, int board);

    Game *game() const;
    ScoreSelector* clone(QWidget *parent);
    void setGame(Game *newGame);
    QString toString();
    bool checked() const;

    bool isChecked() const;

public slots:
    void updateStatus();
    void setWhiteWins();
    void setBlackWins();
    QString winner();
    void setDraw();
signals:
    void updatePlayers();

};

#endif // SCORESELECTOR_H
