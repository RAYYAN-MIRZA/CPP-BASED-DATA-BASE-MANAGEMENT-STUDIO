#include "MyTableWidget.h"

MyTableWidget::MyTableWidget(QWidget *parent) : QWidget(parent)
{
    qDebug() << "object bn gay hai 1";
    myMap.insert(0, 'n');
    myMap.insert(1, 's');
    myMap.insert(2, 'b');
    myMap.insert(3, 'd');
    myMap.insert(4, 't');

    myDataTypeMap.insert('n', "Number");
    myDataTypeMap.insert('s', "String");
    myDataTypeMap.insert('b', "Boolean");
    myDataTypeMap.insert('d', "Date");
    myDataTypeMap.insert('t', "Time");

    this->tableWidget = nullptr;
    qDebug() << "object bn gay hai 2";

    this->layout = nullptr;
    qDebug() << "object bn gay hai 3";

    this->colNames = nullptr;
    qDebug() << "object bn gay hai 4";
    this->tableData = nullptr;
    this->currTablePath = "";
    this->tableFilterComboBox = nullptr;
    this->filterLayout = nullptr;
    this->filterInpBox = nullptr;
    this->tableFilterBox = nullptr;
    this->currColumnIndex = 0;
    this->saveChanges = false;
}
// for the form
void MyTableWidget::createTable(int row, int col)
{
    this->tableWidget = new QTableWidget(this);
    this->colNames = new QVector<QPair<QString, QChar>>(col);
    this->layout = new QVBoxLayout(this);
    tableWidget->setRowCount(row);    // Set the number of rows
    tableWidget->setColumnCount(col); // Set the number of columns

    // Make the entire table editable
    tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

    connect(tableWidget, &QTableWidget::itemChanged, this, &MyTableWidget::onItemChangedOfFormTable);

    QDateTime s;
    qDebug() << "curr time=" << s.currentDateTime().date();
    qDebug() << "curr time=" << s.currentDateTime().time(); // 25 hour deta format

    layout->addWidget(tableWidget);
    setLayout(layout);
    for (int col = 0; col < tableWidget->columnCount(); ++col)
    {
        // Create a new QTableWidgetItem with text "Item " + (col + 1)
        QTableWidgetItem *item = new QTableWidgetItem("Set Col#" + QString::number(col + 1) + "Name");
        // Set the created item at the first row (row index 0) and the current column (col) in the tableWidget.

        tableWidget->setItem(0, col, item);
    }
    // Create a QComboBox and add three options for each column
    for (int col = 0; col < tableWidget->columnCount(); ++col)
    {
        QComboBox *comboBox = new QComboBox(this);
        comboBox->setPlaceholderText("Set DataType");
        comboBox->addItem("Number");
        comboBox->addItem("String");
        comboBox->addItem("Boolean");
        comboBox->addItem("Date");
        comboBox->addItem("Time");

        // Set the QComboBox as an item in the tableWidget at row 1, current column
        tableWidget->setCellWidget(1, col, comboBox);

        // Connect the currentIndexChanged signal to a custom slot
        connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, col](int index)
                { onComboBoxIndexChanged(col, index); });
    }

    layout->addWidget(tableWidget);
    setLayout(layout);
}
// for the form
void MyTableWidget::DeleteAndRemake(int row, int col)
{
    qDebug() << "JA raha1 finally ";
    if (this->tableWidget)
    {
        delete this->tableWidget;
        this->tableWidget = nullptr;
    }
    qDebug() << "JA raha2 finally ";
    if (this->layout)
        delete this->layout;
    qDebug() << "JA raha3 finally ";
    if (this->colNames)
        delete this->colNames;
    qDebug() << "JA raha4 finally ";

    this->createTable(row, col);
}

void MyTableWidget::onComboBoxIndexChanged(int col, int index)
{
    qDebug() << "Combo clicked";
    qDebug() << "col == " << col;
    qDebug() << "index == " << index;
    // map kara leo
    (*this->colNames)[col].second = this->myMap[index];
}

QVector<QPair<QString, QChar>> *MyTableWidget::getTableInitalData()
{
    QVector<QPair<QString, QChar>> *data = this->colNames;
    this->colNames = nullptr; // Set to nullptr to indicate ownership transfer
    return data;
}
void MyTableWidget::onItemChangedOfFormTable(QTableWidgetItem *item)
{
    qDebug() << "Some from item->column()=" << item->column() << "and "
             << "item->row()=" << item->row() << "was edited!";
    qDebug() << item->text();
    if (item->row() == 0)
    {
        (*this->colNames)[item->column()].first = item->text();
        qDebug() << "arr[" << item->column() << "]==" << (*this->colNames)[item->column()].first;
    }
}

void MyTableWidget::handleTimeChanged(const QTime &time)
{
    QTimeEdit *timeEdit = qobject_cast<QTimeEdit *>(sender());
    if (timeEdit)
    {
        qDebug() << "Time changed at "
                 << " Time:" << time.toString("hh:mm:ss");
        this->saveChanges = true;
    }
}

void MyTableWidget::handletrueFalseComboBoxIndexChanged(int index)
{
    qDebug() << "Ixdex  === " << index;
    this->saveChanges = true;
}

int MyTableWidget::getColumns()
{
    return this->tableWidget->columnCount();
}

// ye wala specific function or right wale side wale table k lye hai
void MyTableWidget::receiveData(QString path)
{
    if (this->saveChanges)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                                  "Save Changes",
                                                                  "Want to save the previous changes?",
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            this->SaveChangesOnTableInFile();
            // User clicked "Yes"
            // Add your code for handling "Yes" here
        }
        else
        {
            qDebug() << " User clicked No or closed the message box";
            this->saveChanges = false;
        }
    }
    qDebug() << "PAth=" << path;
    this->currTablePath = path;
    this->DeleteAndRemakeRightTable();
}

void MyTableWidget::addaNewRow()
{
    int numRows = (*this->tableWidget).rowCount();
    this->tableWidget->insertRow(numRows);

    for (int col = 0; col < (*this->tableWidget).columnCount(); ++col)
    {
        QTableWidgetItem *item = new QTableWidgetItem();

        if ((*this->tableData)[1][col] == "s")
        {
            item->setData(Qt::EditRole, QString("Default String"));
        }
        else if ((*this->tableData)[1][col] == "d")
        {
            item->setData(Qt::EditRole, QDate::currentDate());
        }
        else if ((*this->tableData)[1][col] == "n")
        {
            item->setData(Qt::EditRole, 0);
        }
        else if ((*this->tableData)[1][col] == "b")
        {
            // QCheckBox* checkBox = new QCheckBox();
            QComboBox *trueFalseComboBox = new QComboBox();
            trueFalseComboBox->addItem("True");
            trueFalseComboBox->addItem("False");
            connect(trueFalseComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MyTableWidget::handletrueFalseComboBoxIndexChanged);
            this->tableWidget->setCellWidget(numRows, col, trueFalseComboBox);
        }
        else if ((*this->tableData)[1][col] == "t")
        {
            QTimeEdit *timeEdit = new QTimeEdit(QTime::currentTime()); // Set the current time as default
            connect(timeEdit, &QTimeEdit::timeChanged, this, &MyTableWidget::handleTimeChanged);
            this->tableWidget->setCellWidget(numRows, col, timeEdit);

            item->setData(Qt::EditRole, timeEdit->time());
        }

        if (numRows == 0)
        {
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }
        else
        {
            item->setFlags(item->flags() | Qt::ItemIsEditable);
        }

        this->tableWidget->setItem(numRows, col, item);
    }

    this->saveChanges = true;
}

void MyTableWidget::SaveChangesOnTableInFile()
{
    qDebug() << "SAve button here";
    if (this->saveChanges)
        this->writeCSV();
}

// for the right side of screen;
void MyTableWidget::createTableForRightSide()
{

    //  sikk
    this->currColumnIndex = 0;
    int row = (*this->tableData).size();
    int col = (*this->tableData)[0].size();
    this->tableWidget = new QTableWidget(this);

    this->layout = new QVBoxLayout(this);

    tableWidget->setColumnCount(col); // Set the number of columns

    QDateTime s;
    qDebug() << "curr time=" << s.currentDateTime().date();
    qDebug() << "curr time=" << s.currentDateTime().time(); // 25 hour deta format

    this->tableFilterComboBox = new QComboBox(this);
    this->tableFilterComboBox->setPlaceholderText("Select Column");

    // Column names
    this->tableWidget->setHorizontalHeaderLabels((*this->tableData)[0]);

    tableFilterComboBox->addItems((*this->tableData)[0]);
    if (row - 1 > 0)
    {
        tableWidget->setRowCount(row - 1); // Set the number of rows
        int k;
        for (int i = 0, k = 1; i < row - 1; i++, k++) // jaan boojh k 1 se
        {
            for (int j = 0; j < col; j++)
            {
                if (i == 0)
                {
                    QTableWidgetItem *item = new QTableWidgetItem();
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                    item->setText(myDataTypeMap[(*this->tableData)[1][j][0]]);
                    this->tableWidget->setItem(i, j, item);
                    continue;
                }
                QTableWidgetItem *item = new QTableWidgetItem();

                if ((*this->tableData)[1][j] == "s")
                {
                    item->setData(Qt::EditRole, (*this->tableData)[k][j]);
                }
                else if ((*this->tableData)[1][j] == "d")
                {
                    QString dateString = (*this->tableData)[k][j];
                    QDate date = QDate::fromString(dateString, "yyyy-MM-dd"); // Adjust the format as needed
                    item->setData(Qt::EditRole, date);
                }
                else if ((*this->tableData)[1][j] == "n")
                {
                    item->setData(Qt::EditRole, (*this->tableData)[k][j]);
                }
                else if ((*this->tableData)[1][j] == "b")
                {

                    // QCheckBox* checkBox = new QCheckBox((*this->tableData)[i][j]);

                    // Assuming this code is in a function where you create QComboBox
                    QComboBox *trueFalseComboBox = new QComboBox();
                    trueFalseComboBox->addItem("True");
                    trueFalseComboBox->addItem("False");
                    trueFalseComboBox->setCurrentIndex((*this->tableData)[k][j] == "True" ? 0 : 1);
                    connect(trueFalseComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MyTableWidget::handletrueFalseComboBoxIndexChanged);

                    // Add this combo box to your table as needed...
                    this->tableWidget->setCellWidget(i, j, trueFalseComboBox);
                }
                else if ((*this->tableData)[1][j] == "t")
                {
                    QTimeEdit *timeEdit = new QTimeEdit(QTime::fromString((*this->tableData)[k][j], "hh:mm:ss")); // Set the current time as default
                    connect(timeEdit, &QTimeEdit::timeChanged, this, &MyTableWidget::handleTimeChanged);
                    this->tableWidget->setCellWidget(i, j, timeEdit);

                    // Set the time value in the QTableWidgetItem
                    item->setData(Qt::EditRole, timeEdit->time());
                }

                // QTableWidgetItem *item = new QTableWidgetItem();

                // item->setText((*this->tableData)[i][j]);
                item->setFlags(item->flags() | Qt::ItemIsEditable);
                this->tableWidget->setItem(i, j, item);
            }
        }
    }

    this->tableFuntionLayout = new QHBoxLayout(this);
    this->addbut = new QPushButton("Add a new Row", this);

    connect(this->addbut, &QPushButton::clicked, this, &MyTableWidget::addaNewRow);
    this->delbut = new QPushButton("Delete a  Row", this);
    connect(this->delbut, &QPushButton::clicked, this, &MyTableWidget::TakeRowToBeDeleted);

    this->savbut = new QPushButton("Save Changes", this);
    connect(this->savbut, &QPushButton::clicked, this, &MyTableWidget::SaveChangesOnTableInFile);

    this->tableFuntionLayout->addWidget(this->addbut);
    this->tableFuntionLayout->addWidget(this->delbut);
    this->tableFuntionLayout->addWidget(this->savbut);

    this->tableFuntionLayout->addStretch(1);

    this->buttonContainer = new QWidget(this);
    this->buttonContainer->setLayout(tableFuntionLayout);

    QHBoxLayout *filterLayout = new QHBoxLayout(this);
    QLineEdit *filterInpBox = new QLineEdit(this);

    filterInpBox->setPlaceholderText("Filter By Column");
    connect(filterInpBox, &QLineEdit::textChanged, this, &MyTableWidget::onFilterTextChanged);

    // uper combo box bn chuka hai
    filterLayout->addWidget(filterInpBox);
    filterLayout->addWidget(tableFilterComboBox);
    // filterLayout->addStretch(1);     // latest stretch

    this->tableFilterBox = new QWidget(this);
    this->tableFilterBox->setLayout(filterLayout);

    // Connect the currentIndexChanged signal to a custom slot
    connect(tableFilterComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, col](int index)
            { onTableFilterComboBoxIndexChanged(col, index); });

    // Make the entire table editable
    // tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

    connect(tableWidget, &QTableWidget::itemChanged, this, &MyTableWidget::onItemChangedOfRightTable);

    layout->addWidget(tableFilterBox);
    layout->addWidget(tableWidget);
    // layout->addStretch(1);
    layout->addWidget(this->buttonContainer);
    setLayout(layout);
}

void MyTableWidget::TakeRowToBeDeleted()
{
    bool ok;
    int value = QInputDialog::getInt(
        this,
        tr("Row Deletion Box"),
        tr("Enter Valid Row Number:"),
        0,       // Default value
        INT_MIN, // Minimum value for int
        INT_MAX, // Maximum value for int
        1,       // Step size
        &ok);
    if (ok)
    {
        qDebug() << "Input received:" << value;
        if (value > 1 && value <= this->tableWidget->rowCount())
        {
            this->tableWidget->removeRow(value - 1);
            this->saveChanges = true;
        }
        else
        {
            QMessageBox::warning(this, "Error Deleting Row", "Invalid Row Entered");
        }
    }
    else
    {
        qDebug() << "User canceled the input.";
    }
}

// for the right side table
void MyTableWidget::DeleteAndRemakeRightTable()
{
    qDebug() << "JA raha1 finally ";
    if (this->tableWidget)
    {
        delete this->tableWidget;
        this->tableWidget = nullptr;
    }
    qDebug() << "JA raha2 finally ";
    if (this->layout)
        delete this->layout;
    qDebug() << "JA raha3 finally ";
    if (this->tableData)
        delete this->tableData;

    this->readCSV();
    if (this->tableData == nullptr)
    {
        qDebug() << "Errro reading the file";
        return;
    }
    this->createTableForRightSide();
}

// reading the csv from the signal
void MyTableWidget::readCSV()
{
    // Open the CSV file
    QFile file(this->currTablePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Handle error opening the file
        qDebug() << "Error opening file: " << file.errorString();
        // return this->tableData;
    }

    this->tableData = new QVector<QVector<QString>>;
    // Read the file line by line
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        // Add the fields to the QVector
        (*this->tableData).push_back(fields.toVector());
    }
    // Close the file
    file.close();
    // return this->tableData;
}
// rewrite data after after changes on the file
void MyTableWidget::writeCSV()
{
    qDebug() << "JA raha likne " << this->currTablePath;
    // Open the CSV file for writing
    QFile file(this->currTablePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // Handle error opening the file
        qDebug() << "Error opening file for writing : " << file.errorString();
        return;
    }

    // Create a QTextStream to write to the file
    QTextStream out(&file);
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (*this->tableData)[0].size(); ++j)
        {

            out << (*this->tableData)[i][j];
            if (j < (*this->tableData)[0].size() - 1)
                out << ",";
        }
        out << "\n";
    }
    // 1 se beacuse 1st one would be types of data
    for (int i = 1; i < this->tableWidget->rowCount(); i++)
    {
        for (int j = 0; j < this->tableWidget->columnCount(); j++)
        {
            // QTableWidgetItem *item = this->tableWidget->item(i, j);
            // QString data = item->data(Qt::EditRole).toString();

            // qDebug()<<data;
            // out<<data;
            if ((*this->tableData)[1][j][0] == 's')
            {
                QTableWidgetItem *item = this->tableWidget->item(i, j);
                // item->setData(Qt::EditRole,(*this->tableData)[k][j]);
                out << item->text();
            }
            else if ((*this->tableData)[1][j][0] == 'd')
            {
                QTableWidgetItem *item = this->tableWidget->item(i, j);
                out << item->text();
                // QDate date = QDate::fromString(dateString, "yyyy-MM-dd"); // Adjust the format as needed
                // item->setData(Qt::EditRole, date);
            }
            else if ((*this->tableData)[1][j][0] == 'n')
            {
                QTableWidgetItem *item = this->tableWidget->item(i, j);
                out << item->text();
            }
            else if ((*this->tableData)[1][j][0] == 'b')
            {

                QComboBox *trueFalseComboBox = qobject_cast<QComboBox *>(this->tableWidget->cellWidget(i, j));
                if (trueFalseComboBox)
                {

                    // You can access the properties or methods of QComboBox here
                    // For example, to get the selected index's text:
                    QString selectedIndexText = trueFalseComboBox->currentText();
                    qDebug() << "Selected Index Text: " << selectedIndexText;
                    out << trueFalseComboBox->currentText();
                }
                else
                {
                    qDebug() << "sala  under nhi aya";
                }
            }
            else if ((*this->tableData)[1][j][0] == 't')
            {
                QTimeEdit *timeEdit = qobject_cast<QTimeEdit *>(this->tableWidget->cellWidget(i, j));
                if (timeEdit)
                {
                    QTime time = timeEdit->time();
                    QString formattedTime = time.toString("hh:mm:ss");

                    // Now you can use 'formattedTime' as needed
                    qDebug() << "Formatted Time: " << formattedTime;
                    out << formattedTime;
                }
                else
                {
                    qDebug() << "sala under nhi ayaha";
                }
            }
            if (j < this->tableWidget->columnCount() - 1)
                out << ",";
        }
        out << "\n";
    }
    // Close the file
    file.close();
    qDebug() << "Data written to CSV file: " << this->currTablePath;
    // this->showMessageWithTimeout("Table Updated Successfully", 2000);
    QMessageBox::information(this, "Message", "Changes Saved Successfully!");
    this->saveChanges = false;
}

void MyTableWidget::onItemChangedOfRightTable(QTableWidgetItem *item)
{
    qDebug() << "Some from item->column()=" << item->column() << "and "
             << "item->i()=" << item->row() << "was edited!";
    qDebug() << item->text();
    this->saveChanges = true;
}

void MyTableWidget::onTableFilterComboBoxIndexChanged(int col, int index)
{
    qDebug() << "Combo TAble clicked";
    qDebug() << "col == " << col;
    qDebug() << "index == " << index;
    this->currColumnIndex = index;
}

void MyTableWidget::onFilterTextChanged(const QString &text)
{
    // Iterate through all rows in the tableWidget
    qDebug() << "Input was==" << text;

    for (int row = 0; row < tableWidget->rowCount(); ++row)
    {
        bool rowMatches = false;

        // Iterate through all columns in the current row
        // for (int col = 0; col < tableWidget->columnCount(); ++col) {
        QTableWidgetItem *item = tableWidget->item(row, this->currColumnIndex);
        if (item && item->text().contains(text, Qt::CaseInsensitive))
        {
            rowMatches = true;
            // break;
        }
        // }
        // Show/hide the row based on whether it matches the search term
        tableWidget->setRowHidden(row, !rowMatches);
    }
}

MyTableWidget::~MyTableWidget()
{
    if (this->saveChanges)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                                  "Save Changes",
                                                                  "Want to save the previous changes?",
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            this->SaveChangesOnTableInFile();
            // User clicked "Yes"
            // Add your code for handling "Yes" here
        }
        else
        {
            qDebug() << " User clicked No or closed the message box";
            this->saveChanges = false;
        }
    }
    qDebug() << "tablewidger ja rha 1";
    delete this->tableWidget;
    this->tableWidget = nullptr;
    qDebug() << "tablewidger ja rha 2";
    if (this->tableFilterComboBox)
        delete this->tableFilterComboBox;
    if (this->filterInpBox)
        delete this->filterInpBox;
    if (this->filterLayout)
        delete this->filterLayout;
    if (this->tableFilterBox)
        delete this->tableFilterBox;

    this->tableFilterComboBox = nullptr;
    this->filterLayout = nullptr;
    this->filterInpBox = nullptr;
    this->tableFilterBox = nullptr;

    delete this->layout;
    this->layout = nullptr;
    qDebug() << "tablewidger ja rha 3";

    // combo boxxes bhi delete kr bhai
    qDebug() << "tablewidger ja rha 4";

    // For right table
    qDebug() << "Final 6 ja raha ";

    delete this->tableData;
    this->layout = nullptr;

    qDebug() << "Final 7 ja raha ";
    if (this->tableFuntionLayout) // delete karne pe crash horaha yani dangling
        this->tableFuntionLayout = nullptr;

    if (this->buttonContainer)
    {
        qDebug() << "Final 8 ja raha ";
        this->buttonContainer = nullptr;
        qDebug("kuch to hai butt me");
    }
    else
    {
        qDebug() << "already tha deleeted";
    }

    qDebug() << "Final final ja raha ";
}


