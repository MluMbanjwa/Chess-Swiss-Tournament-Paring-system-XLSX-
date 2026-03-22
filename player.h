#ifndef PLAYER_H
#define PLAYER_H
#include <QStringList>
#include <QMap>
#include <QDate>
#include "game.h"
class Player
{
private:
    QString m_name;
    QString m_surname;
    QDate m_dateOfBirth;
    QString m_id;
    QString m_gender;
    QString m_category;
    QList<Game*>m_gameRecords;
    float m_points=0.0;
public:
    Player(QString name,QString surname,QString gender,QDate dob);
    QString color(int num);

    QMap<QString, QList<float> > games() const;
    QString games(QString color) const;
    void addGame(Game *game);
    QString outCome(float n) const;
    int numberOfGames(QString color);
    QString name() const;
    QString surname() const;
    QDate dateOfBirth() const;
    QString dob();
    void setDateOfBirth(const QDate &newDateOfBirth);
    QString toString();
    float points() const;

    QString id() const;
    QString gender() const;
    QString category() const;
    QList<Game *> gameRecords() const;
    void setCategory(const QString &newCategory);
};

#endif // PLAYER_H
