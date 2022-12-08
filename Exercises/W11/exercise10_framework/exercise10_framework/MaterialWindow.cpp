#include <stdio.h>
#include <iostream>

#include "Helpers/Matrix.h"
#include "Helpers/Vector.h"
#include "Helpers/glutApplication.h"
#include "Helpers/GLShader.h"
#include "Helpers/Model.h"
#include "Helpers/World.h"
#include "Helpers/Quaternion.h"
#include "Helpers/Image.h"
#include "Helpers/Textures.h"

#include "AdvancedModels.h"
#include "skybox.h"

#include "BuiltInPath.h"
#include "AdvancedMaterial.h"

using namespace std;

class MaterialWindow: public GlutWindow, public World
{
private:
	Model *m_pHeliBody;
	Model *m_pHeliRotor1;
	Model *m_pHeliRotor2;
	Model *m_pTower;

	Skybox	m_skybox;

	//moving camera
	bool m_doCameraMotion;
	float m_CameraStartTime;
	vector<Matrix> m_CameraViews;
	vector<Vector> m_CameraPositions, m_CameraTangents;
	vector<Quaternion> m_CameraRot_q, m_CameraRot_a;
	vector<float> m_CameraTimes;

	// Flag for each of 256 characters. true = pressed, false = not pressed.
	vector<bool> m_keyPressed;

	// Time in previous call to Update()
	float m_timePrev;

	int m_mouseXPrev;
	int m_mouseYPrev;

public:
	MaterialWindow()
	:	GlutWindow("Material",  GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE),
		m_doCameraMotion(false)
	{
		m_keyPressed.resize(256, false);
		m_timePrev = 0;

		m_mouseXPrev = -1;
		m_mouseYPrev = -1;
	}

	void LoadMaterials()
	{
		// Load cubemap (sky) texture:
		GLTextureCubeMap *skyCubeTex = new GLTextureCubeMap();
		skyCubeTex->loadFromImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Image(MAIN_DIR "/textures/glacier_east.png"));
		skyCubeTex->loadFromImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X, Image(MAIN_DIR "/textures/glacier_west.png"));
		skyCubeTex->loadFromImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Image(MAIN_DIR "/textures/glacier_down.png"));
		skyCubeTex->loadFromImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Image(MAIN_DIR "/textures/glacier_up.png"));
		skyCubeTex->loadFromImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Image(MAIN_DIR "/textures/glacier_north.png"));
		skyCubeTex->loadFromImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Image(MAIN_DIR "/textures/glacier_south.png"));
		skyCubeTex->createMipmaps();

		glCheckandPrintError();

		// Create and add phong material:
		this->addMaterial(new ColoredPhongMaterial());

		glCheckandPrintError();

		// Create and add skybox material:
		SkyboxMaterial *sky = new SkyboxMaterial();
		sky->setSkyboxTexture(skyCubeTex);
		this->addMaterial(sky);

		glCheckandPrintError();

		// Create and add reflection material:
		TextureReflectionMaterial *reflect = new TextureReflectionMaterial();
		reflect->setCubeMap(skyCubeTex);
		this->addMaterial(reflect);

		glCheckandPrintError();

		// TASK 3a)
		// Create an instance of the 'TextureNormalMaterial' material and add it to the
		// collection of available materials.
		// ~TASK

		glCheckandPrintError();
	}

	virtual void LoadEvent() {
		glutReshapeWindow(1024, 768);

#ifdef _WIN32
		wglSwapIntervalEXT(0);
#endif

		// Load Default(0) Material
		LoadMaterials();

		// Load Models
		m_pHeliBody = new Helicopter();

		m_pHeliRotor1 = new Model();
		m_pHeliRotor1->loadFromFile(MAIN_DIR "/models/helicopter_rotor1.ptn");
		m_pHeliRotor1->setDefaultColor(Vector(0.9f,0.4f,0.4f));

		m_pHeliRotor2 = new Model();
		m_pHeliRotor2->loadFromFile(MAIN_DIR "/models/helicopter_rotor2.ptn");
		m_pHeliRotor2->setDefaultColor(Vector(0.4f,0.9f,0.4f));

		m_pTower = new ETHTower();
		m_pTower->setDefaultColor(Vector(187.f, 167.f, 156.f) * (1.0f / 255.0f));
		m_pTower->setGroupColor(m_pTower->findGroup("glass_windows"), Vector(0.8f,0.8f,0.9f));
		m_pTower->setGroupColor(m_pTower->findGroup("untextured"), Vector(1.0f, 0.8f, 0.05f));
		m_pTower->setGroupColor(m_pTower->findGroup("textnorm_brushedmetal"), Vector(.5f, 0.55f, 0.6f));

		m_skybox.buildGeometry();

		// background color
		glClearColor(114.f/255.f, 163.f/255.f, 219.f/255.f, 1.0f);

		//initialize camera
		//position and orientation
		m_cameraPosition = Vector(-5.636f,       2.426f,       6.570f,       1.000f);
		m_cameraDirection = Vector(0.613f,      -0.014f,      -0.796f,       1.000f);
		m_cameraUpVector = Vector(0.f, 1.0f, 0.f);

		//Moving Camera
		//get build in view matrices
		getBuiltInViews(m_CameraViews);

		//construct interpolation path from view matrices
		buildCameraPathFromViews( m_CameraViews );

		glCheckandPrintError();
	}

	virtual void Draw(float time)
	{
		glClear(GL_DEPTH_BUFFER_BIT);  //clear only depth, color is overdrawn by skybox

		// do camera motion
		updateCamera(time);

		updatePredefinedMaterialParamters();

		glDisable(GL_CULL_FACE);
		m_pHeliRotor1->draw(*this, DRAWPASS_FINAL);
		m_pHeliRotor2->draw(*this, DRAWPASS_FINAL);

		glEnable(GL_CULL_FACE);
		m_pTower->draw(*this, DRAWPASS_FINAL);

		glDisable(GL_CULL_FACE);
		m_skybox.draw(*this, DRAWPASS_FINAL);

		// Draw helicopter body last because it contains a transparent object:
		glDisable(GL_CULL_FACE);
		m_pHeliBody->draw(*this, DRAWPASS_FINAL);

		glCheckandPrintError();
	}

	virtual void Update(float time)
	{
		// Speed of camera in units per second
		const float camSpeed = 40.0f;

		float deltaTime = time - m_timePrev;
		m_timePrev = time;

		if(m_keyPressed['w'] || m_keyPressed['W'])
			m_cameraPosition +=  m_cameraDirection * camSpeed * deltaTime;

		if(m_keyPressed['s'] || m_keyPressed['S'])
			m_cameraPosition -=  m_cameraDirection * camSpeed * deltaTime;

		if(m_keyPressed['d'] || m_keyPressed['D'])
			m_cameraPosition +=  m_cameraDirection.cross3(m_cameraUpVector) * camSpeed * deltaTime;

		if(m_keyPressed['a'] || m_keyPressed['A'])
			m_cameraPosition -=  m_cameraDirection.cross3(m_cameraUpVector) * camSpeed * deltaTime;

		//////////////////////////////////////////////////////////////////////////
		// Calculate Transformation
		//////////////////////////////////////////////////////////////////////////
		Matrix helicopter, rotor1, rotor2;
		helicopter = rotor1 = rotor2 = Matrix::createIdentityMatrix();

		//// Rotate rotors
		rotor1 = Matrix::createRotationYMatrix( (float)time*10 );
		rotor2 = Matrix::createRotationZMatrix( (float)time*15 );

		//translate rotors
		rotor1 =  Matrix::createTranslationMatrix( Vector(0.0f, 2.8f, 0.0f) ) * rotor1;
		rotor2 =  Matrix::createTranslationMatrix( Vector(8.09f, 3.27f , 0.24f) ) * rotor2;

		//position heli
		helicopter = Matrix::createScaleMatrix(3.0f, 3.0f, 3.0f);
		helicopter = Matrix::createRotationYMatrix(Matrix::radians(320.f)) * helicopter;
		helicopter = Matrix::createTranslationMatrix(Vector(40.f, 158.f, -5.f)) * helicopter;

		rotor1 = helicopter * rotor1;
		rotor2 = helicopter * rotor2;

		//set new matrices to the models
		m_pHeliRotor1->setModelMatrix(rotor1);
		m_pHeliRotor2->setModelMatrix(rotor2);
		m_pHeliBody->setModelMatrix(helicopter);

		//light moves with camera
		m_lightPositions[0] = Vector(100.f*sin(time/10.f),200.f,-100.f * cos(time/10.f),1.f);

		glutPostRedisplay();
	}

	virtual void KeyEvent(unsigned char key, int state, int modifiers , int x, int y)
	{
		if(state == GLUT_DOWN)
			m_keyPressed[key] = true;
		else
			m_keyPressed[key] = false;

		if(state == GLUT_DOWN) {
			switch(key) {
				case 'p': //print camera position
						cout << "m_cameraPosition=" << (string) m_cameraPosition << endl;
						cout << "m_cameraDirection=" << (string) m_cameraDirection << endl;
						break;
				case 'P':
						cout << (string) m_viewMatrix << endl;
						break;
				case '1':
						m_cameraPosition = Vector(-5.636f,      2.426f,       6.570f,       1.000f);
						m_cameraDirection = Vector(0.613f,      -0.014f,      -0.796f,       1.000f);
						m_cameraUpVector = Vector(0, 1, 0);
						break;
				case '2':
						m_cameraPosition = Vector(84.180f,     200.767f,      56.161f,       1.000f);
						m_cameraDirection = Vector(-0.413f,      -0.573f,      -0.715f,       1.000f);
						m_cameraUpVector = Vector(0, 1, 0);
						break;
				case '3':
						m_cameraPosition = Vector(59.861f,     175.408f,      32.249f,       1.000f);
						m_cameraDirection = Vector(-0.391f,      -0.316f,      -0.900f,       1.000f);
						m_cameraUpVector = Vector(0, 1, 0);
						break;
				case ' ':
						m_doCameraMotion = !m_doCameraMotion;
						m_CameraStartTime = -1.0f;
						break;
				case 27:  // ESCAPE
						this->Close();
						break;
			}
		}
		glutPostRedisplay();
	}

	virtual void MouseEvent(unsigned int buttons, int first_time, int x, int y)
	{
		if(buttons & (1<<GLUT_LEFT_BUTTON))
		{
			if(m_mouseXPrev != -1 && m_mouseYPrev != -1)
			{
				Vector start_direction = m_cameraDirection;
				Vector start_upvector = m_cameraUpVector;
				Vector start_bidirection = m_cameraDirection.cross3(m_cameraUpVector);

				//calculate rotation
				Matrix rot;
				float pixeltorad = PI / 500.0f;  //500 pixel is half sphere

				rot = Matrix::createRotationMatrix(start_bidirection, (y - m_mouseYPrev) * pixeltorad);
				rot *= Matrix::createRotationMatrix(start_upvector, (x - m_mouseXPrev) * pixeltorad);

				//rotate direction. it's a direction therefore transform like a normal
				//and because its only a rotation use transpose instead of inverse
				rot = rot.transposed();
				m_cameraDirection = rot * start_direction;

				// enforce that horizon stays horizontal (no roll)
				Vector bidirection = rot * start_bidirection;
				bidirection[1] = 0;
				m_cameraUpVector = bidirection.cross3(m_cameraDirection);
				m_cameraUpVector = m_cameraUpVector.normalized3();
			}

			m_mouseXPrev = x;
			m_mouseYPrev = y;
		}
		else
		{
			m_mouseXPrev = -1;
			m_mouseYPrev = -1;
		}

		glutPostRedisplay();
	}

	virtual void ReshapeEvent(int width, int height) {
		glViewport(0, 0, width, height);
		//projection
		m_projectionMatrix = Matrix::createPerspectiveMatrix(Matrix::radians(60),
			(float)width/(float)height,
			0.5,
			1000
			);
	}

private:
	//////////////////////////////////////////////////////////////////////////
	// Helper functions
	//////////////////////////////////////////////////////////////////////////
	void updateCamera(float time)
	{
		if(m_doCameraMotion) {
			if(m_CameraStartTime == -1.0f)
				m_CameraStartTime = time;

			float t = time - m_CameraStartTime;

			if(t > m_CameraTimes.back()) {
				t = 0;
				m_CameraStartTime = time;
			}

			doCameraMotion(t);
		} else {
			// set m_viewMatrix by calling createLookAtMatrix
			// with m_cameraPosition, m_cameraDirection, m_cameraUpVector
			// as parameter

			m_viewMatrix = Matrix::createLookAtMatrix(
				m_cameraPosition,
				m_cameraPosition + m_cameraDirection,
				m_cameraUpVector
				);
		}
	}

	void buildCameraPathFromViews(const vector<Matrix> &m_CameraViews)
	{
		// Delete Old Path
		m_CameraPositions.clear();
		m_CameraTangents.clear();
		m_CameraRot_q.clear();
		m_CameraRot_a.clear();
		m_CameraTimes.clear();

		float time = 0.0f;
		for(unsigned int i = 0; i < m_CameraViews.size(); i++) {
			Matrix inv = m_CameraViews[i].inversed();

			m_CameraPositions.push_back(
								Vector(
										inv[0][3],
										inv[1][3],
										inv[2][3],
										1
										)
								);
			m_CameraRot_q.push_back(Quaternion(m_CameraViews[i]));
			m_CameraTimes.push_back(time);
			time += 1.0f;
		}

		calcKochanekBartelsTagents(m_CameraPositions, m_CameraTangents, m_CameraTimes);
		calcInbetweenQuaternions(m_CameraRot_q, m_CameraRot_a, m_CameraTimes );
	}

	void doCameraMotion(float time)
	{
		m_cameraPosition = cubicInterpolate( m_CameraPositions, m_CameraTangents, m_CameraTimes, time);
		Quaternion rot = cubicQuaternion( m_CameraRot_q, m_CameraRot_a, m_CameraTimes, time);

		m_viewMatrix = rot.getRotMatrix() * Matrix::createTranslationMatrix(-m_cameraPosition);

		// Also update viewing direction so camera has the correct orientation
		// when stopping the camera animation:
		m_cameraDirection = rot.getRotMatrix().transposed() * Vector(0, 0, -1);

		m_cameraUpVector = Vector(0.f, 1.0f, 0.f);
	}


	//////////////////////////////////////////////////////////////////////////
	// Does interpolation with Cubic Hermite splines
	//////////////////////////////////////////////////////////////////////////
	Vector cubicInterpolate(const vector<Vector> &positions,
							const vector<Vector> &tangents,
							const vector<float> &times,
							float t,
							unsigned int i = 0)
	{
		assert( positions.size() == tangents.size() &&
				positions.size() == times.size() );
		assert( positions.size() > 1);
		assert( i >= 0 && i < times.size() );

		//find segment i
		if( t <= times.front() )
			return positions.front();

		if( t >= times.back() )
			return positions.back();

		//if i is wrong start from beginning;
		if(times[i] > t)
			i = 0;

		//find the current interval
		for(; i < times.size(); i++) {
			if( times[i] > t)
				break;
		}
		i = i - 1;

		//t in [0,1]
		t = (t - times[i]) / (times[i+1] - times[i]);

		float t3 = t*t*t;
		float t2 = t*t;

		return	positions[i]	* (2*t3 - 3*t2 +1 ) +
				tangents[i]		* (t3 - 2*t2 +t) +
				tangents[i+1]	* (t3-t2) +
				positions[i+1]	* ((-2)*t3 + 3*t2);
	}

	//////////////////////////////////////////////////////////////////////////
	// Does interpolation for quaternions
	//////////////////////////////////////////////////////////////////////////
	Quaternion cubicQuaternion(const vector<Quaternion> &qs,
							   const vector<Quaternion> &as,
							   const vector<float> &times,
							   float t,
							   unsigned int i = 0)
	{
		assert( qs.size() == as.size() &&
				qs.size() == times.size() );
		assert( qs.size() > 1);
		assert( i >= 0 && i < times.size() );

		//find segment i
		if( t <= times.front() )
			return qs.front();

		if( t >= times.back() )
			return qs.back();

		//if i is wrong start from beginning;
		if(times[i] > t)
			i = 0;

		//find the current interval
		for(; i < times.size(); i++) {
			if( times[i] > t)
				break;
		}
		i = i - 1;

		//t in [0,1]
		t = (t - times[i]) / (times[i+1] - times[i]);

		Quaternion res =	Quaternion::slerp(
							Quaternion::slerp( qs[i], qs[i+1], t),
							Quaternion::slerp( as[i], as[i+1], t),
							2*t*(1-t)
							);
		return	res;
	}

	//////////////////////////////////////////////////////////////////////////
	// Calculates the in between tangents which are used by cubicQuaternion
	// this gives smooth transitions from one segment to the next
	//////////////////////////////////////////////////////////////////////////
	void  calcInbetweenQuaternions(vector<Quaternion> &qs,
								   vector<Quaternion> &as,
								   const vector<float> &times)
	{
		assert( qs.size() > 1 );

		as.resize(qs.size());

		as.front() = qs.front();
		as.back()  = qs.back();
		for(unsigned int i = 1; i < as.size(); i++ ) {
			if( qs[i-1].dot(qs[i]) < 0.0 ) {
				qs[i] = -qs[i];
			}
		}

		for(unsigned int i = 1; i < as.size() - 1; i++ ) {
			Quaternion qi = qs[i].inversed();

			as[i] = qs[i] * Quaternion::qexp(
								(
									Quaternion::qlog(qi*qs[i-1]) +
									Quaternion::qlog(qi*qs[i+1])
								)* (-0.25)
							);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Calculates the in between tangents which are used by cubicInterpolate
	// Tangents are chosen according Kochanek-Bartels with the default
	// parameters(a = b = c = 0) which gives a Catmull-Rom spline
	//////////////////////////////////////////////////////////////////////////
	void calcKochanekBartelsTagents(const vector<Vector> &positions,
									 vector<Vector> &tangents,
									 const vector<float> &times)
	{
		assert( positions.size() > 1 );

		tangents.resize(positions.size());

		tangents.front() = positions[1] - positions[0];
		tangents.back()  = positions[positions.size()-1] - positions[positions.size()-2];

		//okay make it simple all parameters a = b = c = 0
		for(unsigned int i = 1; i < tangents.size() - 1; i++ ) {
			tangents[i] = (positions[i] - positions[i-1]) * 0.5 +
				(positions[i+1] - positions[i]) * 0.5;
			//weight with time
			tangents[i] = tangents[i] * ( 2*(times[i+1] - times[i])/ ( times[i] - times[i-1] + times[i+1] - times[i] ));
		}
	}
};

int main(int argc, char **argv)
{
	GlutApplication app(&argc, argv);

	app.setOpenGLVersion(3, 0);

	GlutWindow *window = new MaterialWindow();

	app.addWindow(window);

	app.Run();

	return 0;
}
