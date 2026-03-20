#include "newuser.h"
#include "ui_newuser.h"

extern QString userSurname;
extern QString userPosition;

NewUser::NewUser(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewUser)
{
    ui->setupUi(this);

    QStringList items = {"Ассистент", "Ст. преподаватель", "Доцент"};
    ui->comboBox->addItems(items);




    connect(ui->pushButton, &QPushButton::clicked, this, &NewUser::apply_user);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &NewUser::close_user);

}

NewUser::~NewUser()
{
    delete ui;
}

void NewUser::apply_user()
{
    userSurname = ui->lineEdit->text();
    userPosition = ui->comboBox->currentText();
    accept();
}

void NewUser::close_user()
{
    close();
}
