#ifndef USERLOGINFORM_H
#define USERLOGINFORM_H
#include<QDialog>
#include<QDebug>
#include <QObject>
#include<QWidget>
#include<QLabel>
#include<QComboBox>
#include<QLineEdit>
#include<QPushButton>
#include<QVBoxLayout>
#include<QMessageBox>
#include<QIcon>


// function to read/write
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>


class UserLoginForm:public QDialog
{
    Q_OBJECT
public:
    explicit UserLoginForm(QDialog *parent = nullptr);
    void setupUi(int formIndicator);
    void createLoginForm();
    void createNewUserForm();
    void createUserSettingForm();
    void getUsersVector(QVector<QPair<QString, QString>> *Users);
    QString readFromTextFile(const QString& filePath);
    void writeToTextFile(const QString& filePath, const QString& content);
    QVector<QString> newUserFormData();
    void setCurrPass(QString Pass);
    QString getChangedPass();
    ~UserLoginForm();

signals:
    void validUserEntered(QString userName,QString pass); // to external classPressed();

private slots:
    void onUsersComboBoxIndexChanged(int index);
    void passwordEntered(const QString &text);
    void connectButtonPressed();
    void createButtonClicked();
    void resetButtonClicked();

private:
    QVBoxLayout *layout;
    // Step 1
    QWidget *step1;
    QVBoxLayout *step1Layout;
    QLabel *topTextLabel;
    QLabel *label1;
    QComboBox *userCombo;
    QLabel *label2;
    QLineEdit *lineEdit1,*lineEdit2;
    QPushButton *connectButton,*createButton,*resetButton;
    QLabel *errorMessageLabel;
    int currIndexSeleted;
    QVector<QPair<QString, QString>> *usersVector;
    QString currPass;
};

#endif // USERLOGINFORM_H
