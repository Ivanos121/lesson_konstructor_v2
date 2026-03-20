#include "add_lesson.h"
#include "ui_add_lesson.h"

Add_lesson::Add_lesson(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Add_lesson)
{
    ui->setupUi(this);
    connect(ui->applyButton, &QPushButton::clicked, this, &Add_lesson::onAccept);
    connect(ui->closeButton, &QPushButton::clicked, this, &Add_lesson::closeDialog);

    // Отключаем OK по умолчанию
    ui->applyButton->setEnabled(false);

    QStringList items = {"Лекция", "Лабораторное занятие", "Практическое занятие",
    "Курсовое проектирование по КР", "Консультация по ДР"};
    ui->comboBox->addItems(items);

    QStringList items2 = {"none", "1-я подгруппа раз в 2 недели", "2-я подгруппа раз в две недели",
    "1-я подгруппа раз в неделю","2-я подгруппа раз в неделю"};
    ui->comboBox_2->addItems(items2);

    QStringList items3 = {"none", "Верхняя неделя", "Нижняя неделя"};
    ui->comboBox_3->addItems(items3);

    // Подключаем проверки текста
    connect(ui->subject, &QLineEdit::textChanged, this, &Add_lesson::updateOkButtonState);
    connect(ui->student_group, &QLineEdit::textChanged, this, &Add_lesson::updateOkButtonState);
    connect(ui->classroom, &QLineEdit::textChanged, this, &Add_lesson::updateOkButtonState);


}

Add_lesson::~Add_lesson()
{
    delete ui;

}

void Add_lesson::updateOkButtonState()
{
    bool allNotEmpty = !ui->subject->text().trimmed().isEmpty()
    && !ui->student_group->text().trimmed().isEmpty()
        && !ui->classroom->text().trimmed().isEmpty();
    ui->applyButton->setEnabled(allNotEmpty);
}

void Add_lesson::onAccept()
{
    text11 = ui->subject->text();
    text22 = ui->student_group->text();
    text33 = ui->classroom->text();
    text44 = ui->comboBox->currentText();

    QString txt = ui->comboBox_2->currentText();
    emit selectionConfirmed(txt); // уведомляем слушателей

    accept(); // Закрывает диалог
}

void Add_lesson::closeDialog()
{
    close();
}

