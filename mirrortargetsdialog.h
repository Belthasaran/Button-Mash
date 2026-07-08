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
    explicit MirrorTargetsDialog(InputMirrorManager *manager, QWidget *parent = nullptr);
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

private:
    void loadFromManager();
    void applyToManager();
    void refreshSessionLabel();

    Ui::MirrorTargetsDialog *ui;
    InputMirrorManager *m_manager;
};

#endif
