#ifndef NEWUSER_H
#define NEWUSER_H

#include <QDialog>

namespace Ui {
class NewUser;
}

class NewUser : public QDialog
{
    Q_OBJECT

public:
    explicit NewUser(QWidget *parent = nullptr);
    ~NewUser();

protected slots:
    void apply_user();
    void close_user();
private:
    Ui::NewUser *ui;
};

#endif // NEWUSER_H
