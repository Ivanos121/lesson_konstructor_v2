#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "aboutlesson.h"
#include "newuser.h"
#include "qsqldatabase.h"
#include "newlesson.h"
#include "add_lesson.h"
#include "ui_mainwindow.h"
#include "vertikalheaderview.h"
#include "addline.h"

#include <QListWidget>
#include <QMainWindow>
#include <QPrinter>
#include <QTableWidget>
#include <qspinbox.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;
    aboutLesson *rsc3;
    QComboBox *lessonList;
    QComboBox *monthList;
    QSpinBox *yearSpinBox;
    QTableWidget *tv;
    QString *fisrt_upgroup;
    QString *two_upgroup;
    QString *up_week;
    QString *down_week;
    QString lection;
    QString lab_rab;
    QString pract_rab;
    VertikalheaderView *horHeader, *horHeader7, *horHeader8, *horHeader9, *horHeader4,
        *horHeader10 ;

    void applyRowHeights(QTableWidget *table);
    void CreateNewTable();
    bool print_lesson(QTableWidget *table, QPrinter *printer,
                      const QString &headerText = "", const QString &subHeaderText = "");

    void adjustLabelFont();
    void printFullTable(QTableWidget *tableWidget, QPrinter *printer, QString teacherInfo, QString semesterInfo);


public slots:
    void message_action(QString summary, QString body);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void setColumnAndRowSizes(QTableWidget *tableWidget);
    bool isValidColumn(const QString &columnName) const;
    QTableWidget *getCurrentTable();
    void showEvent(QShowEvent *event) override;

protected slots:
    void showContextMenu(const QPoint &pos);
    bool saveBase(QTableWidget *table, const QString &dbTable);
    void onItemChanged();
    void NewTable();
    void closeEvent(QCloseEvent *event) override;
    void onCheckBoxToggled(Qt::CheckState state);

    void aboutLessons();
    void deleteLesson();
    void ClickedLeftButton(int row, int column);
    void mergeCells();
    void splitCells();
    void printLessonDialog();
    void showPrintPreview();
    void onTabChanged(int index);
    void newUser();
    void addLine();
    void removeLine();
    void ClickedLeftButton2(int row);
    void loadBase(QTableWidget *table, const QString &dbTable);

private:
    newLesson *rsc;
    Add_lesson *rsc2;
    AddLine *rsc5;
    NewUser *rsc4;
    bool isModified = false;
    bool editingEnabled = false;
    QLabel *label;
    QLabel *label2;
    bool isLoaded = false;
    int lastYear;
    int lastMonth;
    int lastLoadedMonth;
    int lastLoadedYear;
    QSqlDatabase db;
    QMap<QString, QTableWidget*> tables;
    QListWidget* tableListWidget;
    QString currentTableName;
    QString controlString;
    QString verticalControlString;
    QSet<QPair<int,int>> mergedLefts_;

    void syncCalendar();
    void loadMonth(int year, int month);
    void saveMonth(int year, int month);
    void updateCellVisuals(int dayIndex, int state, bool isToday);
    void rebuildCalendarGrid(int year, int month);
    void copySelection();
    void pasteSelection();
};
#endif // MAINWINDOW_H
