#include "newlesson.h"
#include "mainwindow.h"
#include "ui_newlesson.h"
#include "ui_mainwindow.h"

extern QString lesson_name;


newLesson::newLesson(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::newLesson)
{
    wf = (MainWindow*)parent;

    ui->setupUi(this);

    QStringList items = {"Дневники", "Заочники"};
    ui->comboBox->addItems(items);

    QStringList items2 = {"Осенний семестр", "Весенний семестр"};
    ui->comboBox_2->addItems(items2);

    QStringList items3 = {"Установочная сессия", "Экзаменационная сессия"};
    ui->comboBox_3->addItems(items3);

    ui->lineEdit_2->setEnabled(false);
    ui->comboBox_3->setEnabled(false);
    ui->label_4->setEnabled(false);
    ui->label_5->setEnabled(false);

    setWindowTitle("Новый пользователь");

    connect(ui->applyButton, &QPushButton::clicked, this, &newLesson::newTable);
    connect(ui->closeButton, &QPushButton::clicked, this, &newLesson::closeTable);
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &newLesson::onComboBoxChanged);
}

newLesson::~newLesson()
{
    delete ui;
}

void newLesson::onComboBoxChanged(int index)
{
    switch (index)
    {
    case 0:
        // Показать одни виджеты
        ui->lineEdit_2->setEnabled(false);
        ui->comboBox_3->setEnabled(false);
        ui->label_4->setEnabled(false);
        ui->label_5->setEnabled(false);
        ui->label_3->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        break;
    case 1:
        // Показать другую группу и скрыть предыдущую
        ui->lineEdit_2->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->label_4->setEnabled(true);
        ui->label_5->setEnabled(true);
        ui->label_3->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        break;
    default:
        // По умолчанию скрыть все дополнительные виджеты
        ui->lineEdit_2->setEnabled(false);
        ui->comboBox_3->setEnabled(false);
        ui->label_4->setEnabled(false);
        ui->label_5->setEnabled(false);
        ui->label_3->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        break;
    }
}

void newLesson::newTable()
{
    /*QString lab_name;
    QString lab_type;
    QString semestr;
    QString session_type;
    lab_name = ui->lineEdit->text();
    lab_type = ui->comboBox->currentText();
    semestr = ui->comboBox_2->currentText();
    session_type = ui->comboBox_3->currentText();


    if(semestr == "Осенний семестр")
        semestr = "Fall_term";
    else if(semestr == "Весенний семестр")
        semestr = "Spring_term";
    if(lab_type == "Дневники")
        lab_type = "Daytime";
    else if(lab_type == "Заочники")
        lab_type = "Part_time";
            lesson_name = "Lesson_" + semestr + "_" + lab_name + "_" + lab_type + "_" + session_type;

    close();
    wf->CreateNewTable();*/

    QString lab_name = ui->lineEdit->text();
    QString lab_type = ui->comboBox->currentText();
    QString semestr = ui->comboBox_2->currentText();
    QString session_type = ui->comboBox_3->currentText();

    if (semestr == "Осенний семестр")
        semestr = "Fall_term";
    else if (semestr == "Весенний семестр")
        semestr = "Spring_term";

    if (lab_type == "Дневники")
        lab_type = "Daytime";
    else if (lab_type == "Заочники")
        lab_type = "Part_time";

    // Проверяем выбранный индекс или текст первого combobox (ui->comboBox)
    int comboIndex = ui->comboBox->currentIndex();
    if (comboIndex == 0)  // первый элемент выбран
    {
        lesson_name = "Lesson_" + semestr + "_" + lab_name + "_" + lab_type;
    }
    else if (comboIndex == 1)  // второй элемент выбран
    {
        lesson_name = "Lesson_" + lab_name + "_" + lab_type + "_" + session_type;
    }
    else
    {
        // На всякий случай, если выбран другой элемент, можно задать дефолтное значение:
        lesson_name = "Lesson_" + semestr + "_" + lab_name + "_" + lab_type;
    }

    // Можно проверить результат через qDebug
    qDebug() << "lesson_name:" << lesson_name;

    close();
    wf->CreateNewTable();
}

void newLesson::closeTable()
{
    close();
}

