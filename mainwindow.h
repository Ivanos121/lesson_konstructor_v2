#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "aboutlesson.h"
#include "newuser.h"
#include "qcheckbox.h"
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
    void CreateNewTable(int pageNum = -1);
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


    void refreshTableDisplay();
    void loadDataToTable(QSqlDatabase &db, QTableWidget *tableWidget);
    void applyRowHeights();
    void addLineslessonList();
    bool print_lesson(QTableWidget *table, QPrinter *printer,
                      const QString &headerText = "", const QString &subHeaderText = "");

    void formatTableItems(QTableWidget *tableWidget);
    void adjustLabelFont();
    // QString getDateForCell(int row, int col, int year, int month);
    // void exportToPDF(QTableWidget *tableWidget, const QString &filename);
    // void traverseTable(QTableWidget *tableWidget);
    void printFullTable(QTableWidget *tableWidget, QPrinter *printer, QString teacherInfo, QString semesterInfo);

public slots:
    void message_action(QString summary, QString body);

protected:
    //void hideColums();
    void resizeEvent(QResizeEvent *event) override;
    void setColumnAndRowSizes(QTableWidget *tableWidget);
    //void updateTable(int month, int year);
    bool isValidColumn(const QString &columnName) const;
    QTableWidget *getCurrentTable();
    void showEvent(QShowEvent *event) override;

protected slots:
    //void onPageChanged(int index);
    bool saveBase(QTableWidget *table, const QString &dbTable);
    void onItemChanged();
    void NewTable();
    void closeEvent(QCloseEvent *event) override;
    void openLesson();
    // void saveCheckboxState(int month, int year);
    bool medgevertikalcells(QTableWidget *tableWidget, int row, int col,
        const QStringList &nonEmptyParts, const QString &verticalControlString, QSet<QPair<int, int> > &mergedPositions);
    QPair<int, int> determineTargetCell(int row, int column, const QString &combo2,
                                        const QString &combo3, int rowCount);
    void onCheckBoxToggled(Qt::CheckState state);

    void medgehorizontalcells();
    void aboutLessons();
    void deleteLesson();
    void ClickedLeftButton(int row, int column);
    void mergeCells();
    void splitCells();
    void onPageChanged2(int index);
    void printLessonDialog();
    void showPrintPreview();
    //void onMonthChanged(int index);
    void onTabChanged(int index);
    void newUser();
    // void loadCheckboxState(int month, int year);
    void addLine();
    void removeLine();
    void ClickedLeftButton2(int row, int column);
    // void on_monthCombo_currentIndexChanged(int index);
    // void on_dateChanged();
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
    QMap<QPair<int,int>, QString> savedCells_;

    void syncCalendar();
    int nextPageNumberFromDb();
    void populateTablesCombo();
    void addLinesComboBox();
    int getCurrentItemId(const QString &tableName, int row);
    QString getUserDataForTable(const QString &tableName, const QString &field, int row);
    //void loadTableData(const QString &tableName);
    void reloadAllTables();
    void loadTableFromDatabase(const QString &tableName, QTableWidget *tableWidget);
    QStringList getTableNames();
    bool connectToDatabase(QSqlDatabase &db, QString path);
    void loadAndAutoMergeFromDb2(QSqlDatabase &db, QTableWidget *tableWidget,
                                 const QString &controlString, const QString &verticalControlString);
    //void clearTableCheckboxes();
    void updateCheckboxInTable(int index, int state);
    void loadMonth(int year, int month);
    void saveMonth(int year, int month);
    // void updateCheckbox(int index, int state);
    // void clearTable();
    // void overwriteMonth(int year, int month);
    void updateCellVisuals(int dayIndex, int state, bool isToday);
    void rebuildCalendarGrid(int year, int month);
    QString getPositionKey(int row, int col);
};
#endif // MAINWINDOW_H
