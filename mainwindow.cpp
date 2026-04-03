#include "mainwindow.h"
#include "aboutlesson.h"
#include "qpainter.h"
#include "ui_mainwindow.h"
#include "mergedheader.h"
#include "newuser.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QSqlRecord>
#include <QMessageBox>
#include <QScreen>
#include <QCloseEvent>
#include <QScrollBar>
#include <QDir>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QScroller>
#include <QCheckBox>
#include <QTimer>
#include <QDBusInterface>
#include <QDBusReply>
#include <QTimer>
#include <utility>

//=======ИНИЦИАЛИЗАЦИЯ ГЛОБАЛЬНЫХ ПЕРЕМЕННЫХ========

QString lesson_name;
QString userSurname;
QString userPosition;

double lections;
double lab_rabs;
double pract_rabs;
double mrs;
double individ_zad;
double kurs_rab;
double konsult;
double recenz;
double zachet;
double exam;
double ucheb_pract;
double zavod_pract;
double diplom_proect;
double gek_gak;
double aspirant;
double sum_semestr;
double sum_year;
QString subject_name;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    //========ОТКРЫТИЕ БАЗЫ БАННЫХ=========
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QString(PROJECT_PATH) + "/base/lesson_base.db";

    qDebug() << "Путь базы данных" << dbPath;

    db.setDatabaseName(dbPath);

    if (!db.open())
    {
        qCritical() << "Cannot open DB:" << db.lastError().text();
        throw std::runtime_error("Database connection failed");
    }

    //======ИНИЦИАЛИЗАЦИЯ СТАРТОВЫХ ЗНАЧЕНИЙ КАЛЕНДАРЯ=========

    // QDate today = QDate::currentDate();
    // lastLoadedMonth = today.month();
    // lastLoadedYear = today.year();

    QToolBar* pToolbar = ui->toolBar;

    lessonList = new QComboBox(this);
    monthList = new QComboBox(this);
    yearSpinBox = new QSpinBox(this);

    yearSpinBox->setRange(2000, 2100);
    yearSpinBox->setValue(2024);
    yearSpinBox->setValue(lastYear);

    // Добавляем элементы (опции)
    lessonList->addItem("Опция 1");
    lessonList->addItem("Опция 2");
    lessonList->addItem("Опция 3");

    // Добавляем элементы (опции)
    monthList->addItem("Январь");
    monthList->addItem("Февраль");
    monthList->addItem("Март");
    monthList->addItem("Апрель");
    monthList->addItem("Май");
    monthList->addItem("Июнь");
    monthList->addItem("Июль");
    monthList->addItem("Август");
    monthList->addItem("Сентябрь");
    monthList->addItem("Октябрь");
    monthList->addItem("Ноябрь");
    monthList->addItem("Декабрь");
    monthList->setCurrentIndex(lastLoadedMonth - 1);

    pToolbar->addWidget(lessonList);
    pToolbar->addWidget(monthList);
    monthList->setCurrentIndex(1);
    pToolbar->addWidget(yearSpinBox);
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pToolbar->addWidget(spacer);

    QFont font;
    font.setPointSize(16);           // Размер шрифта
    font.setBold(true);              // Жирный
    font.setItalic(false);            // Курсив
    font.setFamily("Arial");          // Семейство шрифта

    label = new QLabel("Ст. преподаватель Третьяков А. С.");
    label->setFont(font);
    pToolbar->addWidget(label);

    // Пространство перед сепаратором
    QWidget *spacerBefore = new QWidget();
    spacerBefore->setFixedWidth(15);
    pToolbar->addWidget(spacerBefore);
    pToolbar->addSeparator();
    QWidget *spacerAfter = new QWidget();
    spacerAfter->setFixedWidth(15);
    pToolbar->addWidget(spacerAfter);

    label2 = new QLabel("Весенний семестр 2025/2026 учебного года");
    label2->setFont(font);
    pToolbar->addWidget(label2);
    QWidget *spacerAfter2 = new QWidget();
    spacerAfter2->setFixedWidth(15);
    pToolbar->addWidget(spacerAfter2);

    addLinesComboBox();
    addLineslessonList();

    onPageChanged2(lessonList->currentIndex());
    setWindowTitle("Программатор нагрузки");

    //=====НАСТРОЙКА TABLEWIDGET=====

    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setColumnCount(13);

    // Скрываем технический столбец и настраиваем перенос текста
    ui->tableWidget->setWordWrap(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    ui->tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    // Инициализируем ячейки пустыми объектами (чтобы не было nullptr)
    for (int r = 0; r < ui->tableWidget->rowCount(); ++r) {
        for (int c = 0; c < ui->tableWidget->columnCount(); ++c) {
            ui->tableWidget->setItem(r, c, new QTableWidgetItem(""));
        }
    }

    ui->tableWidget->updateGeometry();

    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

    //==========ИНИЦИАЛИЗАЦИЯ ВЕРТИКАЛЬНОГО ЗАГОЛОВКА==========
    MergedHeader *verticalHeader = new MergedHeader(Qt::Vertical, ui->tableWidget);
    ui->tableWidget->setVerticalHeader(verticalHeader);
    verticalHeader->setModel(ui->tableWidget->model());


    connect(ui->tableWidget->verticalScrollBar(), &QScrollBar::valueChanged,
            verticalHeader, &QHeaderView::setOffset);
    ui->tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);


    QList<QPair<int, int>> verticalMerges = {
        {0, 2},   // строки 0-1
        {2, 2},   // строки 2-3
        {4, 2},   // строки 4-5
        {6, 2},   // строки 6-7
        {8, 2},   // строки 4-5
        {10, 2}   // строки 6-7
    };

    QStringList verticalTexts = {
        "8.30 - 10.05",
        "10.25 - 12.00",
        "12.30 - 14.05",
        "14.20 - 15.55",
        "16.05 - 17.40"
    };

    verticalHeader->setMergeRanges(verticalMerges);
    verticalHeader->setHeaderTexts(verticalTexts);

    verticalHeader->verticalScrollBar()->setEnabled(false);

    ui->tableWidget->verticalHeader()->setFixedWidth(100);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setMinimumSectionSize(40);

    // Для ГОРИЗОНТАЛЬНОГО заголовка (столбцы)
    MergedHeader *horizontalHeader = new MergedHeader(Qt::Horizontal, ui->tableWidget);
    ui->tableWidget->setHorizontalHeader(horizontalHeader);

    QList<QPair<int, int>> horizontalMerges = {
        {1, 2},   // столбцы 0-1
        {3, 2},   // столбцы 2-3
        {5, 2},   // столбцы 2-3
        {7, 2},   // столбцы 0-1
        {9, 2},   // столбцы 2-3
        {11, 2},   // столбцы 2-3
    };

    QStringList horizontalTexts = {
        "Понедельник",
        "Вторник",
        "Среда",
        "Четверг",
        "Пятница",
        "Суббота"
    };

    horizontalHeader->setMergeRanges(horizontalMerges);
    horizontalHeader->setHeaderTexts(horizontalTexts);

    // Настройка растяжения столбцов
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->hideColumn(0);

    // Принудительное обновление геометрии
    ui->tableWidget->updateGeometry();

    QScroller::grabGesture(ui->tableWidget, QScroller::LeftMouseButtonGesture);

    connect(lessonList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onPageChanged2);
    connect(ui->newTable, &QAction::triggered, this, &MainWindow::NewTable);
    connect(ui->saveDoc, &QAction::triggered, this, &MainWindow::saveBase);
    connect(ui->loadDoc, &QAction::triggered, this, &MainWindow::loadBase);
    connect(ui->closeApp, &QAction::triggered, this, &MainWindow::close);
    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &MainWindow::onItemChanged);
    connect(ui->openLesson, &QAction::triggered, this, &MainWindow::openLesson);
    connect(ui->medgeHorizontalCells, &QAction::triggered, this, &MainWindow::medgehorizontalcells);
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, this, &MainWindow::ClickedLeftButton);
    connect(ui->tableWidget_2, &QTableWidget::cellDoubleClicked, this, &MainWindow::ClickedLeftButton);
    connect(ui->tableWidget_4, &QTableWidget::cellDoubleClicked, this, &MainWindow::ClickedLeftButton2);
    connect(ui->medgeCells, &QAction::triggered, this, &MainWindow::mergeCells);
    connect(ui->splitCells, &QAction::triggered, this, &MainWindow::splitCells);
    connect(ui->aboutApp, &QAction::triggered, this, &MainWindow::aboutLessons);
    connect(ui->deleteLesson, &QAction::triggered, this, &MainWindow::deleteLesson);
    connect(ui->print_table, &QAction::triggered, this, &MainWindow::printLessonDialog);
    connect(ui->printDocument, &QAction::triggered, this, &MainWindow::printLessonDialog);
    connect(ui->print_preview, &QAction::triggered, this, &MainWindow::showPrintPreview);
    connect(ui->print_preview_table, &QAction::triggered, this, &MainWindow::showPrintPreview);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    connect(ui->new_user, &QAction::triggered, this, &MainWindow::newUser);
    connect(ui->addUser, &QAction::triggered, this, &MainWindow::newUser);

    // Подключаем месяц к общему методу syncCalendar
    connect(monthList, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::syncCalendar);

    // Подключаем год к тому же методу syncCalendar
    connect(yearSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::syncCalendar);

    connect(ui->save_check, &QAction::triggered, this, [this]()
            {
                ui->save_check->setEnabled(false); // Выключаем кнопку на время записи
                int y = yearSpinBox->value();
                int m = monthList->currentIndex() + 1;
                saveMonth(y, m);
                ui->save_check->setEnabled(true);  // Включаем обратно
            });

    connect(ui->load_check, &QAction::triggered, this, [this]()
            {
                int y = yearSpinBox->value();
                int m = monthList->currentIndex() + 1;
                loadMonth(y, m);
            });

    connect(ui->openCurrentCalendar, &QAction::triggered, this, [this]() {
        QDate current = QDate::currentDate();

        // Блокируем сигналы, чтобы syncCalendar вызвался только один раз
        yearSpinBox->blockSignals(true);
        monthList->blockSignals(true);   

        yearSpinBox->setValue(current.year());
        monthList->setCurrentIndex(current.month() - 1);

        yearSpinBox->blockSignals(false);
        monthList->blockSignals(false);

        rebuildCalendarGrid(current.year(), current.month());
        syncCalendar();

        message_action("Календарь", QString("Переход к: %1 %2")
                                        .arg(monthList->currentText())
                                        .arg(current.year()));
    });

    QDate current = QDate::currentDate();
    yearSpinBox->blockSignals(true);
    monthList->blockSignals(true);

    yearSpinBox->setValue(current.year());
    monthList->setCurrentIndex(current.month() - 1);

    yearSpinBox->blockSignals(false);
    monthList->blockSignals(false);

    rebuildCalendarGrid(current.year(), current.month());
    syncCalendar();

    label2->setText(ui->tabWidget->tabText(0));

    //applyRowHeights();
    //hideColums();

    adjustLabelFont();

    //======НАСТРОЙКА TABLEWIDGET_3======

    QDate currentDate = QDate::currentDate();
    int month = currentDate.month();
    int year = currentDate.year();

    ui->tableWidget_3->setRowCount(6);
    ui->tableWidget_3->setColumnCount(7);

    setColumnAndRowSizes(ui->tableWidget_3);

    for (int i = 0; i < 42; ++i) {
        int row = i / 7;
        int col = i % 7;

        // КРИТИЧЕСКАЯ ПРОВЕРКА: если в ячейке уже что-то есть, пропускаем!
        if (ui->tableWidget_3->cellWidget(row, col) != nullptr) {
            continue;
        }

        QWidget* container = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(container);
        layout->setContentsMargins(10, 0, 10, 0);
        layout->setSpacing(0);

        QCheckBox* checkBox = new QCheckBox();
        checkBox->setObjectName("checkBox");
        checkBox->setFixedSize(20, 20);

        QLabel* dayLabel = new QLabel("");
        dayLabel->setObjectName("dayLabel");
        dayLabel->setAlignment(Qt::AlignCenter);
        dayLabel->setStyleSheet("font-size: 18px; font-weight: bold;");

        connect(checkBox, &QCheckBox::checkStateChanged, this, &MainWindow::onCheckBoxToggled);


        layout->addWidget(checkBox);
        layout->addStretch();
        layout->addWidget(dayLabel);
        layout->addStretch();

        container->setLayout(layout);

        ui->tableWidget_3->setCellWidget(row, col, container);
    }

    // 2. Устанавливаем начальные значения виджетов
    QDate current2 = QDate::currentDate();
    yearSpinBox->blockSignals(true);
    yearSpinBox->setValue(current2.year());
    yearSpinBox->blockSignals(false);

    // 3. САМЫЙ ВАЖНЫЙ ШАГ: Разрешаем работу логики
    isLoaded = true;

    // 4. Теперь вызываем отрисовку вручную один раз
    rebuildCalendarGrid(current2.year(), current2.month());

    yearSpinBox->setValue(QDate::currentDate().year());
    monthList->setCurrentIndex(QDate::currentDate().month() - 1);

    syncCalendar();

    QStringList headers = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресенье"};
    ui->tableWidget_3->setHorizontalHeaderLabels(headers);

    // Исправленный вывод названия месяца через QLocale
    // QLocale russian(QLocale::Russian);
    // ui->currentMonthLabel->setText("0" + QString::number(month) + " " + QString::number(year));

    QStringList monthNames = {
        "", "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
        "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"
    };

    ui->currentMonthLabel->setText(monthNames[month] + " " + QString::number(year));

    ui->tableWidget_3->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget_3->setShowGrid(true); // Отключаем сетку таблицы
    ui->tableWidget_3->verticalHeader()->setVisible(false); // Скрываем номера строк
    ui->tableWidget_3->horizontalHeader()->setVisible(true); // Скрываем заголовки колонок
    ui->tableWidget_3->setSelectionMode(QAbstractItemView::NoSelection); // Запрещаем выделение синим

    //======НАСТРОЙКА TABLEWIDGET_6===============

    ui->tableWidget_6->setRowCount(10);
    ui->tableWidget_6->setColumnCount(13);

    horHeader = new VertikalheaderView(Qt::Horizontal, ui->tableWidget_6);
    ui->tableWidget_6->setHorizontalHeader(horHeader);

    QStringList horizontalTextss = {
        "Лекции",
        "Лабораторные работы",
        "Практические занятия",
        "МРС",
        "Индивидуальные занятия",
        "Курсовое проектирование",
        "Консультации",
        "Рецензирование",
        "Зачеты",
        "Экзамены",
        "Производственная практика",
        "Дипломное проектирование",
        "Итого, часов"
    };

    ui->tableWidget_6->setHorizontalHeaderLabels(horizontalTextss);

    QStringList vertikalTextss = {
        "Февраль",
        "Март",
        "Апрель",
        "Май",
        "Июнь",
        "Июль",
        "Итого по плану",
        "Итого за семестр",
        "Итого по плану за год",
        "Итого за год"

    };

    ui->tableWidget_6->setVerticalHeaderLabels(vertikalTextss);
    ui->tableWidget_6->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int col = 0; col < ui->tableWidget_6->columnCount(); ++col)
    {
        QTableWidgetItem *item = ui->tableWidget_6->item(6, col);

        // Если ячейка пустая, её нужно создать, иначе метод item() вернет 0
        if (!item)
        {
            item = new QTableWidgetItem();
            ui->tableWidget_6->setItem(6, col, item);
        }

        item->setBackground(QColor(0x00FF00));
    }

    for (int col = 0; col < ui->tableWidget_6->columnCount(); ++col)
    {
        QTableWidgetItem *item = ui->tableWidget_6->item(8, col);

        // Если ячейка пустая, её нужно создать, иначе метод item() вернет 0
        if (!item)
        {
            item = new QTableWidgetItem();
            ui->tableWidget_6->setItem(8, col, item);
        }

        item->setBackground(QColor(0x00FF00));
    }

    QTableWidgetItem *item = ui->tableWidget_6->item(6, 0);
    item->setBackground(QColor(0x00FF00));

    ui->tableWidget_6->update();


    //======НАСТРОЙКА TABLEWIDGET_4===============

    ui->tableWidget_4->setRowCount(8);
    ui->tableWidget_4->setColumnCount(17);

    horHeader4 = new VertikalheaderView(Qt::Horizontal, ui->tableWidget_4);
    ui->tableWidget_4->setHorizontalHeader(horHeader4);

    QStringList horizontalTexts4 = {
                                    "Лекции",
                                    "Лабораторные работы",
                                    "Практические занятия",
                                    "МРС",
                                    "Индивидуальные занятия",
                                    "Курсовое проектирование",
                                    "Консультации",
                                    "Рецензирование",
                                    "Зачеты",
                                    "Экзамены",
                                    "Учебная практика",
                                    "Производственная практика",
                                    "Дипломное проектирование",
                                    "ГЭК/ГАК",
                                    "Аспирантура/Маг",
                                    "Итого часов по семестрам",
                                    "Итого часов за год"
    };

    ui->tableWidget_4->setHorizontalHeaderLabels(horizontalTexts4);

    QString n1 = " ";
    QString n2 = "Дисциплина";
    QString n3 = "Дисциплина";
    QString n4 = " ";
    QString n5 = "Дисциплина";
    QString n6 = "Дисциплина";

    QStringList vertikalTexts4 = {n1, n2, n3, n4, n5, n6,
        "Итого по плану",
        "Итого за семестр"
    };

    ui->tableWidget_4->setVerticalHeaderLabels(vertikalTexts4);
    ui->tableWidget_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int col = 0; col < ui->tableWidget_4->columnCount(); ++col)
    {
        QTableWidgetItem *item = ui->tableWidget_4->item(6, col);

        // Если ячейка пустая, её нужно создать, иначе метод item() вернет 0
        if (!item) {
            item = new QTableWidgetItem();
            ui->tableWidget_4->setItem(6, col, item);
        }

        item->setBackground(QColor(0x00FF00));
    }
    ui->tableWidget_4->update();

    ui->tableWidget_4->setSpan(0, 0, 1, 17);
    QTableWidgetItem *item4 = new QTableWidgetItem("Осенний семестр");
    item4->setTextAlignment(Qt::AlignCenter);
    item4->setData(Qt::UserRole, "header"); // Метка заголовка
    ui->tableWidget_4->setItem(0, 0, item4);
    item4->setBackground(QColor(0x00FF00));

    ui->tableWidget_4->setSpan(3, 0, 1, 17);
    QTableWidgetItem *item41 = new QTableWidgetItem("Весенний семестр");
    item41->setTextAlignment(Qt::AlignCenter);
    item41->setData(Qt::UserRole, "header"); // Метка заголовка
    ui->tableWidget_4->setItem(3, 0, item41);
    item41->setBackground(QColor(0x00FF00));

    connect(ui->addLine, &QAction::triggered, this, &MainWindow::addLine);
    connect(ui->deleteLine, &QAction::triggered, this, &MainWindow::removeLine);

    QTableWidgetItem *vHeaderItem = new QTableWidgetItem(" ");
    vHeaderItem->setBackground(QColor(0x00FF00));
    vHeaderItem->setTextAlignment(Qt::AlignLeft);
    ui->tableWidget_4->setVerticalHeaderItem(0, vHeaderItem);

    QTableWidgetItem *vHeaderItem3 = new QTableWidgetItem(" ");
    vHeaderItem3->setBackground(QColor(0x00FF00));
    vHeaderItem3->setTextAlignment(Qt::AlignLeft);
    ui->tableWidget_4->setVerticalHeaderItem(3, vHeaderItem3);

    QTableWidgetItem *vHeaderItem6 = new QTableWidgetItem("Итого по плану");
    vHeaderItem6->setBackground(QColor(0x00FF00));
    vHeaderItem6->setTextAlignment(Qt::AlignLeft);
    ui->tableWidget_4->setVerticalHeaderItem(6, vHeaderItem6);

    //======НАСТРОЙКА TABLEWIDGET_7===============

    ui->tableWidget_7->setRowCount(10);
    ui->tableWidget_7->setColumnCount(13);

    horHeader7 = new VertikalheaderView(Qt::Horizontal, ui->tableWidget_7);
    ui->tableWidget_7->setHorizontalHeader(horHeader7);

    QStringList horizontalTexts7 = {
        "Лекции",
        "Лабораторные работы",
        "Практические занятия",
        "МРС",
        "Индивидуальные занятия",
        "Курсовое проектирование",
        "Консультации",
        "Рецензирование",
        "Зачеты",
        "Экзамены",
        "Производственная практика",
        "Дипломное проектирование",
        "Итого, часов"
    };

    ui->tableWidget_7->setHorizontalHeaderLabels(horizontalTexts7);

    QStringList vertikalTexts7 = {
        "Февраль",
        "Март",
        "Апрель",
        "Май",
        "Июнь",
        "Июль",
        "Итого по плану",
        "Итого за семестр",
        "Итого по плану за год",
        "Итого за год"

    };

    ui->tableWidget_7->setVerticalHeaderLabels(vertikalTexts7);
    ui->tableWidget_7->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int col = 0; col < ui->tableWidget_7->columnCount(); ++col)
    {
        QTableWidgetItem *item = ui->tableWidget_7->item(6, col);

        // Если ячейка пустая, её нужно создать, иначе метод item() вернет 0
        if (!item)
        {
            item = new QTableWidgetItem();
            ui->tableWidget_7->setItem(6, col, item);
            //ui->tableWidget_7->setItem(8, col, item);
        }

        item->setBackground(QColor(0x00FF00));
    }
    for (int col = 0; col < ui->tableWidget_7->columnCount(); ++col)
    {
        QTableWidgetItem *item = ui->tableWidget_7->item(8, col);

        // Если ячейка пустая, её нужно создать, иначе метод item() вернет 0
        if (!item)
        {
            item = new QTableWidgetItem();
            ui->tableWidget_7->setItem(8, col, item);
        }

        item->setBackground(QColor(0x00FF00));
    }
    ui->tableWidget_7->update();

    //======НАСТРОЙКА TABLEWIDGET_8===============

    ui->tableWidget_8->setRowCount(8);
    ui->tableWidget_8->setColumnCount(13);

    horHeader8 = new VertikalheaderView(Qt::Horizontal, ui->tableWidget_8);
    ui->tableWidget_8->setHorizontalHeader(horHeader8);

    QStringList horizontalTexts8 = {
        "Лекции",
        "Лабораторные работы",
        "Практические занятия",
        "МРС",
        "Индивидуальные занятия",
        "Курсовое проектирование",
        "Консультации",
        "Рецензирование",
        "Зачеты",
        "Экзамены",
        "Производственная практика",
        "Дипломное проектирование",
        "Итого, часов"
    };

    ui->tableWidget_8->setHorizontalHeaderLabels(horizontalTexts8);

    QStringList vertikalTexts8 = {
        "Февраль",
        "Март",
        "Апрель",
        "Май",
        "Июнь",
        "Июль",
        "Итого по плану",
        "Итого за семестр"
    };

    ui->tableWidget_8->setVerticalHeaderLabels(vertikalTexts8);
    ui->tableWidget_8->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int col = 0; col < ui->tableWidget_8->columnCount(); ++col)
    {
        QTableWidgetItem *item = ui->tableWidget_8->item(6, col);

        // Если ячейка пустая, её нужно создать, иначе метод item() вернет 0
        if (!item) {
            item = new QTableWidgetItem();
            ui->tableWidget_8->setItem(6, col, item);
        }

        item->setBackground(QColor(0x00FF00));
    }
    ui->tableWidget_8->update();

    //======НАСТРОЙКА TABLEWIDGET_9===============

    ui->tableWidget_9->setRowCount(8);
    ui->tableWidget_9->setColumnCount(17);

    horHeader9 = new VertikalheaderView(Qt::Horizontal, ui->tableWidget_9);
    ui->tableWidget_9->setHorizontalHeader(horHeader9);

    QStringList horizontalTexts9 = {
        "Лекции",
        "Лабораторные работы",
        "Практические занятия",
        "МРС",
        "Индивидуальные занятия",
        "Курсовое проектирование",
        "Консультации",
        "Рецензирование",
        "Зачеты",
        "Экзамены",
        "Учебная практика",
        "Производственная практика",
        "Дипломное проектирование",
        "ГЭК/ГАК",
        "Аспирантура/Маг",
        "Итого часов по семестрам",
        "Итого часов за год"
    };

    ui->tableWidget_9->setHorizontalHeaderLabels(horizontalTexts9);



    QStringList vertikalTexts9 = {
        "Февраль",
        "Март",
        "Апрель",
        "Май",
        "Июнь",
        "Июль",
        "Итого по плану",
        "Итого за семестр"
    };

    ui->tableWidget_9->setVerticalHeaderLabels(vertikalTexts9);
    ui->tableWidget_9->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int col = 0; col < ui->tableWidget_9->columnCount(); ++col)
    {
        QTableWidgetItem *item = ui->tableWidget_9->item(6, col);

        // Если ячейка пустая, её нужно создать, иначе метод item() вернет 0
        if (!item) {
            item = new QTableWidgetItem();
            ui->tableWidget_9->setItem(6, col, item);
        }

        item->setBackground(QColor(0x00FF00));
    }
    ui->tableWidget_9->update();

    //======НАСТРОЙКА TABLEWIDGET_10===============

    ui->tableWidget_10->setRowCount(8);
    ui->tableWidget_10->setColumnCount(17);

    horHeader10 = new VertikalheaderView(Qt::Horizontal, ui->tableWidget_10);
    ui->tableWidget_10->setHorizontalHeader(horHeader10);

    QStringList horizontalTexts10 = {
                                     "Лекции",
                                     "Лабораторные работы",
                                     "Практические занятия",
                                     "МРС",
                                     "Индивидуальные занятия",
                                     "Курсовое проектирование",
                                     "Консультации",
                                     "Рецензирование",
                                     "Зачеты",
                                     "Экзамены",
                                     "Учебная практика",
                                     "Производственная практика",
                                     "Дипломное проектирование",
                                     "ГЭК/ГАК",
                                     "Аспирантура/Маг",
                                     "Итого часов по семестрам",
                                     "Итого часов за год"
    };

    ui->tableWidget_10->setHorizontalHeaderLabels(horizontalTexts10);

    QString n11 = " ";
    QString n21 = "Дисциплина";
    QString n31 = "Дисциплина";
    QString n41 = " ";
    QString n51 = "Дисциплина";
    QString n61 = "Дисциплина";

    QStringList vertikalTexts41 = {n11, n21, n31, n41, n51, n61,
        "Итого по плану",
        "Итого за семестр"
    };

    ui->tableWidget_10->setVerticalHeaderLabels(vertikalTexts41);
    ui->tableWidget_10->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int col = 0; col < ui->tableWidget_10->columnCount(); ++col)
    {
        QTableWidgetItem *item = ui->tableWidget_10->item(6, col);

        // Если ячейка пустая, её нужно создать, иначе метод item() вернет 0
        if (!item) {
            item = new QTableWidgetItem();
            ui->tableWidget_10->setItem(6, col, item);
        }

        item->setBackground(QColor(0x00FF00));
    }
    ui->tableWidget_10->update();

    ui->tableWidget_10->setSpan(0, 0, 1, 17);
    QTableWidgetItem *item10 = new QTableWidgetItem("Осенний семестр");
    item10->setTextAlignment(Qt::AlignCenter);
    item10->setData(Qt::UserRole, "header"); // Метка заголовка
    ui->tableWidget_10->setItem(0, 0, item10);
    item10->setBackground(QColor(0x00FF00));

    ui->tableWidget_10->setSpan(3, 0, 1, 17);
    QTableWidgetItem *item101 = new QTableWidgetItem("Весенний семестр");
    item101->setTextAlignment(Qt::AlignCenter);
    item101->setData(Qt::UserRole, "header"); // Метка заголовка
    ui->tableWidget_10->setItem(3, 0, item101);
    item101->setBackground(QColor(0x00FF00));

    QTableWidgetItem *vHeaderItem1 = new QTableWidgetItem(" ");
    vHeaderItem1->setBackground(QColor(0x00FF00));
    vHeaderItem1->setTextAlignment(Qt::AlignLeft);
    ui->tableWidget_10->setVerticalHeaderItem(0, vHeaderItem1);

    QTableWidgetItem *vHeaderItem31 = new QTableWidgetItem(" ");
    vHeaderItem31->setBackground(QColor(0x00FF00));
    vHeaderItem31->setTextAlignment(Qt::AlignLeft);
    ui->tableWidget_10->setVerticalHeaderItem(3, vHeaderItem31);

    QTableWidgetItem *vHeaderItem61 = new QTableWidgetItem("Итого по плану");
    vHeaderItem61->setBackground(QColor(0x00FF00));
    vHeaderItem61->setTextAlignment(Qt::AlignLeft);
    ui->tableWidget_10->setVerticalHeaderItem(6, vHeaderItem61);

    //======НАСТРОЙКА TABLEWIDGET_2===============

    ui->tableWidget_2->setRowCount(14);
    ui->tableWidget_2->setColumnCount(13);


    // Для ГОРИЗОНТАЛЬНОГО заголовка (столбцы)
    MergedHeader *horizontalHeader2 = new MergedHeader(Qt::Horizontal, ui->tableWidget_2);
    ui->tableWidget_2->setHorizontalHeader(horizontalHeader2);

    QList<QPair<int, int>> horizontalMerges2 =
    {
        {1, 2},   // столбцы 0-1
        {3, 2},   // столбцы 2-3
        {5, 2},   // столбцы 2-3
        {7, 2},   // столбцы 0-1
        {9, 2},   // столбцы 2-3
        {11, 2},   // столбцы 2-3
    };

    QStringList horizontalTexts2 = {
        "Понедельник",
        "Вторник",
        "Среда",
        "Четверг",
        "Пятница",
        "Суббота"
    };

    horizontalHeader2->setMergeRanges(horizontalMerges2);
    horizontalHeader2->setHeaderTexts(horizontalTexts2);

    //==========ИНИЦИАЛИЗАЦИЯ ВЕРТИКАЛЬНОГО ЗАГОЛОВКА==================
    MergedHeader *verticalHeader2 = new MergedHeader(Qt::Vertical, ui->tableWidget_2);
    ui->tableWidget_2->setVerticalHeader(verticalHeader2);
    verticalHeader2->setModel(ui->tableWidget_2->model());


    connect(ui->tableWidget_2->verticalScrollBar(), &QScrollBar::valueChanged,
            verticalHeader2, &QHeaderView::setOffset);
    ui->tableWidget_2->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    QList<QPair<int, int>> verticalMerges2 = {
        {0, 2},   // строки 0-1
        {2, 2},   // строки 2-3
        {4, 2},   // строки 4-5
        {6, 2},   // строки 6-7
        {8, 2},   // строки 4-5
        {10, 2},   // строки 6-7
        {12, 2},   // строки 6-7

    };

    QStringList verticalTexts2 = {
        "8.30 - 10.05",
        "10.25 - 12.00",
        "12.30 - 14.05",
        "14.20 - 15.55",
        "16.05 - 17.40",
        "17.50 - 19.20",
        "19.25 - 21.10"
    };

    verticalHeader2->setMergeRanges(verticalMerges2);
    verticalHeader2->setHeaderTexts(verticalTexts2);

    verticalHeader->verticalScrollBar()->setEnabled(false);
    ui->tableWidget_2->verticalHeader()->setFixedWidth(100);

    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->hideColumn(0);

    int totalRows = ui->tableWidget_2->rowCount();

    for (int i = totalRows - 4; i < totalRows; ++i)
    {
        ui->tableWidget_2->setRowHidden(i, true);
    }

    //QTimer::singleShot(100, this, &MainWindow::loadBase);

    // ТОЛЬКО ТЕПЕРЬ разрешаем работу логики
    //isLoaded = true;

    // Вызываем построение сетки вручную ОДИН раз при старте
    //rebuildCalendarGrid(current.year(), current.month());
    loadBase();
}

MainWindow::~MainWindow()
{
    tables.clear();
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}



void MainWindow::saveMonth(int year, int month)
{
    if (!db.isOpen()) {
        message_action("Ошибка", "База данных не открыта");
        return;
    }

    if (!db.transaction()) {
        message_action("Ошибка БД", "Не удалось начать транзакцию: " + db.lastError().text());
        return;
    }

    QSqlQuery query(db);
    bool allOk = true;

    for (int i = 0; i < 42; ++i) {
        QWidget* cell = ui->tableWidget_3->cellWidget(i / 7, i % 7);
        if (!cell) continue;

        QCheckBox* cb = cell->findChild<QCheckBox*>();
        int state = (cb && cb->isChecked()) ? 1 : 0;

        QLabel* l = cell->findChild<QLabel*>();
        if (!l || l->text().isEmpty()) continue;

        query.prepare("INSERT OR REPLACE INTO calendar_states (year, month, cell_index, state) VALUES (?, ?, ?, ?)");
        query.addBindValue(year);
        query.addBindValue(month);
        query.addBindValue(i);
        query.addBindValue(state);

        if (!query.exec()) {
            qDebug() << "Ошибка в ячейке" << i << ":" << query.lastError().text();
            allOk = false;
            break; // Если один запрос упал, прерываем цикл
        }
    }

    if (allOk && db.commit()) {
        message_action("Сохранение", QString("График за %1.%2 успешно сохранен").arg(month).arg(year));
    } else {
        QString errorDesc = db.lastError().text();
        if (errorDesc.isEmpty()) errorDesc = "Ошибка в одном из запросов цикла";

        message_action("Ошибка сохранения", errorDesc);
        db.rollback(); // Откатываем изменения, если была ошибка
    }
}

void MainWindow::loadMonth(int year, int month)
{
    QSqlQuery query;
    query.prepare("SELECT cell_index, state FROM calendar_states WHERE year = ? AND month = ?");
    query.addBindValue(year);
    query.addBindValue(month);

    // Получаем текущую системную дату для сравнения
    QDate today = QDate::currentDate();

    if (query.exec()) {
        int loadedCount = 0;
        while (query.next()) {
            int idx = query.value(0).toInt();
            int state = query.value(1).toInt();

            QWidget* cell = ui->tableWidget_3->cellWidget(idx / 7, idx % 7);
            if (!cell) continue;

            QLabel* l = cell->findChild<QLabel*>();

            if (l && !l->text().isEmpty()) {
                // ВЫЧИСЛЯЕМ: является ли эта ячейка сегодняшним днем
                int dayNum = l->text().toInt();
                bool isToday = (dayNum == today.day() &&
                                month == today.month() &&
                                year == today.year());

                // Передаем вычисленный флаг
                updateCellVisuals(idx, state, isToday);
                loadedCount++;
            }
        }
        if (loadedCount > 0) {
            message_action("Загрузка", QString("Применено %1 ручных правок").arg(loadedCount));
        }
    } else {
        message_action("Ошибка БД", query.lastError().text());
    }
}

void MainWindow::updateCellVisuals(int cellIdx, int state, bool isToday)
{
    int row = cellIdx / 7;
    int col = cellIdx % 7;
    QWidget* cell = ui->tableWidget_3->cellWidget(row, col);
    if (!cell) return;

    // 1. Ищем чекбокс и лейбл внутри ячейки
    QCheckBox* cb = cell->findChild<QCheckBox*>("checkBox");
    QLabel* l = cell->findChild<QLabel*>("dayLabel");

    // 2. Устанавливаем состояние чекбокса
    if (cb) {
        cb->blockSignals(true);
        cb->setChecked(state == 1);
        cb->show();
        cb->blockSignals(false);
    }

    // 3. Выбираем цвет фона
    QString color = (state == 1) ? "#00FF00" : "red";

    // 4. Логика рамки для текущего дня
    // Если сегодня: синяя рамка 3px, если нет: серая 1px
    QString borderStyle = isToday ? "3px solid blue" : "1px solid gray";

    // 5. Применяем стиль
    // cell->setStyleSheet(
    //     QString("QWidget { background-color: %1; border: %2; } ")
    //         .arg(color).arg(borderStyle) +
    //     "QLabel { color: black; background: transparent; border: none; font-size: 18px; font-weight: bold; } "
    //     "QCheckBox { background: transparent; border: none; }"
    //     );
    cell->setStyleSheet(
        QString("QWidget { background-color: %1; border: %2; } "
                "QLabel { color: black; background: transparent; border: none; font-size: 18px; font-weight: bold; } "
                "QCheckBox { background: transparent; border: none; }")
            .arg(color, borderStyle)
        );

    if (l) l->show();
}

// void MainWindow::clearTable()
// {
//     for (int i = 0; i < 42; ++i)
//         if (auto item = ui->tableWidget->item(i/7, i%7))
//             item->setCheckState(Qt::Unchecked);
// }

// void MainWindow::updateCheckbox(int index, int state)
// {
//     int row = index / 7;
//     int col = index % 7;
//     if (auto item = ui->tableWidget->item(row, col))
//         item->setCheckState(state == 1 ? Qt::Checked : Qt::Unchecked);
// }

// void MainWindow::clearTableCheckboxes()
// {
//     for (int row = 0; row < 6; ++row)
//     {
//         for (int col = 0; col < 7; ++col)
//         {
//             QTableWidgetItem *item = ui->tableWidget_3->item(row, col);
//             if (item)
//             {
//                 // Снимаем галочку
//                 item->setCheckState(Qt::Unchecked);
//             }
//         }
//     }
// }

// void MainWindow::on_dateChanged()
// {
//     // 1. Сохраняем тот месяц, который был открыт ДО клика
//     saveMonth(lastYear, lastMonth);

//     // 2. Получаем новые значения из интерфейса
//     int newMonth = monthList->currentIndex() + 1;
//     int newYear = yearSpinBox->value();

//     // 3. Загружаем данные для нового месяца
//     loadMonth(newYear, newMonth);

//     // 4. Запоминаем текущие значения как "старые" для следующего шага
//     lastYear = newYear;
//     lastMonth = newMonth;
// }

// void MainWindow::on_monthCombo_currentIndexChanged(int index)
// {
//     int targetMonth = index + 1; // Январь = 1
//     int targetYear = yearSpinBox->value();

//     QSqlQuery query;
//     query.prepare("SELECT day_index, state FROM calendar_states WHERE year = ? AND month = ?");
//     query.addBindValue(targetYear);
//     query.addBindValue(targetMonth);
//     query.exec();

//     // Сначала сбрасываем все чекбоксы в таблице в 0
//     clearTableCheckboxes();

//     while (query.next()) {
//         int dayIdx = query.value(0).toInt();
//         int state = query.value(1).toInt();

//         // Находим ячейку по индексу и ставим чекбокс
//         //updateCheckboxInTable(dayIdx, state);
//     }
// }

// void MainWindow::overwriteMonth(int year, int month) {
//     QSqlDatabase::database().transaction(); // Начинаем транзакцию для скорости

//     for (int i = 0; i < 42; ++i)
//     {
//         int row = i / 7;
//         int col = i % 7;

//         // Получаем состояние чекбокса из вашего cellWidget
//         QWidget* cell = ui->tableWidget_3->cellWidget(row, col);
//         if (!cell) continue;
//         QCheckBox* cb = cell->findChild<QCheckBox*>();
//         int state = (cb && cb->isChecked()) ? 1 : 0;

//         QSqlQuery query;
//         // Ключевая команда: INSERT OR REPLACE
//         query.prepare("INSERT OR REPLACE INTO calendar_states (year, month, day_index, state) "
//                       "VALUES (:year, :month, :idx, :state)");
//         query.bindValue(":year", year);
//         query.bindValue(":month", month);
//         query.bindValue(":idx", i);
//         query.bindValue(":state", state);

//         if (!query.exec()) {
//             qDebug() << "Ошибка при перезаписи:" << query.lastError().text();
//         }
//     }

//     if (QSqlDatabase::database().commit()) {
//         qDebug() << "Данные для месяца" << month << year << "успешно обновлены/перезаписаны.";
//     }
// }

// QString MainWindow::getDateForCell(int row, int col, int year, int month)
// {
//     QDate date(year, month, 1); // Start with the first day of the month

//     // Calculate the first weekday of the month
//     int firstDayOfWeek = date.dayOfWeek(); // 1 = Sunday ... 7 = Saturday

//     // Calculate day based on row and column
//     int day = row * 7 + col - (firstDayOfWeek == 7 ? 0 : firstDayOfWeek - 1);

//     // Check if day is valid
//     if (day > 0 && day <= date.daysInMonth())
//     {
//         date.setDate(year, month, day); // Set the date
//         return date.toString("yyyy-MM-dd"); // Return in desired format (e.g., "yyyy-MM-dd")
//     }
//     else
//     {
//         return QString(); // Return empty string if the date is invalid
//     }
// }

// void MainWindow::saveCheckboxState(int month, int year)
// {
//     // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//     // db.setDatabaseName("lesson_base.db");

//     // Используем существующее соединение или создаем новое правильно
//     QSqlDatabase db = QSqlDatabase::contains("qt_sql_default_connection")
//                           ? QSqlDatabase::database()
//                           : QSqlDatabase::addDatabase("QSQLITE");

//     // Формируем ПОЛНЫЙ путь к базе данных в папке проекта
//     QString dbPath = QString(PROJECT_PATH) + "/base/lesson_base.db";
//     db.setDatabaseName(dbPath);

//     if (!db.open()) {
//         QMessageBox::warning(this, "Ошибка", "Не удалось найти файл базы по пути: " + dbPath);
//         return;
//     }

//     int weekNumber = QDate(year, month, 1).weekNumber(); // Получаем номер недели
//     QStringList dayNames = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

//     bool hasError = false;

//     for (int rowIndex = 0; rowIndex < ui->tableWidget_3->rowCount(); ++rowIndex) {
//         for (int dayIndex = 0; dayIndex < 7; ++dayIndex) {
//             QWidget* cellWidget = ui->tableWidget_3->cellWidget(rowIndex, dayIndex);
//             if (cellWidget == nullptr) {
//                 qDebug() << "Ячейка пустая:" << rowIndex << dayIndex;
//                 continue; // Переходим к следующей ячейке
//             }

//             QCheckBox* checkbox = cellWidget->findChild<QCheckBox*>();
//             if (checkbox == nullptr) {
//                 qDebug() << "Чекбокс не найден в ячейке:" << rowIndex << dayIndex;
//                 continue;
//             }

//             int state = checkbox->isChecked() ? 1 : 0;
//             QString dayName = dayNames[dayIndex];
//             int id = rowIndex + 1; // Adjusting to use IDs from 1 to 6

//             // Проверка существования столбца в таблице
//             if (!isValidColumn(dayName))
//             {
//                 qDebug() << "Некорректное имя столбца:" << dayName;
//                 continue; // Пропускаем выполнение запроса, если столбец не корректен
//             }

//             // 1. Формируем строку ЗАРАНЕЕ.
//             // Используем %1 для столбца и ? для значений (это стандарт для SQLite)
//             QString queryText = QString("UPDATE Save_check SET %1 = ? WHERE id = ?").arg(dayName);

//             QSqlQuery query;

//             // 2. Сначала подготавливаем СФОРМИРОВАННУЮ строку
//             if (!query.prepare(queryText)) {
//                 qDebug() << "Ошибка подготовки запроса:" << query.lastError().text();
//                 continue;
//             }

//             // 3. Привязываем значения ПО ПОРЯДКУ появления знаков вопроса
//             query.addBindValue(state); // заменит первый '?'
//             query.addBindValue(id);    // заменит второй '?'

//             // Log the constructed SQL query for debugging
//             qDebug() << "Executing SQL:" << queryText
//                      << ", State:" << state
//                      << ", ID:" << id;

//             // Execute the query
//             if (!query.exec()) {
//                 qDebug() << "Ошибка выполнения запроса для" << dayName
//                          << "State:" << state << "ID:" << id
//                          << "Error:" << query.lastError().text();
//             } else {
//                 qDebug() << "Успешно обновлено:" << dayName
//                          << "State:" << state << "ID:" << id;
//             }
//         }
//     }

//     db.close();

//     if (hasError) {
//         QMessageBox::warning(this, "Ошибка", "Произошла ошибка при обновлении данных в базе.");
//     }
// }

bool MainWindow::isValidColumn(const QString &columnName) const
{
    QStringList validColumns = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
    return validColumns.contains(columnName);
}

void MainWindow::adjustLabelFont()
{
    // Get the primary screen
    QScreen *screen = QGuiApplication::primaryScreen();

    // Get screen dimensions
    QRect screenGeometry = screen->geometry();
    int width = screenGeometry.width();
    int height = screenGeometry.height();

    // Calculate the diagonal in pixels
    double diagonal = sqrt(pow(width, 2) + pow(height, 2));

    // Adjust the scaling constant
    double scalingFactor = 100.0;  // Increase this value to reduce the font size

    // Calculate an appropriate font size based on diagonal
    int fontSize = static_cast<int>(diagonal / scalingFactor);

    // Set minimum and maximum font size limits
    if (fontSize < 10) fontSize = 10;  // Minimum font size
    if (fontSize > 100) fontSize = 100; // Maximum font size

    // Set the font size for both labels
    QFont font = label->font(); // Assuming you have label1 and label2
    font.setPointSize(fontSize);
    label->setFont(font);
    label2->setFont(font);
}

void MainWindow::onTabChanged(int index)
{
    // Get the title of the currently active tab
    QString tabText = ui->tabWidget->tabText(index);

    // Update the QLabel with the current tab's name
    label2->setText(tabText);
}

// void MainWindow::updateTable(int month, int year)
// {
//     QDate currentDate = QDate::currentDate();
//     month = currentDate.month();
//     year = currentDate.year();

//     tv = ui->tableWidget_3;
//     tv->setRowCount(6);
//     tv->setColumnCount(7);

//     QStringList headers = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота", "Воскресенье"};
//     tv->setHorizontalHeaderLabels(headers);

//     // Display the current month and year
//     ui->currentMonthLabel->setText("0" + QString::number(month) + " " + QString::number(year));

//     tv->verticalHeader()->hide();
//     setColumnAndRowSizes(tv);
//     //onMonthChanged(monthList->currentIndex());
// }

// void MainWindow::onMonthChanged(int index)
// {
//     tv = ui->tableWidget_3; // Получаем указатель на QTableWidget из .ui

//     // Очищаем таблицу
//     tv->clearContents();

//     // Получаем количество дней в месяце
//     int year = 2026;  // Убедитесь, что используете правильный год
//     int daysInMonth;
//     switch (index) {
//     case 0: daysInMonth = 31; break; // Январь
//     case 1: daysInMonth = QDate::isLeapYear(year) ? 29 : 28; break; // Февраль
//     case 2: daysInMonth = 31; break; // Март
//     case 3: daysInMonth = 30; break; // Апрель
//     case 4: daysInMonth = 31; break; // Май
//     case 5: daysInMonth = 30; break; // Июнь
//     case 6: daysInMonth = 31; break; // Июль
//     case 7: daysInMonth = 31; break; // Август
//     case 8: daysInMonth = 30; break; // Сентябрь
//     case 9: daysInMonth = 31; break; // Октябрь
//     case 10: daysInMonth = 30; break; // Ноябрь
//     case 11: daysInMonth = 31; break; // Декабрь
//     }

//     // Получение первого дня месяца
//     QDate firstDayOfMonth(year, index + 1, 1);
//     int firstDayOfWeek = firstDayOfMonth.dayOfWeek(); // 1 = Пн, ..., 7 = Вс

//     // Обработка смещения для корректного отображения
//     int startColumn = (firstDayOfWeek == 7) ? 6 : firstDayOfWeek - 1; // Воскресенье = 6

//     // Заполнение таблицы
//     int currentRow = 0;
//     for (int day = 1; day <= daysInMonth; ++day)
//     {
//         // // Create a horizontal layout for checkbox and label
//         // QWidget *widget = new QWidget();
//         // QHBoxLayout *layout = new QHBoxLayout(widget);

//         // // Creating the checkbox
//         // QCheckBox *checkBox = new QCheckBox();

//         // // Determine if it is a weekend
//         // bool isWeekend = (firstDayOfWeek == 7 || firstDayOfWeek == 6);
//         // checkBox->setChecked(!isWeekend); // Check for weekdays, uncheck for weekends

//         // // Set initial color based on the day type
//         // widget->setStyleSheet(isWeekend ? "background-color: red;" : "background-color: #00FF00;");

//         // // Connect checkbox state change to a slot
//         // connect(checkBox, &QCheckBox::checkStateChanged, this, [=](int state) {
//         //     if (state == Qt::Checked) {
//         //         widget->setStyleSheet("background-color: #00FF00;"); // Light green for checked state
//         //     } else {
//         //         widget->setStyleSheet("background-color: red;");
//         //     }
//         // });

//         // layout->addWidget(checkBox);

//         // // Creating the label
//         // QLabel *label = new QLabel(QString::number(day));
//         // layout->addWidget(label);

//         // // Увеличение размера шрифта
//         // QFont font = label->font();
//         // font.setPointSize(16); // Укажите нужный размер
//         // label->setFont(font);

//         // // Set layout spacing to 0
//         // layout->setContentsMargins(10, 0, 0, 0);

//         // // Set this layout to the widget
//         // widget->setLayout(layout);

//         // // Set the widget in the table cell
//         // tv->setCellWidget(currentRow, startColumn, widget);

//         QWidget *widget = new QWidget();
//         QHBoxLayout *layout = new QHBoxLayout(widget);
//         QCheckBox *checkBox = new QCheckBox();
//         bool isWeekend = (firstDayOfWeek == 7 || firstDayOfWeek == 6);
//         checkBox->setChecked(!isWeekend);

//         // ⭐ СТИЛИЗУЙТЕ ВЕСЬ КОНТЕЙНЕР И ВСЕ ДОЧЕРНИЕ ВИДЖЕТЫ
//         QString checkedStyle =
//             "QWidget { background-color: #00FF00; } "
//             "QCheckBox { background-color: #00FF00; } "
//             "QLabel { background-color: #00FF00; }";

//         QString uncheckedStyle =
//             "QWidget { background-color: red; } "
//             "QCheckBox { background-color: red; } "
//             "QLabel { background-color: red; }";

//         widget->setStyleSheet(isWeekend ? uncheckedStyle : checkedStyle);

//         connect(checkBox, &QCheckBox::checkStateChanged, this, [=](int state) {
//             if (state == Qt::Checked) {
//                 widget->setStyleSheet(checkedStyle);
//             } else {
//                 widget->setStyleSheet(uncheckedStyle);
//             }
//         });

//         // layout->addWidget(checkBox);
//         // QLabel *label = new QLabel(QString::number(day));
//         // layout->addWidget(label);
//         // QFont font = label->font();
//         // font.setPointSize(16);
//         // label->setFont(font);
//         // layout->setContentsMargins(10, 0, 0, 0);
//         // widget->setLayout(layout);
//         // tv->setCellWidget(currentRow, startColumn, widget);

//         startColumn++;
//         if (startColumn >= 7)
//         { // Переход на следующую строку
//             startColumn = 0; // Сброс столбца
//             currentRow++;
//         }

//         // Update firstDayOfWeek for the next day
//         firstDayOfWeek++;
//         if (firstDayOfWeek > 7) firstDayOfWeek = 1; // Reset to Monday
//     }
//         // tv->verticalHeader()->hide();

//         // setColumnAndRowSizes(tv);

//         // onMonthChanged(monthList->currentIndex());
// }

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    // Обновляем размеры столбцов и строк при изменении размера окна
    setColumnAndRowSizes(ui->tableWidget_3);
    adjustLabelFont();
}

void MainWindow::setColumnAndRowSizes(QTableWidget *tv)
{
    // Используем переданный указатель, если он валиден, иначе берем из ui
    if (!tv) {
        tv = ui->tableWidget_3;
    }

    if (!tv) {
        qDebug() << "Invalid QTableWidget pointer.";
        return;
    }

    // Устанавливаем минимальные размеры
    tv->setMinimumSize(200, 100);

    // 1. Растягиваем СТОЛБЦЫ на всю ширину
    // Stretch автоматически распределяет свободное пространство между всеми колонками
    tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 2. Растягиваем СТРОКИ на всю высоту
    // Это именно то, что заменяет ваш цикл setRowHeight и решает проблему с высотой
    tv->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Опционально: отключаем полосы прокрутки, если хотим, чтобы таблица была строго по размеру окна
    tv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    qDebug() << "Table layout updated to Stretch mode.";
}

void MainWindow::formatTableItems(QTableWidget *tableWidget)
{
    // Учитываем высоту заголовка и внутренние отступы
    int availableHeight = tableWidget->height() - tableWidget->horizontalHeader()->height() - tableWidget->frameWidth();
    int availableWidth = tableWidget->width() - tableWidget->verticalHeader()->width() - tableWidget->frameWidth();

    // Проверяем, что доступное пространство не отрицательное
    if (availableHeight > 0 && availableWidth > 0)
    {
        // Устанавливаем пропорциональную ширину столбцов
        int columnWidth = availableWidth / tableWidget->columnCount();
        for (int col = 0; col < tableWidget->columnCount(); ++col)
        {
            tableWidget->setColumnWidth(col, columnWidth);
        }

        // Устанавливаем пропорциональную высоту строк
        int rowHeight = availableHeight / tableWidget->rowCount();
        for (int row = 0; row < tableWidget->rowCount(); ++row)
        {
            tableWidget->setRowHeight(row, rowHeight);
        }
    }
}

void MainWindow::refreshTableDisplay()
{
    // Отключаем обновление UI для производительности
    ui->tableWidget->setUpdatesEnabled(false);

    // Перезагружаем данные
    //loadTableData(currentTableName);

    // Включаем обновление UI
    ui->tableWidget->setUpdatesEnabled(true);
    ui->tableWidget->viewport()->update();
}

void MainWindow::splitCells()
{
    // QTableWidget *table = ui->tableWidget;
    // QList<QTableWidgetSelectionRange> ranges = table->selectedRanges();

    // if (ranges.isEmpty()) {
    //     //QMessageBox::warning(this, "Ошибка", "Выделите смежные ячейки");
    //     message_action("Ошибка", "Выделите смежные ячейки");
    //     return;
    // }

    // QTableWidgetSelectionRange range = ranges.first();
    // int rCount = range.rowCount();
    // int cCount = range.columnCount();
    // int top = range.topRow();
    // int left = range.leftColumn();

    // // 1. Проверка допустимых размеров (1x2, 2x1 или 2x2)
    // bool isHorizontal = (rCount == 1 && cCount == 2);
    // bool isVertical = (rCount == 2 && cCount == 1);
    // bool isSquare = (rCount == 2 && cCount == 2);

    // if (!isHorizontal && !isVertical && !isSquare) {
    //     //QMessageBox::warning(this, "Ошибка", "Выделите ровно 2 смежные ячейки или блок 2x2");
    //     message_action("Ошибка", "Выделите ровно 2 смежные ячейки или блок 2x2");
    //     return;
    // }

    // // 2. Проверка, не поглощены ли уже ячейки другими объединениями
    // for (int r = top; r < top + rCount; ++r) {
    //     for (int c = left; c < left + cCount; ++c) {
    //         if (table->rowSpan(r, c) > 1 || table->columnSpan(r, c) > 1) {
    //             //QMessageBox::warning(this, "Ошибка", "В области есть уже объединенные ячейки");
    //             message_action("Ошибка", "В области есть уже объединенные ячейки");
    //             return;
    //         }
    //     }
    // }

    // // 3. Сбор текста изо всех ячеек выделения
    // QStringList texts;
    // for (int r = top; r < top + rCount; ++r) {
    //     for (int c = left; c < left + cCount; ++c) {
    //         if (QTableWidgetItem *it = table->item(r, c)) {
    //             if (!it->text().trimmed().isEmpty()) {
    //                 texts << it->text().trimmed();
    //             }
    //         }
    //     }
    // }

    // // 4. Выполнение объединения
    // table->setSpan(top, left, rCount, cCount);

    // // 5. Установка объединенного текста в главную ячейку
    // if (QTableWidgetItem *mainItem = table->item(top, left)) {
    //     // Используем \n для вертикальных/квадратных и пробел для горизонтальных
    //     QString separator = (isHorizontal) ? " " : "\n";
    //     mainItem->setText(texts.join(separator));
    //     mainItem->setTextAlignment(Qt::AlignCenter);
    // }

    // // Уведомление
    // QString desc = isSquare ? "блок 2x2" : (isHorizontal ? "горизонтально" : "вертикально");
    // message_action("Успех", QString("Ячейки объединены (%1)").arg(desc));

    QTableWidget *table = ui->tableWidget;
    QList<QTableWidgetSelectionRange> ranges = table->selectedRanges();

    if (ranges.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выделите область для разъединения");
        return;
    }

    int splitCount = 0;
    table->blockSignals(true); // Блокируем сигналы для скорости

    for (const QTableWidgetSelectionRange &range : std::as_const(ranges))
    {
        for (int r = range.topRow(); r <= range.bottomRow(); ++r) {
            for (int c = range.leftColumn(); c <= range.rightColumn(); ++c)
            {

                int rSpan = table->rowSpan(r, c);
                int cSpan = table->columnSpan(r, c);

                // Если ячейка является началом объединения (Span > 1)
                if (rSpan > 1 || cSpan > 1) {
                    table->setSpan(r, c, 1, 1);
                    splitCount++;
                }
            }
        }
    }

    table->blockSignals(false);
    table->viewport()->update();

    if (splitCount > 0) {
        message_action("Успех", QString("Разъединено ячеек: %1").arg(splitCount));
    } else {
        QMessageBox::warning(this, "Информация", "В выделенной области нет объединенных ячеек");
    }
}

void MainWindow::mergeCells()
{
    // QTableWidget *table = ui->tableWidget;
    // QList<QTableWidgetSelectionRange> ranges = table->selectedRanges();

    // if (ranges.isEmpty()) {
    //     //QMessageBox::warning(this, "Ошибка", "Выделите две смежные ячейки");
    //     message_action("Ошибка", "Выделите две смежные ячейки");

    //     return;
    // }

    // QTableWidgetSelectionRange range = ranges.first();
    // int topRow = range.topRow();
    // int bottomRow = range.bottomRow();
    // int leftCol = range.leftColumn();
    // int rightCol = range.rightColumn();

    // // Определяем направление объединения
    // bool isHorizontal = (topRow == bottomRow) && (rightCol - leftCol + 1 == 2);
    // bool isVertical = (leftCol == rightCol) && (bottomRow - topRow + 1 == 2);

    // if (!isHorizontal && !isVertical) {
    //     // QMessageBox::warning(this, "Ошибка",
    //     //                      "Выделите ровно две смежные ячейки (горизонтально или вертикально)");
    //     message_action("Ошибка", "Выделите ровно две смежные ячейки (горизонтально или вертикально)");
    //     return;
    // }

    // // Проверяем, что ячейки не уже объединены
    // if (isHorizontal) {
    //     if (table->columnSpan(topRow, leftCol) > 1 ||
    //         table->columnSpan(topRow, rightCol) > 1) {
    //         QMessageBox::warning(this, "Ошибка", "Одна из ячеек уже объединена");
    //         message_action("Ошибка", "Одна из ячеек уже объединена");
    //         return;
    //     }
    // } else {
    //     if (table->rowSpan(topRow, leftCol) > 1 ||
    //         table->rowSpan(bottomRow, leftCol) > 1) {
    //         //QMessageBox::warning(this, "Ошибка", "Одна из ячеек уже объединена");
    //         message_action("Ошибка", "Одна из ячеек уже объединена");

    //         return;
    //     }
    // }

    // // Получаем содержимое ячеек
    // QTableWidgetItem *item1, *item2;
    // QString separator;

    // if (isHorizontal) {
    //     item1 = table->item(topRow, leftCol);
    //     item2 = table->item(topRow, rightCol);
    //     separator = " ";
    //     table->setSpan(topRow, leftCol, 1, 2);
    // } else {
    //     item1 = table->item(topRow, leftCol);
    //     item2 = table->item(bottomRow, leftCol);
    //     separator = "\n";
    //     table->setSpan(topRow, leftCol, 2, 1);
    // }

    // // Объединяем текст
    // if (item1 && item2) {
    //     QString combinedText = item1->text() + separator + item2->text();
    //     item1->setText(combinedText);
    // }

    // // Логирование и уведомление
    // QString direction = isHorizontal ? "горизонтально" : "вертикально";
    // qDebug() << "Ячейки объединены" << direction;

    // // QMessageBox::information(this, "Успех",
    // //                          QString("Ячейки успешно объединены %1").arg(direction));
    // message_action("Успех", QString("Ячейки объединены (%1)").arg(direction));

    QTableWidget *table = ui->tableWidget;
    QList<QTableWidgetSelectionRange> ranges = table->selectedRanges();

    if (ranges.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выделите смежные ячейки");
        return;
    }

    QTableWidgetSelectionRange range = ranges.first();
    int rCount = range.rowCount();
    int cCount = range.columnCount();
    int top = range.topRow();
    int left = range.leftColumn();

    // 1. Проверка допустимых размеров (1x2, 2x1 или 2x2)
    bool isHorizontal = (rCount == 1 && cCount == 2);
    bool isVertical = (rCount == 2 && cCount == 1);
    bool isSquare = (rCount == 2 && cCount == 2);

    if (!isHorizontal && !isVertical && !isSquare) {
        QMessageBox::warning(this, "Ошибка",
                             "Выделите ровно 2 смежные ячейки или блок 2x2");
        return;
    }

    // 2. Проверка, не поглощены ли уже ячейки другими объединениями
    for (int r = top; r < top + rCount; ++r) {
        for (int c = left; c < left + cCount; ++c) {
            if (table->rowSpan(r, c) > 1 || table->columnSpan(r, c) > 1) {
                QMessageBox::warning(this, "Ошибка", "В области есть уже объединенные ячейки");
                return;
            }
        }
    }

    // 3. Сбор текста изо всех ячеек выделения
    QStringList texts;
    for (int r = top; r < top + rCount; ++r) {
        for (int c = left; c < left + cCount; ++c) {
            if (QTableWidgetItem *it = table->item(r, c)) {
                if (!it->text().trimmed().isEmpty()) {
                    texts << it->text().trimmed();
                }
            }
        }
    }

    // 4. Выполнение объединения
    table->setSpan(top, left, rCount, cCount);

    // 5. Установка объединенного текста в главную ячейку
    if (QTableWidgetItem *mainItem = table->item(top, left)) {
        // Используем \n для вертикальных/квадратных и пробел для горизонтальных
        QString separator = (isHorizontal) ? " " : "\n";
        mainItem->setText(texts.join(separator));
        mainItem->setTextAlignment(Qt::AlignCenter);
    }

    // Уведомление
    QString desc = isSquare ? "блок 2x2" : (isHorizontal ? "горизонтально" : "вертикально");
    message_action("Успех", QString("Ячейки объединены (%1)").arg(desc));
}

void MainWindow::ClickedLeftButton(int row, int column)
{
    if (editingEnabled)
        return;

    QScreen *screen = QGuiApplication::primaryScreen();
    rsc2 = new Add_lesson(this);
    rsc2->setWindowTitle("Введите данные для ячейки");
    rsc2->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            rsc2->size(),
            screen->geometry()));

    QPair<int,int> key(row, column);

    // Если уже объединено — разъединяем
    if (mergedLefts_.contains(key))
    {
        ui->tableWidget->setSpan(row, column, 1, 1);
        if (column + 1 < ui->tableWidget->columnCount() && !ui->tableWidget->item(row, column+1))
            ui->tableWidget->setItem(row, column+1, new QTableWidgetItem(""));
        mergedLefts_.remove(key);
        rsc2->deleteLater();
        return;
    }

    // Выполняем диалог
    if (rsc2->exec() != QDialog::Accepted)
    {
        rsc2->deleteLater();
        return;
    }

    // Читаем данные
    QString comboChoice2 = rsc2->ui->comboBox_2->currentText().trimmed();
    QString comboChoice3 = rsc2->ui->comboBox_3->currentText().trimmed();
    QString combinedText = rsc2->text11 + "\n" + rsc2->text22 + "\n" + rsc2->text33 + "\n" + rsc2->text44;

    qDebug() << "\n=== ОТЛАДКА ===";
    qDebug() << "row=" << row << "column=" << column;
    qDebug() << "comboBox_2='" << comboChoice2 << "'";
    qDebug() << "comboBox_3='" << comboChoice3 << "'";

    // ПРАВИЛЬНАЯ логика чётности: нечётные индексы (1, 3, 5...)
    bool isRowOdd = (row % 2 == 1);    // row 1, 3, 5...
    bool isColOdd = (column % 2 == 1); // column 1, 3, 5...
    qDebug() << "isRowOdd=" << isRowOdd << "isColOdd=" << isColOdd;

    int targetRow = row;
    bool shouldMergeHorizontal = false;
    bool shouldMergeVertical = false;

    // СЦЕНАРИЙ 1: none + (Верхняя или Нижняя неделя)
    if (comboChoice2 == "none" &&
        (comboChoice3.contains("Верхняя") || comboChoice3.contains("Нижняя")))
    {
        qDebug() << "СЦЕНАРИЙ 1: none + неделя";
        shouldMergeHorizontal = true;

        if (comboChoice3.contains("Верхняя"))
        {
            targetRow = row;
            qDebug() << "Верхняя неделя — остаёмся в строке" << targetRow;
        }
        else if (comboChoice3.contains("Нижняя"))
        {
            if (isRowOdd)
            {
                // Нечётная строка (1, 3, 5...) → переходим в чётную (2, 4, 6...)
                if (row + 1 < ui->tableWidget->rowCount())
                {
                    targetRow = row + 1;
                    qDebug() << "Нечётная строка, Нижняя неделя — вниз на" << targetRow;
                }
                else
                {
                    targetRow = row - 1;
                    qDebug() << "Последняя строка, Нижняя неделя — вверх на" << targetRow;
                }
            }
            else
            {
                // Чётная строка (0, 2, 4...) → переходим в нечётную (1, 3, 5...)
                if (row - 1 >= 0)
                {
                    targetRow = row - 1;
                    qDebug() << "Чётная строка, Нижняя неделя — вверх на" << targetRow;
                }
                else
                {
                    targetRow = row + 1;
                    qDebug() << "Первая строка, Нижняя неделя — вниз на" << targetRow;
                }
            }
        }
    }
    // СЦЕНАРИЙ 2: 1-я подгруппа раз в 2 недели + Верхняя/Нижняя неделя
    else if (comboChoice2.contains("1-я подгруппа раз в 2 недели"))
    {
        qDebug() << "СЦЕНАРИЙ 2: 1-я подгруппа раз в 2 недели";

        // Должна быть чётная строка (индекс 0, 2, 4...) И нечётный столбец (индекс 1, 3, 5...)
        if (!isRowOdd && isColOdd)
        {
            if (comboChoice3.contains("Верхняя"))
            {
                targetRow = row;
                qDebug() << "Верхняя неделя, чётная строка и нечётный столбец — остаёмся в" << targetRow;
                // shouldMergeHorizontal остаётся false
            }
            else if (comboChoice3.contains("Нижняя"))
            {
                if (row + 1 < ui->tableWidget->rowCount())
                {
                    targetRow = row + 1;
                    qDebug() << "Нижняя неделя, чётная строка и нечётный столбец — переходим в нижнюю ячейку на строку" << targetRow;
                    // shouldMergeHorizontal остаётся false
                }
                else
                {
                    qDebug() << "Ошибка: нет нижней строки для перехода";
                    rsc2->deleteLater();
                    return;
                }
            }
            else
            {
                qDebug() << "Ошибка: не выбрана Верхняя или Нижняя неделя";
                rsc2->deleteLater();
                return;
            }
        }
        else if (!isColOdd)
        {
            // Чётный столбец — ничего не делаем
            qDebug() << "Ошибка: ячейка в чётном столбце, сценарий 2 невозможен";
            rsc2->deleteLater();
            return;
        }
        else
        {
            // Нечётная строка (индекс 1, 3, 5...) — ошибка
            qDebug() << "Ошибка: ячейка должна быть в чётной строке (визуально нечётной)";
            rsc2->deleteLater();
            return;
        }
    }

    // СЦЕНАРИЙ 3: 2-я подгруппа раз в 2 недели + Верхняя/Нижняя неделя
    else if (comboChoice2.contains("2-я подгруппа раз в две недели"))  // Условие на выбор
    {
        qDebug() << "СЦЕНАРИЙ 3: 2-я подгруппа раз в две недели";

        // Объявляем переменные для четности
        bool isRowOdd = (row % 2 != 0);    // НЕЧЕТНАЯ строка
        bool isColOdd = (column % 2 != 0); // НЕЧЕТНЫЙ столбец

        // Требуются: четная строка и четный столбец (для 2-й подгруппы)
        // Значит: !isRowOdd && !isColOdd
        if (!isRowOdd && !isColOdd) {
            if (comboChoice3.contains("Верхняя")) {
                targetRow = row;
                qDebug() << "Верхняя неделя, четная строка и четный столбец — остаемся в " << targetRow;
                // Дальнейшее действие, например, оставить текущую ячейку
            }
            else if (comboChoice3.contains("Нижняя")) {
                if (row + 1 < ui->tableWidget->rowCount()) {
                    targetRow = row + 1;
                    qDebug() << "Нижняя неделя, четная строка и четный столбец — переходим в" << targetRow;
                    // Действия перехода
                } else {
                    qDebug() << "Ошибка: нет следующей строки для перехода вниз.";
                    rsc2->deleteLater();
                    return;
                }
            } else {
                qDebug() << "Ошибка: не выбрана Верхняя или Нижняя неделя.";
                rsc2->deleteLater();
                return;
            }
        }
        else if (isColOdd || isRowOdd) {
            // Если столбец или строка не соответствуют нужной
            qDebug() << "Ошибка: ячейка не соответствует условию для 2-й подгруппы.";
            rsc2->deleteLater();
            return;
        }
    }
    // СЦЕНАРИЙ 4: 1-я подгруппа раз в неделю + none
    else if (comboChoice2.contains("1-я подгруппа раз в неделю") && comboChoice3.contains("none")) {
        bool isRowEven = (row % 2 == 0);
        bool isColOdd = (column % 2 != 0);  // нечетный столбец
        bool isRowOdd = (row % 2 != 0);     // нечетная строка
        //bool isColEven = (column % 2 == 0);

        qDebug() << "СЦЕНАРИЙ 4: 1-я подгруппа раз в неделю + none";

        // Для четной строки + нечетного столбца — объединяем снизу
        if (isRowEven && isColOdd) {
            if (row + 1 < ui->tableWidget->rowCount()) {
                targetRow = row;
                shouldMergeVertical = true; // объединять снизу
                qDebug() << "Ячейка в нечетном столбце и четной строке — объединение снизу.";
            } else {
                qDebug() << "Нет нижней строки для объединения.";
                rsc2->deleteLater();
                return;
            }
        }
        // Для нечетной строки + нечетного столбца — объединяем сверху
        else if (isRowOdd && isColOdd) {
            if (row - 1 >= 0) {
                targetRow = row - 1;
                shouldMergeVertical = true; // объединять сверху
                qDebug() << "Ячейка в нечетном столбце и нечетной строке — объединение сверху.";
            } else {
                qDebug() << "Нет верхней строки для объединения.";
                rsc2->deleteLater();
                return;
            }
        }
    }
    // СЦЕНАРИЙ 4: 2-я подгруппа раз в неделю + none
    else if (comboChoice2.contains("2-я подгруппа раз в неделю") && comboChoice3.contains("none")) {
        bool isRowEven = (row % 2 == 0); // четная строка
        bool isRowOdd = (row % 2 != 0);  // нечетная строка
        bool isColEven = (column % 2 == 0); // четный столбец

        qDebug() << "СЦЕНАРИЙ: 2-я подгруппа раз в неделю + none";

        // Четный столбец + четная строка — объединение вниз
        if (isColEven && isRowEven) {
            if (row + 1 < ui->tableWidget->rowCount()) {
                targetRow = row;
                shouldMergeVertical = true; // объединить со следующей
                qDebug() << "Четная строка + четный столбец — объединение снизу.";
            } else {
                qDebug() << "Нет нижней строки для объединения.";
                rsc2->deleteLater();
                return;
            }
        }
        // Четный столбец + нечетная строка — объединение сверху
        else if (isColEven && isRowOdd) {
            if (row - 1 >= 0) {
                targetRow = row - 1;
                shouldMergeVertical = true; // объединить сверху
                qDebug() << "Нечетная строка + четный столбец — объединение сверху.";
            } else {
                qDebug() << "Нет верхней строки для объединения.";
                rsc2->deleteLater();
                return;
            }
        }
    }
    else
    {
        qDebug() << "Не совпадает ни с одним сценарием";
        qDebug() << "Записываем текст в текущую ячейку";
    }

    qDebug() << "targetRow=" << targetRow << "shouldMergeHorizontal=" << shouldMergeHorizontal
             << "shouldMergeVertical=" << shouldMergeVertical;

    // Проверка валидности
    if (targetRow < 0 || targetRow >= ui->tableWidget->rowCount())
    {
        qDebug() << "ОШИБКА: targetRow вне диапазона";
        rsc2->deleteLater();
        return;
    }

    // Записываем текст
    if (!ui->tableWidget->item(targetRow, column))
        ui->tableWidget->setItem(targetRow, column, new QTableWidgetItem());

    ui->tableWidget->item(targetRow, column)->setText(combinedText);
    ui->tableWidget->item(targetRow, column)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    qDebug() << "Текст записан в ячейку [" << targetRow << "," << column << "]";

    // Горизонтальное объединение
    if (shouldMergeHorizontal && column + 1 < ui->tableWidget->columnCount())
    {
        if (!ui->tableWidget->item(targetRow, column + 1))
            ui->tableWidget->setItem(targetRow, column + 1, new QTableWidgetItem());

        ui->tableWidget->setSpan(targetRow, column, 1, 2);
        mergedLefts_.insert(QPair<int,int>(targetRow, column));
        ui->tableWidget->item(targetRow, column + 1)->setText("");

        qDebug() << "Горизонтальное объединение выполнено";
    }

    // Вертикальное объединение
    if (shouldMergeVertical && row + 1 < ui->tableWidget->rowCount())
    {
        if (!ui->tableWidget->item(row + 1, column))
            ui->tableWidget->setItem(row + 1, column, new QTableWidgetItem());

        ui->tableWidget->setSpan(targetRow, column, 2, 1);
        mergedLefts_.insert(QPair<int,int>(targetRow, column));
        ui->tableWidget->item(row + 1, column)->setText("");

        qDebug() << "Вертикальное объединение выполнено";
    }

    ui->tableWidget->resizeRowsToContents();
    rsc2->deleteLater();

    // 1. Принудительно завершаем редактирование через делегат (публичный способ)
    if (ui->tableWidget->isPersistentEditorOpen(ui->tableWidget->currentItem())) {
        ui->tableWidget->closePersistentEditor(ui->tableWidget->currentItem());
    }

    // // 2. Снимаем выделение (синяя заливка)
    // ui->tableWidget->clearSelection();

    // // 3. Убираем пунктирную рамку фокуса с ячейки
    // ui->tableWidget->setCurrentItem(nullptr);

    // // 4. САМОЕ ВАЖНОЕ: Переводим фокус с таблицы на само окно.
    // // Это закроет активный текстовый редактор QLineEdit в ячейке.
    // this->setFocus();

    // // 5. На всякий случай сбрасываем состояние фокуса таблицы
    // ui->tableWidget->clearFocus();

    QTimer::singleShot(0, this, [this]()
    {
        ui->tableWidget->clearSelection();
        ui->tableWidget->setCurrentItem(nullptr);
        ui->tableWidget->clearFocus();
        this->setFocus();
    });
}

// Реализация функции определения целевой ячейки
QPair<int,int> MainWindow::determineTargetCell(int row, int column,
                        const QString &combo2, const QString &combo3, int rowCount)
{
    int targetRow = row;
    int targetCol = column;
    bool isOddRow = (row % 2 == 1);
    //bool isOddColumn = (column % 2 == 1);

    // Обработка для "none" и "Верхняя/Нижняя"
    if (combo2.trimmed().compare("none", Qt::CaseInsensitive) == 0 &&
        (combo3.contains("Верхняя", Qt::CaseInsensitive) || combo3.contains("Нижняя", Qt::CaseInsensitive)))
    {
        if (combo3.contains("Верхняя", Qt::CaseInsensitive))
        {
            // Остаемся в текущей строке
            targetRow = row;
        }
        else if (combo3.contains("Нижняя", Qt::CaseInsensitive))
        {
            if (isOddRow)
            {
                // Вниз или вверх
                targetRow = (row + 1 < rowCount) ? row + 1 : row - 1;
            }
            else
            {
                targetRow = (row - 1 >= 0) ? row - 1 : row + 1;
            }
        }
    }
    // Обработка для других условий подгрупп
    else if (combo2.contains("1-я подгруппа раз в 2 недели", Qt::CaseInsensitive) &&
             combo3.contains("Верхняя", Qt::CaseInsensitive))
    {
        // Например, остаемся или переходим в соответствии с логикой
        // Можно добавить логику, аналогичную вам
        targetRow = row;
    }
    else if (combo2.contains("2-я подгруппа раз в 2 недели", Qt::CaseInsensitive) &&
             combo3.contains("Нижняя", Qt::CaseInsensitive))
    {
        // Переход
        if (isOddRow)
        {
            targetRow = (row + 1 < rowCount) ? row + 1 : row - 1;
        }
        else
        {
            targetRow = (row - 1 >= 0) ? row - 1 : row + 1;
        }
    }
    else if (combo2.contains("2-я подгруппа раз в неделю", Qt::CaseInsensitive) &&
             combo3.trimmed().compare("none", Qt::CaseInsensitive) == 0)
    {
        // Ваши условия для этой группы
        targetRow = row;
    }

    // Вернуть вычисленную целевую ячейку
    return QPair<int, int>(targetRow, targetCol);
}

void MainWindow::deleteLesson()
{
    // int row = ui->tableWidget->currentRow();
    // int column = ui->tableWidget->currentColumn();
    // if (row < 0 || column < 0) return;
    // if (auto *it = ui->tableWidget->item(row, column))
    // {
    //     it->setText(QString());
    // }

    // 1. Получаем список всех выделенных ячеек
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();

    if (items.isEmpty()) return;

    // 2. Блокируем сигналы, чтобы не вызывать Sync/Save на каждую ячейку (избегаем спама в логах)
    ui->tableWidget->blockSignals(true);

    // 3. Проходим циклом по всем выделенным элементам и очищаем их
    for (QTableWidgetItem* it : std::as_const(items))
    {
        if (it)
        {
            it->setText(QString());
        }
    }

    // 4. Разблокируем сигналы и обновляем вьюпорт
    ui->tableWidget->blockSignals(false);
    ui->tableWidget->viewport()->update();

}

void MainWindow::aboutLessons()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    rsc3 = new aboutLesson(this);
    rsc3->setWindowTitle("О программе");
    rsc3->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            rsc3->size(),
            screen->geometry()));

    rsc3->exec();
}

bool MainWindow::connectToDatabase(QSqlDatabase &db, QString path)
{
    db = QSqlDatabase::addDatabase("QSQLITE", "connection1");
    db.setDatabaseName(path); // путь к вашей базе

    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
        return false;
    }
    qDebug() << "База подключена!";
    return true;
}

void MainWindow::medgehorizontalcells()
{
    if (!ui->tableWidget) return;
    int row = ui->tableWidget->currentRow();
    int col = ui->tableWidget->currentColumn();
    if (row < 0 || col < 0) return;

    int anchorRow = row, anchorCol = col;
    bool found = false;
    for (int r = 0; r <= row && !found; ++r) {
        for (int c = 0; c <= col; ++c) {
            int rs = ui->tableWidget->rowSpan(r, c);
            int cs = ui->tableWidget->columnSpan(r, c);
            if ((rs > 1 || cs > 1) && r <= row && row < r + rs && c <= col && col < c + cs) {
                anchorRow = r; anchorCol = c; found = true; break;
            }
        }
    }
    // если не найден объединённый anchor, но возможно сам (row,col) — проверим
    int rspan = ui->tableWidget->rowSpan(anchorRow, anchorCol);
    int cspan = ui->tableWidget->columnSpan(anchorRow, anchorCol);
    if (rspan == 1 && cspan == 1) return; // ничего не разделять

    // снимем span
    ui->tableWidget->setSpan(anchorRow, anchorCol, 1, 1);

    // восстановим дочерние ячейки
    for (int dr = 0; dr < rspan; ++dr) {
        for (int dc = 0; dc < cspan; ++dc) {
            int r = anchorRow + dr;
            int c = anchorCol + dc;
            if (r == anchorRow && c == anchorCol) continue;
            if (!ui->tableWidget->item(r, c)) {
                QPair<int,int> key(r, c);
                if (savedCells_.contains(key)) {
                    ui->tableWidget->setItem(r, c, new QTableWidgetItem(savedCells_.value(key)));
                    savedCells_.remove(key);
                } else {
                    ui->tableWidget->setItem(r, c, new QTableWidgetItem(QString()));
                }
            }
        }
    }
    mergedLefts_.remove(qMakePair(anchorRow, anchorCol));
    ui->tableWidget->resizeRowsToContents();
}

void MainWindow::openLesson()
{
    QSqlDatabase db;
    controlString = "up_week";
    verticalControlString = "up_group";

    if (connectToDatabase(db, "/home/elf/sqlite_prog/qtsqlite/base/lesson_base.db"))
    {
        // Проверяем все таблицы в БД
        QStringList tables = db.tables();
        qDebug() << "Таблицы в БД:" << tables;

        // Проверяем каждую таблицу
        for (const QString &tableName : std::as_const(tables))
        {
            QSqlQuery countQuery(db);
            if (countQuery.exec(QString("SELECT COUNT(*) FROM %1").arg(tableName)))
            {
                countQuery.next();
                int count = countQuery.value(0).toInt();
                qDebug() << "Таблица" << tableName << "- строк:" << count;

                // Выводим первую строку
                QSqlQuery sampleQuery(db);
                if (sampleQuery.exec(QString("SELECT * FROM %1 LIMIT 1").arg(tableName)))
                {
                    if (sampleQuery.next())
                    {
                        QSqlRecord rec = sampleQuery.record();
                        qDebug() << "Первая строка из" << tableName << ":";
                        for (int i = 0; i < rec.count(); ++i)
                        {
                            qDebug() << rec.fieldName(i) << "=" << sampleQuery.value(i).toString();
                        }
                    }
                }
            }
        }

        loadAndAutoMergeFromDb2(db, ui->tableWidget, controlString, verticalControlString);
    }
}

void MainWindow::loadAndAutoMergeFromDb2(QSqlDatabase &db,QTableWidget *tableWidget,
            const QString &controlString,const QString &verticalControlString)  // vertical tag
{

    if (!db.isOpen()) {
        qDebug() << "Database is not open.";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM Lessons")) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    QVector<QStringList> rowsData;
    int columnCount = 0;

    while (query.next()) {
        if (columnCount == 0)
            columnCount = query.record().count();

        QStringList rowFields;
        for (int c = 0; c < columnCount; ++c) {
            rowFields << query.value(c).toString();
        }
        rowsData.append(rowFields);
    }

    qDebug() << "Загружено строк из БД:" << rowsData.size();

    tableWidget->clear();
    tableWidget->setColumnCount(columnCount);

    if (!rowsData.isEmpty()) {
        QSqlQuery headQuery(db);
        if (headQuery.exec("SELECT * FROM Lessons LIMIT 1")) {
            QSqlRecord headRec = headQuery.record();
            QStringList headers;
            for (int i = 0; i < columnCount; ++i)
                headers << headRec.fieldName(i);
            tableWidget->setHorizontalHeaderLabels(headers);
            qDebug() << "Заголовки установлены:" << headers;
        }
    }

    tableWidget->horizontalHeader()->setVisible(true);
    tableWidget->verticalHeader()->setVisible(true);
    tableWidget->setRowCount(rowsData.size());

    QSet<QPair<int, int>> mergedCells;
    int currentRow = 0;

    for (const QStringList &rowFields : rowsData) {
        tableWidget->setVerticalHeaderItem(currentRow, new QTableWidgetItem(QString::number(currentRow + 1)));

        for (int col = 0; col < columnCount; ++col) {
            if (mergedCells.contains(qMakePair(currentRow, col))) {
                qDebug() << "Пропускаем уже объединённую ячейку" << currentRow << col;
                continue;
            }

            QString raw = rowFields.value(col);

            // Парсим: текст\npositionKey\nlessonType
            QStringList parts = raw.split('\n');

            QString displayText = parts.value(0, "").trimmed();  // Основной текст (отображаем)
            QString positionKey = parts.value(1, "").trimmed();  // Ключ позиции (не отображаем)
            QString lessonType = parts.value(2, "").trimmed();   // Тип занятия (не отображаем)

            qDebug() << "Row:" << currentRow << "Col:" << col
                     << "positionKey:" << positionKey
                     << "displayText:" << displayText;

            if (!tableWidget->item(currentRow, col))
                tableWidget->setItem(currentRow, col, new QTableWidgetItem());

            QTableWidgetItem *item = tableWidget->item(currentRow, col);

            // Устанавливаем ПОЛНОЕ содержимое в UserRole (для saveBase)
            item->setData(Qt::UserRole, displayText);

            // Обрабатываем в зависимости от ключа позиции
            if (positionKey == "up_left" || positionKey == "up_right" ||
                positionKey == "down_left" || positionKey == "down_right") {

                item->setText(displayText);
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                qDebug() << "  Установлена обычная ячейка";
            }
            else if (positionKey == "left_right_up" || positionKey == "left_right_down") {

                if (col + 1 < tableWidget->columnCount()) {
                    if (!tableWidget->item(currentRow, col + 1))
                        tableWidget->setItem(currentRow, col + 1, new QTableWidgetItem());

                    tableWidget->setSpan(currentRow, col, 1, 2);
                    mergedCells.insert(qMakePair(currentRow, col + 1));

                    item->setText(displayText);
                    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    qDebug() << "  Объединено горизонтально";
                } else {
                    item->setText(displayText);
                    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                }
            }
            else if (positionKey == "up_down_left" || positionKey == "up_down_right") {

                if (currentRow + 1 < tableWidget->rowCount()) {
                    if (!tableWidget->item(currentRow + 1, col))
                        tableWidget->setItem(currentRow + 1, col, new QTableWidgetItem());

                    tableWidget->setSpan(currentRow, col, 2, 1);
                    mergedCells.insert(qMakePair(currentRow + 1, col));

                    item->setText(displayText);
                    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    qDebug() << "  Объединено вертикально";
                } else {
                    item->setText(displayText);
                    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                }
            }
            else {
                // Пустой или неизвестный ключ
                item->setText(displayText);
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                if (positionKey.isEmpty()) {
                    qDebug() << "  Пустой ключ позиции (обычная ячейка)";
                } else {
                    qDebug() << "  Неизвестный ключ позиции:" << positionKey;
                }
            }
        }
        ++currentRow;
    }

    tableWidget->resizeRowsToContents();
    tableWidget->viewport()->update();
    ui->tableWidget->hideColumn(0);
}

bool MainWindow::medgevertikalcells(QTableWidget *tableWidget,
                                    int row,
                                    int col,
                                    const QStringList &nonEmptyParts,
                                    const QString &verticalControlString,
                                    QSet<QPair<int,int>> &mergedPositions)
{
    if (!tableWidget) return false;
    if (row + 1 >= tableWidget->rowCount()) return false;
    if (nonEmptyParts.size() != 5) return false;
    if (nonEmptyParts.at(4).trimmed().compare(verticalControlString.trimmed(), Qt::CaseInsensitive) != 0)
        return false;

    QString mergedText = nonEmptyParts.mid(0, 4).join("\n");

    if (!tableWidget->item(row, col))
        tableWidget->setItem(row, col, new QTableWidgetItem());
    if (!tableWidget->item(row + 1, col))
        tableWidget->setItem(row + 1, col, new QTableWidgetItem());

    tableWidget->setSpan(row, col, 2, 1);
    mergedPositions.insert(qMakePair(row, col));

    tableWidget->item(row, col)->setText(mergedText);
    tableWidget->item(row, col)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    tableWidget->item(row + 1, col)->setText("");
    tableWidget->item(row + 1, col)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    qDebug() << "Merged vertically at" << row << col;
    return true;
}

void MainWindow::onItemChanged()
{
    setWindowTitle("Программатор нагрузки" + QString("*"));
    isModified = true;
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    if (!isModified)
    {
        event->accept();
        return;
    }

    QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Сохранить изменения?"),
                                                           tr("Данные таблицы были изменены. Сохранить перед выходом?"),
                                                           QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                                           QMessageBox::Save);

    if (ret == QMessageBox::Save)
    {
        if (saveBase())
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    else if (ret == QMessageBox::Discard)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::NewTable()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    rsc = new newLesson(this);
    rsc->setWindowTitle("Введите данные студентов");
    rsc->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            rsc->size(),
            screen->geometry()));

    rsc->exec();
}

void MainWindow::newUser()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    rsc4 = new NewUser(this);
    rsc4->setWindowTitle("Новый пользователь");
    rsc4->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            rsc4->size(),
            screen->geometry()));
    rsc4->exec();
    delete rsc4; // Clean up the dialog after closing

    // Update the label with the collected data
    QString fullText = userPosition + " " + userSurname; // Concatenate the strings
    label->setText(fullText); // Assuming you have a QLabel named label in the UI
}

bool MainWindow::saveBase()
{
    QSqlQuery query;
    bool success = true;
    if (!query.exec("DELETE FROM lessons_spring_semester")) return false;

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        for (int col = 1; col < ui->tableWidget->columnCount(); ++col)
        {
            // Пропускаем ячейки, которые поглощены объединением (не главные в Span)
            if (ui->tableWidget->item(row, col) == nullptr ||
                ui->tableWidget->isPersistentEditorOpen(ui->tableWidget->model()->index(row, col)))
                // В QTableWidget лучше проверять визуально через Span:
                if (row > 0 && ui->tableWidget->rowSpan(row-1, col) > 1) continue;
            if (col > 0 && ui->tableWidget->columnSpan(row, col-1) > 1) continue;

            QTableWidgetItem* item = ui->tableWidget->item(row, col);
            QString cellText = item->text().trimmed();
            QString finalContent;

            if (cellText.isEmpty()) {
                finalContent = "[EMPTY]";
            }
            else {
                // 1-4 строки: Текст
                QStringList lines = cellText.split('\n', Qt::KeepEmptyParts);
                QStringList firstFour;
                for (int i = 0; i < qMin(4, lines.count()); ++i) firstFour << lines[i].trimmed();
                QString parts = firstFour.join("\n");

                // Определение positionKey (5-я строка)
                int rSpan = ui->tableWidget->rowSpan(row, col);
                int cSpan = ui->tableWidget->columnSpan(row, col);
                bool isRowOdd = (row % 2 != 0); // 1, 3, 5... (нижняя неделя)
                bool isColOdd = (col % 2 != 0); // 1, 3, 5... (левая подгруппа)

                QString posKey = "none";
                if (rSpan == 1 && cSpan == 1) {
                    if (!isRowOdd && isColOdd)  posKey = "up_left";
                    if (!isRowOdd && !isColOdd) posKey = "up_right";
                    if (isRowOdd && isColOdd)   posKey = "down_left";
                    if (isRowOdd && !isColOdd)  posKey = "down_right";
                }
                else if (rSpan == 1 && cSpan > 1) {
                    posKey = isRowOdd ? "left_right_down" : "left_right_up";
                }
                else if (rSpan > 1 && cSpan == 1) {
                    posKey = isColOdd ? "up_down_left" : "up_down_right";
                }
                else if (rSpan > 1 && cSpan > 1) {
                    posKey = "full_cell";
                }

                // 6-я строка: Тип занятия
                QString lessonType = "none";
                if (cellText.contains("лекция", Qt::CaseInsensitive)) lessonType = "lection";
                else if (cellText.contains("лабораторное", Qt::CaseInsensitive)) lessonType = "lab_rab";
                else if (cellText.contains("практическое", Qt::CaseInsensitive)) lessonType = "pract_rab";

                finalContent = parts + "\n" + posKey + "\n" + lessonType;
            }

            query.prepare("INSERT INTO lessons_spring_semester (row_num, col_num, content) VALUES (?, ?, ?)");
            query.addBindValue(row);
            query.addBindValue(col);
            query.addBindValue(finalContent);
            if (!query.exec()) success = false;
        }
    }
    if (success) message_action("Success", "Данные сохранены с ключами позиций");
    return success;
}

// Вспомогательная функция для определения позиции
QString MainWindow::getPositionKey(int row, int col)
{
    bool isRowOdd = (row % 2 == 1);
    bool isColOdd = (col % 2 == 1);

    if (!isRowOdd && !isColOdd)
        return "up_left";
    else if (!isRowOdd && isColOdd)
        return "up_right";
    else if (isRowOdd && !isColOdd)
        return "down_left";
    else
        return "down_right";
}

void MainWindow::loadBase()
{
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->clearSpans();

    // Чистим текст и ставим центрирование (высота строк НЕ меняется)
    for (int r = 0; r < ui->tableWidget->rowCount(); ++r) {
        for (int c = 1; c < ui->tableWidget->columnCount(); ++c) {
            if (auto item = ui->tableWidget->item(r, c)) {
                item->setText("");
                item->setTextAlignment(Qt::AlignCenter);
            }
        }
    }

    QSqlQuery query("SELECT row_num, col_num, content FROM lessons_spring_semester", db);
    while (query.next())
    {
        int r = query.value(0).toInt();
        int c = query.value(1).toInt();
        QString fullContent = query.value(2).toString();

        if (r < ui->tableWidget->rowCount() && c < ui->tableWidget->columnCount())
        {
            QTableWidgetItem *item = ui->tableWidget->item(r, c);
            if (!item) continue;
            item->setTextAlignment(Qt::AlignCenter);

            if (fullContent == "[EMPTY]") {
                item->setText("");
            }
            else {
                QStringList parts = fullContent.split('\n');
                // Отображаем первые 4 строки
                QStringList displayLines;
                for (int i = 0; i < qMin(4, parts.size()); ++i) displayLines << parts.at(i);
                item->setText(displayLines.join("\n"));

                // Восстановление объединений по ключам
                if (fullContent.contains("left_right_up") || fullContent.contains("left_right_down")) {
                    ui->tableWidget->setSpan(r, c, 1, 2);
                }
                else if (fullContent.contains("up_down_left") || fullContent.contains("up_down_right")) {
                    ui->tableWidget->setSpan(r, c, 2, 1);
                }
                else if (fullContent.contains("full_cell")) {
                    ui->tableWidget->setSpan(r, c, 2, 2);
                }
            }
        }
    }

    ui->tableWidget->blockSignals(false);
    ui->tableWidget->viewport()->update();
    message_action("Success", "Загрузка завершена: позиции восстановлены");
}

// void MainWindow::reloadAllTables()
// {
//     // for (auto it = tables.begin(); it != tables.end(); ++it) {
//     //     QString tableName = it.key();
//     //     QTableWidget* tableWidget = it.value();

//     //     loadTableData(currentTableName);
//     // }

//     for (auto it = tables.begin(); it != tables.end(); ++it) {
//         QString tableName = it.key();
//         // можно сразу вызвать нужную функцию
//         //loadTableData(tableName);
//     }
// }

// void MainWindow::loadTableData(const QString& tableName)
// {
//     /*QTableWidget* tableWidget = tables.value(tableName);
//     if (!tableWidget) return;

//     // Полностью очищаем таблицу
//     tableWidget->setRowCount(0);

//     QSqlQuery query(QString("SELECT id, name, value FROM \"%1\"").arg(tableName));
//     while (query.next())
//     {
//         int row = tableWidget->rowCount();  // текущий индекс
//         tableWidget->insertRow(row);

//         tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(query.value(0).toInt())));
//         tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
//         tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
//     }*/

//     if (tableName.isEmpty()) {
//         qWarning() << "Пустое название таблицы";
//         return;
//     }

//     QTableWidget* tableWidget = ui->tableWidget; // или tables.value(tableName) если используете map

//     if (!tableWidget) {
//         qWarning() << "TableWidget не найден";
//         return;
//     }

//     // Получаем структуру таблицы
//     QSqlQuery structQuery;
//     QString structSql = QString("PRAGMA table_info(\"%1\")").arg(tableName);

//     if (!structQuery.exec(structSql)) {
//         qWarning() << "Ошибка получения структуры таблицы:" << structQuery.lastError().text();
//         return;
//     }

//     QStringList columnNames;
//     while (structQuery.next()) {
//         columnNames << structQuery.value(1).toString(); // имя колонки
//     }

//     if (columnNames.isEmpty()) {
//         qWarning() << "Таблица не имеет колонок";
//         return;
//     }

//     qDebug() << "Колонки таблицы:" << columnNames;

//     // Полностью очищаем таблицу виджета
//     tableWidget->clear();
//     tableWidget->setRowCount(0);
//     tableWidget->setColumnCount(columnNames.size());
//     tableWidget->setHorizontalHeaderLabels(columnNames);

//     // Загружаем данные
//     QSqlQuery query;
//     QString selectSql = QString("SELECT %1 FROM \"%2\"")
//                             .arg(columnNames.join(", "), tableName);

//     qDebug() << "SQL:" << selectSql;

//     if (!query.exec(selectSql)) {
//         qWarning() << "Ошибка при выполнении запроса:" << query.lastError().text();
//         return;
//     }

//     // Заполняем строки данными
//     int row = 0;
//     while (query.next()) {
//         tableWidget->insertRow(row);

//         for (int col = 0; col < columnNames.size(); ++col) {
//             QString value = query.value(col).toString();
//             tableWidget->setItem(row, col, new QTableWidgetItem(value));
//         }
//         ++row;
//     }

//     // Если таблица пуста, добавляем пустые строки согласно размерности
//     if (row == 0) {
//         int defaultRows = 10; // или получите из переменной класса
//         tableWidget->setRowCount(defaultRows);

//         for (int r = 0; r < defaultRows; ++r) {
//             for (int c = 0; c < columnNames.size(); ++c) {
//                 if (!tableWidget->item(r, c)) {
//                     tableWidget->setItem(r, c, new QTableWidgetItem(""));
//                 }
//             }
//         }
//         qDebug() << "Пустая таблица загружена с" << defaultRows << "строками";
//     }

//     qDebug() << "Загружено строк:" << row;
// }

void MainWindow::loadTableFromDatabase(const QString &tableName, QTableWidget *tableWidget)
{
    if (!db.isOpen()) {
        return;
    }

    // Очищаем таблицу
    tableWidget->setRowCount(0);

    // Загружаем данные из БД
    QString sql = QString("SELECT id, name, value FROM \"%1\"").arg(tableName);
    QSqlQuery query;
    query.prepare(sql);

    if (!query.exec()) {
        qDebug() << "Ошибка загрузки таблицы" << tableName << ":" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        tableWidget->insertRow(row);

        tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // id
        tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString())); // name
        tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString())); // value

        row++;
    }

    // Добавляем несколько пустых строк для ввода новых данных
    for (int i = 0; i < 5; ++i) {
        tableWidget->insertRow(tableWidget->rowCount());
    }

    qDebug() << "Всего таблиц загружено:" << tables.size();
}

QString MainWindow::getUserDataForTable(const QString& tableName, const QString& field, int row)
{
    QTableWidget* table = tables[tableName];

    if (field == "id") {
        return table->item(row, 0) ? table->item(row, 0)->text() : "";
    } else if (field == "name") {
        return table->item(row, 1) ? table->item(row, 1)->text() : "";
    } else if (field == "value") {
        return table->item(row, 2) ? table->item(row, 2)->text() : "0";
    }
    return "";
}

// Получение id текущей строки — например, из скрытого столбца или данных
int MainWindow::getCurrentItemId(const QString& tableName, int row)
{
    if (!tables.contains(tableName)) {
        qWarning() << "Table not found:" << tableName;
        return -1;
    }

    QTableWidget* table = tables[tableName];
    if (table->item(row, 0))
        return table->item(row, 0)->text().toInt();
    return -1;
}

void MainWindow::addLinesComboBox()
{
    if (!lessonList) return;

    lessonList->clear();

    QSqlQuery q;
    // Исключаем системные таблицы sqlite
    if (!q.exec("SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%'"))
    {
        qWarning() << "DB error:" << q.lastError().text();
        return;
    }

    QStringList tableNames;

    while (q.next()) {
        QString name = q.value(0).toString().trimmed();
        tableNames.append(name);
    }

    lessonList->addItems(tableNames);
    qDebug() << "Loaded table names:" << tableNames;
}

void MainWindow::addLineslessonList()
{
    if (!lessonList) {
        qWarning() << "lessonList is nullptr!";
        lessonList = new QComboBox(this);  // Переинициализируем, если нужно
    }

    lessonList->clear();

    QSqlQuery q;
    // Исключаем системные таблицы sqlite
    if (!q.exec("SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%'"))
    {
        qWarning() << "DB error:" << q.lastError().text();
        return;
    }

    QStringList tableNames;

    while (q.next()) {
        QString name = q.value(0).toString().trimmed();
        tableNames.append(name);
    }

    lessonList->addItems(tableNames);
    qDebug() << "Loaded table names:" << tableNames;

}

int MainWindow::nextPageNumberFromDb()
{
    QSqlQuery q;
    // Execute query to find all tables starting with 'page'
    if (!q.exec("SELECT name FROM sqlite_master WHERE type='table' AND name LIKE 'page%'"))
    {
        qWarning() << "DB error (LIKE 'page%'):" << q.lastError().text();
        return 1; // Return 1 by default in case of error
    }

    int maxN = 0;
    // Declare the QRegularExpression as static here
    static const QRegularExpression re("^page(\\d+)$");

    while (q.next()) {
        QString name = q.value(0).toString().trimmed();

        QRegularExpressionMatch m = re.match(name);
        if (!m.hasMatch()) {
            qDebug() << "Skipping non-matching table name:" << name;
            continue;
        }

        bool ok = false;
        int n = m.captured(1).toInt(&ok);
        if (!ok) {
            qDebug() << "Failed to parse number from:" << name;
            continue;
        }
        if (n > maxN) maxN = n;
    }

    int next = maxN + 1;
    qDebug() << "nextPageNumberFromDb: maxN =" << maxN << " next =" << next;
    return next;
}

void MainWindow::CreateNewTable(int pageNum)
{

    /*pageNum = nextPageNumberFromDb();
    //QString name = QString("Lessons%1").arg(pageNum);
    QString name = lesson_name;

    QSqlQuery q;
    QString create = QString("CREATE TABLE IF NOT EXISTS \"%1\" (id INTEGER PRIMARY KEY AUTOINCREMENT, Понедельник TEXT, "
                             "Понедельник_2 TEXT, Вторник TEXT, Вторник_2 TEXT, Среда TEXT, Среда_2 TEXT,"
                             "Четверг TEXT, Четверг_2 TEXT, Пятница TEXT, Пятница_2 TEXT, Суббота TEXT, Суббота_2 TEXT)").arg(lesson_name);
    if (!q.exec(create)) { qWarning() << "Create failed:" << q.lastError().text(); return; }

    QSqlQuery ins;
    QString insSql = QString("INSERT INTO \"%1\" (Понедельник, Понедельник_2, Вторник, Вторник_2, Среда, Среда_2,"
                             "Четверг, Четверг_2, Пятница, Пятница_2, Суббота, Суббота_2) VALUES "
                             "(:Понедельник, :Понедельник_2, :Вторник, :Вторник_2, :Среда, :Среда_2,"
                             ":Четверг, :Четверг_2, :Пятница, :Пятница_2, :Суббота, :Суббота_2)").arg(lesson_name);

    if (!ins.prepare(insSql))
    {
        qWarning() << ins.lastError().text();
        return;
    }

    ins.bindValue("name", " ");
    ins.bindValue(":name", " ");
    ins.exec();

    ins.bindValue("name_2", " ");
    ins.bindValue(":name_2", " ");
    ins.exec();

    ins.bindValue("name_3", " ");
    ins.bindValue(":name_3", " ");
    ins.exec();

    ins.bindValue("name_4", " ");
    ins.bindValue(":name_4", " ");
    ins.exec();

    ins.bindValue("name_5", " ");
    ins.bindValue(":name_5", " ");
    ins.exec();

    ins.bindValue("name_6", " ");
    ins.bindValue(":name_6", " ");
    ins.exec();

    ins.bindValue("name_7", " ");
    ins.bindValue(":name_7", " ");
    ins.exec();

    ins.bindValue("name_8", " ");
    ins.bindValue(":name_8", " ");
    ins.exec();

    ins.bindValue("name_9", " ");
    ins.bindValue(":name_9", " ");
    ins.exec();

    ins.bindValue("name_10", " ");
    ins.bindValue(":name_10", " ");
    ins.exec();

    ins.bindValue("name_11", " ");
    ins.bindValue(":name_11", " ");
    ins.exec();

    ins.bindValue("name_12", " ");
    ins.bindValue(":name_12", " ");
    ins.exec();

    int idx = ui->comboBox->findText(lesson_name);
    if (idx < 0)
    {
        ui->comboBox->addItem(lesson_name, pageNum); // <- важна вторая переменная
        idx = ui->comboBox->findText(lesson_name);
    } else
    {
        ui->comboBox->setItemData(idx, pageNum); // обновляем data, если элемент уже был
    }

    qDebug() << "Added combo item:" << lesson_name << "index=" << idx << "data=" << ui->comboBox->itemData(idx);
    ui->comboBox->setCurrentIndex(idx); // вызовет слот загрузки*/

    pageNum = nextPageNumberFromDb();
    QString name = lesson_name.trimmed();

    if (name.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Имя таблицы пустое, создание отменено.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    QStringList tables = db.tables(QSql::Tables);

    bool tableExists = tables.contains(name, Qt::CaseInsensitive);

    if (tableExists)
    {
        QMessageBox::information(this, "Внимание", QString("Таблица с именем \"%1\" уже существует.").arg(name));
        return;
    }

    QSqlQuery q;
    QString create = QString(
                         "CREATE TABLE IF NOT EXISTS \"%1\" ("
                         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "Понедельник TEXT, Понедельник_2 TEXT, "
                         "Вторник TEXT, Вторник_2 TEXT, "
                         "Среда TEXT, Среда_2 TEXT, "
                         "Четверг TEXT, Четверг_2 TEXT, "
                         "Пятница TEXT, Пятница_2 TEXT, "
                         "Суббота TEXT, Суббота_2 TEXT)").arg(name);

    if (!q.exec(create))
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать таблицу:\n" + q.lastError().text());
        return;
    }

    QSqlQuery ins;
    QString insSql = QString(
                         "INSERT INTO \"%1\" "
                         "(Понедельник, Понедельник_2, Вторник, Вторник_2, Среда, Среда_2, "
                         "Четверг, Четверг_2, Пятница, Пятница_2, Суббота, Суббота_2) "
                         "VALUES (:mon, :mon2, :tue, :tue2, :wed, :wed2, :thu, :thu2, :fri, :fri2, :sat, :sat2)").arg(name);

    if (!ins.prepare(insSql))
    {
        QMessageBox::critical(this, "Ошибка", "Ошибка подготовки запроса вставки:\n" + ins.lastError().text());
        return;
    }

    ins.bindValue(":mon", "");
    ins.bindValue(":mon2", "");
    ins.bindValue(":tue", "");
    ins.bindValue(":tue2", "");
    ins.bindValue(":wed", "");
    ins.bindValue(":wed2", "");
    ins.bindValue(":thu", "");
    ins.bindValue(":thu2", "");
    ins.bindValue(":fri", "");
    ins.bindValue(":fri2", "");
    ins.bindValue(":sat", "");
    ins.bindValue(":sat2", "");

    if (!ins.exec())
    {
        QMessageBox::critical(this, "Ошибка", "Ошибка вставки данных в таблицу:\n" + ins.lastError().text());
        return;
    }

    int idx = lessonList->findText(name);
    if (idx < 0)
    {
        lessonList->addItem(name, pageNum);
        idx = lessonList->findText(name);
    }
    else
    {
        lessonList->setItemData(idx, pageNum);
    }

    qDebug() << "Добавлен элемент comboBox:" << name << "индекс=" << idx << "данные=" << lessonList->itemData(idx);

    lessonList->setCurrentIndex(idx);
}

// void MainWindow::onPageChanged(int index)
// {
//     if (index == -1)
//     {
//         qWarning() << "Нет выбранного элемента в ComboBox.";
//         return;
//     }

//     // Получаем название таблицы напрямую из текста ComboBox
//     QString tableName = ui->comboBox->currentText().trimmed();
//     if (tableName.isEmpty())
//     {
//         qWarning() << "Пустое название таблицы.";
//         return;
//     }

//     qDebug() << "Selected table:" << tableName;

//     // Проверяем наличие таблицы
//     QSqlQuery qCheck;
//     if (!qCheck.exec(QString("SELECT name FROM sqlite_master WHERE type='table' AND name='%1'")
//                          .arg(tableName)))
//     {
//         qWarning() << "Check query failed:" << qCheck.lastError().text();
//         return;
//     }

//     if (!qCheck.next())
//     {
//         qWarning() << "Table does not exist:" << tableName;
//         ui->tableWidget->clear();
//         ui->tableWidget->setRowCount(0);
//         ui->tableWidget->setColumnCount(0);
//         return;
//     }

//     qDebug() << "Found table in sqlite_master:" << qCheck.value(0).toString();

//     // Формируем и выполняем запрос
//     QString sql = QString("SELECT * FROM \"%1\" ORDER BY id").arg(tableName);
//     qDebug() << "SQL:" << sql;

//     QSqlQuery query;
//     if (!query.exec(sql))
//     {
//         qWarning() << "Query failed:" << query.lastError().text();
//         return;
//     }

//     // Очистка таблицы виджета
//     ui->tableWidget->clear();
//     ui->tableWidget->setRowCount(0);

//     // Установка колонок/заголовков
//     int colCount = query.record().count();
//     ui->tableWidget->setColumnCount(colCount);
//     QStringList headers;
//     for (int c = 0; c < colCount; ++c)
//         headers << query.record().fieldName(c);
//     ui->tableWidget->setHorizontalHeaderLabels(headers);

//     // Заполнение строк
//     int row = 0;
//     while (query.next())
//     {
//         ui->tableWidget->setRowCount(row + 1);
//         for (int c = 0; c < colCount; ++c)
//         {
//             ui->tableWidget->setItem(row, c, new QTableWidgetItem(query.value(c).toString()));
//         }
//         ++row;
//     }

//     // Если таблица пуста, отображаем пустые строки и столбцы согласно размерности
//     if (row == 0 && colCount > 0)
//     {
//         // Получаем текущую размерность tableWidget
//         int defaultRows = ui->tableWidget->rowCount();
//         if (defaultRows == 0)
//         {
//             // Если rowCount не установлен, устанавливаем стандартное значение
//             // (можете изменить на нужное вам значение)
//             defaultRows = 10; // или другое значение по умолчанию
//             ui->tableWidget->setRowCount(defaultRows);
//         }

//         // Заполняем пустыми ячейками
//         for (int r = 0; r < defaultRows; ++r)
//         {
//             for (int c = 0; c < colCount; ++c)
//             {
//                 if (!ui->tableWidget->item(r, c))
//                 {
//                     ui->tableWidget->setItem(r, c, new QTableWidgetItem(""));
//                 }
//             }
//         }

//         qDebug() << "Empty table loaded with" << defaultRows << "rows and" << colCount << "columns";
//     }
//     else
//     {
//         qDebug() << "rows loaded:" << row;
//     }
// }

void MainWindow::onPageChanged2(int index)
{
    if (index == -1)
    {
        qWarning() << "Нет выбранного элемента в ComboBox.";
        return;
    }

    // Получаем название таблицы напрямую из текста ComboBox
    QString tableName = lessonList->currentText().trimmed();
    if (tableName.isEmpty())
    {
        qWarning() << "Пустое название таблицы.";
        return;
    }

    qDebug() << "Selected table:" << tableName;

    // Проверяем наличие таблицы
    QSqlQuery qCheck;
    if (!qCheck.exec(QString("SELECT name FROM sqlite_master WHERE type='table' AND name='%1'")
                         .arg(tableName)))
    {
        qWarning() << "Check query failed:" << qCheck.lastError().text();
        return;
    }

    if (!qCheck.next())
    {
        qWarning() << "Table does not exist:" << tableName;
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setColumnCount(0);
        return;
    }

    qDebug() << "Found table in sqlite_master:" << qCheck.value(0).toString();

    // Формируем и выполняем запрос
    QString sql = QString("SELECT * FROM \"%1\" ORDER BY id").arg(tableName);
    qDebug() << "SQL:" << sql;

    QSqlQuery query;
    if (!query.exec(sql))
    {
        qWarning() << "Query failed:" << query.lastError().text();
        return;
    }

    // Очистка таблицы виджета
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);

    // Установка колонок/заголовков
    int colCount = query.record().count();
    ui->tableWidget->setColumnCount(colCount);
    QStringList headers;
    for (int c = 0; c < colCount; ++c)
        headers << query.record().fieldName(c);
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Заполнение строк
    int row = 0;
    while (query.next())
    {
        ui->tableWidget->setRowCount(row + 1);
        for (int c = 0; c < colCount; ++c)
        {
            ui->tableWidget->setItem(row, c, new QTableWidgetItem(query.value(c).toString()));
        }
        ++row;
    }

    // Если таблица пуста, отображаем пустые строки и столбцы согласно размерности
    if (row == 0 && colCount > 0)
    {
        // Получаем текущую размерность tableWidget
        int defaultRows = ui->tableWidget->rowCount();
        if (defaultRows == 0)
        {
            // Если rowCount не установлен, устанавливаем стандартное значение
            // (можете изменить на нужное вам значение)
            defaultRows = 10; // или другое значение по умолчанию
            ui->tableWidget->setRowCount(defaultRows);
        }

        // Заполняем пустыми ячейками
        for (int r = 0; r < defaultRows; ++r)
        {
            for (int c = 0; c < colCount; ++c)
            {
                if (!ui->tableWidget->item(r, c))
                {
                    ui->tableWidget->setItem(r, c, new QTableWidgetItem(""));
                }
            }
        }

        qDebug() << "Empty table loaded with" << defaultRows << "rows and" << colCount << "columns";
    }
    else
    {
        qDebug() << "rows loaded:" << row;
    }
}

QStringList MainWindow::getTableNames()
{
    QStringList tableNames;
    QSqlQuery query;

    // Выполняем запрос
    if (!query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%'")) {
        qWarning() << "Ошибка выполнения запроса:" << query.lastError().text();
        return tableNames;
    }

    // Обрабатываем результаты
    while (query.next()) {
        QString tableName = query.value(0).toString();
        tableNames << tableName;
    }
    return tableNames;
}
void MainWindow::loadDataToTable(QSqlDatabase &db, QTableWidget *tableWidget)
{
    ui->tableWidget->setFont(QFont("Arial", 10));

    if (!db.isOpen())
    {
        qDebug() << "Database is not open.";
        return; // Exit if the database is not open
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT * FROM Lessons"))
    {
        qDebug() << "Error executing query:" << query.lastError().text();
        return; // Exit on query failure
    }

    // Выполняем SELECT-запрос
    if (query.exec("SELECT * FROM Lessons"))
    {
        // Очищаем таблицу
        tableWidget->clear();

        // Получаем количество колонок
        int columnCount = query.record().count();
        //int totalColumns = columnCount;

        // Устанавливаем число колонок
        tableWidget->setColumnCount(columnCount);

        // Получаем имена полей из базы для заголовков
        QStringList headers;
        for (int i = 0; i < columnCount; ++i)
        {
            headers << query.record().fieldName(i);
        }
        // Устанавливаем горизонтальные заголовки
        tableWidget->setHorizontalHeaderLabels(headers);
        tableWidget->horizontalHeader()->setVisible(true);
        tableWidget->verticalHeader()->setVisible(true);

        // Установка количества строк
        tableWidget->setRowCount(0);

        int currentRow = 0;
        // Заполняем таблицу данными
        while (query.next())
        {
            tableWidget->insertRow(currentRow);
            // int currentColumn = 0;
            for (int col = 0; col < columnCount; ++col)
            {
                QString originalValue = query.value(col).toString(); // Данные из базы
                // qDebug() << "Значение из базы:" << originalValue;

                originalValue.replace("\\n", "\n");  // заменить буквы '\\n' на перенос строки
                // qDebug() << "Значение из базы2:" << originalValue;

                QStringList parts = originalValue.split('\n');
                QString text1, text2, text3, text4, text5;
                QString displayText;
                if (parts.size() == 4)
                {
                    // Есть ровно три части — можно их использовать или обработать
                    text1 = parts.at(0);
                    text2 = parts.at(1);
                    text3 = parts.at(2);
                    text4 = parts.at(3);

                    // Пример использования:
                    displayText = text1 + "\n" + text2 + "\n" + text3 + "\n" + text4;
                }
                else if (parts.size() == 1 && !parts.at(0).isEmpty())
                {
                    // Только один непустой фрагмент — оставить содержимое без изменений
                    QString finalValue = originalValue;
                    displayText = finalValue;
                    // Используйте finalValue далее по необходимости


                } else
                {
                    // Все остальные случаи — оставить как есть или обработать по необходимости
                    QString finalValue = originalValue;
                    displayText = finalValue;
                    //qDebug() << "Разделенные части:" << parts;
                }

                QStringList nonEmptyParts;

                for (int i = 0; i < parts.size(); ++i)
                {
                    const QString &part = parts[i];
                    if (!part.isEmpty())
                    {
                        nonEmptyParts << part;
                    }
                }

                //QString displayText;
                if (!nonEmptyParts.isEmpty()) {
                    displayText = nonEmptyParts.join("\n"); // или любой разделитель
                } else {
                    displayText = ""; // или "Пусто"
                }

                // Заполняем существующую ячейку
                QTableWidgetItem *item = ui->tableWidget->item(currentRow, col);
                if (!item)
                {
                    item = new QTableWidgetItem();
                    ui->tableWidget->setItem(currentRow, col, item);
                }
                item->setText(displayText);
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
            ui->tableWidget->setVerticalHeaderItem(currentRow, new QTableWidgetItem(QString::number(currentRow + 1)));

            ++currentRow;
        }
    } else
    {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
    }

    // Предположим, у вас есть таблица с N строками
    int rowCount = ui->tableWidget->rowCount();

    // Массив данных для вертикальных заголовков
    QStringList verticalHeaders = {"8.30-10.05\nВерхняя неделя", "8.30-10.05\nНижняя неделя",
                                   "10.25-12.00\nВерхняя неделя", "10.25-12.00\nНижняя неделя",
                                   "12.30-14.05\nВерхняя неделя", "12.30-14.05\nНижняя неделя",
                                   "14.20-15.55\nВерхняя неделя", "14.20-15.55\nНижняя неделя",
                                   "16.05-17.40\nВерхняя неделя", "16.05-17.40\nНижняя неделя",
                                   "17.50-19.20\nВерхняя неделя", "17.50-19.20\nНижняя неделя",
                                   "19.25-21.10\nВерхняя неделя", "19.25-21.10\nНижняя неделя"};

    // Убедитесь, что количество элементов в verticalHeaders совпадает с количеством строк
    Q_ASSERT(verticalHeaders.size() >= rowCount);

    for (int row = 0; row < rowCount; ++row)
    {
        //ui->tableWidget->setVerticalHeaderItem(row, new QTableWidgetItem(verticalHeaders.at(row)));

        QTableWidgetItem *hi = new QTableWidgetItem(verticalHeaders.at(row));
        hi->setTextAlignment(Qt::AlignCenter);
        // Разрешаем перенос строк в тексте (в заголовках QTableWidgetItem это учитывается автоматически)
        ui->tableWidget->setVerticalHeaderItem(row, hi);
    }

    // Увеличим высоту строки заголовка (высоту строки таблицы), чтобы поместился многострочный заголовок
    for (int row = 0; row < rowCount; ++row)
    {
        ui->tableWidget->setRowHeight(row, 50); // подберите нужное значение
    }

    for (int col = 0; col < tableWidget->columnCount(); ++col)
    {
        QString headerText = tableWidget->horizontalHeaderItem(col)->text();
        if (headerText == "id")
        { // замените на точное имя заголовка
            tableWidget->hideColumn(col);
            break;
        }
    }

    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->viewport()->update(); // попробуйте добавить

    ui->tableWidget->hideColumn(0);
}

void MainWindow::applyRowHeights() {
    qDebug() << "\n=== APPLYING ROW HEIGHTS ===";

    // ВАЖНО: Отключите автоматическое изменение размера
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    QFontMetrics fm(ui->tableWidget->font());
    QString fiveLineText = "Группа 1\nГруппа 2\nГруппа 3\nГруппа 4\nГруппа 5";

    QRect textRect = fm.boundingRect(QRect(0, 0, 90, 1000),
                                     Qt::AlignCenter | Qt::TextWordWrap,
                                     fiveLineText);

    int rowHeight = textRect.height() + 8;
    qDebug() << "Calculated row height:" << rowHeight;

    // Установите для КАЖДОЙ строки
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        ui->tableWidget->setRowHeight(i, rowHeight);
    }

    ui->tableWidget->verticalHeader()->setDefaultSectionSize(rowHeight);

    qDebug() << "Applied" << rowHeight << "to all rows";
    qDebug() << "=== DONE ===\n";
}

// void MainWindow::hideColums()
// {
//     int totalRows = ui->tableWidget->rowCount();

//     // Проходим циклом по последним 4 индексам
//     for (int i = totalRows - 4; i < totalRows; ++i) {
//         if (i >= 0) { // Защита на случай, если в таблице меньше 4 строк
//             ui->tableWidget->setRowHidden(i, true);
//         }
//     }
// }

void MainWindow::printLessonDialog()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize::A4);

    // Используйте Portrait вместо Landscape
    QPageLayout layout(QPageSize(QPageSize::A4), QPageLayout::Portrait,
                       QMarginsF(0, 0, 0, 0), QPageLayout::Millimeter);
    printer.setPageLayout(layout);

    printer.setResolution(300);
    printer.setFullPage(true);

    QPrintDialog dlg(&printer, nullptr);
    dlg.setWindowTitle("Печать таблицы");
    if (dlg.exec() == QDialog::Accepted)
    {
        bool ok = print_lesson(ui->tableWidget, &printer);
        if (ok) qDebug() << "Печать выполнена";
        else qDebug() << "Ошибка печати";
    }
    else
    {
        qDebug() << "Печать отменена";
    }
}

bool MainWindow::print_lesson(QTableWidget *table, QPrinter *printer,
                              const QString &headerText, const QString &subHeaderText)
{
    // Получите реальный размер таблицы
    int realTableW = table->width();
    int realTableH = table->height();

    qDebug() << "Реальный размер таблицы: W=" << realTableW << "H=" << realTableH;

    // --- logical sizes ---
    int logicalW = table->verticalHeader()->width();
    for (int c = 0; c < table->model()->columnCount(); ++c)
        logicalW += table->columnWidth(c);

    logicalW += table->model()->columnCount() * table->gridStyle();
    logicalW += 30;

    int logicalH = table->horizontalHeader()->height();
    for (int r = 0; r < table->model()->rowCount(); ++r)
        logicalH += table->rowHeight(r);

    logicalH += (table->model()->rowCount() + 1) * table->gridStyle();
    logicalH += 30;

    qDebug() << "Логический размер: W=" << logicalW << "H=" << logicalH;

    if (logicalW <= 0 || logicalH <= 0) return false;

    // --- АВТОМАТИЧЕСКИЙ ВЫБОР ОРИЕНТАЦИИ ---
    if (logicalW > logicalH) {
        printer->setPageOrientation(QPageLayout::Landscape);
        qDebug() << "Установлена ГОРИЗОНТАЛЬНАЯ ориентация";
    } else {
        printer->setPageOrientation(QPageLayout::Portrait);
        qDebug() << "Установлена ВЕРТИКАЛЬНАЯ ориентация";
    }

    // --- paper rect in device pixels ---
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    QRectF paperRect = printer->paperRect(QPrinter::DevicePixel);
#else
    QRectF paperRect = QRectF(printer->paperRect(QPrinter::DevicePixel));
#endif

    const int innerPad = 8;
    const int marginPx = 16;
    const int maxDeviceDim = 16000;
    const int headerHeight = 55;      // Первая строка
    const int subHeaderHeight = 55;   // Вторая строка
    const int spacingHeight = 30;     // Воздушное пространство между второй строкой и таблицей

    // Добавляем место для заголовков и пространства
    int totalLogicalH = logicalH + headerHeight + subHeaderHeight + spacingHeight;

    qreal availW = qMax<qreal>(1.0, paperRect.width() - 2.0 * marginPx);
    qreal availH = qMax<qreal>(1.0, paperRect.height() - 2.0 * innerPad);

    int targetW = int(std::floor(availW + 0.5));
    int targetH = int(std::floor(availH + 0.5));

    qreal downscale = 1.0;
    if (targetW > maxDeviceDim || targetH > maxDeviceDim) {
        qreal sx = qreal(maxDeviceDim) / qMax(targetW, 1);
        qreal sy = qreal(maxDeviceDim) / qMax(targetH, 1);
        downscale = qMin(sx, sy);
        targetW = qMax(1, int(targetW * downscale));
        targetH = qMax(1, int(targetH * downscale));
    }

    qreal sx = qreal(targetW) / qMax<qreal>(1.0, logicalW);
    qreal sy = qreal(targetH) / qMax<qreal>(1.0, totalLogicalH);
    qreal renderScale = qMin(sx, sy);
    if (renderScale <= 0) renderScale = 1.0;

    int imgW = qMax(1, int(std::floor(logicalW * renderScale + 0.5)));
    int imgH = qMax(1, int(std::floor(totalLogicalH * renderScale + 0.5)));

    if (imgW > maxDeviceDim || imgH > maxDeviceDim) {
        qreal s = qMin(qreal(maxDeviceDim) / imgW, qreal(maxDeviceDim) / imgH);
        imgW = qMax(1, int(imgW * s));
        imgH = qMax(1, int(imgH * s));
    }

    QImage deviceImg(imgW, imgH, QImage::Format_ARGB32);
    deviceImg.setDevicePixelRatio(1);
    deviceImg.fill(Qt::white);

    qDebug() << "logicalW,logicalH:" << logicalW << logicalH
             << "imgW,imgH:" << imgW << imgH
             << "renderScale:" << renderScale;

    // Рендеринг с текстом и таблицей
    {
        QPainter devicePainter(&deviceImg);
        devicePainter.setRenderHint(QPainter::TextAntialiasing, true);
        devicePainter.setRenderHint(QPainter::Antialiasing, true);
        devicePainter.setRenderHint(QPainter::SmoothPixmapTransform, true);

        devicePainter.save();
        devicePainter.scale(renderScale, renderScale);

        // Шрифт 14 pt для обеих строк
        QFont font = devicePainter.font();
        font.setPointSize(14);
        devicePainter.setFont(font);
        devicePainter.setPen(Qt::black);

        // Рисуем первый заголовок (жирный)
        if (!headerText.isEmpty()) {
            QFont boldFont = font;
            boldFont.setBold(true);
            devicePainter.setFont(boldFont);

            QRect headerRect(0, 0, logicalW, headerHeight);
            devicePainter.drawText(headerRect, Qt::AlignCenter | Qt::TextWordWrap, headerText);
        }

        // Рисуем второй заголовок (обычный)
        if (!subHeaderText.isEmpty()) {
            QFont normalFont = font;
            normalFont.setPointSize(14);
            normalFont.setBold(true);
            devicePainter.setFont(normalFont);

            QRect subHeaderRect(0, headerHeight, logicalW, subHeaderHeight);
            devicePainter.drawText(subHeaderRect, Qt::AlignCenter | Qt::TextWordWrap, subHeaderText);
        }

        // Рисуем таблицу ниже текста с учетом пространства
        devicePainter.save();
        devicePainter.translate(0, headerHeight + subHeaderHeight + spacingHeight);
        QRect fullLogicalRect(0, 0, logicalW, logicalH);
        table->render(&devicePainter, QPoint(0, 0), QRegion(fullLogicalRect), QWidget::DrawChildren);
        devicePainter.restore();

        devicePainter.restore();
        devicePainter.end();
    }

    deviceImg.save("output.jpg", nullptr, 90);

    // Рисуем на принтере
    QPainter painter(printer);
    if (!painter.isActive()) {
        qWarning() << "Failed to activate painter for printer";
        return false;
    }
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    qreal fitScale = qMin(availW / deviceImg.width(), availH / deviceImg.height());
    if (fitScale <= 0) fitScale = 1.0;

    qreal finalW = std::floor(deviceImg.width() * fitScale + 0.5);
    qreal finalH = std::floor(deviceImg.height() * fitScale + 0.5);

    qreal startX = paperRect.x() + (paperRect.width() - finalW) / 2.0;
    qreal startY = paperRect.y() + (paperRect.height() - finalH) / 2.0;
    qreal drawX = std::floor(startX + 0.5);
    qreal drawY = std::floor(startY + 0.5);

    qreal rightLimit = paperRect.x() + paperRect.width();
    qreal bottomLimit = paperRect.y() + paperRect.height();
    if (drawX + finalW > rightLimit) finalW = rightLimit - drawX;
    if (drawY + finalH > bottomLimit) finalH = bottomLimit - drawY;
    if (finalW < 1) finalW = 1;
    if (finalH < 1) finalH = 1;

    QRectF finalDst(drawX, drawY, finalW, finalH);
    QRectF srcRect(0.0, 0.0, qreal(deviceImg.width()), qreal(deviceImg.height()));

    painter.drawImage(finalDst, deviceImg, srcRect);
    painter.end();

    return true;
}

void MainWindow::showPrintPreview()
{
    // 1. ОПРЕДЕЛЯЕМ АКТИВНУЮ ТАБЛИЦУ
    // Получаем виджет текущей вкладки (предположим, имя таб-виджета: ui->tabWidget)
    QWidget *currentTab = ui->tabWidget->currentWidget();
    if (!currentTab) return;

    // Ищем QTableWidget внутри этой вкладки (универсальный способ для всех 6 вкладок)
    QTableWidget *activeTable = currentTab->findChild<QTableWidget *>();
    if (!activeTable) return; // Если во вкладке нет таблицы, выходим

    // 2. НАСТРОЙКА ПРИНТЕРА
    QPrinter *printer = new QPrinter(QPrinter::HighResolution);

    QPageLayout layout = printer->pageLayout();
    layout.setPageSize(QPageSize(QPageSize::A4));
    layout.setOrientation(QPageLayout::Landscape); // Альбомная ориентация
    layout.setMargins(QMarginsF(10, 10, 10, 10));  // Поля 10мм
    printer->setPageLayout(layout);

    printer->setResolution(300);

    QPrintPreviewDialog *previewDialog = new QPrintPreviewDialog(printer, this);

    // 3. ОТРИСОВКА
    // Захватываем указатель activeTable в лямбда-выражение
    connect(previewDialog, &QPrintPreviewDialog::paintRequested, this, [this, activeTable](QPrinter *p) {
        printFullTable(activeTable, p,
                       "Ст. преподаватель Третьяков А. С.",
                       "Весенний семестр 2025/2026 учебного года");
    });

    // 4. ОЧИСТКА ПАМЯТИ
    connect(previewDialog, &QDialog::finished, [previewDialog, printer]() {
        previewDialog->deleteLater();
        delete printer;
    });

    previewDialog->showMaximized();
    previewDialog->exec();
}

// void MainWindow::exportToPDF(QTableWidget *tableWidget, const QString &filename)
// {
//     QPrinter printer(QPrinter::HighResolution);
//     printer.setOutputFormat(QPrinter::PdfFormat);
//     printer.setOutputFileName(filename);

//     QPainter painter(&printer);
//     int cellWidth = 100;  // Ширина ячейки
//     int cellHeight = 30;  // Высота ячейки

//     for (int row = 0; row < tableWidget->rowCount(); ++row) {
//         for (int col = 0; col < tableWidget->columnCount(); ++col) {
//             QTableWidgetItem *item = tableWidget->item(row, col);
//             if (item) {
//                 QRect rect(col * cellWidth, row * cellHeight, cellWidth, cellHeight);
//                 painter.drawText(rect, item->text());
//             }
//         }
//     }

//     painter.end();
// }

// void MainWindow::traverseTable(QTableWidget *tableWidget) {
//     for (int row = 0; row < tableWidget->rowCount(); ++row) {
//         for (int col = 0; col < tableWidget->columnCount(); ++col) {
//             QTableWidgetItem *item = tableWidget->item(row, col);
//             if (item) {
//                 qDebug() << item->text();  // Обработка ячейки
//             }
//         }
//     }
// }

void MainWindow::printFullTable(QTableWidget *tableWidget, QPrinter *printer, QString teacherInfo, QString semesterInfo)
{
    QPainter painter(printer);
    if (!painter.isActive()) return;

    // 1. ГЕОМЕТРИЯ
    double mmToPx = printer->resolution() / 25.4;
    QRect paintRect = printer->pageLayout().paintRectPixels(printer->resolution());

    double margin = 8.0 * mmToPx;
    double pageWidth = paintRect.width() - (margin * 2);
    double startX = paintRect.left() + margin;

    // МАКСИМАЛЬНЫЙ ПОДЪЕМ
    double currentY = paintRect.top() + (2.0 * mmToPx);

    int colCount = tableWidget->columnCount();
    double colW = pageWidth / (double)colCount;

    QPen tablePen(Qt::black, 0.5);
    tablePen.setCosmetic(true);
    painter.setPen(tablePen);

    auto getX = [&](int col) { return qRound(startX + (col * colW)); };

    // 2. ШАПКА (СВЕРХКОМПАКТНО)
    painter.setFont(QFont("Arial", 9));
    painter.drawText(QRectF(startX, currentY, pageWidth, 4 * mmToPx), Qt::AlignLeft, teacherInfo);
    currentY += 4 * mmToPx;
    painter.drawText(QRectF(startX, currentY, pageWidth, 4 * mmToPx), Qt::AlignLeft, semesterInfo);
    currentY += 5 * mmToPx;

    // 3. ГОРИЗОНТАЛЬНЫЕ ЗАГОЛОВКИ (РАСЧЕТ МИНИМАЛЬНОЙ ВЫСОТЫ)
    painter.setFont(QFont("Arial", 9, QFont::Bold));
    QStringList hHeaders = {"ID", "Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};

    int minHHeight = 0;
    for (int i = 0; i < hHeaders.size(); ++i) {
        double span = (i == 0) ? 1.0 : 2.0;
        double w = colW * span;
        // Вычисляем минимальный прямоугольник для текста
        QRectF bRect = painter.boundingRect(QRectF(0, 0, w - (4 * mmToPx), 1000),
                                            Qt::AlignCenter | Qt::TextWordWrap, hHeaders.at(i));
        minHHeight = qMax(minHHeight, qRound(bRect.height() + (2 * mmToPx)));
    }

    int tableTop = qRound(currentY);

    int curCol = 0;
    for (int i = 0; i < hHeaders.size(); ++i) {
        int span = (i == 0) ? 1 : 2;
        QRect r(getX(curCol), tableTop, getX(curCol + span) - getX(curCol), minHHeight);
        painter.drawRect(r);
        painter.drawText(r.adjusted(2, 1, -2, -1), Qt::AlignCenter | Qt::TextWordWrap, hHeaders.at(i));
        curCol += span;
    }

    // 4. СТРОКИ ДАННЫХ (Остаются высокими для 5 строк текста)
    //painter.setFont(QFont("Arial", 8.5));
    QFont rowFont("Arial");
    rowFont.setPointSizeF(8.5); // Устанавливаем дробный размер через специальный метод
    painter.setFont(rowFont);

    int rHeight = qRound(16.5 * mmToPx);
    QStringList vHeaders = {"1 пара", "2 пара", "3 пара", "4 пара", "5 пара"};
    int rowStartY = tableTop + minHHeight;

    for (int i = 0; i < 5; ++i) {
        int blockTop = rowStartY + (i * 2 * rHeight);

        // ВЕРТИКАЛЬНЫЙ ХИДЕР
        QRect vRect(getX(0), blockTop, getX(1) - getX(0), 2 * rHeight);
        painter.drawRect(vRect);
        if (i < vHeaders.size()) {
            painter.drawText(vRect.adjusted(2, 1, -2, -1), Qt::AlignCenter | Qt::TextWordWrap, vHeaders.at(i));
        }

        for (int sub = 0; sub < 2; ++sub) {
            int currentR = blockTop + (sub * rHeight);
            int absRowIdx = i * 2 + sub;

            for (int col = 1; col < colCount; ++col) {
                QRect cellR(getX(col), currentR, getX(col + 1) - getX(col), rHeight);
                painter.drawRect(cellR);

                if (absRowIdx < tableWidget->rowCount()) {
                    QTableWidgetItem *it = tableWidget->item(absRowIdx, col);
                    if (it) {
                        painter.drawText(cellR.adjusted(4, 0, -4, 0),
                                         Qt::AlignCenter | Qt::TextWordWrap, it->text());
                    }
                }
            }
        }
    }
    painter.end();
}

// void MainWindow::loadCheckboxState(int month, int year)
// {
//     QString dbPath = QString(PROJECT_PATH) + "/base/lesson_base.db";

//     if (db.isOpen()) {
//         db.close();
//     }

//     db.setDatabaseName(dbPath);

//     if (!db.open()) {
//         qDebug() << "❌ ОШИБКА: Не удалось открыть базу";
//         return;
//     }

//     qDebug() << "=== НАЧАЛО loadCheckboxState ===";

//     QStringList dayNames = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

//     for (int rowIndex = 0; rowIndex < ui->tableWidget_3->rowCount(); ++rowIndex) {
//         int id = rowIndex + 1;

//         for (int dayIndex = 0; dayIndex < 7; ++dayIndex) {
//             QWidget* cellWidget = ui->tableWidget_3->cellWidget(rowIndex, dayIndex);
//             if (cellWidget == nullptr) {
//                 qDebug() << "  ⚠️ cellWidget == nullptr в" << rowIndex << dayIndex;
//                 continue;
//             }

//             QCheckBox* checkbox = cellWidget->findChild<QCheckBox*>();
//             if (checkbox == nullptr) {
//                 qDebug() << "  ⚠️ checkbox == nullptr в" << rowIndex << dayIndex;
//                 continue;
//             }

//             QString dayName = dayNames[dayIndex];
//             QString selectQuery = QString("SELECT %1 FROM Save_check WHERE id = ?").arg(dayName);

//             QSqlQuery query(db);
//             if (!query.prepare(selectQuery)) {
//                 qDebug() << "  ❌ ОШИБКА подготовки:" << query.lastError().text();
//                 continue;
//             }

//             query.addBindValue(id);

//             if (!query.exec()) {
//                 qDebug() << "  ❌ ОШИБКА выполнения:" << query.lastError().text();
//                 continue;
//             }

//             if (query.next()) {
//                 int state = query.value(0).toInt();
//                 bool isChecked = (state == 1);

//                 // ⭐ Блокируем сигналы, чтобы не срабатывал connect при программной установке
//                 checkbox->blockSignals(true);
//                 checkbox->setChecked(isChecked);
//                 checkbox->blockSignals(false);

//                 // ⭐ СТИЛИЗУЙТЕ ВСЕ ЭЛЕМЕНТЫ В КОНТЕЙНЕРЕ
//                 QString bgColor = isChecked ? "#00FF00" : "red";
//                 QString styleSheet = QString(
//                                          "QWidget { background-color: %1; } "
//                                          "QCheckBox { background-color: %1; } "
//                                          "QLabel { background-color: %1; }"
//                                          ).arg(bgColor);

//                 cellWidget->setStyleSheet(styleSheet);

//                 qDebug() << "  Ячейка" << rowIndex << dayIndex
//                          << "| State:" << state
//                          << "| Checked:" << isChecked
//                          << "| Цвет:" << bgColor;
//             } else {
//                 qDebug() << "  ⚠️ Нет результатов для" << rowIndex << dayIndex;
//             }
//         }
//     }

//     // ⭐ ОБНОВИТЕ ВЕСЬ VIEWPORT
//     ui->tableWidget_3->viewport()->update();

//     qDebug() << "=== КОНЕЦ loadCheckboxState ===\n";
// }

void MainWindow::addLine()
{
    QTableWidget *table = getCurrentTable();
    if (!table) return;

    int row = table->currentRow();
    int total = table->rowCount();

    // Проверки: выделение, не заголовок (span) и не последние строки
    if (row < 0 || table->columnSpan(row, 0) > 1 || row >= total - 2) {
        return;
    }

    int insertPos = row + 1;
    table->insertRow(insertPos);

    // 1. Устанавливаем "Дисциплина" в ВЕРТИКАЛЬНЫЙ заголовок новой строки
    QTableWidgetItem *vHeaderItem = new QTableWidgetItem("Дисциплина");
    vHeaderItem->setTextAlignment(Qt::AlignLeft); // По центру
    table->setVerticalHeaderItem(insertPos, vHeaderItem);

    // 2. В саму ячейку таблицы ставим пустой элемент (чтобы она была готова к работе)
    table->setItem(insertPos, 0, new QTableWidgetItem(""));

    // 3. Автоматически расширяем заголовок под текст, если он длинный
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // 4. Переносим фокус на новую ячейку
    table->setCurrentCell(insertPos, 0);
}

void MainWindow::removeLine() {
    QTableWidget *table = getCurrentTable();
    if (!table) return;

    int row = table->currentRow();
    int total = table->rowCount();

    // Те же правила безопасности: нельзя удалять заголовки и последние 3 строки
    if (row < 0 || table->columnSpan(row, 0) > 1 || row >= total - 2) {
        return;
    }

    table->removeRow(row);
}

QTableWidget* MainWindow::getCurrentTable()
{
    int index = ui->tabWidget_3->currentIndex();

    if (index == 0) return ui->tableWidget_4;
    if (index == 1) return ui->tableWidget_10;

    return nullptr; // Если есть другие вкладки без этих таблиц
}

void MainWindow::ClickedLeftButton2(int row, int column)
{
    if (editingEnabled) return;

    QScreen *screen = QGuiApplication::primaryScreen();
    rsc5 = new AddLine(this);
    rsc5->setWindowTitle("Введите данные для ячейки");
    rsc5->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, rsc5->size(), screen->geometry()));

    // 1. Запускаем диалог. Внутри AddLine вы должны присваивать значения глобальным переменным
    if (rsc5->exec() != QDialog::Accepted) {
        rsc5->deleteLater();
        return;
    }

    // 2. Определяем строку (используем ту, по которой кликнули)
    if (row < 0) row = ui->tableWidget_4->currentRow();

    qDebug() << "Result" << row;
    if (row < 0) return; // Защита от пустого выделения

    // 3. Вставка данных с принудительной конвертацией в строку
    // Используем QString::number(), если переменные — int или double
    ui->tableWidget_4->setItem(row, 0, new QTableWidgetItem(QString::number(lections)));
    ui->tableWidget_4->setItem(row, 1, new QTableWidgetItem(QString::number(lab_rabs)));
    ui->tableWidget_4->setItem(row, 2, new QTableWidgetItem(QString::number(pract_rabs)));
    ui->tableWidget_4->setItem(row, 3, new QTableWidgetItem(QString::number(mrs)));
    ui->tableWidget_4->setItem(row, 4, new QTableWidgetItem(QString::number(individ_zad)));
    ui->tableWidget_4->setItem(row, 5, new QTableWidgetItem(QString::number(kurs_rab)));
    ui->tableWidget_4->setItem(row, 6, new QTableWidgetItem(QString::number(konsult)));
    ui->tableWidget_4->setItem(row, 7, new QTableWidgetItem(QString::number(recenz)));
    ui->tableWidget_4->setItem(row, 8, new QTableWidgetItem(QString::number(zachet)));
    ui->tableWidget_4->setItem(row, 9, new QTableWidgetItem(QString::number(exam)));
    ui->tableWidget_4->setItem(row, 10, new QTableWidgetItem(QString::number(ucheb_pract)));
    ui->tableWidget_4->setItem(row, 11, new QTableWidgetItem(QString::number(zavod_pract)));
    ui->tableWidget_4->setItem(row, 12, new QTableWidgetItem(QString::number(diplom_proect)));
    ui->tableWidget_4->setItem(row, 13, new QTableWidgetItem(QString::number(gek_gak)));
    ui->tableWidget_4->setItem(row, 14, new QTableWidgetItem(QString::number(aspirant)));
    ui->tableWidget_4->setItem(row, 15, new QTableWidgetItem(QString::number(sum_semestr)));
    ui->tableWidget_4->setItem(row, 16, new QTableWidgetItem(QString::number(sum_year)));

    // 1. Устанавливаем текст в вертикальный заголовок для конкретной строки
    QTableWidgetItem *vHeaderItem = new QTableWidgetItem(subject_name);
    vHeaderItem->setTextAlignment(Qt::AlignLeft);
    ui->tableWidget_4->setVerticalHeaderItem(row, vHeaderItem);
    ui->tableWidget_4->verticalHeader()->setSectionResizeMode(row, QHeaderView::ResizeToContents);

    rsc5->deleteLater();
}

void MainWindow::syncCalendar()
{   
    qDebug() << "--- Sync Вызван ---";
    qDebug() << "Обновление для:" << yearSpinBox->value() << monthList->currentIndex() + 1;

    // 1. ЗАМОРАЖИВАЕМ ВИДЖЕТЫ
    monthList->blockSignals(true);
    yearSpinBox->blockSignals(true);

    // 2. БЕРЕМ ЧИСТЫЕ ДАННЫЕ
    int y = yearSpinBox->value();
    int m = monthList->currentIndex() + 1;

    // 3. ПОЛНАЯ ПЕРЕРИСОВКА
    rebuildCalendarGrid(y, m); // Сначала строим сетку
    loadMonth(y, m);           // Потом накладываем правки из базы

    // 4. РАЗМОРАЖИВАЕМ
    monthList->blockSignals(false);
    yearSpinBox->blockSignals(false);

    qDebug() << "Синхронизация завершена для:" << lastLoadedMonth << lastLoadedYear;
}

void MainWindow::rebuildCalendarGrid(int year, int month)
{
    if (!isLoaded) return;

    // 1. Математика конкретного месяца
    QDate firstDay(year, month, 1);
    int daysInMonth = firstDay.daysInMonth();
    int offset = firstDay.dayOfWeek() - 1; // Пн=0, Вс=6

    // Получаем текущую системную дату для сравнения
    QDate today = QDate::currentDate();

    // 2. Условия "Красной зоны"
    bool isSummer = (month == 7 || month == 8);
    bool isSept2027Plus = (year > 2027) || (year == 2027 && month >= 9);
    bool forceRed = isSummer || isSept2027Plus;

    // 3. Единый цикл прохода по всем 42 ячейкам
    for (int i = 0; i < 42; ++i) {
        int row = i / 7;
        int col = i % 7;
        QWidget* cell = ui->tableWidget_3->cellWidget(row, col);

        if (!cell) continue;

        QLabel* label = cell->findChild<QLabel*>();
        QCheckBox* cb = cell->findChild<QCheckBox*>();

        // ПРОВЕРКА: входит ли индекс в диапазон текущего месяца
        if (i >= offset && i < (offset + daysInMonth)) {
            int dayNum = i - offset + 1;

            // Проверяем, является ли эта ячейка сегодняшним днем
            bool isToday = (dayNum == today.day() &&
                            month == today.month() &&
                            year == today.year());

            cell->show();
            cell->setVisible(true);
            cell->setEnabled(true);

            if (label) {
                label->setText(QString::number(dayNum));
                label->show();
            }

            // Логика состояния: 1 - будни, 0 - выходные/красный
            int state = forceRed ? 0 : (col < 5 ? 1 : 0);

            // ПРИМЕНЯЕМ ВИЗУАЛИЗАЦИЮ (передаем параметр isToday)
            // Убедитесь, что сигнатура updateCellVisuals обновлена на (int, int, bool)
            updateCellVisuals(i, state, isToday);

        } else {
            // ЯЧЕЙКА ВНЕ МЕСЯЦА
            cell->hide();
            if (label) label->clear();
            if (cb) {
                cb->blockSignals(true);
                cb->setChecked(false);
                cb->hide();
                cb->blockSignals(false);
            }
            cell->setStyleSheet("background-color: transparent; border: none;");
        }
    }

    // Выводим уведомление о переключении месяца/года
    QLocale russian(QLocale::Russian);
    QString monthName = russian.monthName(month).toUpper();
    message_action("Календарь", QString("Отображен период: %1 %2").arg(monthName).arg(year));

    ui->tableWidget_3->viewport()->update();
}

void MainWindow::onCheckBoxToggled(Qt::CheckState state)
{
    // Узнаем, какой чекбокс нажат
    QCheckBox* cb = qobject_cast<QCheckBox*>(sender());
    if (!cb) return;

    // Находим контейнер (ячейку), в которой лежит этот чекбокс
    QWidget* cell = cb->parentWidget();
    if (!cell) return;

    // Определяем цвет: если галочка стоит (2) -> зеленый, если снята (0) -> красный
    QString color = (state == Qt::Checked) ? "#00FF00" : "red";

    // Обновляем стиль только этой конкретной ячейки
    cell->setStyleSheet(
        QString("QWidget { background-color: %1; border: 1px solid gray; } ").arg(color) +
        "QLabel, QCheckBox { background: transparent; border: none; }"
        );
}

void MainWindow::message_action(QString summary_s, QString body_s)
{
    QDBusInterface notifyInterface("org.freedesktop.Notifications",
                                   "/org/freedesktop/Notifications",
                                   "org.freedesktop.Notifications",
                                   QDBusConnection::sessionBus());

    if (!notifyInterface.isValid())
    {
        qWarning() << "Failed to connect to notifications service";
        return;
    }

    // Parameters for Notify method:
    // app_name, replaces_id, app_icon, summary, body, actions, hints, expire_timeout
    QString appName = QCoreApplication::applicationName();
    uint replacesId = 0; // 0 means no existing notification to replace
    QString appIcon = "/home/elf/undoredo/data/IM_96x96.png"; // icon name or path
    QString summary = summary_s;
    QString body = body_s;
    QStringList actions; // empty list means no actions
    QVariantMap hints;   // additional hints (empty here)
    int expireTimeout = 5000; // milliseconds

    QDBusReply<uint> reply = notifyInterface.call("Notify",
                                                  appName,
                                                  replacesId,
                                                  appIcon,
                                                  summary,
                                                  body,
                                                  actions,
                                                  hints,
                                                  expireTimeout);

    if (reply.isValid()) {
        uint notificationId = reply.value();
        qDebug() << "Notification sent with ID:" << notificationId;
    } else {
        qWarning() << "Failed to send notification:" << reply.error().message();
        return;
    }
}
