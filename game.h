#ifndef GAME_H
#define GAME_H
#include <QMap>
#include <QObject>
class Game: public QObject
{
    Q_OBJECT
private:
    int m_board=0;
    QString m_players[2];
    float m_Status[2];
public:
Game(QString playerIds[2]);


int board() const;
void setBoard(int newBoard);

public slots:
    void draw();
    void whiteWins();
    void blackWins();
    bool bye();
    QString white();
    Game* clone();
    QString black();
    QString whiteId();
    QString blackId();
    QString toString();
    void setScore(float w, float b);
    float results(QString id) const;

};

#endif // GAME_H
