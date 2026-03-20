/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSet>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "saveBase",
        "",
        "onItemChanged",
        "NewTable",
        "closeEvent",
        "QCloseEvent*",
        "event",
        "openLesson",
        "saveCheckboxState",
        "month",
        "year",
        "medgevertikalcells",
        "QTableWidget*",
        "tableWidget",
        "row",
        "col",
        "nonEmptyParts",
        "verticalControlString",
        "QSet<std::pair<int,int>>&",
        "mergedPositions",
        "determineTargetCell",
        "std::pair<int,int>",
        "column",
        "combo2",
        "combo3",
        "rowCount",
        "medgehorizontalcells",
        "aboutLessons",
        "deleteLesson",
        "ClickedLeftButton",
        "mergeCells",
        "splitCells",
        "onPageChanged2",
        "index",
        "printLessonDialog",
        "showPrintPreview",
        "onMonthChanged",
        "onTabChanged",
        "newUser"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'saveBase'
        QtMocHelpers::SlotData<bool()>(1, 2, QMC::AccessProtected, QMetaType::Bool),
        // Slot 'onItemChanged'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'NewTable'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'closeEvent'
        QtMocHelpers::SlotData<void(QCloseEvent *)>(5, 2, QMC::AccessProtected, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Slot 'openLesson'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'saveCheckboxState'
        QtMocHelpers::SlotData<void(int, int)>(9, 2, QMC::AccessProtected, QMetaType::Void, {{
            { QMetaType::Int, 10 }, { QMetaType::Int, 11 },
        }}),
        // Slot 'medgevertikalcells'
        QtMocHelpers::SlotData<bool(QTableWidget *, int, int, const QStringList &, const QString &, QSet<QPair<int,int> > &)>(12, 2, QMC::AccessProtected, QMetaType::Bool, {{
            { 0x80000000 | 13, 14 }, { QMetaType::Int, 15 }, { QMetaType::Int, 16 }, { QMetaType::QStringList, 17 },
            { QMetaType::QString, 18 }, { 0x80000000 | 19, 20 },
        }}),
        // Slot 'determineTargetCell'
        QtMocHelpers::SlotData<QPair<int,int>(int, int, const QString &, const QString &, int)>(21, 2, QMC::AccessProtected, 0x80000000 | 22, {{
            { QMetaType::Int, 15 }, { QMetaType::Int, 23 }, { QMetaType::QString, 24 }, { QMetaType::QString, 25 },
            { QMetaType::Int, 26 },
        }}),
        // Slot 'medgehorizontalcells'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'aboutLessons'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'deleteLesson'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'ClickedLeftButton'
        QtMocHelpers::SlotData<void(int, int)>(30, 2, QMC::AccessProtected, QMetaType::Void, {{
            { QMetaType::Int, 15 }, { QMetaType::Int, 23 },
        }}),
        // Slot 'mergeCells'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'splitCells'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'onPageChanged2'
        QtMocHelpers::SlotData<void(int)>(33, 2, QMC::AccessProtected, QMetaType::Void, {{
            { QMetaType::Int, 34 },
        }}),
        // Slot 'printLessonDialog'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'showPrintPreview'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessProtected, QMetaType::Void),
        // Slot 'onMonthChanged'
        QtMocHelpers::SlotData<void(int)>(37, 2, QMC::AccessProtected, QMetaType::Void, {{
            { QMetaType::Int, 34 },
        }}),
        // Slot 'onTabChanged'
        QtMocHelpers::SlotData<void(int)>(38, 2, QMC::AccessProtected, QMetaType::Void, {{
            { QMetaType::Int, 34 },
        }}),
        // Slot 'newUser'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessProtected, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = _t->saveBase();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->onItemChanged(); break;
        case 2: _t->NewTable(); break;
        case 3: _t->closeEvent((*reinterpret_cast<std::add_pointer_t<QCloseEvent*>>(_a[1]))); break;
        case 4: _t->openLesson(); break;
        case 5: _t->saveCheckboxState((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 6: { bool _r = _t->medgevertikalcells((*reinterpret_cast<std::add_pointer_t<QTableWidget*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<QSet<std::pair<int,int>>&>>(_a[6])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { std::pair<int,int> _r = _t->determineTargetCell((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[5])));
            if (_a[0]) *reinterpret_cast<std::pair<int,int>*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->medgehorizontalcells(); break;
        case 9: _t->aboutLessons(); break;
        case 10: _t->deleteLesson(); break;
        case 11: _t->ClickedLeftButton((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 12: _t->mergeCells(); break;
        case 13: _t->splitCells(); break;
        case 14: _t->onPageChanged2((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->printLessonDialog(); break;
        case 16: _t->showPrintPreview(); break;
        case 17: _t->onMonthChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 18: _t->onTabChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 19: _t->newUser(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QTableWidget* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}
QT_WARNING_POP
