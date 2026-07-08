#ifndef MASHEDITORCANVAS_H
#define MASHEDITORCANVAS_H

#include "masheditormodel.h"
#include <QGraphicsView>
#include <QSet>
#include <QString>

class MashEditorCanvas : public QGraphicsView
{
    Q_OBJECT
public:
    enum class Tool { Pointer, PlaceButton };
    enum class ViewMode { Edit, Preview };

    explicit MashEditorCanvas(QWidget *parent = nullptr);

    void setModel(MashEditorModel *model);
    void rebuildScene();
    void setTool(Tool t) { m_tool = t; }
    QString placeAsset() const { return m_placeAsset; }
    void setPlaceAsset(const QString &fileName) { m_placeAsset = fileName; }
    void setViewMode(ViewMode m);
    void setPreviewButtons(const QSet<QString> &names);
    void setGridVisible(bool v);
    void zoomBy(double factor);

    QString selectedObjectKey() const { return m_selectedKey; }

signals:
    void selectionChanged(const QString &objectKey);
    void modelChanged();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void clearSelection();
    void selectItem(QGraphicsItem *item, const QString &key);
    bool isLocked(const QString &key) const;
    void setLocked(const QString &key, bool locked);
    QGraphicsItem *itemAtScenePos(const QPointF &sp) const;
    void updateItemVisuals();
    void placeNewButtonAt(const QPointF &sp);
    void bringToFront(const QString &key);

    MashEditorModel *m_model = nullptr;
    Tool m_tool = Tool::Pointer;
    QString m_placeAsset;
    ViewMode m_viewMode = ViewMode::Edit;
    QSet<QString> m_previewButtons;
    bool m_gridVisible = true;
    QString m_selectedKey;
    QGraphicsItem *m_selectedItem = nullptr;
    QMap<QString, QGraphicsItem *> m_itemMap;

    bool m_dragging = false;
    bool m_resizing = false;
    int m_resizeHandle = -1;
    QPointF m_dragStartScene;
    QPointF m_itemStartPos;
    QRectF m_itemStartRect;
};

#endif
