#include "MultiStepForm.h"

MultiStepForm::MultiStepForm(QDialog *parent)
    : QDialog{parent}
{
    setupUi();
    first = true;
    previnputcols=0;

}

void MultiStepForm::setupUi()
{
    qDebug() << "form bana1";

    this->layout = new QVBoxLayout(this);
    this->stackedWidget = new QStackedWidget(this);
    // Step 1
    QWidget *step1 = new QWidget;
    QVBoxLayout *step1Layout = new QVBoxLayout(step1);
    this->label1 = new QLabel("Enter Table Name :", step1);
    lineEdit1 = new QLineEdit(step1); // Add a QLineEdit
    connect(lineEdit1, &QLineEdit::textChanged, this, &MultiStepForm::onLineEdit1TextChanged);
    this->label2 = new QLabel("Enter Number of Columns:", step1);
    lineEdit2 = new QLineEdit(step1); // Add a QLineEdit
    // connect(lineEdit2,&QLineEdit::textChanged, this, &MultiStepForm::onLineEdit2TextChanged);
    // Restrict input to only integers for lineEdit2
    lineEdit2->setValidator(new QIntValidator(lineEdit2));

    QPushButton *nextButton = new QPushButton("Next Step", step1);
    connect(nextButton, &QPushButton::clicked, this, &MultiStepForm::nextStep);

    step1Layout->addWidget(label1);
    step1Layout->addWidget(lineEdit1);
    step1Layout->addWidget(label2);
    step1Layout->addWidget(lineEdit2);
    step1Layout->addStretch(1);
    step1Layout->addWidget(nextButton);

    // Step 2
    QWidget *step2 = new QWidget;
    QVBoxLayout *step2Layout = new QVBoxLayout(step2);
    QPushButton *saveButton = new QPushButton("Save", step2);
    QPushButton *prevButton = new QPushButton("Previous Step", step2);

    connect(prevButton, &QPushButton::clicked, this, &MultiStepForm::previousStep);
    // if user changes the column number .
    connect(saveButton, &QPushButton::clicked, this, &MultiStepForm::saveInformation);
    // ek table ayega idher
    qDebug() << "form bana2";
    this->tab = new MyTableWidget(this);
    qDebug() << "form bana3";

    label3 = new QLabel(this); // isko delete kr
    step2Layout->addWidget(label3);
    step2Layout->addWidget(tab);
    // Increase the size of the form
    // setFixedSize(500, 500);

    step2Layout->addStretch(1);
    step2Layout->addWidget(prevButton);
    step2Layout->addWidget(saveButton);

    stackedWidget->addWidget(step1);
    stackedWidget->addWidget(step2);

    layout->addWidget(stackedWidget);
    // layout->addStretch(1);
    setLayout(layout);
}

void MultiStepForm::nextStep()
{

    qDebug()<<"first ki value =="<<first;
    qDebug()<<"stainputcols = "<<previnputcols;

    int inp_columns = this->lineEdit2->text().toInt();
    qDebug() << "fuckksds   == inp cols== " << inp_columns;

    int currentIndex = stackedWidget->currentIndex();
    if (currentIndex < stackedWidget->count() - 1)
    {
        stackedWidget->setCurrentIndex(currentIndex + 1);
    }
    qDebug() << "yha tk theek hi tha";
    if (first)
    {
        qDebug() << "Sada table bnae ja raha ";
        this->tab->createTable(2, inp_columns);
        first = false;
        previnputcols =inp_columns;
    }
    else
    {
        if (inp_columns > 0 && previnputcols != inp_columns)
        {
            qDebug() << "next wale me bnega";
            this->tab->DeleteAndRemake(2, inp_columns);
            qDebug() << "number is :" << this->lineEdit2->text().toInt();
            previnputcols = inp_columns;
        }
    }
    qDebug() << "Kuch nhi hua be ";
}

void MultiStepForm::previousStep()
{
    int currentIndex = stackedWidget->currentIndex();
    if (currentIndex > 0)
    {
        stackedWidget->setCurrentIndex(currentIndex - 1);
    }
}
void MultiStepForm::saveInformation()
{
    qDebug() << "line one: " << lineEdit1->text();
    qDebug() << "line two: " << lineEdit2->text();
    this->tableData = this->tab->getTableInitalData();
    qDebug() << "saved";
    accept(); // signal to tell that form is closed
    close();
}

QVector<QPair<QString, QChar>> *MultiStepForm::getTableData()
{
    QVector<QPair<QString, QChar>> *data = this->tableData;
    this->tableData = nullptr; // Set to nullptr to indicate ownership transfer
    return data;
    // return this->tableData;
}

QString MultiStepForm::getTableName()
{
    QString a = this->lineEdit1->text();
    return a;
}

void MultiStepForm::onLineEdit1TextChanged(const QString &text)
{
    qDebug() << "text was == " << text;
    label3->setText("EDIT TABLE: " + this->lineEdit1->text());
}

MultiStepForm::~MultiStepForm()
{
    qDebug() << "Form janey laga hai";

    delete this->layout; // sb ek hi line me1
}
