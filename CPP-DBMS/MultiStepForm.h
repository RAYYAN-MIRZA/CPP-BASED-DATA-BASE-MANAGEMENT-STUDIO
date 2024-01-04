#ifndef MULTISTEPFORM_H
#define MULTISTEPFORM_H

#include <QObject>
#include <QWidget>
#include<QDialog>
#include <QStackedWidget>
#include <QVBoxLayout>
#include<QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include<QDebug>
#include <QLineEdit> // Include QLineEdit
#include"MyTableWidget.h"

class MultiStepForm : public QDialog {
    Q_OBJECT
public:
    explicit MultiStepForm(QDialog *parent = nullptr);
    QString getTableName();
    QVector<QPair<QString, QChar>>* getTableData();
    ~MultiStepForm();

private slots:
    void nextStep();
    void previousStep();
    void saveInformation();
    void onLineEdit1TextChanged(const QString &text);
    // void onLineEdit2TextChanged(const QString &text){
    // }
private:
    void setupUi();

private:
    QLineEdit *lineEdit1;
    QLineEdit *lineEdit2;
    QLabel *label1;//
    QLabel *label2;//use kar donu ko be
    QLabel *label3;
    QStackedWidget *stackedWidget;
    MyTableWidget *tab;
    QVBoxLayout *layout;
    QVector<QPair<QString, QChar>>* tableData;
    bool first;
    int previnputcols;


};
#endif // MULTISTEPFORM_H
