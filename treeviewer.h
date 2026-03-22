#ifndef TREEVIEWER_H
#define TREEVIEWER_H
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "playerlist.h"
class TreeViewer:public QTreeWidget
{
private:
    QTreeWidgetItem* m_root;
    QList<PlayerList*>m_lists;
public:
    void addRoots();
    void addChild(QTreeWidgetItem* rootItem,Player* p);
    void addPlayerList(PlayerList *list);
    TreeViewer(QWidget *parent);
    void updateLists();
};

#endif // TREEVIEWER_H
