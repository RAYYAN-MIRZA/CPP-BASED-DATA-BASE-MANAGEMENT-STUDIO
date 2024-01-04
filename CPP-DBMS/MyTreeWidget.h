#ifndef MYTREEWIDGET_H
#define MYTREEWIDGET_H

#include<QTreeWidgetItem>
#include<QTreeWidget>
#include<iostream>
#include <QObject>
#include <QWidget>
#include<QTableWidget>
#include<QTreeWidgetItem>
#include<QDir>
#include<QFile>
#include<QMenu>
#include<QVariant>
#include<QMouseEvent>
#include <QInputDialog>
#include <QMessageBox>
#include"MultiStepForm.h"
#include<QTextStream>

class MyTableWidget;

class MyTreeWidget:public QWidget
{
    Q_OBJECT
public:
    explicit MyTreeWidget(QWidget *parent = nullptr);
    void starttree();
    void mousePressEvent(QMouseEvent *event);
    void getDataBases();
    void createDatabase(QString db_name);
    void CreateTable(QTreeWidgetItem* &db,QString db_name,QString tablename);
    void GetTables(QString database,QTreeWidgetItem* &curr_root);
    void showInputDialog(int caller,QTreeWidgetItem *item);
    void getUserName(QString user);
    void mytreedelete();
    MyTableWidget *returnCommonObject();
    ~MyTreeWidget();

signals:
    void signalmain(QString a);
    void sendDataToTable(QString path); // to external class


private slots:
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void onItemChanged(QTreeWidgetItem *item, int column);
    void DeleteTb(QTreeWidgetItem *item);
    void RenameDb(QTreeWidgetItem *item);
    void DeleteDb(QTreeWidgetItem *item);
    void RenameTb(QTreeWidgetItem *item);
    void deleteTreeItem(QTreeWidgetItem* item);

private://insb ko release krna hai
    QTreeWidget* tree;
    QString basedir; // path of the base app
    QString currdir; // path of all current databases
    QString currUserName;
    int user;
    int all_db;
    int indiv_db;// identifier for the db tree widget
    int indiv_tb;// same
    QString to_be_renamed;
    QVector<QPair<QString, QChar>> *tabledata;
    MultiStepForm *form;
    MyTableWidget *tablewid;
};
#endif // MYTREEWIDGET_H
