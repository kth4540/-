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
	float z;
	float angle;
	float sin;
	float cos;
	float force;
};

float width;
float height;

float grv = 1;

float Wzoom = 100;
float Wyangle = 135;
float Wcamangle = 0;
float Bzoom = 100;
float Byangle = 135;
float Bcamangle = 180;
int choose = 0;

float block_x[4];
float block_y[4];


BALL blue_ball[5] = { { -60,-60,-7.5 },{ 0,-60,-7.5 },{ 60,-60,-7.5 },{ -30,-60,-7.5 },{ 30,-60,-7.5 } };
BALL white_ball[5] = { { -60,60,-7.5 },{ 0,60,-7.5 },{ 60,60,-7.5 },{ -30,60,-7.5 },{ 30,60,-7.5 } };

bool white_death[5];
bool blue_death[5];

bool white_shot_check[5];
bool blue_shot_check[5];

bool turn = false;
bool mode = false;
void Keyboard(unsigned char key, int x, int y);
void Timer(int value);


float GetAngle(int ball1, int ball2);
float Same_GetAngle(int ball1, int ball2);
float Block_GetAngle(int ball, int block);

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

	if (mode == false)
	{
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-400, 400, -300, 300, -800, 800);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glColor3f(0.74, 0.62, 0.35);
		glRectf(-100, -100, 100, 100);

		glColor3f(0, 0, 0);
		glLineWidth(1);
		for (int i = 0; i < 19; ++i)
		{
			glBegin(GL_LINES);
			{
				glVertex3f(-90, 90 - (10 * i), 10);
				glVertex3f(90, 90 - (10 * i), 10);
			}
			glEnd();

			glBegin(GL_LINES);
			{
				glVertex3f(-90 + (10 * i), 90, 10);
				glVertex3f(-90 + (10 * i), -90, 10);
			}
			glEnd();

		}

		glColor3f(1, 0, 0);
		glLineWidth(10);
		glBegin(GL_LINE_STRIP);
		{
			glVertex3f(-40, 40, 20);
			glVertex3f(40, 40, 20);
			glVertex3f(40, -40, 20);
			glVertex3f(-40, -40, 20);
			glVertex3f(-40, 40, 20);
		}
		glEnd();
		glColor3f(1, 1, 0);
		for (int i = 0; i < 4; ++i)
		{
			glPushMatrix();
			glTranslatef(block_x[i], block_y[i], 30);
			glutSolidSphere(6, 6, 6);
			glPopMatrix();
		}

	}
	else if (mode == true)
	{
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0f, width / height, 1.0, 1000.0);
		glTranslatef(0.0, 0.0, -300.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (turn == 0)
		{
			gluLookAt((Wzoom*sin(PI / 180 * Wcamangle)), (Wzoom * sin(PI / 180 * Wyangle)), (Wzoom* cos(PI / 180 * Wcamangle)), 0, 0, 0, 0, 1, 0);
		}
		else if (turn == 1)
		{
			gluLookAt((Bzoom*sin(PI / 180 * Bcamangle)), (Bzoom * sin(PI / 180 * Byangle)), (Bzoom* cos(PI / 180 * Bcamangle)), 0, 0, 0, 0, 1, 0);
		}

		GLfloat AmbientLight2[] = { 0.2f,0.2f,0.2f, 0.2f }; // 녹색조명
		GLfloat DiffuseLight2[] = { 0.2f,0.2f,0.2f, 0.2f }; // 적색조명
		GLfloat SpecularLight2[] = { 0.2f, 0.2f, 0.2f, 0.2f }; // 백색조명
		GLfloat lightPos2[] = { 0, 100, 0, 1.0 }; // 위치: (10, 5, 20)

		glEnable(GL_LIGHTING);

		glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight2);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLight2);
		glLightfv(GL_LIGHT1, GL_SPECULAR, SpecularLight2);
		glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);

		glEnable(GL_LIGHT1);

		glPushMatrix();
		{
			glTranslatef(0, -20, 0);
			glColor3f(0.74, 0.62, 0.35);
			glScalef(1, 0.1, 1);
			glutSolidCube(200);
			glColor3f(0, 0, 0);
			glutWireCube(201);
		}
		glPopMatrix();	// 바둑판

		glColor3f(0, 0, 0);
		glLineWidth(1);
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
		glPopMatrix();	// 바둑판 줄

		for (int i = 0; i < 5; ++i)
		{
			glColor3f(1, 1, 1);
			glPushMatrix();
			{
				glTranslatef(white_ball[i].x, white_ball[i].z, white_ball[i].y);
				glScalef(1, 0.5, 1);
				glutSolidSphere(5, 20, 20);
			}
			glPopMatrix();

			glColor3f(0, 0, 1);
			glPushMatrix();
			{
				glTranslatef(blue_ball[i].x, blue_ball[i].z, blue_ball[i].y);
				glScalef(1, 0.5, 1);
				glutSolidSphere(5, 20, 20);
			}
			glPopMatrix();
		}	// 바둑알

		glPushMatrix();
		{
			if (turn == false)
				glColor3f(1, 0, 0);
			else
				glColor3f(0, 1, 0);
			if (turn == false)
				glTranslatef(white_ball[choose].x, 20, white_ball[choose].y);
			else
				glTranslatef(blue_ball[choose].x, 20, blue_ball[choose].y);
			if (turn == false)
				glRotatef(180, 1, 0, 0);

			if (turn == false)
				glRotatef(-1 * white_ball[choose].angle, 0, 1, 0);
			else
				glRotatef(blue_ball[choose].angle, 0, 1, 0);
			if (turn == false)
				glScalef(1 + white_ball[choose].force, 1 + white_ball[choose].force, 1 + white_ball[choose].force);
			else
				glScalef(1 + blue_ball[choose].force, 1 + blue_ball[choose].force, 1 + blue_ball[choose].force);
			glutSolidCone(5, 10, 10, 10);
		}
		glPopMatrix();

		glColor3f(1, 1, 0);
		for (int i = 0; i < 4; ++i)
		{
			glPushMatrix();
			glTranslatef(block_x[i], 20, block_y[i]);
			glRotatef(90, 1, 0, 0);
			glutSolidCylinder(2.5, 30, 15, 15);
			glPopMatrix();
		}
	}


	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h)
{
	width = w;
	height = h;
	glEnable(GL_COLOR_MATERIAL);
	for (int i = 0; i < 5; ++i)
	{
		blue_shot_check[i] = false;
		white_shot_check[i] = false;

		white_ball[i].angle = 0;
		white_ball[i].cos = 1;
		white_ball[i].sin = 0;
		white_ball[i].force = 0;

		blue_ball[i].angle = 0;
		blue_ball[i].cos = 1;
		blue_ball[i].sin = 0;
		blue_ball[i].force = 0;
		white_death[i] = false;
		blue_death[i] = false;
	}

	for (int i = 0; i < 4; ++i)
	{
		block_x[i] = rand() % 80 - 40;
		block_y[i] = rand() % 80 - 40;
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '+':
		if (turn == 0)
		{
			Wzoom -= 5;
		}
		else if (turn == 1)
		{
			Bzoom -= 5;
		}
		break;
	case '-':
		if (turn == 0)
		{
			Wzoom += 5;
		}
		else if (turn == 1)
		{
			Bzoom += 5;
		}
		break;
	case 'w':
		if (turn == 0)
		{
			Wyangle += 3;
		}
		else if (turn == 1)
		{
			Byangle += 3;
		}
		break;
	case 's':
		if (turn == 0)
		{
			Wyangle -= 3;
		}
		else if (turn == 1)
		{
			Byangle -= 3;
		}
		break;
	case 'a':
		if (turn == 0)
		{
			Wcamangle -= 3;
		}
		else if (turn == 1)
		{
			Bcamangle -= 3;
		}
		break;
	case 'd':
		if (turn == 0)
		{
			Wcamangle += 3;
		}
		else if (turn == 1)
		{
			Bcamangle += 3;
		}
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
		if (turn == false)
		{
			white_ball[choose].angle += 3;
			white_ball[choose].sin = sin(PI / 180 * white_ball[choose].angle);
			white_ball[choose].cos = cos(PI / 180 * white_ball[choose].angle);
		}
		else
		{
			blue_ball[choose].angle += 3;
			blue_ball[choose].sin = sin(PI / 180 * blue_ball[choose].angle);
			blue_ball[choose].cos = cos(PI / 180 * blue_ball[choose].angle);
		}
		break;
	case 'e':
		if (turn == false)
		{
			white_ball[choose].angle -= 3;
			white_ball[choose].sin = sin(PI / 180 * white_ball[choose].angle);
			white_ball[choose].cos = cos(PI / 180 * white_ball[choose].angle);
		}
		else
		{
			blue_ball[choose].angle -= 3;
			blue_ball[choose].sin = sin(PI / 180 * blue_ball[choose].angle);
			blue_ball[choose].cos = cos(PI / 180 * blue_ball[choose].angle);
		}
		break;
	case ' ':
		if (turn == false)
			white_ball[choose].force += 0.15;
		else
			blue_ball[choose].force += 0.15;
		break;

	case 'f':
		if (turn == false && white_death[choose] == false)
		{
			white_shot_check[choose] = true;
		}
		else if (turn == true && blue_death[choose] == false)
		{
			blue_shot_check[choose] = true;
		}
		break;

	case '=':
		if (mode == false)
			mode = true;
		break;
	case 'r':
		for (int i = 0; i < 4; ++i)
		{
			block_x[i] = rand() % 80 - 40;
			block_y[i] = rand() % 80 - 40;
		}
	}

	glutPostRedisplay();
}




void Timer(int value)
{
	for (int i = 0; i < 5; ++i)
	{

		if (white_shot_check[i] == true)
		{
			white_ball[i].x -= white_ball[i].force * white_ball[i].sin;
			white_ball[i].y -= white_ball[i].force * white_ball[i].cos;
			white_ball[i].force -= 0.01*FRICTION;
			if (white_ball[i].force <= 0)
			{
				white_shot_check[i] = false;
				if (i == choose)
				{
					turn = true;
					for (int i = 0; i < 5; ++i)
					{
						blue_shot_check[i] = false;
						white_shot_check[i] = false;

						white_ball[i].angle = 0;
						white_ball[i].cos = 1;
						white_ball[i].sin = 0;
						white_ball[i].force = 0;

						blue_ball[i].angle = 0;
						blue_ball[i].cos = 1;
						blue_ball[i].sin = 0;
						blue_ball[i].force = 0;
					}
				}
			}
		}
		if (blue_shot_check[i] == true)
		{
			blue_ball[i].x += blue_ball[i].force * blue_ball[i].sin;
			blue_ball[i].y += blue_ball[i].force * blue_ball[i].cos;
			blue_ball[i].force -= 0.01*FRICTION;
			if (blue_ball[i].force <= 0)
			{
				blue_shot_check[i] = false;
				if (i == choose)
				{
					turn = false;
					for (int i = 0; i < 5; ++i)
					{
						blue_shot_check[i] = false;
						white_shot_check[i] = false;

						white_ball[i].angle = 0;
						white_ball[i].cos = 1;
						white_ball[i].sin = 0;
						white_ball[i].force = 0;

						blue_ball[i].angle = 0;
						blue_ball[i].cos = 1;
						blue_ball[i].sin = 0;
						blue_ball[i].force = 0;
					}
				}
			}
		}

		if (turn == false)
		{
			for (int j = 0; j < 5; ++j)
			{
				if (pow(white_ball[i].x - blue_ball[j].x, 2) + pow(white_ball[i].y - blue_ball[j].y, 2) < 100)
				{
					blue_ball[j].force = white_ball[i].force;
					blue_ball[j].cos = sin(GetAngle(i, j));
					blue_ball[j].sin = cos(GetAngle(i, j));
					blue_shot_check[j] = true;
				}

				if (i != j && pow(white_ball[i].x - white_ball[j].x, 2) + pow(white_ball[i].y - white_ball[j].y, 2) < 100)
				{
					white_ball[i].cos = sin(Same_GetAngle(i, j));
					white_ball[i].sin = cos(Same_GetAngle(i, j));
				}

			}

			for (int j = 0; j < 4; ++j)
			{
				if (pow(white_ball[i].x - block_x[j], 2) + pow(white_ball[i].y - block_y[j], 2) < 100)
				{
					white_ball[i].cos = -sin(Block_GetAngle(i, j));
					white_ball[i].sin = -cos(Block_GetAngle(i, j));
				}

			}

		}
		else if (turn == true)
		{
			for (int j = 0; j < 5; ++j)
			{
				if (pow(blue_ball[i].x - white_ball[j].x, 2) + pow(blue_ball[i].y - white_ball[j].y, 2) < 100)
				{
					white_ball[j].force = blue_ball[i].force;
					white_ball[j].cos = sin(GetAngle(j, i));
					white_ball[j].sin = cos(GetAngle(j, i));
					white_shot_check[j] = true;
				}
				if (i != j && pow(blue_ball[i].x - blue_ball[j].x, 2) + pow(blue_ball[i].y - blue_ball[j].y, 2) < 100)
				{
					blue_ball[i].cos = sin(Same_GetAngle(j, i));
					blue_ball[i].sin = cos(Same_GetAngle(j, i));
				}
			}
			for (int j = 0; j < 4; ++j)
			{
				if (pow(blue_ball[i].x - block_x[j], 2) + pow(blue_ball[i].y - block_y[j], 2) < 100)
				{
					blue_ball[i].cos = sin(Block_GetAngle(i, j));
					blue_ball[i].sin = cos(Block_GetAngle(i, j));
				}

			}

		}
		if ((white_ball[i].x > 100) || (white_ball[i].x < -100) || (white_ball[i].y > 100) || (white_ball[i].y < -100))
		{
			if (white_ball[i].z > -600)
			{
				white_ball[i].z -= pow(grv, 2.0);
				grv += 0.07f;
				if (white_ball[i].z <= -600)
				{
					white_death[i] = true;

				}
			}

		}

		if ((blue_ball[i].x > 100) || (blue_ball[i].x < -100) || (blue_ball[i].y > 100) || (blue_ball[i].y < -100))
		{
			if (blue_ball[i].z > -600)
			{
				blue_ball[i].z -= pow(grv, 2.0);
				grv += 0.07f;
				if (white_ball[i].z <= -600)
				{

					blue_death[i] = true;

				}
			}

		}
	}



	glutPostRedisplay();
	glutTimerFunc(10, Timer, 1);
}

float GetAngle(int ball1, int ball2)
{
	float angle;
	float deltaX = blue_ball[ball2].x - white_ball[ball1].x;
	float deltaY = blue_ball[ball2].y - white_ball[ball1].y;
	angle = acosf(deltaX / 10);

	if (deltaY < 0)
		angle = 3.14 + (3.14 - angle);

	return angle;
}

float Same_GetAngle(int ball1, int ball2)
{
	float angle;
	float deltaX;
	float deltaY;
	if (turn == false)
	{
		deltaX = white_ball[ball2].x - white_ball[ball1].x;
		deltaY = white_ball[ball2].y - white_ball[ball1].y;
	}
	else if (turn == true)
	{
		deltaX = blue_ball[ball2].x - blue_ball[ball1].x;
		deltaY = blue_ball[ball2].y - blue_ball[ball1].y;
	}
	angle = acosf(deltaX / 10);

	if (deltaY < 0)
		angle = 3.14 + (3.14 - angle);

	return angle;
}

float Block_GetAngle(int ball, int block)
{
	float angle;
	float deltaX;
	float deltaY;
	if (turn == false)
	{
		deltaX = white_ball[ball].x - block_x[block];
		deltaY = white_ball[ball].y - block_y[block];
	}
	else if (turn == true)
	{
		deltaX = blue_ball[ball].x - block_x[block];
		deltaY = blue_ball[ball].y - block_y[block];
	}
	angle = acosf(deltaX / 10);

	if (deltaY < 0)
		angle = 3.14 + (3.14 - angle);

	return angle;
}