#ifndef ADD_LESSON_H
#define ADD_LESSON_H

#include "ui_add_lesson.h"
#include <QDialog>

namespace Ui {
class Add_lesson;
}

class Add_lesson : public QDialog
{
    Q_OBJECT

public:
    explicit Add_lesson(QWidget *parent = nullptr);
    ~Add_lesson();

    QString text11;
    QString text22;
    QString text33;
    QString text44;
    Ui::Add_lesson *ui;

protected slots:
    void closeDialog();
    void updateOkButtonState();

signals:
    void selectionConfirmed(const QString &text);

private slots:
    void onAccept();

private:
};

#endif // ADD_LESSON_H
