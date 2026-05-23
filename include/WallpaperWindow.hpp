#pragma once

#include <QOpenGLContext>
#include <QOpenGLWindow>
#include <QTimer>

#include <mpv/client.h>
#include <mpv/render_gl.h>

class WallpaperWindow : public QOpenGLWindow
{
public:
  explicit WallpaperWindow(QWindow* parent = nullptr);
  ~WallpaperWindow() override;

  void   play(const QString& path);
  void   pause();
  void   resume();
  void   togglePause();
  bool   isPaused() const;
  void   setSpeed(double speed);
  double speed() const;

protected:
  void initializeGL() override;
  void paintGL() override;

private:
  mpv_handle*         mpv    = nullptr;
  mpv_render_context* mpv_gl = nullptr;

  int flip_y = 1;

  static void* get_proc_address(void* ctx, const char* name);
  static void  on_update(void* ctx);
};
