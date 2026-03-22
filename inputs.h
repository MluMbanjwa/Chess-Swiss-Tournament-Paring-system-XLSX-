#ifndef INPUTS_H
#define INPUTS_H

#include "QLineEdit"
#include <QDateEdit>
#include <QSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <xlsxdocument.h>
#include <QPushButton>
#include <QMessageBox>
#include <QComboBox>
#include <QFileDialog>
#include <QCheckBox>
#include <QGroupBox>
class CategorySelector: public QWidget{
Q_OBJECT
private:
    QVBoxLayout* m_layout;
    QHBoxLayout* m_layCHeck;
    QStringList m_categories;
    QList<QCheckBox*>m_options;
    QGroupBox * m_group;
    QPushButton* m_btnDone;
public:
    CategorySelector(QWidget *parent, QStringList list);
public slots:
    void selected();
signals:
    void categories(QStringList);
};


    class MyLabel : public QLabel
{
    Q_OBJECT

public:
    explicit MyLabel(QWidget *parent = nullptr);
    void setText(const QString &text) ;

signals:
    void textChanged(const QString &newText);
};
class inputs: public QWidget{
    Q_OBJECT
public:
    QFormLayout * layout;
    QDateEdit* date;
    QLineEdit* name;
    QLineEdit* organiser;
    QLineEdit* venue;
    QSpinBox * rounds;
    QComboBox * gender;
    QPushButton* btnLoadFile;
    QLineEdit* sheetName;
    CategorySelector *m_categorySelect;
    MyLabel *fileName;
    QStringList categories;
    QXlsx::Document *m_doc;
    inputs(QWidget *parent);
    ~inputs();
public slots:
    void done(QStringList catego);
    void players();
    void setCategories();
signals:
    void finish();
};
#endif // INPUTS_H
