/********************************************************************************
** Form generated from reading UI file 'newlesson.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWLESSON_H
#define UI_NEWLESSON_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_newLesson
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_3;
    QComboBox *comboBox_3;
    QLineEdit *lineEdit;
    QComboBox *comboBox_2;
    QLineEdit *lineEdit_2;
    QComboBox *comboBox;
    QLabel *label_2;
    QWidget *widget;
    QGridLayout *gridLayout_3;
    QPushButton *applyButton;
    QPushButton *closeButton;

    void setupUi(QDialog *newLesson)
    {
        if (newLesson->objectName().isEmpty())
            newLesson->setObjectName("newLesson");
        newLesson->resize(333, 290);
        gridLayout_2 = new QGridLayout(newLesson);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(newLesson);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_4 = new QLabel(newLesson);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label_5 = new QLabel(newLesson);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        label_3 = new QLabel(newLesson);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        comboBox_3 = new QComboBox(newLesson);
        comboBox_3->setObjectName("comboBox_3");

        gridLayout->addWidget(comboBox_3, 4, 1, 1, 1);

        lineEdit = new QLineEdit(newLesson);
        lineEdit->setObjectName("lineEdit");

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        comboBox_2 = new QComboBox(newLesson);
        comboBox_2->setObjectName("comboBox_2");

        gridLayout->addWidget(comboBox_2, 1, 1, 1, 1);

        lineEdit_2 = new QLineEdit(newLesson);
        lineEdit_2->setObjectName("lineEdit_2");

        gridLayout->addWidget(lineEdit_2, 3, 1, 1, 1);

        comboBox = new QComboBox(newLesson);
        comboBox->setObjectName("comboBox");

        gridLayout->addWidget(comboBox, 2, 1, 1, 1);

        label_2 = new QLabel(newLesson);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 2, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        widget = new QWidget(newLesson);
        widget->setObjectName("widget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(0, 20));
        gridLayout_3 = new QGridLayout(widget);
        gridLayout_3->setObjectName("gridLayout_3");
        applyButton = new QPushButton(widget);
        applyButton->setObjectName("applyButton");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(applyButton->sizePolicy().hasHeightForWidth());
        applyButton->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(applyButton, 0, 0, 1, 1);

        closeButton = new QPushButton(widget);
        closeButton->setObjectName("closeButton");
        sizePolicy1.setHeightForWidth(closeButton->sizePolicy().hasHeightForWidth());
        closeButton->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(closeButton, 0, 1, 1, 1);


        gridLayout_2->addWidget(widget, 1, 0, 1, 1, Qt::AlignmentFlag::AlignRight);


        retranslateUi(newLesson);

        QMetaObject::connectSlotsByName(newLesson);
    } // setupUi

    void retranslateUi(QDialog *newLesson)
    {
        newLesson->setWindowTitle(QCoreApplication::translate("newLesson", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("newLesson", "\320\244\320\260\320\274\320\270\320\273\320\270\321\217", nullptr));
        label_4->setText(QCoreApplication::translate("newLesson", "\320\223\321\200\321\203\320\277\320\277\320\260", nullptr));
        label_5->setText(QCoreApplication::translate("newLesson", "\320\222\320\270\320\264 \321\201\320\265\321\201\321\201\320\270\320\270", nullptr));
        label_3->setText(QCoreApplication::translate("newLesson", "\320\241\320\265\320\274\320\265\321\201\321\202\321\200", nullptr));
        label_2->setText(QCoreApplication::translate("newLesson", "\320\244\320\276\321\200\320\274\320\260 \320\276\320\261\321\203\321\207\320\265\320\275\320\270\321\217", nullptr));
        applyButton->setText(QCoreApplication::translate("newLesson", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
        closeButton->setText(QCoreApplication::translate("newLesson", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class newLesson: public Ui_newLesson {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWLESSON_H
