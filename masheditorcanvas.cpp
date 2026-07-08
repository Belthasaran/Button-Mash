#include "masheditorcanvas.h"

#include <QContextMenuEvent>
#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QInputDialog>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QWheelEvent>

static constexpr int kHandleSize = 6;

static QGraphicsRectItem *makeHandle(QGraphicsItem *parent, int corner)
{
    auto *h = new QGraphicsRectItem(-kHandleSize / 2, -kHandleSize / 2, kHandleSize, kHandleSize, parent);
    h->setBrush(Qt::white);
    h->setPen(QPen(Qt::blue, 1));
    h->setData(0, corner);
    h->setData(1, QStringLiteral("handle"));
    h->setZValue(100);
    return h;
}

static void layoutHandles(QGraphicsItem *parent)
{
    const QRectF r = parent->boundingRect();
    const qreal positions[8][2] = {
        {r.left(), r.top()}, {r.left() + r.width() / 2, r.top()}, {r.right(), r.top()},
        {r.right(), r.top() + r.height() / 2},
        {r.right(), r.bottom()}, {r.left() + r.width() / 2, r.bottom()}, {r.left(), r.bottom()},
        {r.left(), r.top() + r.height() / 2},
    };
    int i = 0;
    for (QGraphicsItem *ch : parent->childItems()) {
        if (ch->data(1).toString() != QLatin1String("handle"))
            continue;
        if (i < 8) {
            ch->setPos(positions[i][0], positions[i][1]);
            ++i;
        }
    }
}

MashEditorCanvas::MashEditorCanvas(QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(new QGraphicsScene(this));
    setRenderHint(QPainter::Antialiasing, false);
    setDragMode(QGraphicsView::NoDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void MashEditorCanvas::setModel(MashEditorModel *model)
{
    m_model = model;
}

void MashEditorCanvas::setViewMode(ViewMode m)
{
    m_viewMode = m;
    updateItemVisuals();
}

void MashEditorCanvas::setPreviewButtons(const QSet<QString> &names)
{
    m_previewButtons = names;
    updateItemVisuals();
}

void MashEditorCanvas::setGridVisible(bool v)
{
    m_gridVisible = v;
    viewport()->update();
}

void MashEditorCanvas::zoomBy(double factor)
{
    scale(factor, factor);
}

bool MashEditorCanvas::isLocked(const QString &key) const
{
    if (!m_model)
        return false;
    const auto &map = (m_model->mode == MashEditMode::Regular) ? m_model->metadata.regularObjects
                                                                : m_model->metadata.pianoObjects;
    return map.value(key).locked;
}

void MashEditorCanvas::setLocked(const QString &key, bool locked)
{
    if (!m_model)
        return;
    auto &map = (m_model->mode == MashEditMode::Regular) ? m_model->metadata.regularObjects
                                                        : m_model->metadata.pianoObjects;
    ObjectMeta m = map.value(key);
    m.locked = locked;
    map[key] = m;
}

void MashEditorCanvas::clearSelection()
{
    m_selectedKey.clear();
    m_selectedItem = nullptr;
    emit selectionChanged(QString());
}

void MashEditorCanvas::selectItem(QGraphicsItem *item, const QString &key)
{
    m_selectedItem = item;
    m_selectedKey = key;
    emit selectionChanged(key);
    for (QGraphicsItem *gi : scene()->items()) {
        if (gi->data(1).toString() == QLatin1String("handle"))
            gi->setVisible(m_viewMode == ViewMode::Edit && gi->parentItem() == item);
    }
}

void MashEditorCanvas::rebuildScene()
{
    if (!m_model)
        return;
    scene()->clear();
    m_itemMap.clear();
    clearSelection();

    if (m_model->mode == MashEditMode::Regular) {
        const RegularSkin *v = m_model->activeRegularVariant();
        const QString dir = QFileInfo(v->file.isEmpty() ? m_model->skinDir : v->file).absolutePath();
        if (!v->background.isEmpty()) {
            QPixmap bg(dir + QLatin1Char('/') + v->background);
            if (bg.isNull())
                bg = QPixmap(240, 121);
            scene()->setSceneRect(0, 0, bg.width(), bg.height());
            auto *bgItem = scene()->addPixmap(bg);
            bgItem->setZValue(0);
            bgItem->setData(2, QStringLiteral("background"));
            m_itemMap[QStringLiteral("background")] = bgItem;
        } else {
            scene()->setSceneRect(0, 0, 240, 121);
        }
        for (auto it = v->buttons.constBegin(); it != v->buttons.constEnd(); ++it) {
            const RegularButtonSkin &b = it.value();
            QPixmap pix(dir + QLatin1Char('/') + b.image);
            if (pix.isNull())
                pix = QPixmap(b.width > 0 ? b.width : 16, b.height > 0 ? b.height : 16);
            auto *pi = new QGraphicsPixmapItem(pix.scaled(b.width, b.height));
            pi->setPos(b.x, b.y);
            pi->setZValue(10);
            const QString key = QStringLiteral("button:") + b.name;
            pi->setData(2, key);
            scene()->addItem(pi);
            m_itemMap[key] = pi;
            for (int c = 0; c < 8; ++c)
                makeHandle(pi, c);
            layoutHandles(pi);
        }
    } else {
        const PianoSkin &p = m_model->piano;
        scene()->setSceneRect(0, 0, p.width, p.height);
        auto *bg = scene()->addRect(0, 0, p.width, p.height, QPen(Qt::black), QBrush(p.bgcolor));
        bg->setZValue(0);
        bg->setData(2, QStringLiteral("mainarea"));
        m_itemMap[QStringLiteral("mainarea")] = bg;
        for (auto it = p.buttons.constBegin(); it != p.buttons.constEnd(); ++it) {
            const PianoButton &b = it.value();
            auto *ri = scene()->addRect(b.x, 0, b.width, p.height, QPen(Qt::darkGray), QBrush(b.color));
            ri->setZValue(10);
            const QString key = QStringLiteral("button:") + b.name;
            ri->setData(2, key);
            m_itemMap[key] = ri;
            for (int c = 0; c < 8; ++c)
                makeHandle(ri, c);
            layoutHandles(ri);
        }
    }
    updateItemVisuals();
}

void MashEditorCanvas::updateItemVisuals()
{
    for (auto it = m_itemMap.constBegin(); it != m_itemMap.constEnd(); ++it) {
        const QString key = it.key();
        if (key == QLatin1String("background") || key == QLatin1String("mainarea"))
            continue;
        const QString btnName = key.mid(7);
        QGraphicsItem *gi = it.value();
        const bool active = m_previewButtons.isEmpty() || m_previewButtons.contains(btnName);
        if (m_viewMode == ViewMode::Preview) {
            gi->setVisible(active);
            gi->setOpacity(1.0);
        } else {
            gi->setVisible(true);
            if (active)
                gi->setOpacity(1.0);
            else {
                if (auto *pi = qgraphicsitem_cast<QGraphicsPixmapItem *>(gi))
                    pi->setOpacity(0.15);
                else if (auto *ri = qgraphicsitem_cast<QGraphicsRectItem *>(gi))
                    ri->setBrush(QBrush(Qt::transparent));
            }
        }
        for (QGraphicsItem *ch : gi->childItems()) {
            if (ch->data(1).toString() == QLatin1String("handle"))
                ch->setVisible(m_viewMode == ViewMode::Edit && gi == m_selectedItem);
        }
    }
}

void MashEditorCanvas::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);
    if (!m_gridVisible || m_viewMode != ViewMode::Edit)
        return;
    const int step = m_model ? m_model->metadata.gridStep : 8;
    painter->setPen(QPen(QColor(255, 255, 255, 40), 1));
    const QRectF sr = sceneRect();
    for (qreal x = 0; x <= sr.width(); x += step)
        painter->drawLine(QPointF(x, sr.top()), QPointF(x, sr.bottom()));
    for (qreal y = 0; y <= sr.height(); y += step)
        painter->drawLine(QPointF(sr.left(), y), QPointF(sr.right(), y));
}

void MashEditorCanvas::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    Q_UNUSED(event);
    if (m_viewMode != ViewMode::Edit)
        return;
    QPainter p(viewport());
    p.setPen(QColor(80, 80, 80));
    p.fillRect(0, 0, viewport()->width(), 16, QColor(220, 220, 220));
    p.fillRect(0, 0, 16, viewport()->height(), QColor(220, 220, 220));
    const QRectF vr = mapToScene(viewport()->rect()).boundingRect();
    const int startX = int(vr.left());
    const int endX = int(vr.right());
    const int startY = int(vr.top());
    const int endY = int(vr.bottom());
    for (int x = startX - (startX % 10); x <= endX; x += 10) {
        const QPoint vp = mapFromScene(QPointF(x, vr.top()));
        if (vp.x() < 16)
            continue;
        p.drawLine(vp.x(), 0, vp.x(), 12);
        if (x % 50 == 0)
            p.drawText(vp.x() + 2, 12, QString::number(x));
    }
    for (int y = startY - (startY % 10); y <= endY; y += 10) {
        const QPoint vp = mapFromScene(QPointF(vr.left(), y));
        if (vp.y() < 16)
            continue;
        p.drawLine(0, vp.y(), 12, vp.y());
        if (y % 50 == 0)
            p.drawText(2, vp.y() - 2, QString::number(y));
    }
}

QGraphicsItem *MashEditorCanvas::itemAtScenePos(const QPointF &sp) const
{
    const auto items = scene()->items(sp);
    for (QGraphicsItem *gi : items) {
        if (gi->data(1).toString() == QLatin1String("handle"))
            return gi;
        if (gi->data(2).isValid())
            return gi;
    }
    return nullptr;
}

void MashEditorCanvas::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        zoomBy(event->angleDelta().y() > 0 ? 1.15 : 1.0 / 1.15);
        event->accept();
        return;
    }
    QGraphicsView::wheelEvent(event);
}

void MashEditorCanvas::placeNewButtonAt(const QPointF &sp)
{
    if (!m_model)
        return;
    int nx = int(sp.x());
    int ny = int(sp.y());
    if (m_model->metadata.snapToGrid) {
        const int g = m_model->metadata.gridStep;
        nx = (nx / g) * g;
        ny = (ny / g) * g;
    }
    if (m_model->mode == MashEditMode::Regular) {
        RegularSkin *v = m_model->activeRegularVariant();
        QString name = QInputDialog::getText(this, tr("Button name"), tr("SNES button name:"),
                                             QLineEdit::Normal, QStringLiteral("a"));
        name = name.trimmed().toLower();
        if (name.isEmpty())
            return;
        RegularButtonSkin b;
        b.name = name;
        b.image = m_placeAsset.isEmpty() ? QStringLiteral("button.png") : m_placeAsset;
        b.x = nx;
        b.y = ny;
        const QString dir = m_model->skinDir;
        QPixmap pix(dir + QLatin1Char('/') + b.image);
        b.width = pix.isNull() ? 16 : pix.width();
        b.height = pix.isNull() ? 16 : pix.height();
        v->buttons[name] = b;
        m_model->dirtyRegular = true;
    } else {
        QString name = QInputDialog::getText(this, tr("Strip name"), tr("Button label:"),
                                             QLineEdit::Normal, QStringLiteral("key"));
        name = name.trimmed();
        if (name.isEmpty())
            return;
        PianoButton b;
        b.name = name;
        b.x = nx;
        b.width = 20;
        b.color = Qt::gray;
        m_model->piano.buttons[name] = b;
        m_model->dirtyPiano = true;
    }
    rebuildScene();
    emit modelChanged();
}

void MashEditorCanvas::bringToFront(const QString &key)
{
    if (!m_model || !m_itemMap.contains(key))
        return;
    qreal maxZ = 10;
    for (QGraphicsItem *gi : m_itemMap)
        maxZ = qMax(maxZ, gi->zValue());
    m_itemMap[key]->setZValue(maxZ + 1);
    auto &map = (m_model->mode == MashEditMode::Regular) ? m_model->metadata.regularObjects
                                                          : m_model->metadata.pianoObjects;
    ObjectMeta om = map.value(key);
    om.zIndex = int(maxZ + 1);
    map[key] = om;
}

void MashEditorCanvas::mousePressEvent(QMouseEvent *event)
{
    if (!m_model || m_viewMode != ViewMode::Edit) {
        QGraphicsView::mousePressEvent(event);
        return;
    }
    const QPointF sp = mapToScene(event->pos());
    if (m_tool == Tool::PlaceButton && event->button() == Qt::LeftButton) {
        placeNewButtonAt(sp);
        return;
    }
    QGraphicsItem *hit = itemAtScenePos(sp);
    if (hit && hit->data(1).toString() == QLatin1String("handle")) {
        m_resizing = true;
        m_resizeHandle = hit->data(0).toInt();
        m_selectedItem = hit->parentItem();
        m_selectedKey = m_selectedItem->data(2).toString();
        m_dragStartScene = sp;
        m_itemStartRect = m_selectedItem->boundingRect();
        m_itemStartPos = m_selectedItem->pos();
        return;
    }
    if (hit && hit->data(2).isValid()) {
        selectItem(hit, hit->data(2).toString());
        if (!isLocked(m_selectedKey)) {
            m_dragging = true;
            m_dragStartScene = sp;
            m_itemStartPos = hit->pos();
        }
        return;
    }
    clearSelection();
    QGraphicsView::mousePressEvent(event);
}

void MashEditorCanvas::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_model || m_viewMode != ViewMode::Edit || !m_selectedItem || isLocked(m_selectedKey)) {
        QGraphicsView::mouseMoveEvent(event);
        return;
    }
    const QPointF sp = mapToScene(event->pos());
    if (m_dragging) {
        QPointF delta = sp - m_dragStartScene;
        int nx = int(m_itemStartPos.x() + delta.x());
        int ny = int(m_itemStartPos.y() + delta.y());
        if (m_model->metadata.snapToGrid) {
            const int g = m_model->metadata.gridStep;
            nx = (nx / g) * g;
            ny = (ny / g) * g;
        }
        m_selectedItem->setPos(nx, ny);
        emit modelChanged();
        return;
    }
    if (m_resizing) {
        QRectF r = m_itemStartRect;
        const QPointF delta = sp - m_dragStartScene;
        qreal newX = m_itemStartPos.x();
        qreal newY = m_itemStartPos.y();
        if (m_resizeHandle == 2 || m_resizeHandle == 3 || m_resizeHandle == 4)
            r.setWidth(qMax(1.0, r.width() + delta.x()));
        if (m_resizeHandle == 0 || m_resizeHandle == 6 || m_resizeHandle == 7) {
            r.setWidth(qMax(1.0, r.width() - delta.x()));
            newX = m_itemStartPos.x() + delta.x();
        }
        if (m_resizeHandle == 4 || m_resizeHandle == 5 || m_resizeHandle == 6)
            r.setHeight(qMax(1.0, r.height() + delta.y()));
        if (m_resizeHandle == 0 || m_resizeHandle == 1 || m_resizeHandle == 2) {
            r.setHeight(qMax(1.0, r.height() - delta.y()));
            newY = m_itemStartPos.y() + delta.y();
        }
        m_selectedItem->setPos(newX, newY);
        if (auto *pi = qgraphicsitem_cast<QGraphicsPixmapItem *>(m_selectedItem)) {
            QPixmap pm = pi->pixmap();
            pi->setPixmap(pm.scaled(int(r.width()), int(r.height())));
        } else if (auto *ri = qgraphicsitem_cast<QGraphicsRectItem *>(m_selectedItem)) {
            ri->setRect(0, 0, r.width(), r.height());
        }
        emit modelChanged();
        return;
    }
    QGraphicsView::mouseMoveEvent(event);
}

void MashEditorCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragging = false;
    m_resizing = false;
    m_resizeHandle = -1;
    QGraphicsView::mouseReleaseEvent(event);
}

void MashEditorCanvas::contextMenuEvent(QContextMenuEvent *event)
{
    const QPointF sp = mapToScene(event->pos());
    QGraphicsItem *hit = itemAtScenePos(sp);
    if (!hit || !hit->data(2).isValid())
        return;
    const QString key = hit->data(2).toString();
    QMenu menu(this);
    QAction *lockAct = menu.addAction(isLocked(key) ? tr("Unlock") : tr("Lock"));
    QAction *frontAct = menu.addAction(tr("Bring to front"));
    QAction *delAct = menu.addAction(tr("Delete"));
    QAction *chosen = menu.exec(event->globalPos());
    if (!chosen)
        return;
    if (chosen == lockAct) {
        setLocked(key, !isLocked(key));
    } else if (chosen == frontAct) {
        bringToFront(key);
    } else if (chosen == delAct) {
        const QString name = key.mid(7);
        if (m_model->mode == MashEditMode::Regular) {
            m_model->activeRegularVariant()->buttons.remove(name);
            m_model->dirtyRegular = true;
        } else {
            m_model->piano.buttons.remove(name);
            m_model->dirtyPiano = true;
        }
        rebuildScene();
        emit modelChanged();
    }
}
