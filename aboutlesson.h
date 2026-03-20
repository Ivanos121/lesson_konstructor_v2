#ifndef ABOUTLESSON_H
#define ABOUTLESSON_H

#include <QDialog>

namespace Ui {
class aboutLesson;
}

class aboutLesson : public QDialog
{
    Q_OBJECT

public:
    explicit aboutLesson(QWidget *parent = nullptr);
    ~aboutLesson();

private slots:
    void on_pushButton_clicked();

private:
    Ui::aboutLesson *ui;
};

#endif // ABOUTLESSON_H
