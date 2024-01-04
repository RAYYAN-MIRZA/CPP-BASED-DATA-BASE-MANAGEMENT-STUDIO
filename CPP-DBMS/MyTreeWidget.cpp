
#include "MyTreeWidget.h"
#include "MyTableWidget.h"
MyTreeWidget::MyTreeWidget(QWidget *parent)
    : QWidget{parent}
{
    qDebug() << "HUluuhhuhuhuhuhuh";
    this->tree = nullptr;
    // this->tablewid=new MyTableWidget();// one which will be shared
    this->currUserName = "";
    this->tabledata = nullptr;
    this->form = nullptr;
    this->tablewid = nullptr;
}
void MyTreeWidget::starttree()
{
    //  unique identifier for the tree Widget
    user = -1;
    indiv_db = 1;
    indiv_tb = 2;
    all_db = 3;
    to_be_renamed = "";
    //  to make/get the folder for all databases for a single user
    this->basedir = QDir::currentPath();
    this->basedir.push_back('/');
    this->basedir = this->basedir + "All_Users";

    this->currdir = this->basedir;
    this->currdir.push_back('/');
    this->currdir = this->currdir + this->currUserName;
    qDebug() << "Curr is ==" << this->currdir;

    // till here base folder created for storing data
    setContextMenuPolicy(Qt::CustomContextMenu);
    this->tree = new QTreeWidget(this);
    this->tree->setContextMenuPolicy(Qt::CustomContextMenu);
    this->tree->setColumnCount(1);
    tree->setHeaderLabel("Object Explorer");

    // Root item
    QTreeWidgetItem *root = new QTreeWidgetItem();
    // this->tree->setHeaderItem(root);

    root->setData(0, Qt::UserRole, QVariant(user));
    root->setText(0, this->currUserName);
    tree->addTopLevelItem(root);

    // Child of root
    QTreeWidgetItem *child = new QTreeWidgetItem();
    child->setText(0, "DataBases");
    child->setData(0, Qt::UserRole, QVariant(all_db));
    root->addChild(child);

    // ghaltiyan
    //  connect(this, &MainWindow::customContextMenuRequested, this, &MainWindow::chalo);

    qDebug() << "Is customContextMenuRequested connected ?: " << connect(this->tree, &QTreeWidget::customContextMenuRequested, this, &MyTreeWidget::on_treeWidget_customContextMenuRequested);

    qDebug() << "Connecting tree widget Edit : " << connect(this->tree, &QTreeWidget::itemChanged, this, &MyTreeWidget::onItemChanged);

    this->tablewid = new MyTableWidget(this);
    qDebug() << "Is tree connected to Mytablewidget ?: " << connect(this, &MyTreeWidget::sendDataToTable, this->tablewid, &MyTableWidget::receiveData);
    this->getDataBases();
}

void MyTreeWidget::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    static bool editcheck = false;
    int topLevelItemCount = tree->topLevelItemCount();
    qDebug() << "top level count== " << topLevelItemCount;
    QTreeWidgetItem *clickedItem = tree->itemAt(pos);
    QTreeWidgetItem *parentItem = this->tree->topLevelItem(1);
    if (parentItem)
    {
        qDebug() << "parent is ===" << parentItem->data(0, Qt::UserRole);
    }
    else
    {
        qDebug() << "parent isnullllll";
    }
    if (clickedItem)
    {
        QVariant ItemType = clickedItem->data(0, Qt::UserRole);
        int itemType = ItemType.toInt();
        std::cout << "Item-Type == " << itemType << std::endl;

        QMenu contextMenu(this);
        if (itemType == user)
        {
            qDebug() << "User clicked";
            contextMenu.addAction("User Action 1");
            contextMenu.addAction("User Action 2");
        }
        else if (itemType == all_db)
        {
            // Example: Add actions specific to the databases level
            qDebug() << " all Databases  clicked";
            contextMenu.addAction("Create New Database");
            contextMenu.addAction("Refresh Databases");
        }
        else if (itemType == indiv_db)
        {
            // Example: Add actions specific to the databases level
            qDebug() << "Indiv db  clicked";
            contextMenu.addAction("Create New Table");
            contextMenu.addAction("Rename DataBase");
            contextMenu.addAction("Delete DataBase");
        }
        else if (itemType == indiv_tb)
        {
            qDebug() << "Indiv tb  clicked";
            contextMenu.addAction("View/Edit Table");
            contextMenu.addAction("Rename Table");
            contextMenu.addAction("Delete Table");
        }
        else
            contextMenu.addAction("Default Action");

        QPoint globalPos = tree->mapToGlobal(pos);
        QAction *selectedAction = contextMenu.exec(globalPos);

        // Handle the selected action as needed
        if (selectedAction)
        {
            if (itemType == user)
            {
                // Handle actions specific to the user level
                if (selectedAction->text() == "User Action 1")
                {
                    // Perform action 1 for the user level
                }
                else if (selectedAction->text() == "User Action 2")
                {
                    // Perform action 2 for the user level
                }
            }
            else if (itemType == all_db)
            {
                // Handle actions specific to the databases level
                if (selectedAction->text() == "Create New Database")
                {
                    // this->hulu("db#");
                    this->showInputDialog(all_db, clickedItem);
                }
            }
            else if (itemType == indiv_db)
            {
                if (selectedAction->text() == "Create New Table")
                {
                    this->form = new MultiStepForm();
                    // Use exec() instead of show()
                    qDebug() << "form starts here";
                    if (this->form->exec() == QDialog::Accepted)
                    {
                        qDebug() << "printing shurhurhu";
                        // Code to execute after the form is closed and user has saved the info
                        this->tabledata = this->form->getTableData();
                        for (int i = 0; i < (*this->tabledata).size(); i++)
                        {
                            qDebug() << "name == " << (*this->tabledata)[i].first << " type ==" << (*this->tabledata)[i].second;
                        }
                        qDebug() << "jarahi file banney";
                        this->CreateTable(clickedItem, clickedItem->text(0), this->form->getTableName());
                    }
                }
                else if (selectedAction->text() == "Rename DataBase")
                {
                    this->tree->editItem(clickedItem);
                    this->to_be_renamed = clickedItem->text(0);
                }
                else if (selectedAction->text() == "Delete DataBase")
                {
                    qDebug() << "dlete hone ja rhai Db";
                    this->DeleteDb(clickedItem);
                }
            }
            else if (itemType == indiv_tb)
            {
                if (selectedAction->text() == "Rename Table")
                {
                    qDebug() << "Rename clicked";
                    this->tree->editItem(clickedItem);
                    this->to_be_renamed = clickedItem->text(0);
                }
                else if (selectedAction->text() == "Delete Table")
                {
                    qDebug() << "Action tha delete";
                    this->DeleteTb(clickedItem);
                }
                else if (selectedAction->text() == "View/Edit Table")
                {
                    //       yaha pe bool wale variable se save wagera krne ka option do table me changes
                    QTreeWidgetItem *db = clickedItem->parent();
                    if (db)
                    {
                        qDebug() << db->text(0) << "and  I am the father ";
                    }
                    else
                    {
                        qDebug() << "Invalid PArent;";
                        return;
                    }
                    // Construct the new path
                    QString Path = this->currdir + "/" + db->text(0) + "/" + clickedItem->text(0) + ".csv";
                    emit sendDataToTable(Path);
                }
            }
        }
    }
    else
    {
        qDebug() << "clcikd hi null hia sala ";
    }
}

MyTableWidget *MyTreeWidget::returnCommonObject()
{ // return the object which will be used for right side
    return this->tablewid;
}

void MyTreeWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        emit tree->customContextMenuRequested(event->pos());
    }
    else if (event->button() == Qt::LeftButton)
    {
        qDebug() << "left mouse clicked";
    }
}

// Edit Functionality
void MyTreeWidget::onItemChanged(QTreeWidgetItem *item, int column)
{
    qDebug() << "edit hua ";
    if (item && column == 0)
    {
        // Assuming you are interested in changes to the first column (column index 0)
        QVariant ItemType = item->data(0, Qt::UserRole);
        if (ItemType == indiv_db)
        {
            qDebug() << "A db is clicked";
            this->RenameDb(item);
        }
        else if (ItemType == indiv_tb)
        {
            qDebug() << "Any table is clicked ";
            this->RenameTb(item);
        }
        QString newText = item->text(column);
        qDebug() << "Item changed: " << newText;
        // You can handle the changed text as needed
    }
}

void MyTreeWidget::showInputDialog(int caller, QTreeWidgetItem *item)
{
    bool ok;
    if (caller == all_db)
    {
        QString text = QInputDialog::getText(
            this,
            tr("DataBase Creator"),
            tr("Enter DataBase Name:"),
            QLineEdit::Normal,
            QString(),
            &ok);

        if (ok && !text.isEmpty())
        {
            // Handle the input, e.g., save it or perform an action
            qDebug() << "Input received:" << text;
            this->createDatabase(text);
        }
        else
        {
            qDebug() << "User canceled the input.";
        }
    }
}

void MyTreeWidget::getUserName(QString user)
{
    this->currUserName = user;
}

void MyTreeWidget::CreateTable(QTreeWidgetItem *&db, QString db_name, QString tablename)
{
    QString path = this->currdir + "/" + db_name + "/" + tablename + ".csv";
    qDebug() << "path == " << path;
    QFile file(path);

    // Open the file in write mode
    if (QFile::exists(path))
    {
        qDebug() << "Table already exists";
        return;
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);

        // Write the first line of the CSV file
        // this->tabledata->size();
        for (int i = 0; i < (*this->tabledata).size(); ++i)
        {
            out << (*this->tabledata)[i].first;

            // Add a comma if it's not the last element
            if (i < (*this->tabledata).size() - 1)
                out << ",";
        }

        out << "\n"; // Move to the next line

        // Write the second line of the CSV file
        for (int i = 0; i < (*this->tabledata).size(); ++i)
        {
            out << (*this->tabledata)[i].second;

            // Add a comma if it's not the last element
            if (i < (*this->tabledata).size() - 1)
                out << ",";
        }

        qDebug() << "CSV file created successfully";
        qDebug() << "Kiderruka1";

        // last onwer has freed the memory
        delete this->tabledata;
        this->tabledata = nullptr;
        // Delete the form after it's closed

        qDebug() << "Kiderruka12";
        delete this->form;
        this->form = nullptr;
        // Close the file
        file.close();

        QTreeWidgetItem *table = new QTreeWidgetItem();
        table->setData(0, Qt::UserRole, QVariant(indiv_tb));
        table->setText(0, tablename);
        table->setFlags(table->flags() | Qt::ItemIsEditable);
        db->addChild(table);
        qDebug() << "Neeche aya?";
    }
    else
    {
        qDebug() << "Error creating CSV file";
    }
}

// to retrieve the user data on next arrival
void MyTreeWidget::GetTables(QString database, QTreeWidgetItem *&curr_root)
{

    QString path = this->currdir + "/" + database + "/";
    QDir dir(path);
    QStringList all_tables = dir.entryList(QStringList() << "*.csv", QDir::Files);

    for (const QString &a : all_tables)
    {
        // like a subs str funtion
        qDebug() << "table retirieveed ,=" << a.section('.', 0, 0);
        QTreeWidgetItem *table = new QTreeWidgetItem();
        table->setData(0, Qt::UserRole, indiv_tb);
        table->setText(0, a.section('.', 0, 0));
        table->setFlags(table->flags() | Qt::ItemIsEditable);
        curr_root->addChild(table);
    }
}

void MyTreeWidget::RenameTb(QTreeWidgetItem *item)
{
    QTreeWidgetItem *db = item->parent();
    if (db)
    {
        qDebug() << db->text(0) << "and  I am the father ";
    }
    else
    {
        qDebug() << "Invalid PArent;";
        return;
    }

    if (item)
    {
        QString oldPath = this->currdir + "/" + db->text(0) + "/" + this->to_be_renamed + ".csv";

        // Construct the new path
        QString newPath = this->currdir + "/" + db->text(0) + "/" + item->text(0) + ".csv";
        // Rename the file+
        qDebug() << " old path:" << oldPath;
        qDebug() << " new path:" << newPath;
        QFile file(oldPath);
        if (file.rename(newPath))
        {
            // Update the item's text in the tree widget
            qDebug() << "Table renamed/updated successfully!";
        }
        else
        {
            qDebug() << "Renaming failed";
        }
    }
    else
    {
        qDebug() << "Item is NULL";
    }
}
void MyTreeWidget::DeleteTb(QTreeWidgetItem *item)
{
    qDebug() << "Delete Speaking";
    QTreeWidgetItem *db = item->parent();
    if (db)
    {
        qDebug() << db->text(0) << " and I am the father ";
    }
    else
    {
        qDebug() << "Invalid Parent";
        return;
    }
    if (item)
    {
        QString filePath = this->currdir + "/" + db->text(0) + "/" + item->text(0) + ".csv";
        // Delete the file
        QFile file(filePath);
        if (file.remove())
        {
            // Remove the item from the QTreeWidget
            deleteTreeItem(item);
            qDebug() << "Table deleted successfully!";
        }
        else
        {
            qDebug() << "Deletion failed";
        }
    }
    else
    {
        qDebug() << "Item is NULL";
    }
}

/**************************************************************************************/

//                Curd on Db
// get db
// to retrieve the user data on next arrival
void MyTreeWidget::getDataBases()
{
    QDir dir(this->currdir);
    qDebug() << "Get  Db speadking here and  path == " << this->currdir;
    QStringList all_dbs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &folderName : all_dbs)
    {
        qDebug() << folderName;
        //        child->setText(0,folderName);
        QTreeWidgetItem *parentItem = this->tree->topLevelItem(0);
        QTreeWidgetItem *child = parentItem->child(0);
        QTreeWidgetItem *newdb = new QTreeWidgetItem();
        newdb->setData(0, Qt::UserRole, indiv_db);
        newdb->setText(0, folderName);
        newdb->setFlags(newdb->flags() | Qt::ItemIsEditable);
        child->addChild(newdb);
        this->GetTables(folderName, newdb);
    }
}
// create database
void MyTreeWidget::createDatabase(QString db_name)
{
    QDir dir(this->currdir);
    if (!dir.exists(db_name))
    {
        if (dir.mkdir(db_name))
        {
            QTreeWidgetItem *parentItem = this->tree->topLevelItem(0);
            qDebug() << " parent == ";
            QTreeWidgetItem *database = nullptr;
            if (parentItem)
            {
                database = parentItem->child(0);
            }
            else
            {
                qDebug() << "No parent exits!!";
            }
            QTreeWidgetItem *child = new QTreeWidgetItem();
            child->setText(0, db_name);
            child->setData(0, Qt::UserRole, indiv_db);
            if (!database)
            {
                qDebug() << "database folder not found";
                return;
            }
            database->addChild(child);
            //\\ 2 new lines for rename
            child->setFlags(child->flags() | Qt::ItemIsEditable);
            this->tree->editItem(child, 0);
            qDebug() << "Data base named=" << db_name << "Created successfully";
        }
        else
        {
            qDebug() << "Data base named=" << db_name << "Not Created Duo to some error!!!";
        }
    }
    else
    {
        qDebug() << "Data base == " << db_name << ", already exists!";
    }
}

//  function to create table inside a data base

void MyTreeWidget::RenameDb(QTreeWidgetItem *item)
{
    if (item)
    {
        QString oldPath = this->currdir + "/" + this->to_be_renamed;

        // Construct the new path
        QString newPath = this->currdir + "/" + item->text(0);

        // Rename the folder
        QDir dir(this->currdir);
        if (dir.rename(oldPath, newPath))
        {
            // Update the item's text in the tree widget
            qDebug() << "db renamed/ updated SucessFully!";
        }
        else
        {
            qDebug() << "Renaming failed";
        }
    }
    else
    {
        qDebug() << "Item is NUll";
    }
}

void MyTreeWidget::DeleteDb(QTreeWidgetItem *item)
{
    qDebug() << "hello delete db here";
    if (item)
    {
        QString dbPath = this->currdir + "/" + item->text(0);

        // Check if the database folder exists
        QDir dbDir(dbPath);
        if (dbDir.exists())
        {
            // Remove the database folder and its contents
            if (dbDir.removeRecursively())
            {
                qDebug() << "Database deleted successfully!";
            }
            else
            {
                qDebug() << "Deleting database failed";
            }
        }
        else
        {
            qDebug() << "Database folder does not exist";
        }
    }
    else
    {
        qDebug() << "Item is NULL";
    }
    this->deleteTreeItem(item);
}

void MyTreeWidget::deleteTreeItem(QTreeWidgetItem *item)
{
    qDebug() << "I am delete";
    if (!item)
    {
        qDebug() << "Item is NULL";
        return;
    }
    QTreeWidgetItem *parentItem = item->parent();
    if (parentItem)
    {
        int index = parentItem->indexOfChild(item);
        if (index != -1)
        {
            parentItem->takeChild(index);
            delete item;
            qDebug() << "Item and its children deleted successfully!";
        }
        else
        {
            qDebug() << "Item not found in its parent";
        }
    }
}


void MyTreeWidget::mytreedelete()
{
    qDebug() << "Is customContextMenuRequested connected ?: " << disconnect(this->tree, &QTreeWidget::customContextMenuRequested, this, &MyTreeWidget::on_treeWidget_customContextMenuRequested);
    qDebug() << "Connecting tree widget Edit : " << disconnect(this->tree, &QTreeWidget::itemChanged, this, &MyTreeWidget::onItemChanged);
    qDebug() << "Is tree connected to Mytablewidget ?: " << disconnect(this, &MyTreeWidget::sendDataToTable, this->tablewid, &MyTableWidget::receiveData);


    qDebug() << "Ruka 1";
    if (this->tree)
    {
        QTreeWidgetItem *rootItem = this->tree->topLevelItem(0);
        if (rootItem)
        {
            delete rootItem; // This will automatically delete all children
            rootItem = nullptr;
        }
        qDebug() << "neeche aya?";
        // delete this->tree;
        this->tree = nullptr;
    }
}

MyTreeWidget::~MyTreeWidget()
{
    qDebug() << "Pure Tree destructed";
    qDebug() << "Is customContextMenuRequested connected ?: " << disconnect(this->tree, &QTreeWidget::customContextMenuRequested, this, &MyTreeWidget::on_treeWidget_customContextMenuRequested);
    qDebug() << "Connecting tree widget Edit : " << disconnect(this->tree, &QTreeWidget::itemChanged, this, &MyTreeWidget::onItemChanged);
    qDebug() << "Is tree connected to Mytablewidget ?: " << disconnect(this, &MyTreeWidget::sendDataToTable, this->tablewid, &MyTableWidget::receiveData);
    // Disconnect signals
    // Delete the tree widget if it exists

    // Delete other dynamically allocated objects
    // delete this->tab;
    // delete this->tablewid;
    // ...
    qDebug() << "neeche arha?";

    delete this->tree;
    this->tree = nullptr;

    if(this->tablewid){
        delete this->tablewid;
        this->tablewid = nullptr;
    }
    qDebug() << "Pure Tree destructed";
}
