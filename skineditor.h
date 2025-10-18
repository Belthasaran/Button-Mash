#ifndef SKINEDITOR_H
#define SKINEDITOR_H

#include <QDir>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QButtonGroup>
#include "inputprovider.h"
#include "skinparser.h"

namespace Ui {
class SkinEditor;
}

class SkinEditor : public QWidget
{
    Q_OBJECT

public:
    explicit SkinEditor(QWidget *parent = nullptr);
    ~SkinEditor();
    void    openSkin(const QDir& dir);
    void    openSkin(const QString xmlPath);

private slots:
    void on_openDir_clicked();

    void on_addButtonButton_clicked();

    void on_xSpinBox_valueChanged(int arg1);

    void on_ySpinBox_valueChanged(int arg1);

    void on_heightSpinBox_valueChanged(int arg1);

    void on_widthSpinBox_valueChanged(int arg1);

private:
    Ui::SkinEditor *ui;

    QMap<QString, QGraphicsPixmapItem*> mapItems;
    QMap<QString, QPixmap> mapPixmap;
    QMap<InputProvider::SNESButton, QString> mapButtonToText;
    QMap<QPushButton*, InputProvider::SNESButton> mapButtonToId;
    QButtonGroup*   buttonGroup;
    QDir            skinDir;
    RegularSkin     skin;
    QGraphicsScene* scene;
    QString         defaultSkinPath;
    QGraphicsPixmapItem* currentItem;
    QPixmap*             currentPixmap;

    void     onSnesButtonClicked(QAbstractButton* but);
    void     setCurrentButton(int id);

    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // SKINEDITOR_H
