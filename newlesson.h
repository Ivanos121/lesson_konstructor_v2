#ifndef NEWLESSON_H
#define NEWLESSON_H

#include <QDialog>

class MainWindow;

namespace Ui {
class newLesson;
}

class newLesson : public QDialog
{
    Q_OBJECT

public:
    explicit newLesson(QWidget *parent = nullptr);
    ~newLesson();
    MainWindow *wf;

protected slots:
    void newTable();
    void onComboBoxChanged(int index);
private slots:
    void closeTable();

private:
    Ui::newLesson *ui;    
};

#endif // NEWLESSON_H
