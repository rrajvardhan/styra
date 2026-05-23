#include "SettingsDialog.hpp"
#include <QCloseEvent>
#include <QFileDialog>

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
{
  setWindowTitle("[ styra ]");
  setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
  setFixedSize(340, 170);

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
  fileInfo = new QLabel("No file selected");
  fileInfo->setAlignment(Qt::AlignLeft);
  fileInfo->setContentsMargins(14, 0, 14, 0);
  fileInfo->setObjectName("fileInfo");
  root->addWidget(fileInfo);

  auto* tile = new QFrame();
  tile->setObjectName("tile");
  auto* tileLayout = new QVBoxLayout(tile);
  tileLayout->setAlignment(Qt::AlignCenter);

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

  fileInfo->setText(path);
  emit fileSelected(path);
  playing = true;
}

void
SettingsDialog::closeEvent(QCloseEvent* e)
{
  hide();
  e->ignore();
}
