#ifndef VERTIKALHEADERVIEW_H
#define VERTIKALHEADERVIEW_H

#include <QHeaderView>
#include <QPainter>

class VertikalheaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit VertikalheaderView(Qt::Orientation orientation, QWidget *parent = nullptr);

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
    QSize sizeHint() const override;
};







#endif // VERTIKALHEADERVIEW_H
