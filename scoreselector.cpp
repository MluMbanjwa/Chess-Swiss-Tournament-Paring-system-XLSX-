#include "scoreselector.h"
#include <qmessagebox.h>

Game *ScoreSelector::game() const
{
    return m_game;
}

void ScoreSelector::setGame(Game *newGame)
{
    m_game = newGame;
}

QString ScoreSelector::toString()
{
    return m_game->white()+" vs "+m_game->black();
}



bool ScoreSelector::isChecked() const
{
    return m_isChecked;
}

ScoreSelector::ScoreSelector(QWidget *parent, QString whiteId, QString blackId, int board)
    : QWidget(parent)
{
    QString play[2];
    play[0]=whiteId;
    play[1]=blackId;
    m_game = new Game(play);
    m_game->setBoard(board);

    m_group = new QGroupBox("Select the winner", this);

    m_white = new QRadioButton("WHITE", m_group);
    m_draw  = new QRadioButton("DRAW", m_group);
    m_black = new QRadioButton("BLACK", m_group);

    m_white->setMinimumWidth(100);
    m_draw->setMinimumWidth(100);
    m_black->setMinimumWidth(100);
    m_white->setMinimumHeight(50);
    m_draw->setMinimumHeight(50);
    m_black->setMinimumHeight(50);

    // Group radio buttons
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(m_white);
    btnGroup->addButton(m_draw);
    btnGroup->addButton(m_black);

    // Layout for radio buttons
    QHBoxLayout *groupLayout = new QHBoxLayout();
    groupLayout->addWidget(m_white);
    groupLayout->addWidget(m_draw);
    groupLayout->addWidget(m_black);


    m_group->setLayout(groupLayout);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_group);

    setLayout(mainLayout);

    // Bye handling
    if (play[0] == "bye" || play[1] == "bye") {
        //m_group->setEnabled(false);
    }

    connect(m_white, &QRadioButton::toggled, this, &ScoreSelector::updateStatus);
    connect(m_black, &QRadioButton::toggled, this, &ScoreSelector::updateStatus);
    connect(m_draw,  &QRadioButton::toggled, this, &ScoreSelector::updateStatus);
    this->setStyleSheet("background-color: red");
}

void ScoreSelector::updateStatus()
{
    bool bye=m_game->bye();
    if(bye){
        this->setStyleSheet("background-color: green");
        m_isChecked=true;
        return;
    }
    if(m_white->isChecked()){
        m_game->whiteWins();
        m_isChecked=true;
        this->setStyleSheet("background-color: green");
    }
    else if(m_black->isChecked()){
        this->setStyleSheet("background-color: green");
        m_isChecked=true;
        m_game->blackWins();
    }
    else if(m_draw->isChecked()){
        this->setStyleSheet("background-color: green");
        m_isChecked=true;
        m_game->draw();
    }
    else{
        this->setStyleSheet("background-color: green");
        QMessageBox::warning(this,"ERROR","Set the results for players:"+m_game->white()+" vs "+m_game->black());
    }
   emit updatePlayers();
}

void ScoreSelector::setWhiteWins()
{
    m_white->setChecked(true);
}

void ScoreSelector::setBlackWins()
{
   m_black->setChecked(true);
}

QString ScoreSelector::winner()
{
    if(m_white->isChecked()){
        return "white";
    }
    else if(m_black->isChecked()){
        return "black";
    }
    else if(m_draw->isChecked()){
        return "draw";
    }
    return "No Result!";
}
ScoreSelector* ScoreSelector::clone(QWidget *parent)
{
    Game* g = m_game->clone();

    ScoreSelector* copy = new ScoreSelector(
        parent,
        g->whiteId(),
        g->blackId(),
        g->board()
        );

    copy->setGame(g);

    // Copy score using radio buttons
    float w = g->results(g->whiteId());
    float b = g->results(g->blackId());

    if (m_white->isChecked()) {
        copy->setWhiteWins();
    }
    else if (m_black->isChecked()) {
        copy->setBlackWins();
    }
    else if (m_draw->isChecked()) {
        copy->setDraw();
    }
    copy->hide();
    return copy;

}
void ScoreSelector::setDraw()
{
    m_draw->setChecked(true);
}
