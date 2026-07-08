#include "masheditorwindow.h"
#include "masheditorsettings.h"
#include "skinmetadata.h"
#include "skinutil.h"

#include <QApplication>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QInputDialog>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

static MashEditMode runWelcomeDialog(QWidget *parent, const QString &skinsRoot,
                                     QString *outSkinDir, bool hasPresetSkin)
{
    QDialog dlg(parent);
    dlg.setWindowTitle(QObject::tr("MashEditor — Open or Create Skin"));
    auto *lay = new QVBoxLayout(&dlg);
    lay->addWidget(new QLabel(QObject::tr("Skins folder: %1").arg(skinsRoot)));

    auto *modeCombo = new QComboBox;
    modeCombo->addItem(QObject::tr("Regular skin"), int(MashEditMode::Regular));
    modeCombo->addItem(QObject::tr("Piano display"), int(MashEditMode::Piano));
    lay->addWidget(modeCombo);

    auto *existingRadio = new QRadioButton(QObject::tr("Load existing skin folder"));
    auto *newRadio = new QRadioButton(QObject::tr("Create new skin"));
    existingRadio->setChecked(hasPresetSkin);
    newRadio->setChecked(!hasPresetSkin);
    lay->addWidget(existingRadio);
    lay->addWidget(newRadio);

    auto *folderEdit = new QLineEdit;
    lay->addWidget(folderEdit);

    auto *browseBtn = new QPushButton(QObject::tr("Browse…"));
    lay->addWidget(browseBtn);
    QObject::connect(browseBtn, &QPushButton::clicked, [&]() {
        const QString d = QFileDialog::getExistingDirectory(&dlg, QObject::tr("Skin folder"), skinsRoot);
        if (!d.isEmpty())
            folderEdit->setText(d);
    });

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    lay->addWidget(buttons);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() != QDialog::Accepted)
        return MashEditMode::Regular;

    const MashEditMode mode = MashEditMode(modeCombo->currentData().toInt());
    QString folder = folderEdit->text().trimmed();
    if (newRadio->isChecked()) {
        if (folder.isEmpty())
            folder = QInputDialog::getText(&dlg, QObject::tr("New skin"), QObject::tr("Folder name:"));
        if (folder.isEmpty())
            return mode;
        const QString skinDir = QDir(skinsRoot).filePath(folder);
        if (QFileInfo::exists(skinDir)) {
            QMessageBox::warning(&dlg, QObject::tr("Error"), QObject::tr("Folder already exists."));
            return mode;
        }
        if (!SkinUtil::createSkeletonSkin(skinsRoot, folder, outSkinDir))
            return mode;
    } else {
        if (folder.isEmpty()) {
            folder = QFileDialog::getExistingDirectory(&dlg, QObject::tr("Skin folder"), skinsRoot);
        }
        if (folder.isEmpty())
            return mode;
        *outSkinDir = QFileInfo(folder).canonicalFilePath();
    }
    if (outSkinDir->isEmpty() && !folder.isEmpty())
        *outSkinDir = QDir(skinsRoot).filePath(folder);
    return mode;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("MashEditor"));
    QCoreApplication::setOrganizationName(QStringLiteral("ButtonMash"));

    QString skinsRoot;
    QString presetSkin;
    for (int i = 1; i < argc; ++i) {
        const QString a = QString::fromLocal8Bit(argv[i]);
        if (a == QStringLiteral("--skins-dir") && i + 1 < argc)
            skinsRoot = QString::fromLocal8Bit(argv[++i]);
        else if (a == QStringLiteral("--skin") && i + 1 < argc)
            presetSkin = QString::fromLocal8Bit(argv[++i]);
    }

    QSettings *settings = mashEditorSettings();
    if (skinsRoot.isEmpty())
        skinsRoot = mashEditorSkinsRoot(settings);

    QString skinDir;
    MashEditMode mode = MashEditMode::Regular;
    if (!presetSkin.isEmpty() && QFileInfo::exists(QDir(skinsRoot).filePath(presetSkin))) {
        skinDir = QDir(skinsRoot).filePath(presetSkin);
        SkinMetadata meta = SkinMetadata::load(skinDir);
        mode = (meta.lastMode == QLatin1String("piano")) ? MashEditMode::Piano : MashEditMode::Regular;
    } else {
        mode = runWelcomeDialog(nullptr, skinsRoot, &skinDir, false);
    }

    if (skinDir.isEmpty())
        return 0;

    MashEditorWindow win(skinsRoot);
    win.openSkinDir(skinDir, mode);
    win.show();
    const int rc = app.exec();
    delete settings;
    return rc;
}

#include <QPushButton>
