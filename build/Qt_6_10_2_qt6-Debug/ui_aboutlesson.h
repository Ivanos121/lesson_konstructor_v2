/********************************************************************************
** Form generated from reading UI file 'aboutlesson.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTLESSON_H
#define UI_ABOUTLESSON_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_aboutLesson
{
public:
    QPushButton *pushButton;
    QLabel *label;

    void setupUi(QDialog *aboutLesson)
    {
        if (aboutLesson->objectName().isEmpty())
            aboutLesson->setObjectName("aboutLesson");
        aboutLesson->resize(364, 271);
        pushButton = new QPushButton(aboutLesson);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(260, 220, 88, 34));
        label = new QLabel(aboutLesson);
        label->setObjectName("label");
        label->setGeometry(QRect(40, 70, 321, 71));
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        font.setItalic(true);
        label->setFont(font);
        label->setTextFormat(Qt::TextFormat::MarkdownText);

        retranslateUi(aboutLesson);

        QMetaObject::connectSlotsByName(aboutLesson);
    } // setupUi

    void retranslateUi(QDialog *aboutLesson)
    {
        aboutLesson->setWindowTitle(QCoreApplication::translate("aboutLesson", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("aboutLesson", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", nullptr));
        label->setText(QCoreApplication::translate("aboutLesson", "\320\237\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\260\321\202\320\276\321\200 \320\275\320\260\320\263\321\200\321\203\320\267\320\272\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class aboutLesson: public Ui_aboutLesson {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTLESSON_H
