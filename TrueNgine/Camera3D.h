#pragma once

// Std. Includes
#include <vector>

// GL Includes
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
	Camera3D(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), GLdouble yaw = YAW, GLdouble pitch = PITCH) : Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM) {
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}
	// Constructor with scalar values
	Camera3D(GLdouble posX, GLdouble posY, GLdouble posZ, GLdouble upX, GLdouble upY, GLdouble upZ, GLdouble yaw, GLdouble pitch) : Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM) {
		this->Position = QVector3D(posX, posY, posZ);
		this->WorldUp = QVector3D(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	QMatrix4x4 GetViewMatrix() {
		QMatrix4x4 m;
		m.lookAt(this->Position, this->Position + this->Front, this->Up);
		return m;
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, GLdouble deltaTime) {
		GLdouble velocity = this->MovementSpeed * deltaTime;
		if (direction == FORWARD)
			this->Position += this->Front * velocity;
		if (direction == BACKWARD)
			this->Position -= this->Front * velocity;
		if (direction == LEFT)
			this->Position -= this->Right * velocity;
		if (direction == RIGHT)
			this->Position += this->Right * velocity;
		if (direction == UP)
			this->Position += this->Up * velocity;
		if (direction == DOWN)
			this->Position -= this->Up * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLdouble xoffset, GLdouble yoffset, GLboolean constrainPitch = true) {
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		this->Yaw += xoffset;
		this->Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch) {
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLdouble yoffset) {
		if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
			this->Zoom -= yoffset;
		if (this->Zoom <= 1.0f)
			this->Zoom = 1.0f;
		if (this->Zoom >= 45.0f)
			this->Zoom = 45.0f;
	}

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors() {
		// Calculate the new Front vector
		QVector3D front;
		front.setX(cos(qDegreesToRadians(this->Yaw)) * cos(qDegreesToRadians(this->Pitch)));
		front.setY(sin(qDegreesToRadians(this->Pitch)));
		front.setZ(sin(qDegreesToRadians(this->Yaw)) * cos(qDegreesToRadians(this->Pitch)));
		this->Front =front.normalized();
		// Also re-calculate the Right and Up vector
		// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->Right = QVector3D::crossProduct(this->Front, this->WorldUp).normalized();  
		this->Up = QVector3D::crossProduct(this->Right, this->Front).normalized();
	}
};