#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#define PI 3.141592
#define FRICTION 0.2;
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

struct BALL
{
	float x;
	float y;
	float angle;
	float sin;
	float cos;
	float force;
};

float camx = 0;
float camy = 100;
float camz = 100;

int choose = 0;

BALL blue_ball[5] = { { -60,-60 },{ 0,-60 },{ 60,-60 },{ -30,-30 },{ 30,-30 } };
BALL white_ball[5] = { { -60,60 },{ 0,60 },{ 60,60 },{ -30,30 },{ 30,30 } };

void Keyboard(unsigned char key, int x, int y);
void Timer(int value);
void Shot();


void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA|GLUT_DEPTH); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("Example2"); // ������ ���� (������ �̸�)
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(10, Timer, 1);
	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape);
	glutMainLoop();
}
// ������ ��� �Լ�
GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // �������� 'blue' �� ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camx, camy, camz, 0, 0, -300, 0, 1, 0);

	glPushMatrix();
	{
		glTranslatef(0, -20, 0);
		glColor3f(0.74, 0.62, 0.35);
		glScalef(1, 0.1, 1);
		glutSolidCube(200);
		glColor3f(0, 0, 0);
		glutWireCube(201);
	}
	glPopMatrix();	// �ٵ���

	glColor3f(0, 0, 0);

	glPushMatrix();
	{
		for (int i = 0; i < 19; ++i)
		{
			glBegin(GL_LINES);
			{
				glVertex3f(-90, -9, -90 + (10 * i));
				glVertex3f(90, -9, -90 + (10 * i));
			}
			glEnd();

			glBegin(GL_LINES);
			{
				glVertex3f(-90 + (10 * i), -9, -90);
				glVertex3f(-90 + (10 * i), -9, 90);
			}
			glEnd();
		}
	}
	glPopMatrix();	// �ٵ��� ��

	for (int i = 0; i < 5; ++i)
	{
		glColor3f(1, 1, 1);
		glPushMatrix();
		{
			glTranslatef(white_ball[i].x, 0, white_ball[i].y);
			glScalef(1, 0.5, 1);
			glutSolidSphere(5, 20, 20);
		}
		glPopMatrix();

		glColor3f(0, 0, 1);
		glPushMatrix();
		{
			glTranslatef(blue_ball[i].x, 0, blue_ball[i].y);
			glScalef(1, 0.5, 1);
			glutSolidSphere(5, 20, 20);
		}
		glPopMatrix();
	}	// �ٵϾ�

	glPushMatrix();
	{
		glColor3f(0, 1, 0);
		glTranslatef(white_ball[choose].x, 20, white_ball[choose].y);
		glRotatef(180, 1, 0, 0);
		glRotatef(-1*white_ball[choose].angle, 0, 1, 0);
		glScalef(1 + white_ball[choose].force, 1 + white_ball[choose].force, 1 + white_ball[choose].force);
		glutSolidCone(5, 10, 10, 10);
	}
	glPopMatrix();


	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, w / h, 1.0, 1000.0);
	glTranslatef(0.0, 0.0, -300.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		camy += 3;
		break;
	case 's':
		camy -= 3;
		break;
	case 'a':
		camx -= 3;
		break;
	case 'd':
		camx += 3;
		break;

	case '1':
		choose = 0;
		break;
	case '2':
		choose = 1;
		break;
	case '3':
		choose = 2;
		break;
	case '4':
		choose = 3;
		break;
	case '5':
		choose = 4;
		break;

	case 'q':
		white_ball[choose].angle += 1;
		white_ball[choose].sin = sin(PI / 180 * white_ball[choose].angle);
		white_ball[choose].cos = cos(PI / 180 * white_ball[choose].angle);
		break;
	case 'e':
		white_ball[choose].angle -= 1;
		white_ball[choose].sin = sin(PI / 180 * white_ball[choose].angle);
		white_ball[choose].cos = cos(PI / 180 * white_ball[choose].angle);
		break;
	case ' ':
		white_ball[choose].force += 0.01;
		break;

	case 'f':
		Shot();
		break;
	}
	glutPostRedisplay();
}

void Timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(10, Timer, 1);
}
void Shot()
{
	while (white_ball[choose].force > 0)
	{
		white_ball[choose].x -= white_ball[choose].force * white_ball[choose].sin;
		white_ball[choose].y -= white_ball[choose].force * white_ball[choose].cos;
		white_ball[choose].force -= 0.01*FRICTION;
		printf("%f %f\n", white_ball[choose].x, white_ball[choose].y);
		glutPostRedisplay();
	}
}