#include "inputs.h"


inputs::~inputs()
{
    deleteLater();
}

void inputs::done(QStringList catego)
{
    categories.clear();
    foreach (QString cat, catego) {
        categories<<cat;
    }
    emit finish();
    close();
}

void inputs::players()
{
    QString fName = QFileDialog::getOpenFileName(
        this,
        "Open File",
        "",
        "excel Files (*.xlsx)"
        );

    if (!fName.isEmpty()) {
        qDebug() << "Selected file:" << fName;
    }
    fileName->setText(fName);
}

void inputs::setCategories()
{
    m_doc=new QXlsx::Document(fileName->text());
    if (!m_doc->load()) {
        QMessageBox::warning(0, "Error", "Failed to load file: " + fileName->text());
        return;
    }
    if (!m_doc->selectSheet(sheetName->text())) {
        QMessageBox::warning(0,"ERROR READING: "+fileName->text(),"Sheet not found!");
    }
    QString text;

    for(int i=2;i<m_doc->dimension().rowCount();i++){
        text=m_doc->read(i,9).toString().trimmed();
        auto cell = m_doc->cellAt(i, 9);
         text = cell ? cell->value().toString().trimmed() : "";
        if(!categories.contains(text)){
            categories<<text;
        }
    }
    m_categorySelect=new CategorySelector(this,categories);
    layout->addWidget(m_categorySelect);
    connect(m_categorySelect,SIGNAL(categories(QStringList)),this,SLOT(done(QStringList)));
}
MyLabel::MyLabel(QWidget *parent)
    : QLabel(parent)
{
}

void MyLabel::setText(const QString &text)
{
    if (text != this->text()) {
        QLabel::setText(text);
        emit textChanged(text);
    } else {
        QLabel::setText(text); // optional, keep behavior consistent
    }
}
inputs::inputs(QWidget *parent): QWidget(parent)
{
    layout=new QFormLayout(this);
    name=new QLineEdit(this);
    date=new QDateEdit(this);
    gender=new QComboBox(this);
    date->setDate(QDate::currentDate());
    organiser=new QLineEdit(this);
    venue=new QLineEdit(this);
    rounds=new QSpinBox(this);
    fileName=new MyLabel(this);
    rounds->setMinimum(1);
    gender->addItems({"","male","female"});
    sheetName=new QLineEdit("PLayers for team trials",this);
    btnLoadFile=new QPushButton("Players",this);

    layout->addRow("Tournament Name",name);
    layout->addRow("Date",date);
    layout->addRow("Organiser:",organiser);
    layout->addRow("Venue",venue);
    layout->addRow("rounds: ",rounds);
    layout->addRow("Gender",gender);
    layout->addRow("Click to choose file(excel): ",btnLoadFile);
    layout->addRow("sheet Name(in the file)",sheetName);


    connect(fileName,SIGNAL(textChanged(QString)),this,SLOT(setCategories()));
    connect(btnLoadFile,SIGNAL(clicked(bool)),this,SLOT(players()));

    setLayout(layout);
    show();
}

CategorySelector::CategorySelector(QWidget* parent,QStringList list):QWidget(parent)
{
    m_layout=new QVBoxLayout(this);

    m_group=new QGroupBox(this);
    m_btnDone=new QPushButton("Finish",this);
    m_layCHeck=new QHBoxLayout(m_group);
    QCheckBox* newC;
    foreach (QString i, list) {
        newC=new QCheckBox(i,this);
        m_layCHeck->addWidget(newC);
        m_options.push_back(newC);
    }
    m_layout->addWidget(m_group);
    m_layout->addWidget(m_btnDone);
    connect(m_btnDone,SIGNAL(clicked(bool)),this,SLOT(selected()));

    setLayout(m_layout);
    show();
}

void CategorySelector::selected()
{
    m_categories.clear();
    foreach(QCheckBox * i,m_options){
        if(i->isChecked()){
            m_categories.push_back(i->text());
        }
    }
    emit categories(m_categories);

}
