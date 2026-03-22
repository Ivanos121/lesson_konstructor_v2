#include "addline.h"
#include "ui_addline.h"

extern double lections;
extern double lab_rabs;
extern double pract_rabs;
extern double mrs;
extern double individ_zad;
extern double kurs_rab;
extern double konsult;
extern double recenz;
extern double zachet;
extern double exam;
extern double ucheb_pract;
extern double zavod_pract;
extern double diplom_proect;
extern double gek_gak;
extern double aspirant;
extern double sum_semestr;
extern double sum_year;
extern QString subject_name;

AddLine::AddLine(QWidget *parent): QDialog(parent)
    , ui(new Ui::AddLine)
{
    ui->setupUi(this);
    connect(ui->applyButton, &QPushButton::clicked, this, &AddLine::apply_button);
    connect(ui->closeButton, &QPushButton::clicked, this, &AddLine::close_button);
    setWindowTitle("Введите параметры нагрузки");
}

AddLine::~AddLine()
{
    delete ui;
}

void AddLine::apply_button()
{
    lections = ui->lineEdit->text().toDouble();
    lab_rabs = ui->lineEdit_2->text().toDouble();
    pract_rabs = ui->lineEdit_3->text().toDouble();
    mrs= ui->lineEdit_4->text().toDouble();
    individ_zad= ui->lineEdit_5->text().toDouble();
    kurs_rab= ui->lineEdit_6->text().toDouble();
    konsult= ui->lineEdit_7->text().toDouble();
    recenz= ui->lineEdit_8->text().toDouble();
    zachet= ui->lineEdit_9->text().toDouble();
    exam= ui->lineEdit_17->text().toDouble();
    ucheb_pract= ui->lineEdit_11->text().toDouble();
    zavod_pract= ui->lineEdit_14->text().toDouble();
    diplom_proect= ui->lineEdit_12->text().toDouble();
    gek_gak= ui->lineEdit_15->text().toDouble();
    aspirant= ui->lineEdit_16->text().toDouble();
    sum_semestr= ui->lineEdit_13->text().toDouble();
    sum_year= ui->lineEdit_10->text().toDouble();
    accept();
}

void AddLine::close_button()
{
    close();
}
