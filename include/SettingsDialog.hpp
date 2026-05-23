#pragma once
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget* parent = nullptr);
  ~SettingsDialog() override;

signals:
  void fileSelected(const QString& path);
  void playRequested();
  void pauseRequested();
  void speedChanged(double speed);

protected:
  void closeEvent(QCloseEvent* e) override;

private slots:
  void onBrowseClicked();

private:
  QLabel*      speedLabel  = nullptr;
  QLabel*      fileInfo    = nullptr;
  QSlider*     speedSlider = nullptr;
  QPushButton* browseBtn   = nullptr;
  QPushButton* killBtn     = nullptr;
  QPushButton* settingsBtn = nullptr;

  QPushButton* playPauseBtn = nullptr;
  bool         playing      = false;
};
