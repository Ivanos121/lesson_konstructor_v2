#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "aboutlesson.h"
#include "newuser.h"
#include "qsqldatabase.h"
#include "newlesson.h"
#include "add_lesson.h"
#include "ui_mainwindow.h"
#include "vertikalheaderview.h"

#include <QListWidget>
#include <QMainWindow>
#include <QPrinter>
#include <QTableWidget>

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
    QTableWidget *tv;
    QString *fisrt_upgroup;
    QString *two_upgroup;
    QString *up_week;
    QString *down_week;
    QString lection;
    QString lab_rab;
    QString pract_rab;
    VertikalheaderView *horHeader;


    void refreshTableDisplay();

    void loadDataToTable(QSqlDatabase &db, QTableWidget *tableWidget);
    void applyRowHeights();
    void addLineslessonList();
    bool print_lesson(QTableWidget *table, QPrinter *printer,
                      const QString &headerText = "", const QString &subHeaderText = "");

    void formatTableItems(QTableWidget *tableWidget);
    void adjustLabelFont();
    QString getDateForCell(int row, int col, int year, int month);
    void exportToPDF(QTableWidget *tableWidget, const QString &filename);
    void traverseTable(QTableWidget *tableWidget);
    void printFullTable(QTableWidget *tableWidget, QPrinter *printer, QString teacherInfo, QString semesterInfo);

protected:
    void hideColums();
    void resizeEvent(QResizeEvent *event) override;
    void setColumnAndRowSizes(QTableWidget *tableWidget);
    void updateTable(int month, int year);
    bool isValidColumn(const QString &columnName) const;

protected slots:
    //void onPageChanged(int index);
    bool saveBase();
    void onItemChanged();
    void NewTable();
    void closeEvent(QCloseEvent *event) override;
    void openLesson();
    void saveCheckboxState(int month, int year);
    bool medgevertikalcells(QTableWidget *tableWidget, int row, int col,
        const QStringList &nonEmptyParts, const QString &verticalControlString, QSet<QPair<int, int> > &mergedPositions);
    QPair<int, int> determineTargetCell(int row, int column, const QString &combo2,
                                        const QString &combo3, int rowCount);


    void medgehorizontalcells();
    void aboutLessons();
    void deleteLesson();
    void ClickedLeftButton(int row, int column);
    void mergeCells();
    void splitCells();

    void onPageChanged2(int index);
    void printLessonDialog();
    void showPrintPreview();
    void onMonthChanged(int index);
    void onTabChanged(int index);
    void newUser();

private:
    newLesson *rsc;
    Add_lesson *rsc2;
    NewUser *rsc4;
    bool isModified = false;
    bool editingEnabled = false;
    QLabel *label;
    QLabel *label2;

    QSqlDatabase db;
    QMap<QString, QTableWidget*> tables;
    QListWidget* tableListWidget;
    QString currentTableName;
    QString controlString;
    QString verticalControlString;
    QSet<QPair<int,int>> mergedLefts_;
    QMap<QPair<int,int>, QString> savedCells_;

    int nextPageNumberFromDb();
    void populateTablesCombo();
    void addLinesComboBox();
    int getCurrentItemId(const QString &tableName, int row);
    QString getUserDataForTable(const QString &tableName, const QString &field, int row);
    void loadTableData(const QString &tableName);
    void reloadAllTables();
    void loadTableFromDatabase(const QString &tableName, QTableWidget *tableWidget);
    QStringList getTableNames();
    bool connectToDatabase(QSqlDatabase &db, QString path);
    void loadAndAutoMergeFromDb2(QSqlDatabase &db, QTableWidget *tableWidget, const QString &controlString, const QString &verticalControlString);
};
#endif // MAINWINDOW_H
