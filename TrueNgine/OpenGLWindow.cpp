#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QKeyEvent>

#include "OpenGLWindow.h"

OpenGLWindow::OpenGLWindow(QWindow *parent)
	: QWindow(parent),
	m_animating(false),
	m_context(0),
	m_device(0) {
	setSurfaceType(QWindow::OpenGLSurface);
	for (int i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

OpenGLWindow::~OpenGLWindow() {}

void OpenGLWindow::render(QPainter *painter) {
	Q_UNUSED(painter);
}

void OpenGLWindow::initialize() {}

void OpenGLWindow::render() {
	if (!m_device)
		m_device = new QOpenGLPaintDevice;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_device->setSize(size());

	QPainter painter(m_device);
	render(&painter);
}

void OpenGLWindow::renderLater() {
	requestUpdate();
}

void OpenGLWindow::closeEvent(QEvent *event) {}

bool OpenGLWindow::event(QEvent *event) {
	switch (event->type()) {
	case QEvent::UpdateRequest:
		renderNow();
		return true;
	case QEvent::Close:
		closeEvent(event);
		return QWindow::event(event);
	case QEvent::KeyPress:
	{
		QKeyEvent* key = (QKeyEvent*)(event);
		switch (key->key()) {
		case Qt::Key_Space: keys[OPENGL_WINDOW_SPACE] = 1; break;
		case Qt::Key_Shift: keys[OPENGL_WINDOW_SHIFT] = 1; break;
		case Qt::Key_A: keys[OPENGL_WINDOW_A] = 1; break;
		case Qt::Key_B: keys[OPENGL_WINDOW_B] = 1; break;
		case Qt::Key_C: keys[OPENGL_WINDOW_C] = 1; break;
		case Qt::Key_D: keys[OPENGL_WINDOW_D] = 1; break;
		case Qt::Key_E: keys[OPENGL_WINDOW_E] = 1; break;
		case Qt::Key_F: keys[OPENGL_WINDOW_F] = 1; break;
		case Qt::Key_G: keys[OPENGL_WINDOW_G] = 1; break;
		case Qt::Key_H: keys[OPENGL_WINDOW_H] = 1; break;
		case Qt::Key_I: keys[OPENGL_WINDOW_I] = 1; break;
		case Qt::Key_J: keys[OPENGL_WINDOW_J] = 1; break;
		case Qt::Key_K: keys[OPENGL_WINDOW_K] = 1; break;
		case Qt::Key_L: keys[OPENGL_WINDOW_L] = 1; break;
		case Qt::Key_M: keys[OPENGL_WINDOW_M] = 1; break;
		case Qt::Key_N: keys[OPENGL_WINDOW_N] = 1; break;
		case Qt::Key_O: keys[OPENGL_WINDOW_O] = 1; break;
		case Qt::Key_P: keys[OPENGL_WINDOW_P] = 1; break;
		case Qt::Key_Q: keys[OPENGL_WINDOW_Q] = 1; break;
		case Qt::Key_R: keys[OPENGL_WINDOW_R] = 1; break;
		case Qt::Key_S: keys[OPENGL_WINDOW_S] = 1; break;
		case Qt::Key_T: keys[OPENGL_WINDOW_T] = 1; break;
		case Qt::Key_U: keys[OPENGL_WINDOW_U] = 1; break;
		case Qt::Key_V: keys[OPENGL_WINDOW_V] = 1; break;
		case Qt::Key_W: keys[OPENGL_WINDOW_W] = 1; break;
		case Qt::Key_X: keys[OPENGL_WINDOW_X] = 1; break;
		case Qt::Key_Y: keys[OPENGL_WINDOW_Y] = 1; break;
		case Qt::Key_Z: keys[OPENGL_WINDOW_Z] = 1; break;

		case Qt::Key_Escape: keys[OPENGL_WINDOW_ESC] = 1; break;
		case Qt::Key_Tab: keys[OPENGL_WINDOW_TAB] = 1; break;
		case Qt::Key_Enter: keys[OPENGL_WINDOW_ENTER] = 1; break;
		case Qt::Key_Up: keys[OPENGL_WINDOW_ARROW_UP] = 1; break;
		case Qt::Key_Down: keys[OPENGL_WINDOW_ARROW_DOWN] = 1; break;
		case Qt::Key_Left: keys[OPENGL_WINDOW_ARROW_LEFT] = 1; break;
		case Qt::Key_Right: keys[OPENGL_WINDOW_ARROW_RIGHT] = 1; break;

		case Qt::Key_0: keys[OPENGL_WINDOW_0] = 1; break;
		case Qt::Key_1: keys[OPENGL_WINDOW_1] = 1; break;
		case Qt::Key_2: keys[OPENGL_WINDOW_2] = 1; break;
		case Qt::Key_3: keys[OPENGL_WINDOW_3] = 1; break;
		case Qt::Key_4: keys[OPENGL_WINDOW_4] = 1; break;
		case Qt::Key_5: keys[OPENGL_WINDOW_5] = 1; break;
		case Qt::Key_6: keys[OPENGL_WINDOW_6] = 1; break;
		case Qt::Key_7: keys[OPENGL_WINDOW_7] = 1; break;
		case Qt::Key_8: keys[OPENGL_WINDOW_8] = 1; break;
		case Qt::Key_9: keys[OPENGL_WINDOW_9] = 1; break;

		default:
			break;
		}
		break;
	}
	case QEvent::KeyRelease:
	{
		QKeyEvent* key = (QKeyEvent*)(event);
		switch (key->key()) {
		case Qt::Key_Space: keys[OPENGL_WINDOW_SPACE] = 0; break;
		case Qt::Key_Shift: keys[OPENGL_WINDOW_SHIFT] = 0; break;

		case Qt::Key_A: keys[OPENGL_WINDOW_A] = 0; break;
		case Qt::Key_B: keys[OPENGL_WINDOW_B] = 0; break;
		case Qt::Key_C: keys[OPENGL_WINDOW_C] = 0; break;
		case Qt::Key_D: keys[OPENGL_WINDOW_D] = 0; break;
		case Qt::Key_E: keys[OPENGL_WINDOW_E] = 0; break;
		case Qt::Key_F: keys[OPENGL_WINDOW_F] = 0; break;
		case Qt::Key_G: keys[OPENGL_WINDOW_G] = 0; break;
		case Qt::Key_H: keys[OPENGL_WINDOW_H] = 0; break;
		case Qt::Key_I: keys[OPENGL_WINDOW_I] = 0; break;
		case Qt::Key_J: keys[OPENGL_WINDOW_J] = 0; break;
		case Qt::Key_K: keys[OPENGL_WINDOW_K] = 0; break;
		case Qt::Key_L: keys[OPENGL_WINDOW_L] = 0; break;
		case Qt::Key_M: keys[OPENGL_WINDOW_M] = 0; break;
		case Qt::Key_N: keys[OPENGL_WINDOW_N] = 0; break;
		case Qt::Key_O: keys[OPENGL_WINDOW_O] = 0; break;
		case Qt::Key_P: keys[OPENGL_WINDOW_P] = 0; break;
		case Qt::Key_Q: keys[OPENGL_WINDOW_Q] = 0; break;
		case Qt::Key_R: keys[OPENGL_WINDOW_R] = 0; break;
		case Qt::Key_S: keys[OPENGL_WINDOW_S] = 0; break;
		case Qt::Key_T: keys[OPENGL_WINDOW_T] = 0; break;
		case Qt::Key_U: keys[OPENGL_WINDOW_U] = 0; break;
		case Qt::Key_V: keys[OPENGL_WINDOW_V] = 0; break;
		case Qt::Key_W: keys[OPENGL_WINDOW_W] = 0; break;
		case Qt::Key_X: keys[OPENGL_WINDOW_X] = 0; break;
		case Qt::Key_Y: keys[OPENGL_WINDOW_Y] = 0; break;
		case Qt::Key_Z: keys[OPENGL_WINDOW_Z] = 0; break;

		case Qt::Key_Escape: keys[OPENGL_WINDOW_ESC] = 0; break;
		case Qt::Key_Tab: keys[OPENGL_WINDOW_TAB] = 0; break;
		case Qt::Key_Enter: keys[OPENGL_WINDOW_ENTER] = 0; break;
		case Qt::Key_Up: keys[OPENGL_WINDOW_ARROW_UP] = 0; break;
		case Qt::Key_Down: keys[OPENGL_WINDOW_ARROW_DOWN] = 0; break;
		case Qt::Key_Left: keys[OPENGL_WINDOW_ARROW_LEFT] = 0; break;
		case Qt::Key_Right: keys[OPENGL_WINDOW_ARROW_RIGHT] = 0; break;

		case Qt::Key_0: keys[OPENGL_WINDOW_0] = 0; break;
		case Qt::Key_1: keys[OPENGL_WINDOW_1] = 0; break;
		case Qt::Key_2: keys[OPENGL_WINDOW_2] = 0; break;
		case Qt::Key_3: keys[OPENGL_WINDOW_3] = 0; break;
		case Qt::Key_4: keys[OPENGL_WINDOW_4] = 0; break;
		case Qt::Key_5: keys[OPENGL_WINDOW_5] = 0; break;
		case Qt::Key_6: keys[OPENGL_WINDOW_6] = 0; break;
		case Qt::Key_7: keys[OPENGL_WINDOW_7] = 0; break;
		case Qt::Key_8: keys[OPENGL_WINDOW_8] = 0; break;
		case Qt::Key_9: keys[OPENGL_WINDOW_9] = 0; break;
		default:
			break;
		}
		break;
	}
	default:
		return QWindow::event(event);
	}
	return QWindow::event(event);
}

void OpenGLWindow::exposeEvent(QExposeEvent *event) {
	Q_UNUSED(event);

	if (isExposed())
		renderNow();
}

void OpenGLWindow::renderNow() {
	if (!isExposed())
		return;

	bool needsInitialize = false;

	if (!m_context) {
		m_context = new QOpenGLContext(this);
		m_context->setFormat(requestedFormat());
		m_context->create();

		needsInitialize = true;
	}

	m_context->makeCurrent(this);

	if (needsInitialize) {
		initializeOpenGLFunctions();
		initialize();
	}

	render();

	m_context->swapBuffers(this);

	if (m_animating)
		renderLater();
}

void OpenGLWindow::setAnimating(bool animating) {
	m_animating = animating;

	if (animating)
		renderLater();
}