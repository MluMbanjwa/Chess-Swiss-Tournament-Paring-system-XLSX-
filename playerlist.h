#ifndef PLAYERLIST_H
#define PLAYERLIST_H
#include <QWidget>
#include "player.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <xlsxdocument.h>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QFileDialog>
class PlayerList : public QObject
{
    Q_OBJECT
private:
    QString m_name;
    QXlsx::Document *m_document;
    QList<Player*>m_players;
public:
    explicit PlayerList(QObject *parent = nullptr,QString name="Tournament Players", QString fileName="", QString sheetName="sheet 1", QStringList conditions=QStringList(),QString gender="");
    PlayerList(QString name,QXlsx::Document *doc,QString sheetName, QStringList conditions,QString gender);
    ~PlayerList();
    QList<Player *> players() const;
    Player *player(QString id);
    bool playerExist(QString id);
    void getPlayers(QString gender,QStringList Conditions);
    void setPlayers(const QList<Player *> &newPlayers);
    void sortPlayersByScore();

    QString name() const;
    void setName(const QString &newName);
    void addPlayer(Player *newP);
    void removePlayer(Player *newP);
public slots:
    void done();
    void exportToExcel(QString fileName);
    void printStandings();
    void exportStandings(QString fileName);
    void printPlayerInformation();
signals:
    void playersAdded();
};

#endif // PLAYERLIST_H
