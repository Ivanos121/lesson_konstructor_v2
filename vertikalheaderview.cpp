#include "vertikalheaderview.h"

VertikalheaderView::VertikalheaderView(Qt::Orientation orientation, QWidget *parent): QHeaderView(orientation, parent)
{

}


QSize VertikalheaderView::sizeHint() const
{
    // Получаем базовый размер (ширину колонок и стандартную высоту шрифта)
    QSize s = QHeaderView::sizeHint();

    if (orientation() == Qt::Horizontal)
    {
        QFontMetrics fm(font());
        int maxTextWidth = 0;

        // Проходим по всем секциям и ищем самый длинный текст
        for (int i = 0; i < count(); ++i)
        {
            QString text = model()->headerData(i, orientation(), Qt::DisplayRole).toString();
            // В новых версиях Qt используйте horizontalAdvance, в старых - width
            int textWidth = fm.horizontalAdvance(text);
            if (textWidth > maxTextWidth)
            {
                maxTextWidth = textWidth;
            }
        }

        // Устанавливаем высоту заголовка: длина текста + небольшой отступ (например, 16px)
        s.setHeight(maxTextWidth + 16);
    }
    return s;
}

void VertikalheaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();

    // 1. Рисуем стандартный фон и границы ячейки
    QStyleOptionHeader opt;
    initStyleOption(&opt);
    opt.rect = rect;
    opt.section = logicalIndex;
    opt.text = ""; // Убираем текст, чтобы он не рисовался стандартно
    style()->drawControl(QStyle::CE_Header, &opt, painter, this);

    // 2. Настраиваем поворот текста
    QString text = model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString();
    painter->rotate(-90);

    // После поворота на -90 градусов:
    // Новая координата X = -старая координата Y (bottom)
    // Новая координата Y = старая координата X (left)
    QRect rotatedRect(-rect.bottom(), rect.left(), rect.height(), rect.width());

    painter->drawText(rotatedRect, Qt::AlignCenter, text);

    painter->restore();
}
