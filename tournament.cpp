#include "tournament.h"

Tournament::Tournament(QWidget *parent)
    : QWidget{parent}
{
    m_inputs=new inputs(this);
    m_layout=new QVBoxLayout(this);
    m_inputs->setMaximumWidth(600);
    m_layout->addWidget(m_inputs);
    connect(m_inputs,SIGNAL(finish()),this,SLOT(startTournament()));
    setLayout(m_layout);
    show();
}

QMap<QString, QString> Tournament::tournamentInformation()
{
    QMap<QString,QString> info;
    QString categories;
    info.insert("Name",m_name);
    info.insert("Venue",m_venue);
    info.insert("Organiser",m_Orgniser);
    foreach (QString c, m_inputs->categories) {
        categories+=c+", ";
    }
    categories=categories.trimmed();
    categories.removeLast();
    info.insert("Categories",categories);
    info.insert("Rounds",QString::number(m_inputs->rounds->value()));
    info.insert("Date",m_date.toString("dd/mm/yyyy"));
    info.insert("Gender",m_inputs->gender->currentText());
    info.insert("Number of Players",QString::number(m_playerLists->players().size()));
    return info;
}

void Tournament::nextRound()
{
    if(!m_paring->isfinished()){
        QMessageBox::warning(this,"Score incomplete","Enter scores for all the game then move to the next round!");
        return;
    }
    foreach (Paring *p, m_paring_Rounds) {
        p->setStyleSheet("background-color: transparent");
        foreach (ScoreSelector *s, p->parings()) {
            s->setStyleSheet("background-color: transparent");
            s->setEnabled(false);
        }
        if(p->round()==m_inputs->rounds->value()-1){
            m_paring->setStyleSheet("background-color: transparent");
            m_lblRound->setText("Tournament finished at round: "+QString::number(m_round));
            foreach (ScoreSelector *s, m_paring->parings()) {
                s->setStyleSheet("background-color: transparent");
                s->setEnabled(false);
            }
            m_view->updateLists();
            return;
        }

    }
    m_paring->pair(m_round);
    m_paring_Rounds.push_back(m_paring->clone());
    m_round++;
    m_lblRound->setText("Round: "+QString::number(m_round));
    m_view->updateLists();

}

void Tournament::back()
{

}

void Tournament::startTournament()
{
    QHBoxLayout* paringsLay=new QHBoxLayout;
    m_view=new TreeViewer(this);
    m_name=m_inputs->name->text();
    m_venue=m_inputs->venue->text();
    m_date=m_inputs->date->date();
    m_Orgniser=m_inputs->organiser->text();
    m_Orgniser=m_inputs->organiser->text();
    m_export= new QPushButton("Export(xlsx)",this);
    m_print=new QPushButton("Print",this);
    m_btnNextround=new QPushButton("Next Round",this);
    m_btnBack=new QPushButton("Previous Round",this);
    QHBoxLayout*btnLay=new QHBoxLayout;
    btnLay->addWidget(m_btnBack);
    btnLay->addWidget(m_btnNextround);
    m_layFileHandling=new QVBoxLayout;
    m_groupselection=new QGroupBox("File Selection",this);
    m_btnNextround->setObjectName("nextRoundBtn");
    m_btnBack->setObjectName("backRoundBtn");
    m_btnBack->setEnabled(false);//fix later
    m_btnBack->hide();// fix later
    QHBoxLayout* m_laySelection=new QHBoxLayout;
    QHBoxLayout *m_layTempBtn=new QHBoxLayout;
    QVBoxLayout * laygroup=new QVBoxLayout(m_groupselection);
    m_layTempBtn->addWidget(m_export);
    m_layTempBtn->addWidget(m_print);

    m_layout->setSpacing(10);
    m_layout->setContentsMargins(10,10,10,10);


    m_selection.push_back(new QCheckBox("Parings",this));
    m_selection.push_back(new QCheckBox("Standings",this));
    m_selection.push_back(new QCheckBox("Player Information",this));
    m_selection.push_back(new QCheckBox("Tournament Information",this));

    m_laySelection->addWidget(m_selection[0]);
    m_laySelection->addWidget(m_selection[1]);
    m_laySelection->addWidget(m_selection[2]);
    m_laySelection->addWidget(m_selection[3]);

    laygroup->addLayout(m_layTempBtn);
    laygroup->addLayout(m_laySelection);
    m_groupselection->setLayout(laygroup);


    m_round=0;

    m_lblRound=new QLabel("Round: "+QString::number(m_round+1),this);
    m_lblRound->setStyleSheet("font-size: 18px; font-weight: bold;");
    m_playerLists = new PlayerList(this, m_inputs->name->text(), m_inputs->fileName->text(), m_inputs->sheetName->text(), m_inputs->categories, m_inputs->gender->currentText());
    m_view->addPlayerList(m_playerLists);
    m_view->addRoots();
    m_view->resizeColumnToContents(0);
    m_view->resizeColumnToContents(1);
    m_paring=new Paring(this,m_playerLists);
    m_paring->pair(m_round);
    m_paring_Rounds.push_back(m_paring->clone());
    m_round++;
    paringsLay->addWidget(m_view);
    paringsLay->addWidget(m_paring);
    m_layout->addLayout(paringsLay);
    m_layout->addWidget(m_lblRound);

    m_layout->addWidget(m_groupselection);
    m_layout->addLayout(btnLay);
    connect(m_btnNextround,SIGNAL(clicked(bool)),this,SLOT(nextRound()));
    connect(m_export,SIGNAL(clicked(bool)),this,SLOT(exportFilehandler()));
    connect(m_print,SIGNAL(clicked(bool)),this,SLOT(printFileHandler()));
    connect(m_btnBack,SIGNAL(clicked(bool)),this,SLOT(back()));

    paringsLay->setStretch(0, 1);
    paringsLay->setStretch(1, 3);
}

void Tournament::exportFilehandler()
{
    if(m_selection[0]->isChecked()){
        //parings
        foreach (Paring *p, m_paring_Rounds)
        {
            p->exportToExcel(p->round(),m_name+".xlsx");
        }
    }
    if(m_selection[1]->isChecked()){
        //standings
        m_playerLists->exportStandings(m_name+".xlsx");
    }
    if(m_selection[2]->isChecked()){
        //player info
        m_playerLists->exportToExcel(m_name+".xlsx");
    }
    if(m_selection[3]->isChecked()){
     //tournament info
        m_paring->exportInformation(m_name+".xlsx",tournamentInformation());
    }
}

void Tournament::printFileHandler()
{
    if(m_selection[0]->isChecked()){
        //parings
        m_paring->printParings();
    }
    if(m_selection[1]->isChecked()){
        //standings
        m_playerLists->printStandings();
    }
    if(m_selection[2]->isChecked()){
        //player info
        m_playerLists->printPlayerInformation();
    }
    if(m_selection[3]->isChecked()){
        //tournament info
        m_paring->printInformation(tournamentInformation());
    }
}




