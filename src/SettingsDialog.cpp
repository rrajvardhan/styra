#include "SettingsDialog.hpp"
#include <QCloseEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
{
  setWindowTitle("[ styra ]");
  setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
  setFixedWidth(340);

  auto* root = new QVBoxLayout(this);
  root->setSpacing(0);
  root->setContentsMargins(0, 0, 0, 0);

  // --- Titlebar ---
  auto* titlebar = new QHBoxLayout();
  titlebar->setContentsMargins(8, 10, 8, 14);

  auto* closeBtn = new QPushButton();
  closeBtn->setFixedSize(18, 18);
  closeBtn->setObjectName("closeBtn");
  closeBtn->setIcon(QIcon(":/icons/minus.svg"));
  closeBtn->setIconSize(QSize(16, 16));

  auto* titleLabel = new QLabel("[ styra ]");
  titleLabel->setAlignment(Qt::AlignCenter);
  titlebar->addWidget(titleLabel, 1);
  titlebar->addStretch();
  titlebar->addWidget(closeBtn);
  titlebar->addSpacing(8);
  root->addLayout(titlebar);

  // --- Subtitle ---
  // fileInfo = new QLabel("No file selected");
  // fileInfo->setAlignment(Qt::AlignLeft);
  // fileInfo->setContentsMargins(14, 0, 14, 6);
  // fileInfo->setObjectName("fileInfo");
  // root->addWidget(fileInfo);

  auto* tile = new QFrame();
  tile->setObjectName("tile");
  auto* tileLayout = new QVBoxLayout(tile);
  tileLayout->setAlignment(Qt::AlignCenter);
  auto* recentsLabel = new QLabel("recently used:");
  tileLayout->addWidget(recentsLabel, 1);

  // --- Recent files row ---
  auto* thumbRow = new QHBoxLayout();
  thumbRow->setContentsMargins(0, 8, 0, 8);
  thumbRow->setSpacing(8);

  for (int i = 0; i < 3; i++)
  {
    thumbBtns[i] = new QPushButton();
    thumbBtns[i]->setFixedSize(96, 64);
    thumbBtns[i]->setObjectName("thumbBtn");
    thumbBtns[i]->setIconSize(QSize(96, 64));
    connect(thumbBtns[i], &QPushButton::clicked, this,
            [this, i]()
            {
              const QString path = recentFiles[i];
              if (!path.isEmpty())
                loadFile(path);
            });
    thumbRow->addWidget(thumbBtns[i]);
  }

  tileLayout->addLayout(thumbRow);

  // --- Speed slider inside tile ---
  speedSlider = new QSlider(Qt::Horizontal);
  speedSlider->setRange(25, 400);
  speedSlider->setValue(100);

  speedLabel = new QLabel("1.00×");
  speedLabel->setAlignment(Qt::AlignCenter);
  speedLabel->setFixedWidth(60);

  auto* speedRow = new QHBoxLayout();
  speedRow->setContentsMargins(0, 4, 0, 4);
  speedRow->addWidget(new QLabel("Speed"));
  speedRow->addWidget(speedSlider, 1);
  speedRow->addWidget(speedLabel);

  tileLayout->addLayout(speedRow);

  auto* tileWrapper = new QHBoxLayout();
  tileWrapper->setContentsMargins(12, 0, 12, 0);
  tileWrapper->addWidget(tile);
  root->addLayout(tileWrapper);

  playPauseBtn = new QPushButton();
  playPauseBtn->setIcon(QIcon(":/icons/pause.svg"));
  playPauseBtn->setFixedHeight(44);
  playPauseBtn->setIconSize(QSize(30, 30));

  killBtn = new QPushButton();
  killBtn->setFixedHeight(44);
  killBtn->setIcon(QIcon(":/icons/kill.svg"));
  killBtn->setIconSize(QSize(30, 30));

  browseBtn = new QPushButton();
  browseBtn->setObjectName("browseBtn");
  browseBtn->setIcon(QIcon(":/icons/file.svg"));
  browseBtn->setIconSize(QSize(30, 30));
  browseBtn->setFixedHeight(44);

  auto* btnRow = new QHBoxLayout();
  btnRow->setContentsMargins(12, 10, 12, 0);
  btnRow->setSpacing(10);
  btnRow->addWidget(playPauseBtn);
  btnRow->addWidget(killBtn);
  btnRow->addWidget(browseBtn);
  root->addLayout(btnRow);

  connect(closeBtn, &QPushButton::clicked, this, &SettingsDialog::hide);
  connect(playPauseBtn, &QPushButton::clicked, this,
          [this]()
          {
            playing = !playing;
            if (playing)
            {
              playPauseBtn->setIcon(QIcon(":/icons/pause.svg"));
              emit playRequested();
            }
            else
            {

              playPauseBtn->setIcon(QIcon(":/icons/play.svg"));
              emit pauseRequested();
            }
          });
  connect(killBtn, &QPushButton::clicked, this, []() { qApp->quit(); });
  connect(browseBtn, &QPushButton::clicked, this,
          &SettingsDialog::onBrowseClicked);
  connect(speedSlider, &QSlider::valueChanged, this,
          [this](int value)
          {
            double speed = value / 100.0;
            speedLabel->setText(QString("%1×").arg(speed, 0, 'f', 2));
            emit speedChanged(speed);
          });

  loadRecentFiles();
  adjustSize();
}

SettingsDialog::~SettingsDialog()
{
}

void
SettingsDialog::onBrowseClicked()
{
  const QString path = QFileDialog::getOpenFileName(
      this, "Select Video", {},
      "Videos (*.mp4 *.mkv *.webm *.avi *.mov *.flv *.wmv);;All Files (*)");
  if (path.isEmpty())
    return;
  loadFile(path);
}

void
SettingsDialog::loadFile(const QString& path)
{
  // fileInfo->setText(QFileInfo(path).fileName());
  const QString resolvedPath = path;
  saveRecentFile(resolvedPath);
  loadRecentFiles();
  emit fileSelected(resolvedPath);
  playing = true;
  playPauseBtn->setIcon(QIcon(":/icons/pause.svg"));
}

void
SettingsDialog::saveRecentFile(const QString& path)
{
  QSettings   s("styra", "styra");
  QStringList recent = s.value("recentFiles").toStringList();
  recent.removeAll(path);
  recent.prepend(path);
  while (recent.size() > 3)
    recent.removeLast();
  s.setValue("recentFiles", recent);
}

void
SettingsDialog::loadRecentFiles()
{
  QSettings   s("styra", "styra");
  QStringList recent = s.value("recentFiles").toStringList();
  for (int i = 0; i < 3; i++)
  {
    recentFiles[i] = (i < recent.size()) ? recent[i] : "";
    if (recentFiles[i].isEmpty())
    {
      thumbBtns[i]->setIcon(QIcon(""));
      thumbBtns[i]->setText("·");
    }
    else
    {
      QString thumb = extractThumbnail(recentFiles[i]);
      if (!thumb.isEmpty())
        thumbBtns[i]->setIcon(QIcon(
            QPixmap(thumb).scaled(160, 90, Qt::KeepAspectRatioByExpanding,
                                  Qt::SmoothTransformation)));
      else
        thumbBtns[i]->setText(QFileInfo(recentFiles[i]).baseName().left(12));
    }
  }
}

QString
SettingsDialog::extractThumbnail(const QString& videoPath)
{
  QString outPath
      = QStandardPaths::writableLocation(QStandardPaths::TempLocation)
        + QString("/styra_%1.jpg").arg(qHash(videoPath));
  if (QFile::exists(outPath))
    return outPath;
  QProcess proc;
  proc.start("ffmpeg", { "-y", "-i", videoPath, "-ss", "00:00:03", "-vframes",
                         "1", "-vf", "scale=96:64", outPath });
  proc.waitForFinished(3000);
  return QFile::exists(outPath) ? outPath : "";
}

void
SettingsDialog::closeEvent(QCloseEvent* e)
{
  hide();
  e->ignore();
}
