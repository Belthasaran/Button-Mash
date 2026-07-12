#ifndef MIRRORTARGETSDIALOG_H
#define MIRRORTARGETSDIALOG_H

#include "inputmirrormanager.h"
#include <QDialog>

namespace Ui {
class MirrorTargetsDialog;
}

class MirrorTargetsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MirrorTargetsDialog(InputMirrorManager *manager, bool browserServerRunning,
                                 QWidget *parent = nullptr);
    ~MirrorTargetsDialog() override;

private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void on_clearButton_clicked();
    void on_buttonBox_accepted();
    void on_browseBinButton_clicked();
    void on_browseFullTextButton_clicked();
    void on_browseSyncTextButton_clicked();
    void on_browseLastNButton_clicked();
    void on_browserSourcePortSpin_valueChanged(int value);
    void on_copyMainUrlButton_clicked();
    void on_copyStackedUrlButton_clicked();
    void on_copyPianoUrlButton_clicked();

private:
    void loadFromManager();
    void applyToManager();
    void refreshSessionLabel();
    void refreshBrowserSourceUrls();
    void copyUrlToClipboard(const QString &url);

    Ui::MirrorTargetsDialog *ui;
    InputMirrorManager *m_manager;
    bool m_browserServerRunning;
};

#endif
