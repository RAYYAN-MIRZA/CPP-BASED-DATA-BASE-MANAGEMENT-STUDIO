
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "MyTreeWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // this->setupui();
    this->itsMyFirstTime = true;
    this->currUser = "";
    this->currUserPas = "";
    this->Users = nullptr;
    // nulling
    this->tree = nullptr;
    this->centralWidget = nullptr;
    this->leftLayout = nullptr;

    this->rightLayout = nullptr;
    this->centralLayout = nullptr;

    this->leftWidget = nullptr;
    this->rightWidget = nullptr;
    this->splitter = nullptr;

    this->setupMenuBar();
}
void MainWindow::setupMenuBar()
{

    this->setWindowTitle("C++DBMS");
    // Create a toolbar
    toolBar = addToolBar(tr("Toolbar"));

    // Add buttons to the toolbar
    QAction *connectAction = new QAction(tr("Connect"), this);

    connect(connectAction, &QAction::triggered, this, &MainWindow::ConnectNewUser);
    toolBar->addAction(connectAction);
    toolBar->addSeparator();

    QAction *createNewAction = new QAction(tr("Create New User"), this);
    connect(createNewAction, &QAction::triggered, this, &MainWindow::CreateNewUser);
    toolBar->addAction(createNewAction);
    toolBar->addSeparator();

    QAction *exitAction = new QAction(tr("Exit App"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
    toolBar->addAction(exitAction);
    toolBar->addSeparator();
    // this->setupui();
    this->getUserForm(); // means first time
}

void MainWindow::getUserForm()
{

    this->getAllUsers(false);
    if (this->Users)
    {
        UserLoginForm *formm = new UserLoginForm();
        connect(formm, &UserLoginForm::validUserEntered, this, &MainWindow::userSelected);
        formm->setupUi(0);
        formm->getUsersVector(this->Users);

        if (formm->exec() == QDialog::Accepted)
        {
            qDebug() << "Accepted";

            if (this->itsMyFirstTime == true)
            {
                QAction *currUserSettAction = new QAction(tr("Change User Password"), this);
                connect(currUserSettAction, &QAction::triggered, this, &MainWindow::CurrUserSetting);
                toolBar->addAction(currUserSettAction);
                toolBar->addSeparator();
                QAction *deleteCurrUserAction = new QAction(tr("Delete Current User"), this);
                connect(deleteCurrUserAction, &QAction::triggered, this, &MainWindow::onDeleteCurrUserAction);
                toolBar->addAction(deleteCurrUserAction);
                toolBar->addSeparator();
                this->setupui(false);
                itsMyFirstTime = false;
            }
            else
            {
                qDebug() << "Delete and remake wala call hoorah a";
                deleteAndRemakeUi(false);
            }
        }
        else
        {
            qDebug() << "Rejected form";
        }
        delete formm;
        formm = nullptr;
        delete this->Users;
        this->Users = nullptr;
    }
}

void MainWindow::setupui(bool isDeleted) // set ui ka deleted true arahaaaaa
{
    if (isDeleted)
    {
        qDebug() << "Set up ui ka is deleted= " << isDeleted;
    }
    else
    {
        qDebug() << "Set up ui ka is deleted= " << isDeleted;
    }
    // Create the central widget
    this->centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create the splitter
    this->splitter = new QSplitter(centralWidget);
    // splitter->setStyleSheet("border:1px solid black");

    // Left side (tree widget)
    this->leftLayout = new QVBoxLayout;

    this->createTree(isDeleted);

    leftLayout->addWidget(tree);

    this->leftWidget = new QWidget(this);
    leftWidget->setLayout(leftLayout);

    leftWidget->setMinimumWidth(200);
    leftWidget->setMaximumWidth(400);
    leftWidget->setStyleSheet("border: 0.5px solid grey");

    splitter->addWidget(leftWidget);

    // Right side (table widget)
    this->rightLayout = new QVBoxLayout(this);
    // ye araha hai ek object jo table or tree donu me same hoga

    this->righttable = this->tree->returnCommonObject(); // new try bismillah
    rightLayout->addWidget(righttable);
    this->rightWidget = new QWidget(this);
    this->rightWidget->setLayout(rightLayout);
    this->rightWidget->setStyleSheet("border: 0.5px solid grey");


    this->splitter->addWidget(rightWidget);

    this->centralLayout = new QVBoxLayout(centralWidget);
    this->centralLayout->addWidget(splitter);
}

void MainWindow::deleteAndRemakeUi(bool isDeleted)
{
    this->righttable = nullptr; // cause tree will delete it

    if (this->tree)
    {
        delete this->tree;
        this->tree = nullptr;
    }

    if (this->leftLayout)
    {
        delete this->leftLayout;
        this->leftLayout = nullptr;
    }

    if (this->rightLayout)
    {
        delete this->rightLayout;
        this->rightLayout = nullptr;
    }

    if (this->centralLayout)
    {
        delete this->centralLayout;
        this->centralLayout = nullptr;
    }

    if (this->leftWidget)
    {
        delete this->leftWidget;
        this->leftWidget = nullptr;
    }

    if (this->rightWidget)
    {
        delete this->rightWidget;
        this->rightWidget = nullptr;
    }

    // Delete the splitter
    if (this->splitter)
    {
        delete this->splitter;
        this->splitter = nullptr;
    }
    this->setupui(isDeleted);
}

void MainWindow::createTree(bool ifDeleted)
{
    if (this->tree)
    {
        delete this->tree;
        this->tree = nullptr;
    }
    if (ifDeleted)
    { // means something was deleted
        this->tree = new MyTreeWidget(this);
        this->getAllUsers(true);
        this->tree->getUserName("Sample_User");
        this->tree->starttree();
        delete this->Users;
        this->Users = nullptr;
    }
    else
    {
        this->tree = new MyTreeWidget(this);
        qDebug() << "CUrrent USer =";
        this->tree->getUserName(this->currUser);
        this->tree->starttree();
    }
}

// Function to write to a text file
void MainWindow::writeToTextFile(const QString &filePath, const QString &content)
{
    QFile file(filePath);
    qDebug() << "PAth of txt == " << filePath;
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << content;

        file.close();
        qDebug() << "File written successfully.";
    }
    else
    {
        qDebug() << "Unable to open the file for writing.";
    }
}

// Function to read from a text file
QString MainWindow::readFromTextFile(const QString &filePath)
{
    QFile file(filePath);
    QString content;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        content = in.readAll();

        file.close();
        qDebug() << "File read successfully.";
    }
    else
    {
        qDebug() << "Unable to open the file for reading.";
    }

    return content;
}

void MainWindow::deleteUser() /// dekho isko check ing karo hr chizz ki /or pure tree bhimodify hoga
{
    QString path = this->baseDir + "/" + this->currUser;
    qDebug() << "Delete user speeking == " << path;
    QDir userDir(path);

    if (userDir.exists())
    {
        if (userDir.removeRecursively())
        {
            qDebug() << "User directory deleted successfully";
            QMessageBox::information(this, "Message", "User Deleted Successfully! Redirecting to the Sample_User");
            this->deleteAndRemakeUi(true);
        }
        else
        {
            qDebug() << "Failed to delete user directory";
        }
    }
    else
    {
        qDebug() << "User directory does not exist";    
    }
}

void MainWindow::getAllUsers(bool Sample_User_Banao)
{
    if (this->Users)
    {
        delete this->Users;
        this->Users = nullptr;
    }

    baseDir = QDir::currentPath();
    QString base_folder_name = "All_Users";

    QDir dir(baseDir);

    if (!dir.exists(base_folder_name))
    {

        if (dir.mkdir(base_folder_name))
        {
            qDebug() << "Base Folder created successfully ";
        }
        else
        {
            qDebug() << "Base Folder not created successfuly";
            QMessageBox::warning(this, "Base Folder Creation Error", "Unable To Create Base Folder");
            return;
        }
    }
    else
    {
        qDebug() << "Base Folder Already exists";
    }
    baseDir.push_back('/');
    baseDir += base_folder_name;
    dir.setPath(baseDir);
    QStringList allUsers = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    if (allUsers.empty() || Sample_User_Banao)
    {
        QString sample_user_name = "Sample_User";
        QString currDir = baseDir + "/" + sample_user_name;

        if (!dir.exists(sample_user_name))
        {
            if (dir.mkdir(sample_user_name))
            {

                qDebug() << "Sample Folder created successfully ";
                currDir = currDir + "/" + "Password.txt";
                QString pas = "123";
                this->writeToTextFile(currDir, pas);
            }
            else
            {
                qDebug() << "Sample Folder not created successfuly";
            }
        }
        else
        {
            qDebug() << "Sample Folder Already exists";
        }
    }
    else
    {
        this->Users = new QVector<QPair<QString, QString>>(allUsers.size());
        int i = 0;
        for (const QString &UserName : allUsers)
        {
            qDebug() << "users are:" << UserName;
            (*this->Users)[i].first = UserName;
            QString curr = this->baseDir + "/" + UserName + "/Password.txt";
            (*this->Users)[i].second = this->readFromTextFile(curr);
            i++;
        }
    }
    // allUsers; // string list
}

void MainWindow::userSelected(QString user, QString pass)
{
    qDebug() << "From the Form: " << user;
    this->currUser = user;
    this->currUserPas = pass;
    qDebug() << "USer === " << user;
    qDebug() << "pass === " << pass;

    // ab yaha se ui khule ga tree widget wagera;
}

void MainWindow::ConnectNewUser()
{
    this->getUserForm();
}

void MainWindow::CreateNewUser()
{
    qDebug() << "Create is Speaking";
    UserLoginForm *formm = new UserLoginForm();
    formm->setupUi(1);
    if (formm->exec() == QDialog::Accepted)
    {

        QVector<QString> receivedData = formm->newUserFormData(); // 0 index= user name, 1 index = password
        for (const auto &data : receivedData)
        {
            qDebug() << "rec == " << data;
        }
        //  create New user In Directory
        QDir dir(this->baseDir);
        if (!dir.exists(receivedData[0]))
        {

            if (dir.mkdir(receivedData[0]))
            {

                qDebug() << " Folder ==" << receivedData[0] << "created successfully ";
                QString curr = baseDir + "/" + receivedData[0] + "/" + "Password.txt";
                this->writeToTextFile(curr, receivedData[1]);
                QMessageBox::information(this, "Message", "User Created Successfully! Access from Login Box");
                this->ConnectNewUser();
                // this->deleteAndRemakeUi(false);
            }
            else
            {
                qDebug() << " Folder ==" << receivedData[0] << "NOT created successfully ";
            }
        }
        else
        {
            qDebug() << "User Already exists";
            QMessageBox::warning(this, "Error", "Unable To Create User, User Already Exists!");
            return;
        }
    }
    else
    {
        qDebug() << "User Cancelled ";
    }
    delete formm;
    formm = nullptr;
}

void MainWindow::CurrUserSetting()
{
    qDebug() << "Curent User Setting";
    UserLoginForm *formm = new UserLoginForm();
    formm->setCurrPass(this->currUserPas);
    // q k user aya hi nhi
    formm->setupUi(2);
    if (formm->exec() == QDialog::Accepted)
    {
        qDebug() << "Accepted";
        QString filepath = this->baseDir + "/" + this->currUser + "/" + "Password.txt";
        this->writeToTextFile(filepath, formm->getChangedPass());
    }
    else
    {
        qDebug() << "Form was cancelled ";
    }
    delete formm;
    formm = nullptr;
}

void MainWindow::onDeleteCurrUserAction()
{
    qDebug() << "User wants to deletee";
    // Ask the user a yes/no question
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Oh really? Do you want to Delete?", QMessageBox::Yes | QMessageBox::No);

    // Process the user's choice
    if (reply == QMessageBox::Yes)
    {
        qDebug() << "User clicked Yes";
        this->deleteUser();
    }
    else
    {
        qDebug() << "User clicked No or closed the dialog";
    }
}

MainWindow::~MainWindow()
{
    qDebug() << "Main win destructed";

    // Delete the tree widget
    // this->tree->mytreedelete();

    //  if toolbar
    if (this->toolBar)
    {
        delete this->toolBar;
        this->toolBar = nullptr;
    }

    // form cleaning
    if (this->Users)
    {
        delete this->Users;
        this->Users = nullptr;
    }

    qDebug() << "Main win destructed finally0";

    this->righttable = nullptr;// \\ Cause it will be deleted by tree

    if (this->tree)
    {
        delete this->tree;
        this->tree = nullptr;
    }
    qDebug() << "Main win destructed finally1";


    qDebug() << "Main win destructed finally2";

    if (this->leftLayout)
    {
        delete this->leftLayout;
        this->leftLayout = nullptr;
    }

    if (this->rightLayout)
    {
        delete this->rightLayout;
        this->rightLayout = nullptr;
    }

    qDebug() << "Main win destructed finally4";

    if (this->centralLayout)
    {
        delete this->centralLayout;
        this->centralLayout = nullptr;
    }

    if (this->leftWidget)
    {
        delete this->leftWidget;
        this->leftWidget = nullptr;
    }

    if (this->rightWidget)
    {
        delete this->rightWidget;
        this->rightWidget = nullptr;
    }

    // Delete the splitter
    if (this->splitter)
    {
        delete this->splitter;
        this->splitter = nullptr;
    }

    qDebug() << "Main win destructed finally10";
}


