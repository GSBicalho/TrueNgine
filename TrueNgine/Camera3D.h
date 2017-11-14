#pragma once

#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QtMath>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

// Default camera values
const GLdouble YAW = -90.0f;
const GLdouble PITCH = 0.0f;
const GLdouble SPEED = 3.0f;
const GLdouble SENSITIVTY = 0.25f;
const GLdouble ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera3D {
public:
	// Camera Attributes
	QVector3D Position;
	QVector3D Front;
	QVector3D Up;
	QVector3D Right;
	QVector3D WorldUp;
	// Eular Angles
	GLdouble Yaw;
	GLdouble Pitch;
	// Camera options
	GLdouble MovementSpeed;
	GLdouble MouseSensitivity;
	GLdouble Zoom;

	// Constructor with vectors
	Camera3D(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), 
		QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), 
		GLdouble yaw = YAW, 
		GLdouble pitch = PITCH);
	
	// Constructor with scalar values
	Camera3D(GLdouble posX, GLdouble posY, GLdouble posZ, 
		GLdouble upX, GLdouble upY, GLdouble upZ, 
		GLdouble yaw, GLdouble pitch);

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	QMatrix4x4 GetViewMatrix();

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, GLdouble deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLdouble xoffset, GLdouble yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLdouble yoffset);

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors();
};