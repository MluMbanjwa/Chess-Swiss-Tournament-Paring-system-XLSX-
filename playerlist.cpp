#include "playerlist.h"

QList<Player *> PlayerList::players() const
{
    return m_players;
}

Player *PlayerList::player(QString id)
{
    foreach (Player* p, m_players) {
        if(p->id()==id){
            return p;
        }
    }
    return nullptr;
}

bool PlayerList::playerExist(QString id)
{
    foreach (Player* p, m_players) {
        if(p->id()==id){
            return true;
        }
    }
    return false;
}

void PlayerList::getPlayers(QString gender, QStringList Conditions)
{
    int rowCount=m_document->dimension().lastRow();
    int colomnCount=m_document->dimension().columnCount();
    QString name,surname,g,category;
    QStringList dob;
    int y,m,d;
    Player*newP=nullptr;
    for(int i=2;i<rowCount;i++){
        name=m_document->read(i,3).toString().trimmed();
        surname=m_document->read(i,2).toString().trimmed();
        g=m_document->read(i,5).toString().trimmed();
        auto cell = m_document->cellAt(i, 9);
        QString category = cell ? cell->value().toString().trimmed() : "";
        dob=m_document->read(i,4).toString().trimmed().split("-",Qt::SkipEmptyParts);
        if(name==""){
            continue;
        }
        if (name.isEmpty() || dob.size() != 3) {
            continue;
        }

        y = dob.at(0).toInt();
        m = dob.at(1).toInt();
        d = dob.at(2).toInt();
if((gender.isEmpty() || g.compare(gender, Qt::CaseInsensitive) == 0) &&
    Conditions.contains(category, Qt::CaseInsensitive) || Conditions.isEmpty()){
         newP=new Player(name,surname,g,QDate(y,m,d));
        newP->setCategory(category);
         this->addPlayer(newP);
     }
        else{

     }
    }

    this->done();
}

void PlayerList::setPlayers(const QList<Player *> &newPlayers)
{
    m_players = newPlayers;
}

void PlayerList::sortPlayersByScore()
{
    std::sort(m_players.begin(), m_players.end(),
              [](Player* a, Player* b) {
                  return a->points() > b->points(); // highest score first
              });
}



void PlayerList::addPlayer(Player *newP)
{
    if(playerExist(newP->id())){
        QMessageBox::warning(0,"ERROR ADDING PLAYER","Player Already exists");
        return;
    }
    m_players.push_back(newP);
}

void PlayerList::removePlayer(Player *newP)
{
    for (int i=0;i<m_players.size();i++) {
        if(m_players.at(i)->id()==newP->id()){
            m_players.removeAt(i);
            return;
        }
    }
    QMessageBox::information(0,"ERROR","Player does not exist!");
}

void PlayerList::done()
{
    emit playersAdded();
}

QString PlayerList::name() const
{
    return m_name;
}

void PlayerList::setName(const QString &newName)
{
    m_name = newName;
}

PlayerList::PlayerList(QObject *parent,QString name,QString fileName,QString sheetName,QStringList conditions,QString gender)
    : QObject{parent}
{
    m_name=name;
    m_document = new QXlsx::Document(fileName);
    if (!m_document->load()) {
        QMessageBox::warning(0, "Error", "Failed to load file: " + fileName);
        return;
    }
    if (!m_document->selectSheet(sheetName)) {
       QMessageBox::warning(0,"ERROR READING: "+fileName,"Sheet not found!");
    }
    getPlayers(gender,conditions);
}
void PlayerList::exportStandings(QString fileName)
{
    if (m_players.isEmpty()) {
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
    if (xlsx.sheetNames().contains("Standings"))
        xlsx.deleteSheet("Standings");

    xlsx.addSheet("Standings");
    xlsx.selectSheet("Standings");

    // Headers
    xlsx.write(1, 1, "Player");
    xlsx.write(1, 2, "Points");

    int row = 2;
    for (Player* p : m_players) {
        QString fullName = p->name() + " " + p->surname();
        xlsx.write(row, 1, fullName);
        xlsx.write(row, 2, p->points());
        row++;
    }

    xlsx.saveAs(fileName);
    if (!xlsx.saveAs(fileName)) {
        QMessageBox::warning(nullptr, "Export Failed", "Could not save the file.");
    } else {
        QMessageBox::information(nullptr, "Export", "Standings exported successfully!");
    }
}
void PlayerList::exportToExcel(QString fileName)
{
    if (m_players.isEmpty()) {
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
    if (xlsx.sheetNames().contains("Player Information"))
        xlsx.deleteSheet("Player Information");

    xlsx.addSheet("Player Information");
    xlsx.selectSheet("Player Information");

    // Headers
    xlsx.write(1, 1, "ID");
    xlsx.write(1, 2, "Name");
    xlsx.write(1, 3, "Surname");
    xlsx.write(1, 4, "Gender");
    xlsx.write(1, 5, "Date of Birth");
    xlsx.write(1, 6, "Category");
    xlsx.write(1, 7, "Points");

    int row = 2;
    for (Player* p : m_players) {
        xlsx.write(row, 1, p->id());
        xlsx.write(row, 2, p->name());
        xlsx.write(row, 3, p->surname());
        xlsx.write(row, 4, p->gender());
        xlsx.write(row, 5, p->dateOfBirth().toString("yyyy-MM-dd"));
        xlsx.write(row, 6, p->category());
        xlsx.write(row, 7, p->points());
        row++;
    }

    xlsx.saveAs(fileName);
    if (!xlsx.saveAs(fileName)) {
        QMessageBox::warning(nullptr, "Export Failed", "Could not save the file.");
    } else {
        QMessageBox::information(nullptr, "Export", "Player Information exported successfully!");
    }
}
PlayerList::~PlayerList() {
    qDeleteAll(m_players);
    delete m_document;
}
void PlayerList::printStandings()
{
    if (m_players.isEmpty()) {
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
    html += "<h2 align='center'>Tournament Standings</h2>";
    html += "<table border='1' cellspacing='0' cellpadding='6' width='100%'>";
    html += "<tr>"
            "<th align='left'>Player</th>"
            "<th align='center'>Points</th>"
            "</tr>";

    for (Player* p : m_players) {
        QString fullName = p->name() + " " + p->surname();

        html += "<tr>";
        html += "<td>" + fullName + "</td>";
        html += "<td align='center'>" + QString::number(p->points()) + "</td>";
        html += "</tr>";
    }

    html += "</table>";

    // ===== Print =====
    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);
}

void PlayerList::printPlayerInformation()
{
    if (m_players.isEmpty()) {
        QMessageBox::information(nullptr, "Print", "No players to print.");
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QPrintDialog dialog(&printer);
    if (dialog.exec() != QDialog::Accepted)
        return;

    // ===== Build HTML =====
    QString html;
    html += "<h2 align='center'>Player Information</h2>";
    html += "<table border='1' cellspacing='0' cellpadding='5' width='100%'>";
    html += "<tr>"
            "<th>Name</th>"
            "<th>Surname</th>"
            "<th>Gender</th>"
            "<th>Date of Birth</th>"
            "<th>Category</th>"
            "<th>Points</th>"
            "</tr>";

    for (Player* p : m_players) {

        html += "<tr>";
        html += "<td>" + p->name() + "</td>";
        html += "<td>" + p->surname() + "</td>";
        html += "<td>" + p->gender() + "</td>";
        html += "<td>" + p->dateOfBirth().toString("yyyy-MM-dd") + "</td>";
        html += "<td>" + p->category() + "</td>";
        html += "<td align='center'>" + QString::number(p->points()) + "</td>";
        html += "</tr>";
    }

    html += "</table>";

    // ===== Print =====
    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);
}
