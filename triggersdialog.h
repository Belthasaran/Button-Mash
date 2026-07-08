#ifndef TRIGGERSDIALOG_H
#define TRIGGERSDIALOG_H

#include "inputtriggers.h"
#include <QDialog>

namespace Ui {
class TriggersDialog;
}

class TriggersDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TriggersDialog(InputTriggersEngine *engine, QWidget *parent = nullptr);
    ~TriggersDialog() override;

private slots:
    void on_buttonBox_accepted();
    void on_addExtraButton_clicked();
    void on_removeExtraButton_clicked();

private:
    void loadFromEngine();
    void applyToEngine();
    void addBindingRow(const TriggerBinding &b);

    Ui::TriggersDialog *ui;
    InputTriggersEngine *m_engine;
};

#endif
