/********************************************************************************
** Form generated from reading UI file 'add_lesson.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADD_LESSON_H
#define UI_ADD_LESSON_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Add_lesson
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *subject;
    QLabel *label_2;
    QLineEdit *student_group;
    QLabel *label_3;
    QLineEdit *classroom;
    QLabel *label_5;
    QComboBox *comboBox_2;
    QLabel *label_6;
    QComboBox *comboBox_3;
    QLabel *label_4;
    QComboBox *comboBox;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *applyButton;
    QPushButton *closeButton;

    void setupUi(QDialog *Add_lesson)
    {
        if (Add_lesson->objectName().isEmpty())
            Add_lesson->setObjectName("Add_lesson");
        Add_lesson->resize(370, 305);
        gridLayout_2 = new QGridLayout(Add_lesson);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(Add_lesson);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        subject = new QLineEdit(Add_lesson);
        subject->setObjectName("subject");

        gridLayout->addWidget(subject, 0, 1, 1, 1);

        label_2 = new QLabel(Add_lesson);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        student_group = new QLineEdit(Add_lesson);
        student_group->setObjectName("student_group");

        gridLayout->addWidget(student_group, 1, 1, 1, 1);

        label_3 = new QLabel(Add_lesson);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        classroom = new QLineEdit(Add_lesson);
        classroom->setObjectName("classroom");

        gridLayout->addWidget(classroom, 2, 1, 1, 1);

        label_5 = new QLabel(Add_lesson);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        comboBox_2 = new QComboBox(Add_lesson);
        comboBox_2->setObjectName("comboBox_2");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboBox_2->sizePolicy().hasHeightForWidth());
        comboBox_2->setSizePolicy(sizePolicy);

        gridLayout->addWidget(comboBox_2, 3, 1, 1, 1);

        label_6 = new QLabel(Add_lesson);
        label_6->setObjectName("label_6");

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        comboBox_3 = new QComboBox(Add_lesson);
        comboBox_3->setObjectName("comboBox_3");
        sizePolicy.setHeightForWidth(comboBox_3->sizePolicy().hasHeightForWidth());
        comboBox_3->setSizePolicy(sizePolicy);

        gridLayout->addWidget(comboBox_3, 4, 1, 1, 1);

        label_4 = new QLabel(Add_lesson);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 5, 0, 1, 1);

        comboBox = new QComboBox(Add_lesson);
        comboBox->setObjectName("comboBox");
        sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy);

        gridLayout->addWidget(comboBox, 5, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        widget = new QWidget(Add_lesson);
        widget->setObjectName("widget");
        widget->setMinimumSize(QSize(0, 10));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        applyButton = new QPushButton(widget);
        applyButton->setObjectName("applyButton");

        horizontalLayout->addWidget(applyButton);

        closeButton = new QPushButton(widget);
        closeButton->setObjectName("closeButton");

        horizontalLayout->addWidget(closeButton);


        gridLayout_2->addWidget(widget, 1, 0, 1, 1);


        retranslateUi(Add_lesson);

        QMetaObject::connectSlotsByName(Add_lesson);
    } // setupUi

    void retranslateUi(QDialog *Add_lesson)
    {
        Add_lesson->setWindowTitle(QCoreApplication::translate("Add_lesson", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("Add_lesson", "\320\237\321\200\320\265\320\264\320\274\320\265\321\202", nullptr));
        label_2->setText(QCoreApplication::translate("Add_lesson", "\320\223\321\200\321\203\320\277\320\277\320\260", nullptr));
        label_3->setText(QCoreApplication::translate("Add_lesson", "\320\220\321\203\320\264\320\270\321\202\320\276\321\200\320\270\321\217", nullptr));
        label_5->setText(QCoreApplication::translate("Add_lesson", "\320\237\320\276\320\264\320\263\321\200\321\203\320\277\320\277\320\260", nullptr));
        label_6->setText(QCoreApplication::translate("Add_lesson", "\320\242\320\270\320\277 \320\275\320\265\320\264\320\265\320\273\320\270", nullptr));
        label_4->setText(QCoreApplication::translate("Add_lesson", "\320\242\320\270\320\277 \320\267\320\260\320\275\321\217\321\202\320\270\321\217", nullptr));
        applyButton->setText(QCoreApplication::translate("Add_lesson", "\320\237\321\200\320\270\320\275\321\217\321\202\321\214", nullptr));
        closeButton->setText(QCoreApplication::translate("Add_lesson", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Add_lesson: public Ui_Add_lesson {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADD_LESSON_H
