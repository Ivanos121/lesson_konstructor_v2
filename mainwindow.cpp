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
#include <QShortcut>
#include <QClipboard>
#include <QRegExp>

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

    //connect(lessonList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onPageChanged2);
    connect(ui->newTable, &QAction::triggered, this, &MainWindow::NewTable);
    //connect(ui->saveDoc, &QAction::triggered, this, &MainWindow::saveBase);
    //connect(ui->loadDoc, &QAction::triggered, this, &MainWindow::loadBase);
    connect(ui->closeApp, &QAction::triggered, this, &MainWindow::close);
    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &MainWindow::onItemChanged);
    //connect(ui->openLesson, &QAction::triggered, this, &MainWindow::openLesson);
    //connect(ui->medgeHorizontalCells, &QAction::triggered, this, &MainWindow::medgehorizontalcells);
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

    connect(ui->saveDoc, &QAction::triggered, this, [this]() {
        // 1. Получаем индекс текущей вкладки
        int currentIndex = ui->tabWidget->currentIndex();

        QTableWidget* activeTable = nullptr;
        QString dbName;

        // 2. Определяем таблицу и имя БД по индексу вкладки (самый надежный способ)
        if (currentIndex == 0) { // Предположим, 0 - Весна
            activeTable = ui->tableWidget;
            dbName = "lessons_spring_semester";
        }
        else if (currentIndex == 1) { // 1 - Осень
            activeTable = ui->tableWidget_2;
            dbName = "lessons_autumn_semester";
        }

        // 3. Вызываем сохранение, если таблица определена
        if (activeTable) {
            saveBase(activeTable, dbName);
            // Дополнительно можно вывести уведомление, что именно сохранено
            qDebug() << "Сохранение завершено для таблицы:" << dbName;
        }
    });

    connect(ui->loadDoc, &QAction::triggered, this, [this]() {
        // 1. Определяем, какой виджет сейчас открыт в tabWidget
        QWidget* current = ui->tabWidget->currentWidget();
        QTableWidget* activeTable = qobject_cast<QTableWidget*>(current);

        if (activeTable) {
            // 2. Определяем имя таблицы в БД для текущего виджета
            QString dbName = (activeTable == ui->tableWidget)
                                 ? "lessons_spring_semester"
                                 : "lessons_autumn_semester";

            // 3. Вызываем универсальную функцию загрузки
            loadBase(activeTable, dbName);

            message_action("Загрузка", QString("Данные обновлены для: %1").arg(dbName));
        }
    });

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

    QDate currentDate2 = QDate::currentDate();
    int day2 = currentDate2.day();
    int month2 = currentDate2.month();
    int year2 = currentDate2.year();

    QStringList monthNames = {
        "", "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
        "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"
    };

    ui->label->setText(monthNames[month2].toLower() + ", " +
                                   QString::number(day2) + ", " +
                                   QString::number(year2));

    ui->label->setStyleSheet("padding-right: 10px;");

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

    ui->tableWidget_2->setRowCount(10);
    ui->tableWidget_2->setColumnCount(13);
    ui->tableWidget_2->setWordWrap(true);

    for (int r = 0; r < ui->tableWidget_2->rowCount(); ++r)
    {
        for (int c = 0; c < ui->tableWidget_2->columnCount(); ++c)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget_2->setItem(r, c, item);
        }
    }

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
        {8, 2}   // строки 4-5
    };

    QStringList verticalTexts2 = {
        "8.30 - 10.05",
        "10.25 - 12.00",
        "12.30 - 14.05",
        "14.20 - 15.55",
        "16.05 - 17.40"
    };


    verticalHeader2->setMergeRanges(verticalMerges2);
    verticalHeader2->setHeaderTexts(verticalTexts2);

    verticalHeader->verticalScrollBar()->setEnabled(false);
    ui->tableWidget_2->verticalHeader()->setFixedWidth(100);

    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->hideColumn(0);

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
    connect(ui->tableWidget_2, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);

    // Создаем горячую клавишу Ctrl+S
    QShortcut *saveShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);

    // При нажатии определяем активную таблицу и вызываем сохранение
    connect(saveShortcut, &QShortcut::activated, this, [this]() {
        // Находим активную таблицу на текущей вкладке
        QWidget* currentTab = ui->tabWidget->currentWidget();
        QTableWidget* activeTable = (currentTab) ? currentTab->findChild<QTableWidget*>() : nullptr;

        if (activeTable) {
            QString dbName = (activeTable == ui->tableWidget)
            ? "lessons_spring_semester"
            : "lessons_autumn_semester";

            if (saveBase(activeTable, dbName)) {
                ui->statusbar->showMessage("Сохранено (Ctrl+S): " + dbName, 3000);
            }
        }
    });

    QShortcut *deleteShortcut = new QShortcut(QKeySequence::Delete, this);
    connect(deleteShortcut, &QShortcut::activated, this, &MainWindow::deleteLesson);

    applyRowHeights(ui->tableWidget);
    applyRowHeights(ui->tableWidget_2);

    loadBase(ui->tableWidget, "lessons_spring_semester");
    loadBase(ui->tableWidget_2, "lessons_autumn_semester");
}

MainWindow::~MainWindow()
{
    tables.clear();
    delete ui;
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    // Определяем, какая таблица вызвала меню
    QTableWidget *table = qobject_cast<QTableWidget*>(sender());
    if (!table) return;

    // Создаем объект меню
    QMenu menu(this);
    menu.setStyleSheet("QMenu { border: 1px solid black; padding: 5px; }"); // Опциональный стиль

    // Добавляем действия и привязываем их к вашим существующим функциям
    QAction *actMerge = menu.addAction("Объединить выделенное");
    connect(actMerge, &QAction::triggered, this, &MainWindow::mergeCells);

    QAction *actSplit = menu.addAction("Разъединить ячейки");
    connect(actSplit, &QAction::triggered, this, &MainWindow::splitCells);

    menu.addSeparator(); // Разделительная линия

    QAction *actDelete = menu.addAction("Очистить содержимое");
    connect(actDelete, &QAction::triggered, this, &MainWindow::deleteLesson);

    // Внутри showContextMenu
    QAction *actCopy = menu.addAction("Копировать (Ctrl+C)");
    connect(actCopy, &QAction::triggered, this, &MainWindow::copySelection);

    QAction *actPaste = menu.addAction("Вставить (Ctrl+V)");
    connect(actPaste, &QAction::triggered, this, &MainWindow::pasteSelection);

    //new QShortcut(QKeySequence::Copy, this, SLOT(copySelection()));
    //new QShortcut(QKeySequence::Paste, this, SLOT(pasteSelection()));

    // Показываем меню в глобальных координатах экрана
    menu.exec(table->viewport()->mapToGlobal(pos));
}

void MainWindow::copySelection()
{
    QWidget* currentTab = ui->tabWidget->currentWidget();
    QTableWidget* table = (currentTab) ? currentTab->findChild<QTableWidget*>() : nullptr;
    if (!table) return;

    QList<QTableWidgetSelectionRange> ranges = table->selectedRanges();
    if (ranges.isEmpty()) return;

    const QTableWidgetSelectionRange &range = ranges.first();
    QString fullCopyText;

    for (int r = range.topRow(); r <= range.bottomRow(); ++r) {
        QStringList rowData;
        for (int c = range.leftColumn(); c <= range.rightColumn(); ++c) {
            QTableWidgetItem *it = table->item(r, c);
            if (it) {
                QString text = it->text().trimmed();
                // Если в тексте есть перенос строки, оборачиваем в кавычки для "умного парсера"
                if (text.contains('\n')) {
                    // Экранируем существующие кавычки (заменяем " на "")
                    text.replace("\"", "\"\"");
                    text = "\"" + text + "\"";
                }
                rowData << text;
            } else {
                rowData << "";
            }
        }
        fullCopyText += rowData.join("\t") + "\n";
    }
    QGuiApplication::clipboard()->setText(fullCopyText);
}

void MainWindow::pasteSelection()
{
    QWidget* currentTab = ui->tabWidget->currentWidget();
    QTableWidget* table = (currentTab) ? currentTab->findChild<QTableWidget*>() : nullptr;
    if (!table) return;

    QString data = QGuiApplication::clipboard()->text();
    if (data.isEmpty()) return;

    // Теперь \n гарантированно разделяет только СТРОКИ ТАБЛИЦЫ
    QStringList rows = data.split('\n', Qt::SkipEmptyParts);
    int startRow = table->currentRow();
    int startCol = table->currentColumn();

    table->blockSignals(true);

    for (int i = 0; i < rows.size(); ++i) {
        QStringList cols = rows[i].split('\t');
        for (int j = 0; j < cols.size(); ++j) {
            int r = startRow + i;
            int c = startCol + j;

            if (r < table->rowCount() && c < table->columnCount()) {
                // Пишем только в видимые ячейки (начало Span)
                if (table->rowSpan(r, c) >= 1 && table->columnSpan(r, c) >= 1) {
                    if (!table->item(r, c)) table->setItem(r, c, new QTableWidgetItem());

                    // Вставляем текст (он пришел одной строкой без \n)
                    table->item(r, c)->setText(cols[j].trimmed());
                    table->item(r, c)->setTextAlignment(Qt::AlignCenter);
                }
            }
        }
    }

    table->blockSignals(false);
    applyRowHeights(table);
    table->viewport()->update();
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

void MainWindow::splitCells()
{
    // 1. Динамически находим таблицу на активной вкладке
    QTableWidget *table = qobject_cast<QTableWidget*>(sender());

    if (!table) {
        QWidget* currentTab = ui->tabWidget->currentWidget();
        if (currentTab) {
            table = currentTab->findChild<QTableWidget*>();
        }
    }

    if (!table) {
        QMessageBox::warning(this, "Ошибка", "Не удалось найти активную таблицу");
        return;
    }

    // 2. Получаем выделенные диапазоны в найденной таблице
    QList<QTableWidgetSelectionRange> ranges = table->selectedRanges();

    if (ranges.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выделите область для разъединения");
        return;
    }

    int splitCount = 0;
    table->blockSignals(true); // Блокируем сигналы для скорости и предотвращения спама в БД

    // Используем std::as_const для предотвращения detach (как обсуждали ранее)
    for (const QTableWidgetSelectionRange &range : std::as_const(ranges))
    {
        for (int r = range.topRow(); r <= range.bottomRow(); ++r) {
            for (int c = range.leftColumn(); c <= range.rightColumn(); ++c)
            {
                // Проверяем Span в текущей таблице
                int rSpan = table->rowSpan(r, c);
                int cSpan = table->columnSpan(r, c);

                // Если ячейка является началом объединения
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
    // 1. ОПРЕДЕЛЯЕМ АКТИВНУЮ ТАБЛИЦУ
    QTableWidget *table = qobject_cast<QTableWidget*>(sender());

    if (!table) {
        // Берем текущий виджет вкладки
        QWidget* currentTab = ui->tabWidget->currentWidget();
        if (currentTab) {
            // Ищем QTableWidget внутри этого виджета
            table = currentTab->findChild<QTableWidget*>();
        }
    }

    if (!table) {
        QMessageBox::warning(this, "Ошибка", "Не удалось найти таблицу на активной вкладке");
        return;
    }

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

    // Проверка допустимых размеров (1x2, 2x1 или 2x2)
    bool isHorizontal = (rCount == 1 && cCount == 2);
    bool isVertical = (rCount == 2 && cCount == 1);
    bool isSquare = (rCount == 2 && cCount == 2);

    if (!isHorizontal && !isVertical && !isSquare) {
        QMessageBox::warning(this, "Ошибка",
                             "Выделите ровно 2 смежные ячейки или блок 2x2");
        return;
    }

    // Блокируем сигналы, чтобы не спамить в базу/логи при изменении текста
    table->blockSignals(true);

    // 2. Проверка, не поглощены ли уже ячейки другими объединениями
    for (int r = top; r < top + rCount; ++r) {
        for (int c = left; c < left + cCount; ++c) {
            if (table->rowSpan(r, c) > 1 || table->columnSpan(r, c) > 1) {
                QMessageBox::warning(this, "Ошибка", "В области есть уже объединенные ячейки");
                table->blockSignals(false);
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

    // 4. Выполнение объединения в целевой таблице
    table->setSpan(top, left, rCount, cCount);

    // 5. Установка объединенного текста в главную ячейку
    if (QTableWidgetItem *mainItem = table->item(top, left)) {
        QString separator = (isHorizontal) ? " " : "\n";
        mainItem->setText(texts.join(separator));
        mainItem->setTextAlignment(Qt::AlignCenter);
    }

    table->blockSignals(false);
    table->viewport()->update();

    // Уведомление
    QString desc = isSquare ? "блок 2x2" : (isHorizontal ? "горизонтально" : "вертикально");
    message_action("Успех", QString("Ячейки объединены (%1)").arg(desc));
}

void MainWindow::ClickedLeftButton(int row, int column)
{
    if (editingEnabled)
        return;

    // ОПРЕДЕЛЯЕМ, КАКАЯ ТАБЛИЦА ВЫЗВАЛА ФУНКЦИЮ
    QTableWidget *table = qobject_cast<QTableWidget*>(sender());
    if (!table) return;

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

    // ЛОГИКА РАЗЪЕДИНЕНИЯ
    if (mergedLefts_.contains(key))
    {
        table->setSpan(row, column, 1, 1);
        if (column + 1 < table->columnCount() && !table->item(row, column+1))
            table->setItem(row, column+1, new QTableWidgetItem(""));

        mergedLefts_.remove(key);
        rsc2->deleteLater();
        return;
    }

    if (rsc2->exec() != QDialog::Accepted)
    {
        rsc2->deleteLater();
        return;
    }

    QString comboChoice2 = rsc2->ui->comboBox_2->currentText().trimmed();
    QString comboChoice3 = rsc2->ui->comboBox_3->currentText().trimmed();
    QString combinedText = rsc2->text11 + "\n" + rsc2->text22 + "\n" + rsc2->text33 + "\n" + rsc2->text44;

    // Логика чётности строки (недели)
    bool isRowOdd = (row % 2 == 1);
    int targetRow = row;
    bool shouldMergeHorizontal = false;
    bool shouldMergeVertical = false;

    // СЦЕНАРИИ
    if (comboChoice2 == "none" && (comboChoice3.contains("Верхняя") || comboChoice3.contains("Нижняя")))
    {
        shouldMergeHorizontal = true;
        if (comboChoice3.contains("Верхняя")) {
            targetRow = row;
        } else if (comboChoice3.contains("Нижняя")) {
            if (isRowOdd) targetRow = (row + 1 < table->rowCount()) ? row + 1 : row - 1;
            else targetRow = (row - 1 >= 0) ? row - 1 : row + 1;
        }
    }
    else if (comboChoice2.contains("1-я подгруппа раз в неделю") && comboChoice3.contains("none")) {
        targetRow = (row % 2 == 0) ? row : row - 1;
        shouldMergeVertical = true;
    }

    if (targetRow < 0 || targetRow >= table->rowCount()) {
        rsc2->deleteLater();
        return;
    }

    if (!table->item(targetRow, column))
        table->setItem(targetRow, column, new QTableWidgetItem());

    table->item(targetRow, column)->setText(combinedText);
    table->item(targetRow, column)->setTextAlignment(Qt::AlignCenter);

    // ОБЪЕДИНЕНИЯ
    if (shouldMergeHorizontal && column + 1 < table->columnCount())
    {
        if (!table->item(targetRow, column + 1))
            table->setItem(targetRow, column + 1, new QTableWidgetItem());
        table->setSpan(targetRow, column, 1, 2);
        mergedLefts_.insert(QPair<int,int>(targetRow, column));
        table->item(targetRow, column + 1)->setText("");
    }

    if (shouldMergeVertical && row + 1 < table->rowCount())
    {
        if (!table->item(row + 1, column))
            table->setItem(row + 1, column, new QTableWidgetItem());
        table->setSpan(targetRow, column, 2, 1);
        mergedLefts_.insert(QPair<int,int>(targetRow, column));
        table->item(row + 1, column)->setText("");
    }

    table->resizeRowsToContents();
    rsc2->deleteLater();

    QTimer::singleShot(0, this, [table, this]() {
        table->clearSelection();
        table->setCurrentItem(nullptr);
        table->clearFocus();
        this->setFocus();
    });
    applyRowHeights(table);
}

void MainWindow::deleteLesson()
{
    // 1. Динамически определяем, какая таблица сейчас активна
    QTableWidget *table = qobject_cast<QTableWidget*>(sender());

    if (!table) {
        QWidget* currentTab = ui->tabWidget->currentWidget();
        if (currentTab) {
            table = currentTab->findChild<QTableWidget*>();
        }
    }

    if (!table) return;

    // 2. Получаем список всех выделенных ячеек именно из этой таблицы
    QList<QTableWidgetItem*> items = table->selectedItems();
    if (items.isEmpty()) return;

    // 3. Блокируем сигналы текущей таблицы для производительности
    table->blockSignals(true);

    // 4. Проходим циклом по всем выделенным элементам и очищаем их
    // Используем std::as_const для предотвращения лишнего копирования списка
    for (QTableWidgetItem* it : std::as_const(items))
    {
        if (it)
        {
            it->setText(QString());
        }
    }

    // 5. Разблокируем сигналы и обновляем область отображения
    table->blockSignals(false);
    table->viewport()->update();

    // Можно вывести уведомление в статус-бар о количестве удаленных ячеек
    ui->statusbar->showMessage(QString("Очищено ячеек: %1").arg(items.size()), 3000);
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
        // Сохраняем первый семестр
        bool springOk = saveBase(ui->tableWidget, "lessons_spring_semester");

        // Сохраняем второй семестр
        bool autumnOk = saveBase(ui->tableWidget_2, "lessons_autumn_semester");

        if (springOk && autumnOk)
        {
            event->accept();
        }
        else
        {
            // Если что-то не сохранилось, можно прервать выход
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить данные одного из семестров.");
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

bool MainWindow::saveBase(QTableWidget *table, const QString &dbTable)
{
    if (!table) return false;

    QSqlQuery query;
    bool success = true;

    // 1. Очищаем целевую таблицу в БД
    if (!query.exec(QString("DELETE FROM %1").arg(dbTable))) {
        qDebug() << "Ошибка очистки БД:" << query.lastError().text();
        return false;
    }

    for (int row = 0; row < table->rowCount(); ++row)
    {
        for (int col = 1; col < table->columnCount(); ++col)
        {
            // Пропускаем ячейки, которые поглощены объединением (Span)
            if (row > 0 && table->rowSpan(row - 1, col) > 1) continue;
            if (col > 0 && table->columnSpan(row, col - 1) > 1) continue;

            QTableWidgetItem* item = table->item(row, col);
            if (!item) continue;

            QString cellText = item->text().trimmed();
            QString finalContent;

            if (cellText.isEmpty()) {
                finalContent = "[EMPTY]";
            }
            else {
                // 1. Извлекаем текстовые строки (первые 4 строки контента)
                QStringList lines = cellText.split('\n', Qt::KeepEmptyParts);
                QStringList firstFour;
                for (int i = 0; i < qMin(4, lines.count()); ++i) {
                    firstFour.append(lines[i].trimmed());
                }
                // Дополняем до 4 строк, если текста меньше, чтобы не сместить ключи
                while(firstFour.size() < 4) firstFour.append("");

                QString parts = firstFour.join("\n");

                // 2. Определение ключа позиции positionKey (5-я строка в БД)
                int rSpan = table->rowSpan(row, col);
                int cSpan = table->columnSpan(row, col);
                bool isRowOdd = (row % 2 != 0);
                bool isColOdd = (col % 2 != 0);

                QString posKey = "none";
                if (rSpan == 1 && cSpan == 1) {
                    if (!isRowOdd && isColOdd)       posKey = "up_left";
                    else if (!isRowOdd && !isColOdd)  posKey = "up_right";
                    else if (isRowOdd && isColOdd)    posKey = "down_left";
                    else if (isRowOdd && !isColOdd)   posKey = "down_right";
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

                // 3. Определение технического типа lessonType (6-я строка в БД)
                QString lessonType = "none";
                if (cellText.contains("лекция", Qt::CaseInsensitive)) lessonType = "lection";
                else if (cellText.contains("лабораторное", Qt::CaseInsensitive)) lessonType = "lab_rab";
                else if (cellText.contains("практическое", Qt::CaseInsensitive)) lessonType = "pract_rab";

                // 4. Сборка финальной строки: 4 строки текста + Ключ + Тип
                finalContent = parts + "\n" + posKey + "\n" + lessonType;
            }

            // 5. Выполнение запроса в переданную таблицу БД
            query.prepare(QString("INSERT INTO %1 (row_num, col_num, content) VALUES (?, ?, ?)").arg(dbTable));
            query.addBindValue(row);
            query.addBindValue(col);
            query.addBindValue(finalContent);

            if (!query.exec()) {
                success = false;
                qDebug() << "Ошибка вставки в" << dbTable << ":" << query.lastError().text();
            }
        }
    }

    if (success)
    {
        message_action("Успех", QString("Данные таблицы %1 сохранены").arg(dbTable));
    }

    return success;
}

void MainWindow::loadBase(QTableWidget *table, const QString &dbTable)
{
    if (!table) return;

    // Блокируем сигналы именно той таблицы, которую загружаем
    table->blockSignals(true);
    table->clearSpans();

    // Чистим текст и ставим центрирование во всей целевой таблице
    for (int r = 0; r < table->rowCount(); ++r) {
        for (int c = 1; c < table->columnCount(); ++c) {
            if (auto item = table->item(r, c)) {
                item->setText("");
                item->setTextAlignment(Qt::AlignCenter);
            }
        }
    }

    // Используем dbTable в SQL-запросе
    QSqlQuery query(QString("SELECT row_num, col_num, content FROM %1").arg(dbTable), db);

    while (query.next())
    {
        int r = query.value(0).toInt();
        int c = query.value(1).toInt();
        QString fullContent = query.value(2).toString();

        if (r < table->rowCount() && c < table->columnCount())
        {
            QTableWidgetItem *item = table->item(r, c);
            if (!item) continue;

            item->setTextAlignment(Qt::AlignCenter);

            if (fullContent == "[EMPTY]") {
                item->setText("");
            }
            else {
                QStringList parts = fullContent.split('\n');
                // Отображаем первые 4 строки
                QStringList displayLines;
                for (int i = 0; i < qMin(4, parts.size()); ++i) {
                    displayLines << parts.at(i);
                }
                item->setText(displayLines.join("\n"));

                // Восстановление объединений по ключам в целевой таблице
                if (fullContent.contains("left_right_up") || fullContent.contains("left_right_down")) {
                    table->setSpan(r, c, 1, 2);
                }
                else if (fullContent.contains("up_down_left") || fullContent.contains("up_down_right")) {
                    table->setSpan(r, c, 2, 1);
                }
                else if (fullContent.contains("full_cell")) {
                    table->setSpan(r, c, 2, 2);
                }
            }
        }
    }

    applyRowHeights(table);

    table->blockSignals(false);
    table->viewport()->update();

    qDebug() << "Загружена таблица:" << dbTable;
}

void MainWindow::CreateNewTable()
{
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
}

void MainWindow::applyRowHeights(QTableWidget *table)
{
    if (!table) return;

    // 1. Устанавливаем режим Stretch для вертикального заголовка
    // Это заставит строки ВСЕГДА занимать всё свободное место в viewport
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 2. Устанавливаем минимальную высоту секции, чтобы при малом окне текст не накладывался
    table->verticalHeader()->setMinimumSectionSize(60);

    // 3. Отключаем ручное изменение размера пользователем (опционально)
    table->verticalHeader()->setSectionsClickable(false);

    // 4. Настраиваем горизонтальный заголовок, чтобы колонки тоже вписывались
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 5. Убираем полосы прокрутки, если хотим "чистый" вид без скролла
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    table->viewport()->update();
}

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

void MainWindow::ClickedLeftButton2(int row)
{
    if (editingEnabled) return;

    QScreen *screen = QGuiApplication::primaryScreen();
    rsc5 = new AddLine(this);
    rsc5->setWindowTitle("Введите данные для ячейки");
    rsc5->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, rsc5->size(), screen->geometry()));

    // 1. Запускаем диалог. Внутри AddLine вы должны присваивать значения глобальным переменным
    if (rsc5->exec() != QDialog::Accepted)
    {
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
