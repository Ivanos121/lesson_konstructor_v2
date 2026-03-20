#include "aboutlesson.h"
#include "ui_aboutlesson.h"

aboutLesson::aboutLesson(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::aboutLesson)
{
    ui->setupUi(this);
}

aboutLesson::~aboutLesson()
{
    delete ui;
}

void aboutLesson::on_pushButton_clicked()
{
    close();
}

