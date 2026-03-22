#include "player.h"
#include <qforeach.h>



QString Player::gender() const
{
    return m_gender;
}



QString Player:: color(int num){
    if(num==0){
        return "white";
    }
    else{
        return "black";
    }
}

QString Player::games(QString color) const
{
    QString answer;
    if(color=="white"){
        foreach (Game *g , m_gameRecords) {
            if(g->whiteId()==m_id)
            {
                answer+=outCome(g->results(m_id))+",";
            }
        }

    }
    else{
        foreach (Game *g , m_gameRecords) {
            if(g->blackId()==m_id)
            {
                answer+=outCome(g->results(m_id))+",";
            }
        }
    }

    if (!answer.isEmpty())
        answer.chop(1);

    return answer;
}
void Player::addGame(Game *game)
{
    if(game->whiteId()==m_id || game->blackId()==m_id){
        m_gameRecords.push_back(game);
    }
}
QString Player::outCome(float n) const
{
    if (n == 1.0) return "WIN";
    if (n == 0.0) return "LOSE";
    if (n==0.5) return "DRAW";

    return "NO RESULTS";
}
int Player::numberOfGames(QString color)
{
    int count = 0;

    for (Game *g : m_gameRecords) {
        if (color == "white" && g->whiteId() == m_id)
            count++;
        else if (color == "black" && g->blackId() == m_id)
            count++;
    }

    return count;
}
QString Player::name() const
{
    return m_name;
}


QString Player::surname() const
{
    return m_surname;
}



QDate Player::dateOfBirth() const
{
    return m_dateOfBirth;
}

QString Player::dob()
{
    return m_dateOfBirth.toString("yyyy-MM-dd");
}

void Player::setDateOfBirth(const QDate &newDateOfBirth)
{
    m_dateOfBirth = newDateOfBirth;
    m_id=m_name+"_"+m_surname+"_"+m_dateOfBirth.toString();
}

QString Player::toString()
{
    QString games="white:(";
    QMap<QString,QString>m_games;
    foreach (Game *g, m_gameRecords) {
        if(m_id==g->whiteId()){
            m_games["white"]+=outCome(g->results(m_id));
        }
        if(m_id==g->blackId()){
            m_games["black"]+=outCome(g->results(m_id));
        }
    }

    return m_name+" "+m_surname+"("+m_gender+")"+"->White:"+m_games["white"]+","+"black: "+m_games["black"];
}

float Player::points() const
{
    float total = 0.0;
    for (Game *g : m_gameRecords) {
        float r = g->results(m_id);
        if (r >= 0) {   // ignore invalid (-100)
            total += r;
        }
    }
    return total;
}

QString Player::id() const
{
    return m_id;
}

QString Player::category() const
{
    return m_category;
}



QList<Game *> Player::gameRecords() const
{
    return m_gameRecords;
}

void Player::setCategory(const QString &newCategory)
{
    m_category = newCategory;
}

Player::Player(QString name, QString surname, QString gender, QDate dob) {
    m_name=name.toUpper();
    m_surname=surname.toUpper();
    m_gender=gender;
    m_dateOfBirth=dob;
    m_id=m_name+"_"+m_surname+"_"+dob.toString("yyyy-MM-dd");
}
