#ifndef MERGEDHEADER_H
#define MERGEDHEADER_H

#include <QHeaderView>
#include <QList>
#include <QPair>
#include <QString>

class QTableWidget;

class MergedHeader : public QHeaderView
{
    Q_OBJECT

public:
    explicit MergedHeader(Qt::Orientation orientation, QWidget *parent = nullptr);
    void setMergeRanges(const QList<QPair<int, int>> &ranges);
    void setHeaderTexts(const QStringList &texts);
    QStringList getHeaderTexts() const;
    void setHeaderTextAt(int index, const QString &text);

protected:
    void paintEvent(QPaintEvent *event) override;
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void onVerticalScrolled(int value);
    void onTableScrolled();

private:
    int findMergeRange(int logicalIndex, int &outStart, int &outSpan) const;
    void updateMergedRange(int logicalIndex);
    int logicalIndexAtPos(int pos) const;  // ДОБАВЬТЕ ЭТУ СТРОКУ

    QList<QPair<int, int>> mergeRanges;
    QStringList headerTexts;
    QTableWidget *m_tableWidget;
    int m_lastScrollValue;
    Qt::Orientation m_orientation;
    int m_hoveredSection;
};

#endif // MERGEDHEADER_H
