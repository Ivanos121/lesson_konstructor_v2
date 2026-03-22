#ifndef ADDLINE_H
#define ADDLINE_H

#include "qdialog.h"
#include <QWidget>

namespace Ui {
class AddLine;
}

class AddLine : public QDialog
{
    Q_OBJECT

public:
    explicit AddLine(QWidget *parent = nullptr);
    ~AddLine();

protected slots:
    void apply_button();
    void close_button();
private:
    Ui::AddLine *ui;
};

#endif // ADDLINE_H
