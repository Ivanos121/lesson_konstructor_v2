#include "mergedheader.h"
#include <QPainter>
#include <QMouseEvent>
#include <QAbstractItemModel>
#include <QTimer>
#include <QScrollBar>
#include <QTableWidget>
#include <QStylePainter>
#include <QApplication>

MergedHeader::MergedHeader(Qt::Orientation orientation, QWidget *parent)
    : QHeaderView(orientation, parent),
    m_tableWidget(nullptr),
    m_lastScrollValue(0),
    m_orientation(orientation),
    m_hoveredSection(-1)
{
    mergeRanges.clear();
    headerTexts.clear();

    viewport()->setAttribute(Qt::WA_OpaquePaintEvent, false);
    viewport()->setAttribute(Qt::WA_NoSystemBackground, false);
    setUpdatesEnabled(true);

    if (orientation == Qt::Vertical) {
        QTimer::singleShot(100, this, [this]() {
            QWidget *p = parentWidget();
            while (p) {
                QTableWidget *table = qobject_cast<QTableWidget*>(p);
                if (table) {
                    m_tableWidget = table;

                    // Подключаемся к скроллбару таблицы
                    connect(table->verticalScrollBar(), &QScrollBar::valueChanged,
                            this, &MergedHeader::onVerticalScrolled,
                            Qt::DirectConnection);

                    // Синхронизируем начальное положение
                    onVerticalScrolled(table->verticalScrollBar()->value());
                    break;
                }
                p = p->parentWidget();
            }
        });
    }
}

int MergedHeader::findMergeRange(int logicalIndex, int &outStart, int &outSpan) const
{
    for (int i = 0; i < mergeRanges.size(); ++i) {
        const auto &range = mergeRanges[i];
        if (logicalIndex >= range.first && logicalIndex < range.first + range.second) {
            outStart = range.first;
            outSpan = range.second;
            return i;
        }
    }
    outStart = -1;
    outSpan = 1;
    return -1;
}

void MergedHeader::updateMergedRange(int logicalIndex)
{
    int mergeStart = -1;
    int mergeSpan = -1;
    int mergeIndex = findMergeRange(logicalIndex, mergeStart, mergeSpan);

    if (mergeIndex != -1) {
        int startPos = sectionPosition(mergeStart);
        int totalSize = 0;

        for (int j = mergeStart; j < mergeStart + mergeSpan; ++j) {
            totalSize += sectionSize(j);
        }

        QRect updateRect;
        if (m_orientation == Qt::Horizontal) {
            updateRect = QRect(startPos, 0, totalSize, height());
        } else {
            // Для вертикального заголовка учитываем скроллинг
            int scrollOffset = m_tableWidget ? m_tableWidget->verticalScrollBar()->value() : 0;
            updateRect = QRect(0, startPos - scrollOffset, width(), totalSize);
        }

        viewport()->update(updateRect);
    } else {
        int pos = sectionPosition(logicalIndex);
        int size = sectionSize(logicalIndex);

        QRect updateRect;
        if (m_orientation == Qt::Horizontal) {
            updateRect = QRect(pos, 0, size, height());
        } else {
            // Для вертикального заголовка учитываем скроллинг
            int scrollOffset = m_tableWidget ? m_tableWidget->verticalScrollBar()->value() : 0;
            updateRect = QRect(0, pos - scrollOffset, width(), size);
        }

        viewport()->update(updateRect);
    }
}

void MergedHeader::setMergeRanges(const QList<QPair<int, int>> &ranges)
{
    mergeRanges = ranges;
    viewport()->update();
}

void MergedHeader::setHeaderTexts(const QStringList &texts)
{
    headerTexts = texts;
    viewport()->update();
}

void MergedHeader::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    int mergeStart = -1;
    int mergeSpan = -1;
    int mergeIndex = findMergeRange(logicalIndex, mergeStart, mergeSpan);

    // Если это не первая секция объединения, не рисуем
    if (mergeIndex != -1 && logicalIndex != mergeStart) {
        return;
    }

    QRect drawRect = rect;

    // Если это первая секция объединения
    if (mergeIndex != -1 && logicalIndex == mergeStart) {
        int totalSize = 0;
        int scrollOffset = m_tableWidget ? m_tableWidget->verticalScrollBar()->value() : 0;
        int startPos = sectionPosition(mergeStart);

        for (int j = mergeStart; j < mergeStart + mergeSpan; ++j) {
            totalSize += sectionSize(j);
        }

        if (m_orientation == Qt::Horizontal) {
            drawRect = QRect(startPos, 0, totalSize, height());
        } else {
            int adjustedPos = startPos - scrollOffset;
            drawRect = QRect(0, adjustedPos, width(), totalSize);
        }
    }

    // Проверяем наведение
    bool isHovered = false;
    if (mergeIndex != -1) {
        for (int j = mergeStart; j < mergeStart + mergeSpan; ++j)
        {
            if (m_hoveredSection == j)
            {
                isHovered = true;
                break;
            }
        }
    } else {
        isHovered = (m_hoveredSection == logicalIndex);
    }

    // Рисуем фон
    if (isHovered)
    {
        painter->fillRect(drawRect, palette().color(QPalette::Highlight));
    }
    else
    {
        painter->fillRect(drawRect, palette().color(QPalette::Button));
    }

    // Рисуем границы
    painter->setPen(palette().color(QPalette::Mid));
    painter->drawRect(drawRect.adjusted(0, 0, -1, -1));

    // Рисуем текст
    painter->setPen(isHovered ? palette().color(QPalette::HighlightedText)
                              : palette().color(QPalette::ButtonText));
    painter->setFont(font());

    QString text;
    if (mergeIndex != -1 && mergeIndex < headerTexts.size())
    {
        text = headerTexts.at(mergeIndex);
    }
    else if (mergeIndex == -1)
    {
        text = model()->headerData(logicalIndex, m_orientation, Qt::DisplayRole).toString();
    }
    else
    {
        text = QString("Объединение %1").arg(mergeIndex + 1);
    }

    QRect textRect = drawRect.adjusted(4, 4, -4, -4);
    painter->drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, text);
}

void MergedHeader::mouseMoveEvent(QMouseEvent *event)
{
    int section = logicalIndexAt(event->pos());

    if (section != m_hoveredSection)
    {
        if (m_hoveredSection != -1)
        {
            updateMergedRange(m_hoveredSection);
        }

        m_hoveredSection = section;

        if (m_hoveredSection != -1)
        {
            updateMergedRange(m_hoveredSection);
        }
    }

    QHeaderView::mouseMoveEvent(event);
}

void MergedHeader::leaveEvent(QEvent *event)
{
    if (m_hoveredSection != -1)
    {
        updateMergedRange(m_hoveredSection);
        m_hoveredSection = -1;
    }

    QHeaderView::leaveEvent(event);
}

void MergedHeader::paintEvent(QPaintEvent *event)
{
    if (m_orientation == Qt::Vertical && m_tableWidget) {
        QStylePainter painter(viewport());
        painter.setClipping(true);  // Включаем clipping для корректной отрисовки

        painter.fillRect(viewport()->rect(), palette().color(QPalette::Base));

        int scrollOffset = m_tableWidget->verticalScrollBar()->value();
        int sectionCount = count();
        int viewportHeight = viewport()->height();

        // Рисуем ВСЕ секции, но с учётом скроллинга
        for (int i = 0; i < sectionCount; ++i)
        {
            if (isSectionHidden(i)) {
                continue;
            }

            int pos = sectionPosition(i);
            int size = sectionSize(i);
            int adjustedPos = pos - scrollOffset;

            // Проверяем, пересекается ли секция с видимой областью
            if (adjustedPos + size < 0 || adjustedPos > viewportHeight)
            {
                continue;  // Секция полностью за пределами viewport
            }

            QRect sectionRect(0, adjustedPos, width(), size);
            paintSection(&painter, sectionRect, i);
        }
    } else {
        QHeaderView::paintEvent(event);
    }
}

void MergedHeader::onVerticalScrolled(int value)
{
    if (m_orientation == Qt::Vertical)
    {
        m_lastScrollValue = value;
        // Обновляем весь viewport при скроллировании
        viewport()->update();
    }
}

void MergedHeader::onTableScrolled()
{
    viewport()->update();
}

void MergedHeader::wheelEvent(QWheelEvent *event)
{
    // Передаём событие скроллирования таблице
    if (m_tableWidget)
    {
        QApplication::sendEvent(m_tableWidget->verticalScrollBar(), event);
    }
    else
    {
        QHeaderView::wheelEvent(event);
    }
}

void MergedHeader::mousePressEvent(QMouseEvent *event)
{
    // Не позволяем заголовку обрабатывать скроллирование
    QHeaderView::mousePressEvent(event);
}

QStringList MergedHeader::getHeaderTexts() const
{
    return headerTexts;
}

void MergedHeader::setHeaderTextAt(int index, const QString &text)
{
    if (index >= 0 && index < headerTexts.size())
    {
        headerTexts[index] = text;
        this->viewport()->update(); // Перерисовать хидер
    }
}
