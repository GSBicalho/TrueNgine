#pragma once

#include <QWindow>
#include <QOpenGLFunctions>

class OpenGLWindow : public QWindow, public QOpenGLFunctions {
	Q_OBJECT
public:
	explicit OpenGLWindow(QWindow *parent = 0);
	~OpenGLWindow();

	virtual void render(QPainter *painter);
	virtual void render();

	virtual void initialize();

	//Keyboard

	enum KeyPressMapping {
		OPENGL_WINDOW_SHIFT,
		OPENGL_WINDOW_SPACE,

		OPENGL_WINDOW_A,
		OPENGL_WINDOW_B,
		OPENGL_WINDOW_C,
		OPENGL_WINDOW_D,
		OPENGL_WINDOW_E,
		OPENGL_WINDOW_F,
		OPENGL_WINDOW_G,
		OPENGL_WINDOW_H,
		OPENGL_WINDOW_I,
		OPENGL_WINDOW_J,
		OPENGL_WINDOW_K,
		OPENGL_WINDOW_L,
		OPENGL_WINDOW_M,
		OPENGL_WINDOW_N,
		OPENGL_WINDOW_O,
		OPENGL_WINDOW_P,
		OPENGL_WINDOW_Q,
		OPENGL_WINDOW_R,
		OPENGL_WINDOW_S,
		OPENGL_WINDOW_T,
		OPENGL_WINDOW_U,
		OPENGL_WINDOW_V,
		OPENGL_WINDOW_W,
		OPENGL_WINDOW_X,
		OPENGL_WINDOW_Y,
		OPENGL_WINDOW_Z,

		OPENGL_WINDOW_ESC,
		OPENGL_WINDOW_TAB,
		OPENGL_WINDOW_ENTER,

		OPENGL_WINDOW_ARROW_UP,
		OPENGL_WINDOW_ARROW_DOWN,
		OPENGL_WINDOW_ARROW_LEFT,
		OPENGL_WINDOW_ARROW_RIGHT,

		OPENGL_WINDOW_0,
		OPENGL_WINDOW_1,
		OPENGL_WINDOW_2,
		OPENGL_WINDOW_3,
		OPENGL_WINDOW_4,
		OPENGL_WINDOW_5,
		OPENGL_WINDOW_6,
		OPENGL_WINDOW_7,
		OPENGL_WINDOW_8,
		OPENGL_WINDOW_9,
	};

	bool keys[1024];

	void setAnimating(bool animating);

	public slots:
	void renderLater();
	void renderNow();

	virtual void closeEvent(QEvent *event);

protected:
	bool event(QEvent *event) override;
	void exposeEvent(QExposeEvent *event) override;
	
	QOpenGLPaintDevice *m_device;
	QOpenGLContext *m_context;
private:
	bool m_animating;
};