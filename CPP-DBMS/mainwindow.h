// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #include <QMainWindow>

// QT_BEGIN_NAMESPACE
// namespace Ui {
// class MainWindow;
// }
// QT_END_NAMESPACE

// class MainWindow : public QMainWindow
// {
//     Q_OBJECT

// public:
//     MainWindow(QWidget *parent = nullptr);
//     ~MainWindow();

// private:
//     Ui::MainWindow *ui;
// };
// #endif // MAINWINDOW_H
///////////////////////////////////////////

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<iostream>
#include <QMainWindow>
#include<QTreeWidget>
#include<QTreeWidgetItem>
#include<QVBoxLayout>
#include<QDebug>
#include<QSplitter>
#include<QVBoxLayout>
#include<QTableWidget>
#include<QPushButton>
#include<QHBoxLayout>
#include<QWidget>
#include<QMouseEvent>
#include<QDir>
#include<QMessageBox>
#include<QVector>

#include"MultiStepForm.h"
#include"MyTreeWidget.h"
#include"MyTableWidget.h"
#include"UserLoginForm.h"
#include <QMenuBar>
#include <QMenu>
#include<QToolBar>
#include <QAction>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void setupMenuBar();
    void setupui(bool isDeleted);
    void deleteAndRemakeUi(bool ifDeleted);
    void createTree(bool ifDeleted);
    void getUserForm();
    void getAllUsers(bool Sample_User_Banao);
    void writeToTextFile(const QString& filePath, const QString& content);
    QString readFromTextFile(const QString& filePath);
    void changeTable();
    void deleteUser();
    // void mousePressEvent(QMouseEvent *event);
    ~MainWindow();

public slots:
    void userSelected(QString user,QString pass);

private slots:
    void ConnectNewUser();
    void CreateNewUser();
    void CurrUserSetting();
    void onDeleteCurrUserAction();

private:
    Ui::MainWindow *ui;
    MultiStepForm* form;
    MyTableWidget *righttable;
    QWidget *centralWidget;
    QSplitter *splitter;
    QVBoxLayout *leftLayout;
    MyTreeWidget *tree;
    QWidget *leftWidget ;
    QVBoxLayout *rightLayout ;
    QWidget *rightWidget;
    QVBoxLayout *centralLayout;
    QString baseDir;
    QString currUser,currUserPas;
    QToolBar *toolBar ;
    QVector<QPair<QString,QString>> *Users;
    bool itsMyFirstTime;
};

#endif // MAINWINDOW_H
