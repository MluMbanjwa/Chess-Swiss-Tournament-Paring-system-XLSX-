#include "paring.h"

int Paring::round() const
{
    return m_round;
}

void Paring::setRound(int newRound)
{
    m_round = newRound;
}

PlayerList *Paring::players() const
{
    return m_players;
}

QList<ScoreSelector *> Paring::parings() const
{
    return m_parings;
}

bool Paring::isfinished() const
{
    foreach (ScoreSelector* s, m_parings) {
        if(!s->isChecked()){
            return false;
        }
    }
    return true;
}

Paring::Paring(QWidget *parent, PlayerList *list)
    : QWidget{parent}
{
    m_players=list;
    m_layout=new QVBoxLayout(this);
    m_view=new QTableWidget(this);
    m_view->setWordWrap(true);

    m_view->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    m_view->verticalHeader()->setDefaultSectionSize(20);
    m_view->setColumnCount(3);
    m_view->setHorizontalHeaderLabels({"WHITE","SCORE","black"});
    m_layout->addWidget(m_view);
    setLayout(m_layout);
    show();
}
Paring* Paring::clone()
{
    Paring* copy = new Paring(nullptr, m_players);

    copy->setRound(m_round);

    copy->m_parings.clear();
    copy->m_view->clearContents();
    copy->m_view->setRowCount(0);

    int row = 0;

    for (ScoreSelector* s : m_parings) {

        Game* g = s->game()->clone();

        ScoreSelector* newSelector = s->clone(this);
        // copy the score AFTER widget initialization
        copy->m_parings.push_back(newSelector);
        copy->addToView(newSelector, row++);
    }
    copy->hide();

    return copy;
}
void Paring::pair(int round)
{
    m_round=round;
    m_parings.clear();
    m_view->clearContents();
    m_view->setRowCount(0);
    m_players->sortPlayersByScore();
    if (!m_players) return;

    int size = m_players->players().size();
    int row = 0;

    m_view->setColumnCount(3);
    m_view->setRowCount(size / 2 + (size % 2));
    int board=1;
    for (int i = 0; i < size - 1; i += 2) {

        QStringList players = colorChoser(
            m_players->players().at(i),
            m_players->players().at(i + 1)
            );
        ScoreSelector *newS = new ScoreSelector(
            m_view,
            players.at(0),
            players.at(1)
            ,board);
        m_parings.push_back(newS);
        m_players->player(players.at(0))->addGame(newS->game());
        m_players->player(players.at(1))->addGame(newS->game());
        addToView(newS, row++);
        board++;
    }

    // Handle bye
    if (size % 2 == 1) {
        Player *last = m_players->players().last();

        QStringList players = { last->id(), "bye" };

        ScoreSelector *newS = new ScoreSelector(
            m_view,
            players.at(0),
            players.at(1)
            ,board);

        m_parings.push_back(newS);
        last->addGame(newS->game());
        addToView(newS, row++);
    }

}

QStringList Paring::colorChoser(Player *one, Player *two)
{
    if(one->numberOfGames("white")>two->numberOfGames("white")){
        return {two->id(),one->id()};
    }
    else{
        return {one->id(),two->id()};
    }
}

void Paring::addToView(ScoreSelector *s,int row)
{
    QTableWidgetItem* newItem1=new QTableWidgetItem(s->game()->white());
    QTableWidgetItem* newItem2=new QTableWidgetItem(s->game()->black());
    m_view->setItem(row,0,newItem1);
    m_view->setCellWidget(row,1,s);
    m_view->setItem(row,2,newItem2);
    m_view->resizeRowsToContents();
    m_view->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_view->verticalHeader()->setMinimumSectionSize(1);

}

Game *Paring::game(int board)
{
    foreach (ScoreSelector*s, m_parings) {
        if(s->game()->board()==board){
            return s->game();
        }
    }
    return nullptr;
}

void Paring::exportToExcel(int round, QString fileName)
{
    if (m_parings.isEmpty()) {
        QMessageBox::information(nullptr, "Export", "No players to export.");
        return;
    }
    if (fileName.isEmpty())
        return;

    QXlsx::Document xlsx(fileName);

    // ===== Open existing file if it exists =====
    if (QFile::exists(fileName)) {
        xlsx.load();
    }
    // Remove sheet if it already exists
    if (xlsx.sheetNames().contains("round "+QString::number(round)))
        xlsx.deleteSheet("round "+QString::number(round));

    // ===== Create & Select Sheet =====
    xlsx.addSheet("round "+QString::number(round));
    xlsx.selectSheet("round "+QString::number(round));
    // ===== Headers =====
    xlsx.write(1, 1, "Board");
    xlsx.write(1, 2, "Game: White vs black");
    // ===== Player Data =====
    int row = 2;

    for (ScoreSelector* p : m_parings) {
        xlsx.write(row, 1, QString::number(row-1));
        xlsx.write(row, 2, p->game()->toString().removeFirst().removeFirst());
        row++;
    }
    // ===== Save File =====
    if (!xlsx.saveAs(fileName)) {
        QMessageBox::warning(nullptr, "Export Failed", "Could not save the file.");
    } else {
        QMessageBox::information(nullptr, "Export", "Paring exported successfully!");
    }
}

void Paring::printParings()
{
    if (m_parings.isEmpty()) {
        QMessageBox::information(nullptr, "Print", "No players to print.");
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QPrintDialog dialog(&printer);
    if (dialog.exec() != QDialog::Accepted)
        return;

    // ===== Build HTML Table =====
    QString html;
    html += "<h2 align='center'>Tournament Parings</h2>";
    html += "<table border='1' cellspacing='0' cellpadding='6' width='100%'>";
    html += "<tr>"
            "<th align='left'>Board</th>"
            "<th align='left'>White</th>"
            "<th align='center'>vs</th>"
            "<th align='left'>black</th>"
            "</tr>";
    int board=1;
    for (ScoreSelector* p : m_parings) {


        html += "<tr>";
        html += "<td>" +QString::number(board)+ "</td>";
        html += "<td align='left'>" + p->game()->white() + "</td>";
        html += "<td align='center'> vs </td>";
        html += "<td align='left'>" + p->game()->black() + "</td>";
        html += "</tr>";
        board++;
    }

    html += "</table>";

    // ===== Print =====
    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);
}

void Paring::printInformation(QMap<QString, QString> info)
{
    if (info.isEmpty()) {
        QMessageBox::information(nullptr, "Print", "Information to print.");
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QPrintDialog dialog(&printer);
    if (dialog.exec() != QDialog::Accepted)
        return;

    // ===== Build HTML Table =====
    QString html;
    html += "<h2 align='center'>Tournament Information</h2>";
    html += "<table border='1' cellspacing='0' cellpadding='6' width='100%'>";
    int board=1;
    for (QString i : info.keys()) {


        html += "<tr>";
        html += "<td>" +i+"</td>";
        html += "<td>"+info[i]+"</td>";
        html += "</tr>";
        board++;
    }

    html += "</table>";

    // ===== Print =====
    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);
}

void Paring::exportInformation(QString fileName,QMap<QString,QString>info)
{
    if (info.isEmpty()) {
        QMessageBox::information(nullptr, "Export", "No players to export.");
        return;
    }
    if (fileName.isEmpty())
        return;

    QXlsx::Document xlsx(fileName);

    // ===== Open existing file if it exists =====
    if (QFile::exists(fileName)) {
        xlsx.load();
    }
    // ===== Create & Select Sheet =====
    xlsx.addSheet("Tournament Information");
    xlsx.selectSheet("Tournament Information");


    // ===== Headers =====
    xlsx.write(1, 1, "Description");
    xlsx.write(1, 2, "Value");
    // ===== Player Data =====
    int row = 2;

    for (QString p : info.keys()) {
        xlsx.write(row, 1, p);
        xlsx.write(row, 2, info[p]);
        row++;
    }
    // ===== Save File =====
    if (!xlsx.saveAs(fileName)) {
        QMessageBox::warning(nullptr, "Export Failed", "Could not save the file.");
    } else {
        QMessageBox::information(nullptr, "Export", "Information exported successfully!");
    }
}
