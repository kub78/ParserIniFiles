#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QAction>
#include <QFileDialog>
#include <QtCore>
#include <QtGui>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class QTreeWidgetItem;
class QListWidgetItem;
class IniFile;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    //QTreeWidgetItem* AddRoot(QString selection);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void On_add_file();
    void on_pushButton_clicked();
    //void On_directory_selection();
    void on_Add_pushButton_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButtot_Selectiong_on_clicked();

    void on_pushButton_selection_off_clicked();

    void on_comboBox_selection_currentTextChanged(const QString &arg1);

    void on_Save_triggered();

    //void on_action_4_triggered();

    void on_CreateFile_triggered();

    void on_CombBox_add_currentTextChanged(const QString &arg1);

    void on_directory_selection_triggered();

    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_Close_triggered();

private:
        void showTree();
        bool to_bool(std::string &s);
        bool isCorrectSection(const std::string &line);
        bool isCorrectString(const std::string &line);

private:
    Ui::MainWindow *ui;
    IniFile* _file = nullptr;
};
#endif // MAINWINDOW_H
