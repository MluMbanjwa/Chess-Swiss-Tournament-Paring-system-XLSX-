#include "treeviewer.h"

TreeViewer::TreeViewer(QWidget *parent):QTreeWidget(parent) {

    this->setColumnCount(2);
    this->setHeaderLabels({"Tournament","Player information"});

}

void TreeViewer::updateLists()
{
    foreach (PlayerList* l, m_lists) {
        l->sortPlayersByScore();
    }
    this->clear();
    addRoots();
}
void TreeViewer::addRoots()
{

    QTreeWidgetItem * root;
    foreach (PlayerList* l, m_lists) {
        root=new QTreeWidgetItem(this);
        root->setExpanded(true);
        root->setText(0,l->name());
        foreach (Player* p, l->players()) {
            addChild(root,p);
        }
    }
}
void TreeViewer::addChild(QTreeWidgetItem* rootItem,Player *p)
{
    // Create player item under root
    QTreeWidgetItem *item = new QTreeWidgetItem(rootItem);
    item->setText(0, p->name() + " " + p->surname());
    item->setText(1, QString::number(p->points())+" points");

    QTreeWidgetItem *child0 = new QTreeWidgetItem(item);
    child0->setText(0,"Gender");
    child0->setText(1,p->gender());

    QTreeWidgetItem *child1 = new QTreeWidgetItem(item);
    child1->setText(0,"DOB");
    child1->setText(1, p->dob());

    QTreeWidgetItem *child2 = new QTreeWidgetItem(item);
    child2->setText(0, "Games");
    QString games="";
    foreach (Game* g, p->gameRecords()) {
        games+=g->toString()+", ";
    }
    games=games.trimmed();
    games.removeLast();
    child2->setText(1, games);

}

void TreeViewer::addPlayerList(PlayerList *list)
{

    if(!m_lists.contains(list)){
        m_lists.push_back(list);
    }
}


