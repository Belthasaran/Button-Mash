#ifndef MASHEDITORWINDOW_H
#define MASHEDITORWINDOW_H

#include "masheditormodel.h"
#include "masheditorcanvas.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>

class MashEditorWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MashEditorWindow(const QString &skinsRoot, QWidget *parent = nullptr);

    bool openSkinDir(const QString &skinDir, MashEditMode initialMode);
    bool createNewSkin(const QString &folderName, MashEditMode initialMode);

private slots:
    void onSave();
    void onSaveAs();
    void onLoad();
    void onImportAsset();
    void onProperties();
    void onModeChanged(int idx);
    void onCanvasSelection(const QString &key);
    void onCanvasModelChanged();
    void onViewModeToggled(bool preview);
    void onIndividualPreviewToggled();
    void onAllPreviewToggled(int state);
    void onInspectorChanged();
    void onBrowseImage();
    void onZoomIn();
    void onZoomOut();
    void onGridToggled(bool on);
    void onSnapToggled(bool on);
    void onSubSkinChanged(int idx);
    void onToolPointer();
    void onToolPlace();
    void onToolSetBackground();
    void onPaletteItemClicked(QListWidgetItem *item);
    void refreshAssetList();

private:
    void buildUi();
    void syncCanvasFromModel();
    void syncModelFromCanvas();
    void refreshInspector();
    void applyInspectorToModel();
    void syncAllCheckboxFromIndividuals();
    bool confirmDiscardIfDirty();
    QSet<QString> activePreviewSet() const;
    void setInspectorFieldsForBackground();
    void setInspectorFieldsForButton(const QString &name);

    QString m_skinsRoot;
    MashEditorModel m_model;
    MashEditorCanvas *m_canvas = nullptr;
    QWidget *m_inspector = nullptr;
    QFormLayout *m_inspectorForm = nullptr;
    QComboBox *m_modeCombo = nullptr;
    QComboBox *m_subSkinCombo = nullptr;
    QLineEdit *m_inspName = nullptr;
    QLineEdit *m_inspImage = nullptr;
    QSpinBox *m_inspX = nullptr;
    QSpinBox *m_inspY = nullptr;
    QSpinBox *m_inspW = nullptr;
    QSpinBox *m_inspH = nullptr;
    QLineEdit *m_inspColor = nullptr;
    QPushButton *m_inspBrowse = nullptr;
    QCheckBox *m_chkAll = nullptr;
    QListWidget *m_assetList = nullptr;
    QPushButton *m_ptrBtn = nullptr;
    QPushButton *m_placeBtn = nullptr;
    QPushButton *m_bgBtn = nullptr;
    QMap<QString, QCheckBox *> m_previewChks;
    bool m_updatingInspector = false;
    bool m_updatingPreview = false;
};

#endif
