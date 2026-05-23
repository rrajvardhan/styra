#include <LayerShellQt/Shell>
#include <LayerShellQt/Window>

#include "WallpaperWindow.hpp"

#include <QLabel>
#include <QMetaObject>

void*
WallpaperWindow::get_proc_address(void*, const char* name)
{
  auto* ctx = QOpenGLContext::currentContext();

  return ctx ? reinterpret_cast<void*>(ctx->getProcAddress(name)) : nullptr;
}

void
WallpaperWindow::on_update(void* ctx)
{
  auto* self = static_cast<WallpaperWindow*>(ctx);

  QMetaObject::invokeMethod(
      self, [self]() { self->update(); }, Qt::QueuedConnection);
}

WallpaperWindow::WallpaperWindow(QWindow* parent)
    : QOpenGLWindow(NoPartialUpdate, parent)
{

  auto* layerWindow = LayerShellQt::Window::get(this);
  layerWindow->setLayer(LayerShellQt::Window::LayerBackground);
  layerWindow->setAnchors(LayerShellQt::Window::Anchors(
      LayerShellQt::Window::AnchorTop | LayerShellQt::Window::AnchorBottom
      | LayerShellQt::Window::AnchorLeft | LayerShellQt::Window::AnchorRight));
  layerWindow->setExclusiveZone(-1);
  layerWindow->setKeyboardInteractivity(
      LayerShellQt::Window::KeyboardInteractivityNone);

  mpv = mpv_create();

  if (!mpv)
    qFatal("failed creating mpv");

  mpv_set_option_string(mpv, "vo", "libmpv");
  mpv_set_option_string(mpv, "loop-file", "inf");
  mpv_set_option_string(mpv, "mute", "yes");

  mpv_set_option_string(mpv, "force-window", "no");
  mpv_set_option_string(mpv, "idle", "yes");

  if (mpv_initialize(mpv) < 0)
    qFatal("mpv init failed");
}

WallpaperWindow::~WallpaperWindow()
{
  makeCurrent();

  if (mpv_gl)
    mpv_render_context_free(mpv_gl);

  if (mpv)
    mpv_terminate_destroy(mpv);

  doneCurrent();
}

void
WallpaperWindow::play(const QString& path)
{
  QByteArray  ba    = path.toUtf8();
  const char* cmd[] = { "loadfile", ba.constData(), nullptr };

  mpv_command(mpv, cmd);
}

void
WallpaperWindow::pause()
{
  int yes = 1;
  mpv_set_property(mpv, "pause", MPV_FORMAT_FLAG, &yes);
}

void
WallpaperWindow::resume()
{
  int no = 0;
  mpv_set_property(mpv, "pause", MPV_FORMAT_FLAG, &no);
}

void
WallpaperWindow::togglePause()
{
  const char* cmd[] = { "cycle", "pause", nullptr };
  mpv_command(mpv, cmd);
}

bool
WallpaperWindow::isPaused() const
{
  int paused = 0;
  mpv_get_property(mpv, "pause", MPV_FORMAT_FLAG, &paused);
  return paused == 1;
}

void
WallpaperWindow::setSpeed(double speed)
{
  mpv_set_property(mpv, "speed", MPV_FORMAT_DOUBLE, &speed);
}

double
WallpaperWindow::speed() const
{
  double speed = 1.0;
  mpv_get_property(mpv, "speed", MPV_FORMAT_DOUBLE, &speed);
  return speed;
}

void
WallpaperWindow::initializeGL()
{
  mpv_opengl_init_params gl_init{ .get_proc_address     = get_proc_address,
                                  .get_proc_address_ctx = nullptr };

  mpv_render_param params[] = {
    {           MPV_RENDER_PARAM_API_TYPE, (void*)MPV_RENDER_API_TYPE_OPENGL },

    { MPV_RENDER_PARAM_OPENGL_INIT_PARAMS,                          &gl_init },

    {            MPV_RENDER_PARAM_INVALID,                           nullptr }
  };

  int err = mpv_render_context_create(&mpv_gl, mpv, params);

  if (err < 0)
    qFatal("failed creating mpv render context");

  mpv_render_context_set_update_callback(mpv_gl, on_update, this);
}

void
WallpaperWindow::paintGL()
{
  if (!mpv_gl)
    return;

  mpv_opengl_fbo fbo{ .fbo = static_cast<int>(defaultFramebufferObject()),
                      .w   = width(),
                      .h   = height(),
                      .internal_format = 0 };

  mpv_render_param params[] = {
    { MPV_RENDER_PARAM_OPENGL_FBO,    &fbo },

    {     MPV_RENDER_PARAM_FLIP_Y, &flip_y },

    {    MPV_RENDER_PARAM_INVALID, nullptr }
  };

  mpv_render_context_render(mpv_gl, params);
}
