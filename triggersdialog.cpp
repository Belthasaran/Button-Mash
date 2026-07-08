#include "triggersdialog.h"
#include "ui_triggersdialog.h"
#include "snesbitorder.h"

#include <QAbstractItemView>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QTableWidgetItem>

namespace {

QString snesName(InputProvider::SNESButton b)
{
    switch (b) {
    case InputProvider::Up: return QStringLiteral("Up");
    case InputProvider::Down: return QStringLiteral("Down");
    case InputProvider::Left: return QStringLiteral("Left");
    case InputProvider::Right: return QStringLiteral("Right");
    case InputProvider::A: return QStringLiteral("A");
    case InputProvider::B: return QStringLiteral("B");
    case InputProvider::X: return QStringLiteral("X");
    case InputProvider::Y: return QStringLiteral("Y");
    case InputProvider::L: return QStringLiteral("L");
    case InputProvider::R: return QStringLiteral("R");
    case InputProvider::Start: return QStringLiteral("Start");
    case InputProvider::Select: return QStringLiteral("Select");
    }
    return QStringLiteral("?");
}

InputProvider::SNESButton snesFromName(const QString &n)
{
    if (n == QLatin1String("Up")) return InputProvider::Up;
    if (n == QLatin1String("Down")) return InputProvider::Down;
    if (n == QLatin1String("Left")) return InputProvider::Left;
    if (n == QLatin1String("Right")) return InputProvider::Right;
    if (n == QLatin1String("A")) return InputProvider::A;
    if (n == QLatin1String("B")) return InputProvider::B;
    if (n == QLatin1String("X")) return InputProvider::X;
    if (n == QLatin1String("Y")) return InputProvider::Y;
    if (n == QLatin1String("L")) return InputProvider::L;
    if (n == QLatin1String("R")) return InputProvider::R;
    if (n == QLatin1String("Start")) return InputProvider::Start;
    if (n == QLatin1String("Select")) return InputProvider::Select;
    return InputProvider::B;
}

} // namespace

TriggersDialog::TriggersDialog(InputTriggersEngine *engine, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TriggersDialog)
    , m_engine(engine)
{
    ui->setupUi(this);
    ui->bindingsTable->horizontalHeader()->setStretchLastSection(true);
    ui->bindingsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->bindingsTable->setSelectionMode(QAbstractItemView::SingleSelection);
#ifndef Q_OS_LINUX
    ui->linuxGamepadCheck->setVisible(false);
#endif
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &TriggersDialog::on_buttonBox_accepted);
    loadFromEngine();
}

TriggersDialog::~TriggersDialog()
{
    delete ui;
}

void TriggersDialog::addBindingRow(const TriggerBinding &b)
{
    const int row = ui->bindingsTable->rowCount();
    ui->bindingsTable->insertRow(row);

    QComboBox *src = new QComboBox;
    src->addItem(QStringLiteral("Up"));
    src->addItem(QStringLiteral("Down"));
    src->addItem(QStringLiteral("Left"));
    src->addItem(QStringLiteral("Right"));
    src->addItem(QStringLiteral("A"));
    src->addItem(QStringLiteral("B"));
    src->addItem(QStringLiteral("X"));
    src->addItem(QStringLiteral("Y"));
    src->addItem(QStringLiteral("L"));
    src->addItem(QStringLiteral("R"));
    src->addItem(QStringLiteral("Start"));
    src->addItem(QStringLiteral("Select"));
    for (int i = 0; i < 8; ++i)
        src->addItem(QStringLiteral("Extra%1").arg(i + 1));

    if (b.sourceKind == TriggerBinding::SourceKind::Extra)
        src->setCurrentText(QStringLiteral("Extra%1").arg(b.extraIndex + 1));
    else
        src->setCurrentText(snesName(b.snesButton));
    ui->bindingsTable->setCellWidget(row, 0, src);

    QComboBox *act = new QComboBox;
    act->addItem(QStringLiteral("None"), QStringLiteral("none"));
    act->addItem(QStringLiteral("Keyboard"), QStringLiteral("keyboard"));
    act->addItem(QStringLiteral("Run program"), QStringLiteral("runProgram"));
    if (b.action == TriggerBinding::ActionKind::Keyboard)
        act->setCurrentIndex(1);
    else if (b.action == TriggerBinding::ActionKind::RunProgram)
        act->setCurrentIndex(2);
    else
        act->setCurrentIndex(0);
    ui->bindingsTable->setCellWidget(row, 1, act);

    ui->bindingsTable->setItem(row, 2, new QTableWidgetItem(b.keyCode ? QString::number(b.keyCode) : QString()));
    ui->bindingsTable->setItem(row, 3, new QTableWidgetItem(b.keyLabel));
    ui->bindingsTable->setItem(row, 4, new QTableWidgetItem(b.programPath));
    ui->bindingsTable->setItem(row, 5, new QTableWidgetItem(b.programArgs));
}

void TriggersDialog::loadFromEngine()
{
    const TriggerConfig c = m_engine->config();
    ui->windowTitlesEdit->setPlainText(c.windowTitleSubstrings.join(QLatin1Char('\n')));
    ui->windowClassesEdit->setPlainText(c.windowClassSubstrings.join(QLatin1Char('\n')));
    ui->linuxGamepadCheck->setChecked(c.linuxUinputGamepad);
    ui->bindingsTable->setRowCount(0);
    for (const TriggerBinding &b : c.bindings)
        addBindingRow(b);
}

void TriggersDialog::applyToEngine()
{
    TriggerConfig c = m_engine->config();
    c.windowTitleSubstrings = ui->windowTitlesEdit->toPlainText().split(QLatin1Char('\n'), Qt::SkipEmptyParts);
    c.windowClassSubstrings = ui->windowClassesEdit->toPlainText().split(QLatin1Char('\n'), Qt::SkipEmptyParts);
    c.linuxUinputGamepad = ui->linuxGamepadCheck->isChecked();
    c.bindings.clear();
    for (int row = 0; row < ui->bindingsTable->rowCount(); ++row) {
        TriggerBinding b;
        auto *src = qobject_cast<QComboBox *>(ui->bindingsTable->cellWidget(row, 0));
        auto *act = qobject_cast<QComboBox *>(ui->bindingsTable->cellWidget(row, 1));
        const QString srcText = src ? src->currentText() : QStringLiteral("B");
        if (srcText.startsWith(QLatin1String("Extra"))) {
            b.sourceKind = TriggerBinding::SourceKind::Extra;
            b.extraIndex = qMax(0, srcText.mid(5).toInt() - 1);
        } else {
            b.sourceKind = TriggerBinding::SourceKind::SnesButton;
            b.snesButton = snesFromName(srcText);
        }
        const QString actId = act ? act->currentData().toString() : QStringLiteral("none");
        if (actId == QLatin1String("keyboard"))
            b.action = TriggerBinding::ActionKind::Keyboard;
        else if (actId == QLatin1String("runProgram"))
            b.action = TriggerBinding::ActionKind::RunProgram;
        else
            b.action = TriggerBinding::ActionKind::None;

        b.keyCode = ui->bindingsTable->item(row, 2)
                        ? ui->bindingsTable->item(row, 2)->text().toInt()
                        : 0;
        b.keyLabel = ui->bindingsTable->item(row, 3)
                         ? ui->bindingsTable->item(row, 3)->text()
                         : QString();
        b.programPath = ui->bindingsTable->item(row, 4)
                            ? ui->bindingsTable->item(row, 4)->text()
                            : QString();
        b.programArgs = ui->bindingsTable->item(row, 5)
                            ? ui->bindingsTable->item(row, 5)->text()
                            : QString();
        c.bindings.append(b);
    }
    m_engine->setConfig(c);
}

void TriggersDialog::on_buttonBox_accepted()
{
    applyToEngine();
}

void TriggersDialog::on_addExtraButton_clicked()
{
    TriggerBinding b;
    b.sourceKind = TriggerBinding::SourceKind::Extra;
    b.extraIndex = 0;
    b.action = TriggerBinding::ActionKind::RunProgram;
    addBindingRow(b);
}

void TriggersDialog::on_removeExtraButton_clicked()
{
    const int row = ui->bindingsTable->currentRow();
    if (row >= 0)
        ui->bindingsTable->removeRow(row);
}
