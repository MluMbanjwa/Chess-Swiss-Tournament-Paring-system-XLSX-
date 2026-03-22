#include "game.h"
#include <qdebug.h>

float Game::results(QString id) const
{
    if(id==m_players[0]){
    return m_Status[0];
    }
    else if(id==m_players[1]){
        return m_Status[1];
    }
    return -100;
}

Game* Game::clone()
{
    QString ids[2] = { m_players[0], m_players[1] };
    Game* copy = new Game(ids);

    copy->setBoard(m_board);

    copy->setScore(m_Status[0],m_Status[1]);

    return copy;
}
int Game::board() const
{
    return m_board;
}

void Game::setBoard(int newBoard)
{
    m_board = newBoard;
}

Game::Game(QString playerIds[2]) {
    m_players[0]=playerIds[0];
    m_players[1]=playerIds[1];

    m_Status[0] = -1;  // no result yet
    m_Status[1] = -1;

    bye();
}

void Game::whiteWins()
{
    m_Status[0]=1.0;
    m_Status[1]=0.0;
}

void Game::blackWins()
{
    m_Status[0]=0.0;
    m_Status[1]=1.0;
}

bool Game::bye()
{
    if(m_players[0]=="bye"){
        blackWins();
        return true;
    }
    else if(m_players[1]=="bye"){
        whiteWins();
        return true;
    }
    return false;
}

QString Game::white()
{
    QStringList list=m_players[0].split("_");
    if(list.size()>1){
        return list.at(0)+" " +list.at(1);
    }
    else{
        return list.at(0);
    }
}

QString Game::black()
{
    QStringList list=m_players[1].split("_");
    if(list.size()>1){
      return list.at(0)+" " +list.at(1);
    }
    else{
        return list.at(0);
    }

}

QString Game::whiteId()
{
    return m_players[0];
}

QString Game::blackId()
{
   return m_players[1];
}

QString Game::toString()
{
    return QString::number(m_board)+". "+white()+"("+QString::number(m_Status[0])+")"+" vs "+black()+"("+QString::number(m_Status[1])+")";
}

void Game::setScore(float w,float b)
{
    m_Status[0]=w;
    m_Status[1]=b;
}

void Game::draw()
{
    m_Status[0]=0.5;
    m_Status[1]=0.5;
}
