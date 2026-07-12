#ifndef SKINSELECTOR_H
#define SKINSELECTOR_H

#include "inputdisplay.h"
#include "skinparser.h"
#include "telnetconnection.h"
#include "inputdecoder.h"
#include "inputsourceselector.h"
#include "inputmirrormanager.h"
#include "inputtriggers.h"
#include "browsersourceserver.h"

#include <QListView>
#include <QMainWindow>
#include <QSettings>
#include <QStandardItemModel>

namespace Ui {
class SkinSelector;
}

class SkinSelector : public QMainWindow
{
    Q_OBJECT

public:
    explicit SkinSelector(QWidget *parent = nullptr);
    ~SkinSelector();

private slots:
    void on_startButton_clicked();

    void on_pianoCheckBox_stateChanged(int arg1);

    void on_skinListView_clicked(const QModelIndex &index);

    void    onTimerTimeout();

    void on_skinPathButton_clicked();

    void on_subSkinListView_clicked(const QModelIndex &index);

    void    onDisplayClosed();

    void on_changeSourceButton_clicked();

    void on_shareMirrorCheckBox_toggled(bool checked);

    void on_mirrorTargetsButton_clicked();

    void on_inputTriggersCheckBox_toggled(bool checked);

    void on_configureTriggersButton_clicked();

    void on_skinEditorButton_clicked();

    void on_presetSwitchButton_clicked();

    void on_presetNewButton_clicked();

    void on_presetOverwriteButton_clicked();

    void on_presetDeleteButton_clicked();

    void on_debugConsoleCheckBox_toggled(bool checked);

    void on_legacyDisplayScalingCheckBox_toggled(bool checked);

private:
    Ui::SkinSelector *ui;
    QStandardItemModel* listModel;
    QStandardItemModel* pianoModel;
    QStandardItemModel* subSkinModel;
    InputDisplay*       display;
    InputProvider*      inputProvider;
    QTimer              timer;
    RegularSkin         currentSkin;
    InputSourceSelector* inputSelector;
    InputMirrorManager*  mirrorManager;
    InputTriggersEngine* triggersEngine;
    BrowserSourceServer* browserSourceServer;

    void    setSkinPath(QString path);
    void    addSkinPath(QString path);
    void    setPreviewScene(const RegularSkin &skin);
    void    saveSkinStarted();
    void    restoreLastSkin();
    void    refreshPresetCombo();
    void    flushActiveSettingsToStore();
    void    applyActiveSettingsFromStore();
    QString currentPresetName() const;

};

#endif // SKINSELECTOR_H
