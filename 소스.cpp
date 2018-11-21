#include <GL/freeglut.h>
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

struct BALL
{
	int x;
	int y;
};

BALL blue_ball[5] = { {-60,-60},{0,-60},{60,-60},{-30,-30},{30,-30} };
BALL white_ball[5] = { { -60,60 },{ 0,60 },{ 60,60 },{ -30,30 },{ 30,30 } };

void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA|GLUT_DEPTH); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ����
	glutInitWindowSize(1280, 720); // �������� ũ�� ����
	glutCreateWindow("Example2"); // ������ ���� (������ �̸�)
	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape);
	glutMainLoop();
}
// ������ ��� �Լ�
GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // �������� 'blue' �� ����
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
	glEnable(GL_DEPTH_TEST);

	glPushMatrix();
	{
		gluLookAt(0, 100, 100, 0, 0, -300, 0, 1, 0);
		glTranslatef(0, -50, 0);
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
		gluLookAt(0, 100, 100, 0, 0, -300, 0, 1, 0);
		for (int i = 0; i < 19; ++i)
		{
			glBegin(GL_LINES);
			{
				glVertex3f(-90, -39, -90 + (10 * i));
				glVertex3f(90, -39, -90 + (10 * i));
			}
			glEnd();

			glBegin(GL_LINES);
			{
				glVertex3f(-90 + (10 * i), -39, -90);
				glVertex3f(-90 + (10 * i), -39, 90);
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
			gluLookAt(0, 100, 100, 0, 0, -300, 0, 1, 0);
			glTranslatef(white_ball[i].x, -30, white_ball[i].y);
			glutSolidSphere(5, 20, 20);
		}
		glPopMatrix();

		glColor3f(0, 0, 1);
		glPushMatrix();
		{
			gluLookAt(0, 100, 100, 0, 0, -300, 0, 1, 0);
			glTranslatef(blue_ball[i].x, -30, blue_ball[i].y);
			glutSolidSphere(5, 20, 20);
		}
		glPopMatrix();
	}




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