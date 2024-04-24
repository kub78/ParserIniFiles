#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "IniFile.h"

#include <QDebug>
#include <QAction>
#include <QFileDialog>
#include <QDir>
#include <QListWidgetItem>
#include <QInputDialog>
#include <QMessageBox>

using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //connect(ui->action,&QAction::triggered,this,&MainWindow::On_directory_selection);
    connect(ui->addFile,&QAction::triggered,this,&MainWindow::On_add_file);
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->setHeaderLabels(QStringList() << "key" << "value");
    //ui->treeWidget->setColumnCount(2);
    QStringList ColumnNames;

    //ColumnNames << "Key" << "Value";

    ui->treeWidget->setHeaderLabels(ColumnNames);
    ui->CombBox_add->addItem("int");
    ui->CombBox_add->addItem("double");
    ui->CombBox_add->addItem("string");
    ui->CombBox_add->addItem("bool");
    setWindowTitle("U menya lapki");


}

MainWindow::~MainWindow()
{
    delete ui;
    delete _file;

}

void MainWindow::On_add_file()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                      "./",
                                                      tr("IniFiles (*.ini)"));
    if (_file){
        delete _file;
    }
    if (fileName == nullptr){
        return;
       //QMessageBox::critical(nullptr,"Ошибка","Пустой путь");
    }
    //_file = new IniFile(fileName.toStdString());
    else if (fileName != nullptr){
        _file = new IniFile(fileName.toStdString());
        ui->listWidget->addItem(fileName);
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString section_name = ui->comboBox_selection->currentText();
    QString key = ui->comboBox_delete->currentText();
    if (!section_name.isEmpty() && !key.isEmpty()){
        _file->deleteKey(section_name.toStdString(),key.toStdString());
        showTree();
    }
    else{
        QMessageBox::critical(nullptr,"Ошибка удаления ключа","Невозможно удалить ключ");
    }
}

//void MainWindow::On_directory_selection()
//{

//   QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
//                                                    "/home",
//                                                    QFileDialog::ShowDirsOnly
//                                                     | QFileDialog::DontResolveSymlinks);
//    QDir selecteDir(dir);
//    QStringList filters;
//          filters << "*.ini";
//          selecteDir.setNameFilters(filters);
//     for (const QString &f:selecteDir.entryList()){
//         qDebug() << f;
//         QListWidgetItem* item = new QListWidgetItem(dir + "/" + f);
//         ui->listWidget->addItem(item);

//     }
//}

bool MainWindow::to_bool(std::string &value)
{
    for (size_t i = 0; i < value.size(); i++) {
      if (value[i] >= 'A' && value[i] <= 'Z') {
        value[i] = value[i] - 'A' + 'a';
      }
    }
    for (auto trueValue : trueValues) {
      if (trueValue == value) {
        return true;
      }
    }

    return false;
}

void MainWindow::on_Add_pushButton_clicked()
{
    QString type = ui->CombBox_add->currentText();
    QString key = ui->lineEdit_Key->text();
    QString value = ui->lineEdit_Valuye->text();
    QString sectionName = ui->comboBox_selection->currentText();
    if (!isCorrectString(key.toStdString()) || !isCorrectString(value.toStdString()) || !isCorrectSection(sectionName.toStdString())){
       QMessageBox::critical(this,"Ошибка","Неправильный ключ или значение");
        return;
    }
    if (type == "int"){
        bool ok;
        ui->lineEdit_Key->clear();
        ui->lineEdit_Valuye->clear();
        _file->writeInt(sectionName.toStdString(),key.toStdString(),value.toInt(&ok,10));
        showTree();
    }
    else if(type == "double"){
        ui->lineEdit_Key->clear();
        ui->lineEdit_Valuye->clear();
        bool ok;

        _file->writeInt(sectionName.toStdString(),key.toStdString(),value.toDouble(&ok));
        showTree();

    }
    else if (type == "string"){
        ui->lineEdit_Key->clear();
        ui->lineEdit_Valuye->clear();
        _file->writeString(sectionName.toStdString(),key.toStdString(),value.toStdString());
        showTree();
    }
    else if (type == "bool"){
        string value1 = value.toStdString();

        auto value2 = to_bool(value1);

        _file->writeBool(sectionName.toStdString(),key.toStdString(),value2);

    }
}

void MainWindow::showTree()
{
    ui->treeWidget->clear();
    ui->comboBox_selection->clear();
    for(auto section : _file->getSections()){
        QTreeWidgetItem *itm = new QTreeWidgetItem(ui->treeWidget);
        itm->setText(0,QString::fromStdString(section.first));
        //ui->treeWidget->addTopLevelItem(itm);
        ui->comboBox_selection->addItem(QString::fromStdString(section.first));
        for(auto elem : section.second){
            QTreeWidgetItem *itm1 = new QTreeWidgetItem(itm);
            itm1->setText(0,QString::fromStdString(elem.first));
            itm1->setText(1,QString::fromStdString(elem.second));
            itm->addChild(itm1);
            }
        }
    ui->treeWidget->expandAll();
}
bool MainWindow::isCorrectSection(const std::string &line)
{
  if (line.empty()) {
    return false;
  }
  for (size_t i = 0; i < line.size(); i++) {
    if (line[i] == '=' && line.size() == 1) {
      return true;
    } else {
      for (size_t j = 0; j < line.size(); j++) {
        if ((line[i] == '[' && line[j] == ']') || (line[i] == ']' && line[j] == '[') || line[i] == '=') {
          return false;
        }
      }
    }
  }
  return true;
}

bool MainWindow::isCorrectString(const std::string &line) {
  if (line.empty()) {
    return false;
  }
  for (size_t i = 0; i < line.size(); i++) {
    for (size_t j = 0; j < line.size(); j++) {
      if ((line[i] == '[' && line[j] == ']') || (line[i] == ']' && line[j] == '[') || line[i] == '=') {
        return false;
      }
    }
  }
  return true;
}
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString filename = item->text();
    if (_file){
        delete _file;
    }
    _file = new IniFile(filename.toStdString());
    showTree();
}

void MainWindow::on_pushButtot_Selectiong_on_clicked()
{
    if (!_file){
        return;
    }

    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                              tr("Section Name"), QLineEdit::Normal,
                                              QDir::home().dirName(), &ok);
    if (!isCorrectSection(text.toStdString())){
        QMessageBox::critical(this,"Ошибка","Неправильная секция");

    }
    if (ok && !text.isEmpty()){
        //ui->comboBox_selection->addItem(text);
        _file->addSection(text.toStdString());
        showTree();
        //AddRoot(text);
    }
}

void MainWindow::on_pushButton_selection_off_clicked()
{
    std::string section  = ui->comboBox_selection->currentText().toStdString();
    if (!section.empty()){
        _file->deleteSection(section);
        showTree();
    }
        else{
            QMessageBox::critical(this,"Ошибка","Неправильное удаление");
        }

}

void MainWindow::on_comboBox_selection_currentTextChanged(const QString &arg1)
{
    ui->comboBox_delete->clear();
   auto sections = _file->getSections()[arg1.toStdString()];
   for(auto elem : sections){
         ui->comboBox_delete->addItem(QString::fromStdString(elem.first));
   }
}

void MainWindow::on_CreateFile_triggered()
{
    QString fileName1 = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                      "./jana",
                                                      tr("IniFiles (*.ini)"));
    if (fileName1 == nullptr){
        return;
    }
    if (!fileName1.isEmpty()){
       QFile file(fileName1);
       ui->listWidget->addItem(fileName1);
    }

}

void MainWindow::on_Save_triggered()
{
    if (_file){
       _file->save();

    }
    else{
        QMessageBox::critical(nullptr,"Ошибка сохранения","Файл не был сохранён");
    }
}

void MainWindow::on_CombBox_add_currentTextChanged(const QString &arg1)
{
    if (arg1 == "int"){
        QValidator *validator = new QIntValidator(-1000, 10000, this);
        ui->lineEdit_Valuye->setValidator(validator);
    }
    else if (arg1 == "double"){
        QDoubleValidator *doublevalidator = new QDoubleValidator(-1000,10000,6,this);
        ui->lineEdit_Valuye->setValidator(doublevalidator);
    }
    else if(arg1 == "string"){
        QRegExp rx("^[A-Za-z]+$");
        QValidator *stringvalidator = new QRegExpValidator(rx, this);
        ui->lineEdit_Valuye->setValidator(stringvalidator);
    }
    else if(arg1 == "bool"){
       QRegExp rx("[Tt][Rr][Uu][Ee]|[Ff][Aa][Ll][Ss][Ee]|[Yy][Ee][Ss]|[Nn][Oo]|[0-1]");
       QValidator *boolvalidator = new QRegExpValidator(rx, this);
       ui->lineEdit_Valuye->setValidator(boolvalidator);
    }
}

void MainWindow::on_listWidget_itemActivated(QListWidgetItem *item)
{
    if(_file){
            delete _file;
        }
    _file = new IniFile(item->text().toStdString());
    showTree();
}

void MainWindow::on_directory_selection_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty()){
        return;
    }
    else{

         QDir selecteDir(dir);
         QStringList filters;
               filters << "*.ini";
               selecteDir.setNameFilters(filters);
          for (const QString &f:selecteDir.entryList()){
              //qDebug() << f;
              QListWidgetItem* item = new QListWidgetItem(dir + "/" + f);
                ui->listWidget->addItem(item);
          }
    }
}


