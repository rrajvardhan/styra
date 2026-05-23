#include "TrayIcon.hpp"

#include <QAction>
#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QMenu>
#include <QSystemTrayIcon>
#include <iostream>

QString loadQss(const QString& path);

int
main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  app.setQuitOnLastWindowClosed(false);
  app.setApplicationName("styra");

  int fontId = QFontDatabase::addApplicationFont(":/CascadiaCode.ttf");
  if (fontId != -1)
  {
    QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont   font(family);
    app.setFont(font);
  }
  else
  {
    std::cout << "failed to load CascadiaCode.ttf, please download and place "
                 "it in root...\n using default font"
              << std::endl;
  }

  QString style;
  style += loadQss(":/style.qss");

  app.setStyleSheet(style);

  TrayIcon ti;
  ti.show();

  return app.exec();
}

QString
loadQss(const QString& path)
{

  QFile f(path);
  if (!f.open(QFile::ReadOnly))
    qDebug() << "QSS NOT LOADED";

  return QString::fromUtf8(f.readAll());
}
