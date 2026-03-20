/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *newTable;
    QAction *saveDoc;
    QAction *closeApp;
    QAction *openLesson;
    QAction *medgeHorizontalCells;
    QAction *medgeVertikalCells;
    QAction *action;
    QAction *aboutApp;
    QAction *deleteLesson;
    QAction *splitVertikalSell;
    QAction *splitHorizontalCell;
    QAction *medgeCells;
    QAction *splitCells;
    QAction *print_preview_table;
    QAction *print_table;
    QAction *print_preview;
    QAction *printDocument;
    QAction *new_user;
    QAction *addUser;
    QAction *save_check;
    QAction *load_check;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_5;
    QTableWidget *tableWidget_4;
    QWidget *tab_3;
    QGridLayout *gridLayout_3;
    QTableWidget *tableWidget_2;
    QWidget *tab_2;
    QGridLayout *gridLayout_2;
    QTableWidget *tableWidget;
    QWidget *tab_6;
    QGridLayout *gridLayout_6;
    QTableWidget *tableWidget_5;
    QWidget *tab_4;
    QGridLayout *gridLayout_4;
    QTableWidget *tableWidget_3;
    QLabel *currentMonthLabel;
    QWidget *tab_5;
    QGridLayout *gridLayout_7;
    QTableWidget *tableWidget_6;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        newTable = new QAction(MainWindow);
        newTable->setObjectName("newTable");
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("document-new")));
        newTable->setIcon(icon);
        newTable->setMenuRole(QAction::MenuRole::NoRole);
        saveDoc = new QAction(MainWindow);
        saveDoc->setObjectName("saveDoc");
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave));
        saveDoc->setIcon(icon1);
        saveDoc->setMenuRole(QAction::MenuRole::NoRole);
        closeApp = new QAction(MainWindow);
        closeApp->setObjectName("closeApp");
        openLesson = new QAction(MainWindow);
        openLesson->setObjectName("openLesson");
        medgeHorizontalCells = new QAction(MainWindow);
        medgeHorizontalCells->setObjectName("medgeHorizontalCells");
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::GoNext));
        medgeHorizontalCells->setIcon(icon2);
        medgeHorizontalCells->setMenuRole(QAction::MenuRole::NoRole);
        medgeVertikalCells = new QAction(MainWindow);
        medgeVertikalCells->setObjectName("medgeVertikalCells");
        QIcon icon3(QIcon::fromTheme(QIcon::ThemeIcon::GoDown));
        medgeVertikalCells->setIcon(icon3);
        medgeVertikalCells->setMenuRole(QAction::MenuRole::NoRole);
        action = new QAction(MainWindow);
        action->setObjectName("action");
        aboutApp = new QAction(MainWindow);
        aboutApp->setObjectName("aboutApp");
        deleteLesson = new QAction(MainWindow);
        deleteLesson->setObjectName("deleteLesson");
        QIcon icon4(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete));
        deleteLesson->setIcon(icon4);
        deleteLesson->setMenuRole(QAction::MenuRole::NoRole);
        splitVertikalSell = new QAction(MainWindow);
        splitVertikalSell->setObjectName("splitVertikalSell");
        QIcon icon5(QIcon::fromTheme(QIcon::ThemeIcon::MediaSeekBackward));
        splitVertikalSell->setIcon(icon5);
        splitVertikalSell->setMenuRole(QAction::MenuRole::NoRole);
        splitHorizontalCell = new QAction(MainWindow);
        splitHorizontalCell->setObjectName("splitHorizontalCell");
        QIcon icon6(QIcon::fromTheme(QIcon::ThemeIcon::MediaSeekForward));
        splitHorizontalCell->setIcon(icon6);
        splitHorizontalCell->setMenuRole(QAction::MenuRole::NoRole);
        medgeCells = new QAction(MainWindow);
        medgeCells->setObjectName("medgeCells");
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/img/edit-table-cell-merge.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        medgeCells->setIcon(icon7);
        medgeCells->setMenuRole(QAction::MenuRole::NoRole);
        splitCells = new QAction(MainWindow);
        splitCells->setObjectName("splitCells");
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/img/edit-table-cell-split.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        splitCells->setIcon(icon8);
        splitCells->setMenuRole(QAction::MenuRole::NoRole);
        print_preview_table = new QAction(MainWindow);
        print_preview_table->setObjectName("print_preview_table");
        print_table = new QAction(MainWindow);
        print_table->setObjectName("print_table");
        print_preview = new QAction(MainWindow);
        print_preview->setObjectName("print_preview");
        QIcon icon9(QIcon::fromTheme(QIcon::ThemeIcon::DocumentPrintPreview));
        print_preview->setIcon(icon9);
        print_preview->setMenuRole(QAction::MenuRole::NoRole);
        printDocument = new QAction(MainWindow);
        printDocument->setObjectName("printDocument");
        QIcon icon10(QIcon::fromTheme(QIcon::ThemeIcon::DocumentPrint));
        printDocument->setIcon(icon10);
        printDocument->setMenuRole(QAction::MenuRole::NoRole);
        new_user = new QAction(MainWindow);
        new_user->setObjectName("new_user");
        addUser = new QAction(MainWindow);
        addUser->setObjectName("addUser");
        QIcon icon11(QIcon::fromTheme(QIcon::ThemeIcon::ContactNew));
        addUser->setIcon(icon11);
        addUser->setMenuRole(QAction::MenuRole::NoRole);
        save_check = new QAction(MainWindow);
        save_check->setObjectName("save_check");
        QIcon icon12(QIcon::fromTheme(QIcon::ThemeIcon::ZoomIn));
        save_check->setIcon(icon12);
        save_check->setMenuRole(QAction::MenuRole::NoRole);
        load_check = new QAction(MainWindow);
        load_check->setObjectName("load_check");
        QIcon icon13(QIcon::fromTheme(QIcon::ThemeIcon::ZoomOut));
        load_check->setIcon(icon13);
        load_check->setMenuRole(QAction::MenuRole::NoRole);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tab = new QWidget();
        tab->setObjectName("tab");
        gridLayout_5 = new QGridLayout(tab);
        gridLayout_5->setObjectName("gridLayout_5");
        tableWidget_4 = new QTableWidget(tab);
        tableWidget_4->setObjectName("tableWidget_4");

        gridLayout_5->addWidget(tableWidget_4, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        gridLayout_3 = new QGridLayout(tab_3);
        gridLayout_3->setObjectName("gridLayout_3");
        tableWidget_2 = new QTableWidget(tab_3);
        tableWidget_2->setObjectName("tableWidget_2");

        gridLayout_3->addWidget(tableWidget_2, 0, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        gridLayout_2 = new QGridLayout(tab_2);
        gridLayout_2->setObjectName("gridLayout_2");
        tableWidget = new QTableWidget(tab_2);
        tableWidget->setObjectName("tableWidget");

        gridLayout_2->addWidget(tableWidget, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName("tab_6");
        gridLayout_6 = new QGridLayout(tab_6);
        gridLayout_6->setObjectName("gridLayout_6");
        tableWidget_5 = new QTableWidget(tab_6);
        tableWidget_5->setObjectName("tableWidget_5");

        gridLayout_6->addWidget(tableWidget_5, 0, 0, 1, 1);

        tabWidget->addTab(tab_6, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        gridLayout_4 = new QGridLayout(tab_4);
        gridLayout_4->setObjectName("gridLayout_4");
        tableWidget_3 = new QTableWidget(tab_4);
        tableWidget_3->setObjectName("tableWidget_3");

        gridLayout_4->addWidget(tableWidget_3, 1, 0, 1, 1);

        currentMonthLabel = new QLabel(tab_4);
        currentMonthLabel->setObjectName("currentMonthLabel");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        currentMonthLabel->setFont(font);

        gridLayout_4->addWidget(currentMonthLabel, 0, 0, 1, 1, Qt::AlignmentFlag::AlignRight);

        tabWidget->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName("tab_5");
        gridLayout_7 = new QGridLayout(tab_5);
        gridLayout_7->setObjectName("gridLayout_7");
        tableWidget_6 = new QTableWidget(tab_5);
        tableWidget_6->setObjectName("tableWidget_6");

        gridLayout_7->addWidget(tableWidget_6, 0, 0, 1, 1);

        tabWidget->addTab(tab_5, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 30));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName("menu_2");
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName("menu_3");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menu->addAction(new_user);
        menu->addAction(openLesson);
        menu->addAction(print_preview_table);
        menu->addAction(print_table);
        menu->addAction(closeApp);
        menu_3->addAction(action);
        menu_3->addAction(aboutApp);
        toolBar->addAction(addUser);
        toolBar->addAction(newTable);
        toolBar->addAction(saveDoc);
        toolBar->addSeparator();
        toolBar->addAction(print_preview);
        toolBar->addAction(printDocument);
        toolBar->addSeparator();
        toolBar->addAction(medgeCells);
        toolBar->addAction(splitCells);
        toolBar->addAction(deleteLesson);
        toolBar->addAction(save_check);
        toolBar->addAction(load_check);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        newTable->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\320\267\320\264\320\260\320\275\320\270\320\265 \320\275\320\276\320\262\320\276\320\271 \320\261\320\260\320\267\321\213", nullptr));
        saveDoc->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\264\320\260\320\275\320\275\321\213\320\265", nullptr));
        closeApp->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", nullptr));
        openLesson->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", nullptr));
        medgeHorizontalCells->setText(QCoreApplication::translate("MainWindow", "\320\236\320\261\321\212\320\265\320\264\320\270\320\275\320\265\320\275\320\270\320\265 \320\263\320\276\321\200\320\270\320\267\320\276\320\275\321\202\320\260\320\273\321\214\320\275\321\213\321\205 \321\217\321\207\320\265\320\265\320\272", nullptr));
        medgeVertikalCells->setText(QCoreApplication::translate("MainWindow", "\320\236\320\261\321\212\320\265\320\264\320\270\320\275\320\265\320\275\320\270\320\265 \320\262\320\265\321\200\321\202\320\270\320\272\320\260\320\273\321\214\320\275\321\213\321\205 \321\217\321\207\320\265\320\265\320\272", nullptr));
        action->setText(QCoreApplication::translate("MainWindow", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260", nullptr));
        aboutApp->setText(QCoreApplication::translate("MainWindow", "\320\236 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\320\265", nullptr));
        deleteLesson->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\265\320\275\320\270\320\265 \320\264\320\260\320\275\320\275\321\213\321\205", nullptr));
        splitVertikalSell->setText(QCoreApplication::translate("MainWindow", "\320\240\320\260\320\267\320\261\320\270\321\202\321\214 \320\262\320\265\321\200\321\202\320\270\320\272\320\260\320\273\321\214\320\275\321\203\321\216 \321\217\321\207\320\265\320\271\320\272\321\203 \320\275\320\260 \320\264\320\262\320\265", nullptr));
        splitHorizontalCell->setText(QCoreApplication::translate("MainWindow", "\320\240\320\260\320\267\320\264\320\265\320\273\320\270\321\202\321\214 \320\263\320\276\321\200\320\270\320\267\320\276\320\275\321\202\320\260\320\273\321\214\320\275\321\203\321\216 \321\217\321\207\320\265\320\271\320\272\321\203", nullptr));
        medgeCells->setText(QCoreApplication::translate("MainWindow", "\320\236\320\261\321\212\320\265\320\264\320\270\320\275\320\265\320\275\320\270\320\265 \321\217\321\207\320\265\320\265\320\272", nullptr));
        splitCells->setText(QCoreApplication::translate("MainWindow", "\320\240\320\260\320\267\320\261\320\270\321\202\321\214 \321\217\321\207\320\265\320\271\320\272\320\270", nullptr));
        print_preview_table->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\265\320\264\320\262\320\260\321\200\320\270\321\202\320\265\320\273\321\214\320\275\321\213\320\271 \320\277\321\200\320\276\321\201\320\274\320\276\321\202\321\200", nullptr));
        print_table->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\207\320\260\321\202\321\214", nullptr));
        print_preview->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\265\320\264\320\262\320\260\321\200\320\270\321\202\320\265\320\273\321\214\320\275\321\213\320\271 \320\277\321\200\320\276\321\201\321\207\320\274\320\276\321\202\321\200", nullptr));
        printDocument->setText(QCoreApplication::translate("MainWindow", "\320\237\320\265\321\207\320\260\321\202\321\214 \321\202\320\260\320\261\320\273\320\270\321\206", nullptr));
        new_user->setText(QCoreApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\214", nullptr));
        addUser->setText(QCoreApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\214", nullptr));
        save_check->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\272\320\260\320\273\320\265\320\275\320\264\320\260\321\200\321\214", nullptr));
        load_check->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \320\272\320\260\320\273\320\265\320\275\320\264\320\260\321\200\321\214", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "\320\230\321\201\321\205\320\276\320\264\320\275\321\213\320\265 \320\264\320\260\320\275\320\275\321\213\320\265", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "\320\240\320\260\321\201\320\277\320\270\321\201\320\260\320\275\320\270\320\265 \320\276\321\201\320\265\320\275\320\275\320\270\320\271 \321\201\320\265\320\274\320\265\321\201\321\202\321\200", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "\320\240\320\260\321\201\320\277\320\270\321\201\320\260\320\275\320\270\320\265 \320\262\320\265\321\201\320\265\320\275\320\275\320\270\320\271 \321\201\320\265\320\274\320\265\321\201\321\202\321\200", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QCoreApplication::translate("MainWindow", "\320\240\320\260\321\201\320\277\320\270\321\201\320\260\320\275\320\270\320\265 \320\267\320\260\320\276\321\207\320\275\320\270\320\272\320\276\320\262", nullptr));
        currentMonthLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainWindow", "\320\227\320\260\320\275\321\217\321\202\320\276\321\201\321\202\321\214 \320\267\320\260 \320\274\320\265\321\201\321\217\321\206", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QCoreApplication::translate("MainWindow", "\320\240\320\260\321\201\321\207\320\265\321\202 \320\275\320\260\320\263\321\200\321\203\320\267\320\272\320\270", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", nullptr));
        menu_2->setTitle(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\260\320\262\320\272\320\260", nullptr));
        menu_3->setTitle(QCoreApplication::translate("MainWindow", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
