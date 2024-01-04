#include "UserLoginForm.h"

UserLoginForm::UserLoginForm(QDialog *parent)
    : QDialog{parent}
{
    // this->setupUi();
}

void UserLoginForm::setupUi(int formIndicator)
{
    if (formIndicator == 0)
    {
        this->currIndexSeleted = -1;
        this->usersVector = nullptr;
        qDebug() << "Form is creating ";
        this->createLoginForm();
    }
    else if (formIndicator == 1)
    {
        qDebug() << "Indicator  === 1";
        this->createNewUserForm();
    }
    else if (formIndicator == 2)
    {
        qDebug() << "Indicator  === 2";
        this->createUserSettingForm();
    }
}

void UserLoginForm::createLoginForm()
{
    qDebug() << "form bana1";
    layout = new QVBoxLayout(this);

    // Step 1
    step1 = new QWidget(this);
    step1Layout = new QVBoxLayout(step1);


    QLabel *topTextLabel = new QLabel("Welcome to C++DBMS", this);
    // Create a QFont object with bold style and increased font size
    QFont boldFont = topTextLabel->font();
    boldFont.setBold(true);
    boldFont.setPointSize(boldFont.pointSize() + 2); // Increase the font size by 2 points

    // Set the QFont object to the QLabel
    topTextLabel->setFont(boldFont);

    // Set the font color to blue
    topTextLabel->setStyleSheet("color: blue;");

    topTextLabel->setAlignment(Qt::AlignHCenter);
    layout->addWidget(topTextLabel);

    label1 = new QLabel("Available Users:(For First Time:Select Sample_User)", step1);
    userCombo = new QComboBox(this);
    userCombo->setPlaceholderText("Select User");

    // Capture 'col' in the lambda capture list
    connect(userCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &UserLoginForm::onUsersComboBoxIndexChanged);

    label2 = new QLabel("Enter Password:(For Sample_User:(Password : 123))", step1);
    lineEdit2 = new QLineEdit(step1);
    lineEdit2->setEchoMode(QLineEdit::Password);
    connect(lineEdit2, &QLineEdit::textChanged, this, &UserLoginForm::passwordEntered);
    errorMessageLabel = new QLabel(this);
    errorMessageLabel->setStyleSheet("color: red");
    errorMessageLabel->hide(); // Initially hide the label

    connectButton = new QPushButton("Connect", step1);

    // Add widgets to step1Layout
    step1Layout->addWidget(label1);
    step1Layout->addWidget(userCombo);
    step1Layout->addWidget(label2);
    step1Layout->addWidget(errorMessageLabel);
    step1Layout->addWidget(lineEdit2);
    step1Layout->addStretch(1);
    step1Layout->addWidget(connectButton);

    connect(this->connectButton, &QPushButton::clicked, this, &UserLoginForm::connectButtonPressed);
    // Set layout for step1
    step1->setLayout(step1Layout);

    // Add step1 to the main layout
    layout->addWidget(step1);

    // Set the main layout for the dialog
    this->setWindowTitle("Login");
    setLayout(layout);
}

void UserLoginForm::createNewUserForm()
{
    this->setWindowTitle("Create New User");
    qDebug() << "form bana1";
    layout = new QVBoxLayout(this);

    step1 = new QWidget(this);
    step1Layout = new QVBoxLayout(step1);

    label1 = new QLabel("Enter New User Name:", step1);
    lineEdit1 = new QLineEdit(step1);

    label2 = new QLabel("Set Password:(Remember it :)", step1);
    lineEdit2 = new QLineEdit(step1);

    createButton = new QPushButton("Create", step1);
    connect(createButton, &QPushButton::clicked, this, &UserLoginForm::createButtonClicked);

    // Add widgets to step1Layout
    step1Layout->addWidget(label1);
    step1Layout->addWidget(lineEdit1);
    step1Layout->addWidget(label2);
    step1Layout->addWidget(lineEdit2);
    step1Layout->addStretch(1);
    step1Layout->addWidget(createButton);

    // Set layout for step1
    step1->setLayout(step1Layout);

    // Add step1 to the main layout
    layout->addWidget(step1);

    // Set the main layout for the dialog
    setLayout(layout);
}

void UserLoginForm::createUserSettingForm()
{
    this->setWindowTitle("User Settings");
    qDebug() << "form bana1";
    layout = new QVBoxLayout(this);

    step1 = new QWidget(this);
    step1Layout = new QVBoxLayout(step1);

    label1 = new QLabel("Enter Old Password:", step1);
    lineEdit1 = new QLineEdit(step1);

    label2 = new QLabel("Enter New Password:(Remember it :)", step1);
    lineEdit2 = new QLineEdit(step1);

    this->resetButton = new QPushButton("Reset", step1);
    connect(this->resetButton, &QPushButton::clicked, this, &UserLoginForm::resetButtonClicked);

    errorMessageLabel = new QLabel(this);
    errorMessageLabel->setStyleSheet("color: red");
    errorMessageLabel->hide(); // Initially hide the label

    // Add widgets to step1Layout
    step1Layout->addWidget(label1);
    step1Layout->addWidget(errorMessageLabel);
    step1Layout->addWidget(lineEdit1);
    step1Layout->addWidget(label2);
    step1Layout->addWidget(lineEdit2);
    step1Layout->addStretch(1);
    step1Layout->addWidget(resetButton);

    // Set layout for step1
    step1->setLayout(step1Layout);

    // Add step1 to the main layout
    layout->addWidget(step1);

    // Set the main layout for the dialog
    setLayout(layout);
}

void UserLoginForm::passwordEntered(const QString &text)
{
    errorMessageLabel->setVisible(false);
    qDebug() << "pass ==" << text;
}

void UserLoginForm::connectButtonPressed()
{
    qDebug() << "Connect Pressed";
    if (this->currIndexSeleted == -1)
    {
        this->errorMessageLabel->setText("Select A User First");
        this->errorMessageLabel->setVisible(true);
        return;
    }
    qDebug() << "pass in vector.second == " << (*this->usersVector)[currIndexSeleted].second;
    if ((*this->usersVector)[currIndexSeleted].second == this->lineEdit2->text())
    {
        qDebug() << "Sample User agaya";
        //         signal emit hoga
        emit validUserEntered((*this->usersVector)[currIndexSeleted].first, (*this->usersVector)[currIndexSeleted].second);
        this->accept();
    }
    else
    {
        qDebug() << "Incorrect Password";
        this->errorMessageLabel->setText("Incorrect Password");
        this->errorMessageLabel->setVisible(true);
    }
}

void UserLoginForm::createButtonClicked()
{
    qDebug() << "createbutt clicked";
    if (lineEdit1->text().size() == 0 || lineEdit2->text().size() == 0)
    {
        QMessageBox::warning(this, "Invalid Input", "No empty User or Password allowed!");
        return;
    }
    else
    {
        this->accept();
    }
}

void UserLoginForm::resetButtonClicked()
{
    qDebug() << "Reset Butt clicked";
    if (this->lineEdit1->text() != this->currPass)
    {
        qDebug() << "incorred password";
        this->errorMessageLabel->setText("Password Does Not Match the Old Password");
        this->errorMessageLabel->setVisible(true);
        return;
    }
    if (this->lineEdit2->text() == this->currPass)
    {
        this->errorMessageLabel->setText("Cannot Reuse Old Password");
        this->errorMessageLabel->setVisible(true);
        qDebug() << "OLD password entered again";
        return;
    }
    this->currPass = this->lineEdit2->text();
    this->accept();
}

void UserLoginForm::onUsersComboBoxIndexChanged(int index)
{
    qDebug() << "index = " << index;
    this->currIndexSeleted = index;
}

void UserLoginForm::getUsersVector(QVector<QPair<QString, QString>> *Users)
{ // first = column name
    usersVector = Users;
    for (int i = 0; i < (*Users).size(); i++)
        this->userCombo->addItem((*Users)[i].first);
}

QVector<QString> UserLoginForm::newUserFormData()
{
    QVector<QString> toRet;
    toRet.push_back(lineEdit1->text());
    toRet.push_back(lineEdit2->text());
    return toRet;
}

void UserLoginForm::setCurrPass(QString Pass)
{
    this->currPass = Pass;
}

QString UserLoginForm::getChangedPass()
{
    return currPass;
}

UserLoginForm::~UserLoginForm()
{
    delete this->layout;
    qDebug() << "jarha form";

    // krna hai iska bhi kuch
}
