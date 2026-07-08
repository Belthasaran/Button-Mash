#include "masheditorwindow.h"
#include "skinparser.h"
#include "skinwriter.h"
#include "skinutil.h"

#include <QColorDialog>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>

MashEditorWindow::MashEditorWindow(const QString &skinsRoot, QWidget *parent)
    : QMainWindow(parent)
    , m_skinsRoot(skinsRoot)
{
    setWindowTitle(QStringLiteral("MashEditor"));
    resize(1100, 700);
    buildUi();
}

void MashEditorWindow::buildUi()
{
    auto *central = new QWidget(this);
    auto *rootLay = new QVBoxLayout(central);

    auto *viewBar = new QHBoxLayout;
    auto *editRadio = new QRadioButton(tr("Edit"));
    auto *prevRadio = new QRadioButton(tr("Preview"));
    editRadio->setChecked(true);
    viewBar->addWidget(editRadio);
    viewBar->addWidget(prevRadio);
    connect(editRadio, &QRadioButton::toggled, this, [this](bool on) {
        if (on)
            onViewModeToggled(false);
    });
    connect(prevRadio, &QRadioButton::toggled, this, [this](bool on) {
        if (on)
            onViewModeToggled(true);
    });

    m_chkAll = new QCheckBox(tr("All"));
    m_chkAll->setTristate(true);
    m_chkAll->setCheckState(Qt::Checked);
    viewBar->addWidget(m_chkAll);
    connect(m_chkAll, &QCheckBox::stateChanged, this, &MashEditorWindow::onAllPreviewToggled);

    const struct { const char *label; const char *name; } btns[] = {
        {"B", "b"}, {"A", "a"}, {"X", "x"}, {"Y", "y"}, {"L", "l"}, {"R", "r"},
        {"[SEL]", "select"}, {"[ST]", "start"},
        {"Up", "up"}, {"Down", "down"}, {"Left", "left"}, {"Right", "right"},
    };
    for (const auto &b : btns) {
        auto *cb = new QCheckBox(QString::fromUtf8(b.label));
        cb->setChecked(true);
        m_previewChks[QString::fromUtf8(b.name)] = cb;
        viewBar->addWidget(cb);
        connect(cb, &QCheckBox::toggled, this, &MashEditorWindow::onIndividualPreviewToggled);
    }
    viewBar->addStretch();
    rootLay->addLayout(viewBar);

    auto *body = new QHBoxLayout;

    auto *palette = new QWidget;
    auto *palLay = new QVBoxLayout(palette);
    palLay->addWidget(new QLabel(tr("Tools")));
    m_ptrBtn = new QPushButton(tr("Pointer"));
    m_placeBtn = new QPushButton(tr("Place button"));
    m_bgBtn = new QPushButton(tr("Set background"));
    m_ptrBtn->setCheckable(true);
    m_placeBtn->setCheckable(true);
    m_bgBtn->setCheckable(true);
    m_ptrBtn->setChecked(true);
    palLay->addWidget(m_ptrBtn);
    palLay->addWidget(m_placeBtn);
    palLay->addWidget(m_bgBtn);
    connect(m_ptrBtn, &QPushButton::clicked, this, &MashEditorWindow::onToolPointer);
    connect(m_placeBtn, &QPushButton::clicked, this, &MashEditorWindow::onToolPlace);
    connect(m_bgBtn, &QPushButton::clicked, this, &MashEditorWindow::onToolSetBackground);
    palLay->addWidget(new QLabel(tr("Layers / Assets")));
    m_assetList = new QListWidget;
    m_assetList->setMaximumWidth(140);
    connect(m_assetList, &QListWidget::itemClicked, this, &MashEditorWindow::onPaletteItemClicked);
    palLay->addWidget(m_assetList);
    palLay->addStretch();
    body->addWidget(palette);

    m_canvas = new MashEditorCanvas;
    m_canvas->setMinimumSize(400, 300);
    connect(m_canvas, &MashEditorCanvas::selectionChanged, this, &MashEditorWindow::onCanvasSelection);
    connect(m_canvas, &MashEditorCanvas::modelChanged, this, &MashEditorWindow::onCanvasModelChanged);

    m_inspector = new QWidget;
    m_inspectorForm = new QFormLayout(m_inspector);
    m_inspName = new QLineEdit;
    m_inspImage = new QLineEdit;
    m_inspX = new QSpinBox;
    m_inspY = new QSpinBox;
    m_inspW = new QSpinBox;
    m_inspH = new QSpinBox;
    m_inspColor = new QLineEdit;
    m_inspBrowse = new QPushButton(tr("Browse…"));
    for (QSpinBox *sb : {m_inspX, m_inspY, m_inspW, m_inspH})
        sb->setRange(0, 4096);
    m_inspectorForm->addRow(tr("Name"), m_inspName);
    m_inspectorForm->addRow(tr("Image"), m_inspImage);
    m_inspectorForm->addRow(QString(), m_inspBrowse);
    m_inspectorForm->addRow(tr("X"), m_inspX);
    m_inspectorForm->addRow(tr("Y"), m_inspY);
    m_inspectorForm->addRow(tr("Width"), m_inspW);
    m_inspectorForm->addRow(tr("Height"), m_inspH);
    m_inspectorForm->addRow(tr("Color"), m_inspColor);
    connect(m_inspName, &QLineEdit::editingFinished, this, &MashEditorWindow::onInspectorChanged);
    connect(m_inspImage, &QLineEdit::editingFinished, this, &MashEditorWindow::onInspectorChanged);
    connect(m_inspBrowse, &QPushButton::clicked, this, &MashEditorWindow::onBrowseImage);
    connect(m_inspX, QOverload<int>::of(&QSpinBox::valueChanged), this, &MashEditorWindow::onInspectorChanged);
    connect(m_inspY, QOverload<int>::of(&QSpinBox::valueChanged), this, &MashEditorWindow::onInspectorChanged);
    connect(m_inspW, QOverload<int>::of(&QSpinBox::valueChanged), this, &MashEditorWindow::onInspectorChanged);
    connect(m_inspH, QOverload<int>::of(&QSpinBox::valueChanged), this, &MashEditorWindow::onInspectorChanged);
    connect(m_inspColor, &QLineEdit::editingFinished, this, &MashEditorWindow::onInspectorChanged);

    body->addWidget(m_canvas, 3);
    body->addWidget(m_inspector, 1);
    rootLay->addLayout(body);
    setCentralWidget(central);

    auto *tb = addToolBar(tr("Main"));
    tb->addAction(tr("Save"), this, &MashEditorWindow::onSave);
    tb->addAction(tr("Save As"), this, &MashEditorWindow::onSaveAs);
    tb->addAction(tr("Load"), this, &MashEditorWindow::onLoad);
    m_modeCombo = new QComboBox;
    m_modeCombo->addItem(tr("Regular Skin"), int(MashEditMode::Regular));
    m_modeCombo->addItem(tr("Piano Display"), int(MashEditMode::Piano));
    connect(m_modeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MashEditorWindow::onModeChanged);
    tb->addWidget(m_modeCombo);
    m_subSkinCombo = new QComboBox;
    connect(m_subSkinCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MashEditorWindow::onSubSkinChanged);
    tb->addWidget(m_subSkinCombo);
    tb->addAction(tr("Import Asset"), this, &MashEditorWindow::onImportAsset);
    tb->addAction(tr("Properties"), this, &MashEditorWindow::onProperties);
    tb->addAction(tr("Zoom +"), this, &MashEditorWindow::onZoomIn);
    tb->addAction(tr("Zoom -"), this, &MashEditorWindow::onZoomOut);
    auto *gridAct = tb->addAction(tr("Grid"));
    gridAct->setCheckable(true);
    gridAct->setChecked(true);
    connect(gridAct, &QAction::toggled, this, &MashEditorWindow::onGridToggled);
    auto *snapAct = tb->addAction(tr("Snap"));
    snapAct->setCheckable(true);
    snapAct->setChecked(m_model.metadata.snapToGrid);
    connect(snapAct, &QAction::toggled, this, &MashEditorWindow::onSnapToggled);
}

bool MashEditorWindow::openSkinDir(const QString &skinDir, MashEditMode initialMode)
{
    m_model.loadFromDir(skinDir);
    m_model.mode = initialMode;
    m_canvas->setModel(&m_model);
    m_modeCombo->setCurrentIndex(initialMode == MashEditMode::Regular ? 0 : 1);
    m_subSkinCombo->clear();
    if (!m_model.regular.subSkins.isEmpty()) {
        for (int i = 0; i < m_model.regular.subSkins.size(); ++i)
            m_subSkinCombo->addItem(m_model.regular.subSkins.at(i).name, i);
        m_subSkinCombo->setCurrentIndex(m_model.activeSubSkin);
        m_subSkinCombo->setVisible(true);
    } else {
        m_subSkinCombo->setVisible(false);
    }
    syncCanvasFromModel();
    refreshAssetList();
    m_canvas->setPreviewButtons(activePreviewSet());
    setWindowTitle(QStringLiteral("MashEditor — %1").arg(QFileInfo(skinDir).fileName()));
    return true;
}

bool MashEditorWindow::createNewSkin(const QString &folderName, MashEditMode initialMode)
{
    if (!SkinUtil::createSkeletonSkin(m_skinsRoot, folderName))
        return false;
    return openSkinDir(QDir(m_skinsRoot).filePath(folderName), initialMode);
}

void MashEditorWindow::syncCanvasFromModel()
{
    m_canvas->setModel(&m_model);
    m_canvas->rebuildScene();
    m_canvas->setPreviewButtons(activePreviewSet());
}

void MashEditorWindow::syncModelFromCanvas()
{
    Q_UNUSED(m_model);
    // Canvas drag/resize updates model via onCanvasModelChanged
}

void MashEditorWindow::onCanvasModelChanged()
{
    const QString sel = m_canvas->selectedObjectKey();
    if (!sel.startsWith(QLatin1String("button:")))
        return;
    const QString name = sel.mid(7);
    if (m_model.mode == MashEditMode::Regular) {
        RegularSkin *v = m_model.activeRegularVariant();
        if (!v->buttons.contains(name))
            return;
        RegularButtonSkin &b = v->buttons[name];
        // Find item in scene - re-query from canvas rebuild map by scanning scene
        for (QGraphicsItem *gi : m_canvas->scene()->items()) {
            if (gi->data(2).toString() != sel)
                continue;
            b.x = int(gi->pos().x());
            b.y = int(gi->pos().y());
            if (auto *pi = qgraphicsitem_cast<QGraphicsPixmapItem *>(gi)) {
                b.width = int(pi->boundingRect().width());
                b.height = int(pi->boundingRect().height());
            }
            break;
        }
        m_model.dirtyRegular = true;
    } else {
        if (!m_model.piano.buttons.contains(name))
            return;
        PianoButton &b = m_model.piano.buttons[name];
        for (QGraphicsItem *gi : m_canvas->scene()->items()) {
            if (gi->data(2).toString() != sel)
                continue;
            b.x = int(gi->pos().x());
            if (auto *ri = qgraphicsitem_cast<QGraphicsRectItem *>(gi)) {
                b.width = int(ri->rect().width());
            }
            break;
        }
        m_model.dirtyPiano = true;
    }
    refreshInspector();
}

QSet<QString> MashEditorWindow::activePreviewSet() const
{
    QSet<QString> s;
    for (auto it = m_previewChks.constBegin(); it != m_previewChks.constEnd(); ++it) {
        if (it.value()->isChecked())
            s.insert(it.key());
    }
    return s;
}

void MashEditorWindow::syncAllCheckboxFromIndividuals()
{
    if (m_updatingPreview)
        return;
    int checked = 0;
    for (auto *cb : m_previewChks) {
        if (cb->isChecked())
            ++checked;
    }
    m_updatingPreview = true;
    if (checked == 0)
        m_chkAll->setCheckState(Qt::Unchecked);
    else if (checked == m_previewChks.size())
        m_chkAll->setCheckState(Qt::Checked);
    else
        m_chkAll->setCheckState(Qt::PartiallyChecked);
    m_updatingPreview = false;
}

void MashEditorWindow::onIndividualPreviewToggled()
{
    syncAllCheckboxFromIndividuals();
    m_canvas->setPreviewButtons(activePreviewSet());
}

void MashEditorWindow::onAllPreviewToggled(int state)
{
    if (m_updatingPreview)
        return;
    if (state == Qt::PartiallyChecked)
        return;
    m_updatingPreview = true;
    const bool on = (state == Qt::Checked);
    for (auto *cb : m_previewChks)
        cb->setChecked(on);
    m_updatingPreview = false;
    m_canvas->setPreviewButtons(activePreviewSet());
}

void MashEditorWindow::onViewModeToggled(bool preview)
{
    m_canvas->setViewMode(preview ? MashEditorCanvas::ViewMode::Preview
                                  : MashEditorCanvas::ViewMode::Edit);
    m_canvas->setPreviewButtons(activePreviewSet());
}

void MashEditorWindow::onCanvasSelection(const QString &key)
{
    Q_UNUSED(key);
    refreshInspector();
}

void MashEditorWindow::setInspectorFieldsForBackground()
{
    const RegularSkin *v = m_model.activeRegularVariant();
    m_inspName->setText(v->backgroundName.isEmpty() ? QStringLiteral("Default") : v->backgroundName);
    m_inspImage->setText(v->background);
    m_inspName->setEnabled(true);
    m_inspImage->setEnabled(true);
    m_inspBrowse->setEnabled(true);
    m_inspX->setEnabled(false);
    m_inspY->setEnabled(false);
    m_inspW->setEnabled(false);
    m_inspH->setEnabled(false);
    m_inspColor->setEnabled(false);
}

void MashEditorWindow::setInspectorFieldsForButton(const QString &name)
{
    if (m_model.mode == MashEditMode::Regular) {
        const RegularButtonSkin b = m_model.activeRegularVariant()->buttons.value(name);
        m_inspName->setText(b.name);
        m_inspImage->setText(b.image);
        m_inspX->setValue(b.x);
        m_inspY->setValue(b.y);
        m_inspW->setValue(b.width);
        m_inspH->setValue(b.height);
        m_inspName->setEnabled(true);
        m_inspImage->setEnabled(true);
        m_inspBrowse->setEnabled(true);
        m_inspX->setEnabled(true);
        m_inspY->setEnabled(true);
        m_inspW->setEnabled(true);
        m_inspH->setEnabled(true);
        m_inspColor->setEnabled(false);
    } else {
        const PianoButton b = m_model.piano.buttons.value(name);
        m_inspName->setText(b.name);
        m_inspX->setValue(b.x);
        m_inspW->setValue(b.width);
        m_inspColor->setText(b.color.name());
        m_inspName->setEnabled(true);
        m_inspImage->setEnabled(false);
        m_inspBrowse->setEnabled(false);
        m_inspY->setEnabled(false);
        m_inspH->setEnabled(false);
        m_inspX->setEnabled(true);
        m_inspW->setEnabled(true);
        m_inspColor->setEnabled(true);
    }
}

void MashEditorWindow::refreshInspector()
{
    m_updatingInspector = true;
    const QString sel = m_canvas->selectedObjectKey();
    if (sel == m_model.canvasBackgroundKey() && m_model.mode == MashEditMode::Regular) {
        setInspectorFieldsForBackground();
        m_updatingInspector = false;
        return;
    }
    if (!sel.startsWith(QLatin1String("button:"))) {
        m_updatingInspector = false;
        return;
    }
    setInspectorFieldsForButton(sel.mid(7));
    m_updatingInspector = false;
}

void MashEditorWindow::applyInspectorToModel()
{
    if (m_updatingInspector)
        return;
    const QString sel = m_canvas->selectedObjectKey();
    if (sel == m_model.canvasBackgroundKey() && m_model.mode == MashEditMode::Regular) {
        RegularSkin *v = m_model.activeRegularVariant();
        v->backgroundName = m_inspName->text().trimmed();
        v->background = m_inspImage->text().trimmed();
        m_model.dirtyRegular = true;
        syncCanvasFromModel();
        return;
    }
    if (!sel.startsWith(QLatin1String("button:")))
        return;
    const QString oldName = sel.mid(7);
    const QString newName = m_inspName->text().trimmed().toLower();
    if (m_model.mode == MashEditMode::Regular) {
        RegularSkin *v = m_model.activeRegularVariant();
        if (!v->buttons.contains(oldName))
            return;
        RegularButtonSkin b = v->buttons.take(oldName);
        b.name = newName;
        b.image = m_inspImage->text();
        b.x = m_inspX->value();
        b.y = m_inspY->value();
        b.width = m_inspW->value();
        b.height = m_inspH->value();
        v->buttons[newName] = b;
        m_model.dirtyRegular = true;
    } else {
        if (!m_model.piano.buttons.contains(oldName))
            return;
        PianoButton b = m_model.piano.buttons.take(oldName);
        b.name = newName;
        b.x = m_inspX->value();
        b.width = m_inspW->value();
        b.color = QColor(m_inspColor->text());
        m_model.piano.buttons[newName] = b;
        m_model.dirtyPiano = true;
    }
    syncCanvasFromModel();
}

void MashEditorWindow::onBrowseImage()
{
    if (m_model.skinDir.isEmpty())
        return;
    const QString picked = QFileDialog::getOpenFileName(
        this, tr("Choose image"), m_model.skinDir, tr("Images (*.png *.jpg *.gif)"));
    if (picked.isEmpty())
        return;
    m_inspImage->setText(QFileInfo(picked).fileName());
    onInspectorChanged();
}

void MashEditorWindow::onInspectorChanged()
{
    applyInspectorToModel();
}

bool MashEditorWindow::confirmDiscardIfDirty()
{
    if (!m_model.isDirty())
        return true;
    const auto r = QMessageBox::question(this, tr("Unsaved changes"),
                                         tr("Save changes before continuing?"),
                                         QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (r == QMessageBox::Cancel)
        return false;
    if (r == QMessageBox::Save)
        onSave();
    return true;
}

void MashEditorWindow::onSave()
{
    syncModelFromCanvas();
    if (!m_model.saveCurrentMode()) {
        QMessageBox::warning(this, tr("Save failed"), SkinWriter::lastError);
        return;
    }
    statusBar()->showMessage(tr("Saved"), 3000);
}

void MashEditorWindow::onSaveAs()
{
    const QString name = QInputDialog::getText(this, tr("Save As"), tr("New skin folder name:"));
    if (name.isEmpty())
        return;
    const QString dest = QDir(m_skinsRoot).filePath(name);
    if (QFileInfo::exists(dest)) {
        QMessageBox::warning(this, tr("Save As"), tr("Folder already exists."));
        return;
    }
    onSave();
    if (!SkinUtil::copyDirRecursive(m_model.skinDir, dest))
        return;
    RegularSkin rs = m_model.regular;
    rs.name = name;
    PianoSkin ps = m_model.piano;
    ps.name = name;
    SkinWriter::writeRegularSkin(SkinUtil::regularXmlPath(dest), rs);
    SkinWriter::writePianoSkin(SkinUtil::pianoXmlPath(dest), ps);
    openSkinDir(dest, m_model.mode);
}

void MashEditorWindow::onLoad()
{
    if (!confirmDiscardIfDirty())
        return;
    const QString dir = QFileDialog::getExistingDirectory(this, tr("Load skin folder"), m_skinsRoot);
    if (dir.isEmpty())
        return;
    openSkinDir(dir, m_model.mode);
}

void MashEditorWindow::onImportAsset()
{
    const QString src = QFileDialog::getOpenFileName(this, tr("Import asset"), QString(),
                                                     tr("Images (*.png *.jpg *.gif);;XML (*.xml)"));
    if (src.isEmpty())
        return;
    if (src.endsWith(QStringLiteral(".xml"), Qt::CaseInsensitive)) {
        if (m_model.mode == MashEditMode::Regular) {
            RegularSkin frag = SkinParser::parseRegularSkin(src, false);
            RegularSkin *v = m_model.activeRegularVariant();
            for (auto it = frag.buttons.constBegin(); it != frag.buttons.constEnd(); ++it) {
                RegularButtonSkin b = it.value();
                const QFileInfo sfi(src);
                QString copied;
                SkinUtil::copyFileUnique(sfi.absolutePath() + QLatin1Char('/') + b.image, m_model.skinDir, &copied);
                b.image = copied;
                v->buttons[b.name] = b;
            }
            m_model.dirtyRegular = true;
        } else {
            PianoSkin frag = SkinParser::parsePianoSkin(src);
            for (auto it = frag.buttons.constBegin(); it != frag.buttons.constEnd(); ++it)
                m_model.piano.buttons[it.key()] = it.value();
            m_model.dirtyPiano = true;
        }
        syncCanvasFromModel();
        return;
    }
    QString destName;
    if (!SkinUtil::copyFileUnique(src, m_model.skinDir, &destName))
        return;
    refreshAssetList();
    statusBar()->showMessage(tr("Imported %1").arg(destName), 3000);
}

void MashEditorWindow::refreshAssetList()
{
    m_assetList->clear();
    if (m_model.mode == MashEditMode::Regular) {
        auto *bgItem = new QListWidgetItem(tr("[ Background ]"));
        bgItem->setData(Qt::UserRole, QStringLiteral("layer:background"));
        m_assetList->addItem(bgItem);
    }
    if (m_model.skinDir.isEmpty())
        return;
    const QDir dir(m_model.skinDir);
    const QStringList imgs = dir.entryList({QStringLiteral("*.png"), QStringLiteral("*.gif"),
                                            QStringLiteral("*.jpg")},
                                           QDir::Files, QDir::Name);
    for (const QString &f : imgs) {
        auto *item = new QListWidgetItem(f);
        item->setData(Qt::UserRole, QStringLiteral("asset"));
        m_assetList->addItem(item);
    }
}

void MashEditorWindow::onToolPointer()
{
    m_ptrBtn->setChecked(true);
    m_placeBtn->setChecked(false);
    m_bgBtn->setChecked(false);
    m_canvas->setTool(MashEditorCanvas::Tool::Pointer);
}

void MashEditorWindow::onToolPlace()
{
    m_ptrBtn->setChecked(false);
    m_placeBtn->setChecked(true);
    m_bgBtn->setChecked(false);
    m_canvas->setTool(MashEditorCanvas::Tool::PlaceButton);
}

void MashEditorWindow::onToolSetBackground()
{
    m_ptrBtn->setChecked(false);
    m_placeBtn->setChecked(false);
    m_bgBtn->setChecked(true);
    m_canvas->setTool(MashEditorCanvas::Tool::SetBackground);
}

void MashEditorWindow::onPaletteItemClicked(QListWidgetItem *item)
{
    if (!item)
        return;
    const QString role = item->data(Qt::UserRole).toString();
    if (role == QLatin1String("layer:background")) {
        onToolPointer();
        m_canvas->selectObjectByKey(m_model.canvasBackgroundKey());
        refreshInspector();
        return;
    }
    m_canvas->setPlaceAsset(item->text());
}

void MashEditorWindow::onSubSkinChanged(int idx)
{
    if (!confirmDiscardIfDirty()) {
        m_subSkinCombo->blockSignals(true);
        m_subSkinCombo->setCurrentIndex(m_model.activeSubSkin);
        m_subSkinCombo->blockSignals(false);
        return;
    }
    m_model.activeSubSkin = idx;
    syncCanvasFromModel();
    refreshInspector();
}

void MashEditorWindow::onProperties()
{
    if (m_model.mode == MashEditMode::Regular) {
        bool ok;
        const QString n = QInputDialog::getText(this, tr("Skin properties"), tr("Name:"),
                                                QLineEdit::Normal, m_model.regular.name, &ok);
        if (!ok)
            return;
        const QString a = QInputDialog::getText(this, tr("Skin properties"), tr("Author:"),
                                                QLineEdit::Normal, m_model.regular.author, &ok);
        if (!ok)
            return;
        m_model.regular.name = n;
        m_model.regular.author = a;
        m_model.dirtyRegular = true;
    } else {
        QDialog dlg(this);
        dlg.setWindowTitle(tr("Piano display properties"));
        auto *form = new QFormLayout(&dlg);
        auto *nameEdit = new QLineEdit(m_model.piano.name);
        auto *authEdit = new QLineEdit(m_model.piano.author);
        auto *wSpin = new QSpinBox;
        auto *hSpin = new QSpinBox;
        auto *bgEdit = new QLineEdit(m_model.piano.bgcolor.name());
        auto *laW = new QSpinBox;
        auto *laH = new QSpinBox;
        auto *laBg = new QLineEdit(m_model.piano.labelArea.bgcolor.name());
        auto *fontEdit = new QLineEdit(m_model.piano.labelArea.fontName);
        auto *fontBold = new QCheckBox(tr("Bold"));
        auto *fontSize = new QSpinBox;
        for (QSpinBox *sb : {wSpin, hSpin, laW, laH, fontSize})
            sb->setRange(1, 4096);
        wSpin->setValue(m_model.piano.width);
        hSpin->setValue(m_model.piano.height);
        laW->setValue(m_model.piano.labelArea.width);
        laH->setValue(m_model.piano.labelArea.height);
        fontBold->setChecked(m_model.piano.labelArea.fontBold);
        fontSize->setValue(m_model.piano.labelArea.fontSize);
        form->addRow(tr("Name"), nameEdit);
        form->addRow(tr("Author"), authEdit);
        form->addRow(tr("Main width"), wSpin);
        form->addRow(tr("Main height"), hSpin);
        form->addRow(tr("Main background"), bgEdit);
        form->addRow(tr("Label width"), laW);
        form->addRow(tr("Label height"), laH);
        form->addRow(tr("Label background"), laBg);
        form->addRow(tr("Font"), fontEdit);
        form->addRow(QString(), fontBold);
        form->addRow(tr("Font size"), fontSize);
        auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        form->addRow(buttons);
        connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
        connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
        if (dlg.exec() != QDialog::Accepted)
            return;
        m_model.piano.name = nameEdit->text();
        m_model.piano.author = authEdit->text();
        m_model.piano.width = wSpin->value();
        m_model.piano.height = hSpin->value();
        m_model.piano.bgcolor = QColor(bgEdit->text());
        m_model.piano.labelArea.width = laW->value();
        m_model.piano.labelArea.height = laH->value();
        m_model.piano.labelArea.bgcolor = QColor(laBg->text());
        m_model.piano.labelArea.fontName = fontEdit->text();
        m_model.piano.labelArea.fontBold = fontBold->isChecked();
        m_model.piano.labelArea.fontSize = fontSize->value();
        m_model.dirtyPiano = true;
        syncCanvasFromModel();
    }
}

void MashEditorWindow::onModeChanged(int idx)
{
    if (!confirmDiscardIfDirty()) {
        m_modeCombo->blockSignals(true);
        m_modeCombo->setCurrentIndex(m_model.mode == MashEditMode::Regular ? 0 : 1);
        m_modeCombo->blockSignals(false);
        return;
    }
    m_model.mode = (idx == 0) ? MashEditMode::Regular : MashEditMode::Piano;
    m_subSkinCombo->setVisible(m_model.mode == MashEditMode::Regular && !m_model.regular.subSkins.isEmpty());
    m_bgBtn->setVisible(m_model.mode == MashEditMode::Regular);
    syncCanvasFromModel();
    refreshAssetList();
}

void MashEditorWindow::onZoomIn() { m_canvas->zoomBy(1.2); }
void MashEditorWindow::onZoomOut() { m_canvas->zoomBy(1.0 / 1.2); }
void MashEditorWindow::onGridToggled(bool on) { m_canvas->setGridVisible(on); }
void MashEditorWindow::onSnapToggled(bool on) { m_model.metadata.snapToGrid = on; }
