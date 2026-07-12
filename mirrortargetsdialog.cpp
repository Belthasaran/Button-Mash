#include "mirrortargetsdialog.h"
#include "browsersourceserver.h"
#include "ui_mirrortargetsdialog.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QClipboard>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QCheckBox>
#include <QHeaderView>
#include <QSettings>
#include <QTableWidgetItem>

extern QSettings *globalSetting;

MirrorTargetsDialog::MirrorTargetsDialog(InputMirrorManager *manager, bool browserServerRunning,
                                         QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MirrorTargetsDialog)
    , m_manager(manager)
    , m_browserServerRunning(browserServerRunning)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &MirrorTargetsDialog::on_buttonBox_accepted);
    ui->targetsTable->horizontalHeader()->setStretchLastSection(true);
    ui->targetsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->targetsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    loadFromManager();
    refreshSessionLabel();
    refreshBrowserSourceUrls();

    ui->copyMainUrlButton->setEnabled(m_browserServerRunning);
    ui->copyStackedUrlButton->setEnabled(m_browserServerRunning);
    ui->copyPianoUrlButton->setEnabled(m_browserServerRunning);
}

MirrorTargetsDialog::~MirrorTargetsDialog()
{
    delete ui;
}

void MirrorTargetsDialog::refreshSessionLabel()
{
    QString sid = m_manager->logger()->state().sessionIdHex();
    if (sid.isEmpty())
        sid = QStringLiteral("(generated when logging starts)");
    ui->sessionLabel->setText(tr("Session ID: %1").arg(sid));
}

void MirrorTargetsDialog::refreshBrowserSourceUrls()
{
    const quint16 port = quint16(ui->browserSourcePortSpin->value());
    ui->browserSourceMainUrlEdit->setText(BrowserSourceServer::urlForView(QStringLiteral("main"), port));
    ui->browserSourceStackedUrlEdit->setText(BrowserSourceServer::urlForView(QStringLiteral("stacked"), port));
    ui->browserSourcePianoUrlEdit->setText(BrowserSourceServer::urlForView(QStringLiteral("piano"), port));
}

void MirrorTargetsDialog::copyUrlToClipboard(const QString &url)
{
    if (url.isEmpty())
        return;
    QApplication::clipboard()->setText(url);
}

void MirrorTargetsDialog::loadFromManager()
{
    ui->binCheck->setChecked(m_manager->logger()->binEnabled());
    ui->binPathEdit->setText(m_manager->logger()->binPath());
    ui->fullTextCheck->setChecked(m_manager->logger()->fullTextEnabled());
    ui->fullTextPathEdit->setText(m_manager->logger()->fullTextPath());
    ui->syncTextCheck->setChecked(m_manager->logger()->syncOnlyTextEnabled());
    ui->syncTextPathEdit->setText(m_manager->logger()->syncOnlyTextPath());
    ui->lastNCheck->setChecked(m_manager->logger()->lastNEnabled());
    ui->lastNSpin->setValue(m_manager->logger()->lastN());
    ui->lastNPathEdit->setText(m_manager->logger()->lastNBasePath());

    if (globalSetting != nullptr)
        BrowserSourceServer::loadSettings(*globalSetting);
    ui->browserSourceCheck->setChecked(BrowserSourceServer::enabled());
    ui->browserSourcePortSpin->setValue(BrowserSourceServer::port());

    ui->targetsTable->setRowCount(0);
    const auto targets = m_manager->targets();
    for (const MirrorRemoteTarget &t : targets) {
        const int row = ui->targetsTable->rowCount();
        ui->targetsTable->insertRow(row);

        QTableWidgetItem *onItem = new QTableWidgetItem;
        onItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        onItem->setCheckState(t.enabled ? Qt::Checked : Qt::Unchecked);
        ui->targetsTable->setItem(row, 0, onItem);

        QComboBox *proto = new QComboBox;
        proto->addItem(QStringLiteral("ButtonMash"));
        proto->addItem(QStringLiteral("RetroArchRemotePad"));
        int idx = proto->findText(t.protocol);
        proto->setCurrentIndex(idx >= 0 ? idx : 0);
        ui->targetsTable->setCellWidget(row, 1, proto);

        ui->targetsTable->setItem(row, 2, new QTableWidgetItem(t.host));
        ui->targetsTable->setItem(row, 3, new QTableWidgetItem(QString::number(t.port)));
    }
}

void MirrorTargetsDialog::applyToManager()
{
    m_manager->logger()->setBinLog(ui->binCheck->isChecked(), ui->binPathEdit->text());
    m_manager->logger()->setFullTextLog(ui->fullTextCheck->isChecked(), ui->fullTextPathEdit->text());
    m_manager->logger()->setSyncOnlyTextLog(ui->syncTextCheck->isChecked(), ui->syncTextPathEdit->text());
    m_manager->logger()->setLastNDashboard(ui->lastNCheck->isChecked(), ui->lastNPathEdit->text(),
                                           ui->lastNSpin->value());

    QVector<MirrorRemoteTarget> targets;
    for (int row = 0; row < ui->targetsTable->rowCount(); ++row) {
        MirrorRemoteTarget t;
        QTableWidgetItem *onItem = ui->targetsTable->item(row, 0);
        t.enabled = onItem && onItem->checkState() == Qt::Checked;
        QComboBox *proto = qobject_cast<QComboBox *>(ui->targetsTable->cellWidget(row, 1));
        t.protocol = proto ? proto->currentText() : QStringLiteral("ButtonMash");
        QTableWidgetItem *hostItem = ui->targetsTable->item(row, 2);
        QTableWidgetItem *portItem = ui->targetsTable->item(row, 3);
        t.host = hostItem ? hostItem->text().trimmed() : QString();
        t.port = quint16(portItem ? portItem->text().toUInt() : 27151);
        targets.append(t);
    }
    m_manager->setTargets(targets);

    BrowserSourceServer::setEnabled(ui->browserSourceCheck->isChecked());
    BrowserSourceServer::setPort(quint16(ui->browserSourcePortSpin->value()));
}

void MirrorTargetsDialog::on_browserSourcePortSpin_valueChanged(int value)
{
    Q_UNUSED(value);
    refreshBrowserSourceUrls();
}

void MirrorTargetsDialog::on_copyMainUrlButton_clicked()
{
    copyUrlToClipboard(ui->browserSourceMainUrlEdit->text());
}

void MirrorTargetsDialog::on_copyStackedUrlButton_clicked()
{
    copyUrlToClipboard(ui->browserSourceStackedUrlEdit->text());
}

void MirrorTargetsDialog::on_copyPianoUrlButton_clicked()
{
    copyUrlToClipboard(ui->browserSourcePianoUrlEdit->text());
}

void MirrorTargetsDialog::on_addButton_clicked()
{
    const int row = ui->targetsTable->rowCount();
    ui->targetsTable->insertRow(row);
    QTableWidgetItem *onItem = new QTableWidgetItem;
    onItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    onItem->setCheckState(Qt::Checked);
    ui->targetsTable->setItem(row, 0, onItem);
    QComboBox *proto = new QComboBox;
    proto->addItem(QStringLiteral("ButtonMash"));
    proto->addItem(QStringLiteral("RetroArchRemotePad"));
    ui->targetsTable->setCellWidget(row, 1, proto);
    ui->targetsTable->setItem(row, 2, new QTableWidgetItem(QStringLiteral("127.0.0.1")));
    ui->targetsTable->setItem(row, 3, new QTableWidgetItem(QStringLiteral("27151")));
}

void MirrorTargetsDialog::on_removeButton_clicked()
{
    int row = ui->targetsTable->currentRow();
    if (row >= 0)
        ui->targetsTable->removeRow(row);
}

void MirrorTargetsDialog::on_clearButton_clicked()
{
    ui->targetsTable->setRowCount(0);
}

void MirrorTargetsDialog::on_buttonBox_accepted()
{
    applyToManager();
    accept();
}

void MirrorTargetsDialog::on_browseBinButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Binary input log"), ui->binPathEdit->text(),
                                                tr("Binary log (*.binlog *.bin);;All files (*)"));
    if (!path.isEmpty())
        ui->binPathEdit->setText(path);
}

void MirrorTargetsDialog::on_browseFullTextButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Full text input log"), ui->fullTextPathEdit->text(),
                                                tr("CSV / text (*.csv *.log *.txt);;All files (*)"));
    if (!path.isEmpty())
        ui->fullTextPathEdit->setText(path);
}

void MirrorTargetsDialog::on_browseSyncTextButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Sync-header text log"), ui->syncTextPathEdit->text(),
                                                tr("CSV / text (*.csv *.log *.txt);;All files (*)"));
    if (!path.isEmpty())
        ui->syncTextPathEdit->setText(path);
}

void MirrorTargetsDialog::on_browseLastNButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Last N dashboard base filename"),
                                                ui->lastNPathEdit->text(),
                                                tr("Dashboard base (*.csv);;All files (*)"));
    if (!path.isEmpty()) {
        if (path.endsWith(QLatin1String(".csv"), Qt::CaseInsensitive))
            path.chop(4);
        ui->lastNPathEdit->setText(path);
    }
}
