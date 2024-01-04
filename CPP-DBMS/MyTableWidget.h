#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H

#include <QHash>
#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include<QVBoxLayout>
#include <QTableWidgetItem>
#include<QDebug>
#include<QComboBox>
#include<QDateTime>
#include<QTimeEdit>
#include<QVector>
#include<QPair>
#include<QPushButton>
#include <QInputDialog>
#include <QRegularExpressionValidator>
#include<QMessageBox>
#include <QTimer>
#include <QCheckBox>

//for file raeding
#include <QVector>
#include <QStringList>
#include <QFile>
#include <QTextStream>


class MyTableWidget: public QWidget
{
    Q_OBJECT
public:
    explicit MyTableWidget(QWidget *parent = nullptr);
    void createTable(int row,int col);
    void DeleteAndRemake(int row,int col);
    int getColumns();
    QVector<QPair<QString, QChar>>* getTableInitalData();
    void readCSV();
    void writeCSV();
    void createTableForRightSide();
    void DeleteAndRemakeRightTable();
    ~MyTableWidget();

signals:
    void itemEdited(const QString &text);

public slots:
    void receiveData(QString path);

private slots:
    void onItemChangedOfFormTable(QTableWidgetItem *item);
    void onComboBoxIndexChanged(int col, int index);
    // for right side wala table
    void onItemChangedOfRightTable(QTableWidgetItem *item);
    void TakeRowToBeDeleted();
    void addaNewRow();
    void SaveChangesOnTableInFile();
    void onTableFilterComboBoxIndexChanged(int col, int index);
    void onFilterTextChanged(const QString &text);
    void handleTimeChanged(const QTime& time);
    void handletrueFalseComboBoxIndexChanged(int index);

private:
    QString currTablePath;
    QHash<int,QChar> myMap;
    QHash<QChar,QString> myDataTypeMap;
    QTableWidget *tableWidget;
    QVBoxLayout* layout;
    QVector<QPair<QString, QChar>>* colNames;
    QVector<QVector<QString>> *tableData;
    QHBoxLayout *tableFuntionLayout;
    QPushButton *addbut;
    QPushButton *delbut;
    QPushButton *savbut;
    QWidget *buttonContainer;
    //filter box
    QComboBox  *tableFilterComboBox;
    QHBoxLayout *filterLayout;
    QLineEdit *filterInpBox;
    QWidget *tableFilterBox;
    int currColumnIndex;
    bool saveChanges;
};

#endif // MYTABLEWIDGET_H
