#pragma once

#include "SettingsDialog.hpp"
#include "WallpaperWindow.hpp"

#include <QMenu>
#include <QSystemTrayIcon>

class TrayIcon
{
public:
  TrayIcon();
  ~TrayIcon();

  QSystemTrayIcon tray;
  QMenu           menu;

  SettingsDialog   sd;
  WallpaperWindow* ww = nullptr;

  void show();
};
