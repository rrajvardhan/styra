#include "TrayIcon.hpp"

#include <QApplication>
#include <iostream>

TrayIcon::TrayIcon()
{
  tray.setIcon(QIcon(":/icons/settings.svg"));

  setlocale(LC_NUMERIC, "C");
  if (!ww)
  {
    ww = new WallpaperWindow();
  }

  menu.addAction("Settings",
                 [this]()
                 {
                   std::cout << "Settings" << std::endl;
                   sd.show();
                 });

  menu.addAction("Quit",
                 []()
                 {
                   std::cout << "Quit" << std::endl;
                   qApp->quit();
                 });

  tray.setContextMenu(&menu);

  QAction::connect(&sd, &SettingsDialog::fileSelected, ww,
                   &WallpaperWindow::play);
  QAction::connect(&sd, &SettingsDialog::playRequested, ww,
                   &WallpaperWindow::resume);
  QAction::connect(&sd, &SettingsDialog::pauseRequested, ww,
                   &WallpaperWindow::pause);
  QAction::connect(&sd, &SettingsDialog::speedChanged, ww,
                   &WallpaperWindow::setSpeed);
}

TrayIcon::~TrayIcon()
{
}

void
TrayIcon::show()
{
  tray.show();
  ww->show();
}
