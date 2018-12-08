#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#define PI 3.141592
#define FRICTION 2;
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

float zoom = 100;
float yangle = 135;
float camangle = 0;
float camx = 0;
float camy = 100;
float camz = 100;

int choose = 0;
bool shot_check = 0;

int turn = 0;	// 0 = 흰돌 1 = 파랑돌

BALL blue_ball[5] = { { -60,-60 },{ 0,-60 },{ 60,-60 },{ -30,-30 },{ 30,-30 } };
BALL white_ball[5] = { { -60,60 },{ 0,60 },{ 60,60 },{ -30,30 },{ 30,30 } };

void Keyboard(unsigned char key, int x, int y);
void Timer(int value);
void Shot();


void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("Example2"); // 윈도우 생성 (윈도우 이름)
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(10, Timer, 1);
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape);
	glutMainLoop();
}
// 윈도우 출력 함수
GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 바탕색을 'blue' 로 지정
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt((zoom*sin(PI/180*camangle)), (zoom * sin(PI / 180 * yangle)), (zoom* cos(PI / 180 * camangle)), 0, 0, 0, 0, 1, 0);

	glPushMatrix();
	{
		glTranslatef(0, -20, 0);
		glColor3f(0.74, 0.62, 0.35);
		glScalef(1, 0.1, 1);
		glutSolidCube(200);
		glColor3f(0, 0, 0);
		glutWireCube(201);
	}
	glPopMatrix();   // 바둑판

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
	glPopMatrix();   // 바둑판 줄

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
	}   // 바둑알

	glPushMatrix();
	{
		if (turn == 0)
		{
			glColor3f(0, 1, 0);
			glTranslatef(white_ball[choose].x, 20, white_ball[choose].y);
			glRotatef(180, 1, 0, 0);
			glRotatef(-1 * white_ball[choose].angle, 0, 1, 0);
			glScalef(1 + (0.5*white_ball[choose].force), 1 + (0.5*white_ball[choose].force), 1 + (0.5*white_ball[choose].force));
			glutSolidCone(5, 10, 10, 10);
		}
		else if (turn == 1)
		{
			glColor3f(0, 1, 0);
			glTranslatef(blue_ball[choose].x, 20, blue_ball[choose].y);
			glRotatef(180, 1, 0, 0);
			glRotatef(-1 * blue_ball[choose].angle, 0, 1, 0);
			glScalef(1 + (0.5*blue_ball[choose].force), 1 + (0.5*blue_ball[choose].force), 1 + (0.5*blue_ball[choose].force));
			glutSolidCone(5, 10, 10, 10);
		}
	}
	glPopMatrix();


	glutSwapBuffers(); // 화면에 출력하기
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
	case '+':
		zoom -= 2;
		break;
	case '-':
		zoom += 2;
		break;
	case 'w':
		yangle += 3;
		break;
	case 's':
		yangle -= 3;
		break;
	case 'a':
		camangle -= 3;
		break;
	case 'd':
		camangle += 3;
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
		if (turn == 0)
		{
			white_ball[choose].angle += 2;
			white_ball[choose].sin = sin(PI / 180 * white_ball[choose].angle);
			white_ball[choose].cos = cos(PI / 180 * white_ball[choose].angle);
		}
		else if (turn == 1)
		{
			blue_ball[choose].angle += 2;
			blue_ball[choose].sin = sin(PI / 180 * blue_ball[choose].angle);
			blue_ball[choose].cos = cos(PI / 180 * blue_ball[choose].angle);
		}
		break;
	case 'e':
		if(turn == 0)
		{
			white_ball[choose].angle -= 2;
			white_ball[choose].sin = sin(PI / 180 * white_ball[choose].angle);
			white_ball[choose].cos = cos(PI / 180 * white_ball[choose].angle);
		}
		else if (turn == 1)
		{
			blue_ball[choose].angle -= 2;
			blue_ball[choose].sin = sin(PI / 180 * blue_ball[choose].angle);
			blue_ball[choose].cos = cos(PI / 180 * blue_ball[choose].angle);
		}
		break;
	case ' ':
		if (turn == 0)
		{
			white_ball[choose].force += 0.3;
		}
		else if (turn == 1)
		{
			blue_ball[choose].force += 0.3;
		}
		break;
	case 'f':
		shot_check = 1;
		break;
	}
	glutPostRedisplay();
}

void Timer(int value)
{
	if (shot_check == 1)
	{
		if (turn == 0)
		{
			Shot();
			if (white_ball[choose].force <= 0)
			{
				shot_check = 0;
				turn = 1;
			}
		}
		else if (turn == 1)
		{
			Shot();
			if (blue_ball[choose].force <= 0)
			{
				shot_check = 0;
				turn = 0;
			}
		}
	}
	glutPostRedisplay();
	glutTimerFunc(10, Timer, 1);
}
void Shot()
{
	if (turn == 0)
	{
		if (white_ball[choose].force > 0)
		{
			white_ball[choose].x -= white_ball[choose].force * white_ball[choose].sin;
			white_ball[choose].y -= white_ball[choose].force * white_ball[choose].cos;
			white_ball[choose].force -= 0.01*FRICTION;
			printf("%f %f\n", white_ball[choose].x, white_ball[choose].y);
		}
	}
	else if (turn == 1)
	{
		if (blue_ball[choose].force > 0)
		{
			blue_ball[choose].x -= blue_ball[choose].force * blue_ball[choose].sin;
			blue_ball[choose].y -= blue_ball[choose].force * blue_ball[choose].cos;
			blue_ball[choose].force -= 0.01*FRICTION;
			printf("%f %f\n", blue_ball[choose].x, blue_ball[choose].y);
		}
	}
}

