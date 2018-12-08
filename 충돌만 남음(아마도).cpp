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

float Wzoom = 100;
float Wyangle = 135;
float Wcamangle = 0;
float Bzoom = 100;
float Byangle = 135;
float Bcamangle = 180;

float grv = 1;

float t_angle = 0;
float pre_force = 0;

int choose = 0;
bool shot_check = 0;
bool falling = 0;

int turn = 0;	// 0 = 흰돌 1 = 파랑돌

int Wcount = 0;
int Bcount = 0;

BALL blue_ball[5] = { { -60,-60, -7.5 },{ 0,-60, -7.5 },{ 60,-60, -7.5 },{ -30,-30, -7.5 },{ 30,-30, -7.5 } };
BALL white_ball[5] = { { -60,60, -7.5 },{ 0,60, -7.5 },{ 60,60, -7.5 },{ -30,30, -7.5 },{ 30,30, -7.5 } };

void Keyboard(unsigned char key, int x, int y);
void Timer(int value);


bool white_shot_check[5];
bool blue_shot_check[5];

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
	if (turn == 0)
	{
		gluLookAt((Wzoom*sin(PI / 180 * Wcamangle)), (Wzoom * sin(PI / 180 * Wyangle)), (Wzoom* cos(PI / 180 * Wcamangle)), 0, 0, 0, 0, 1, 0);
	}
	else if (turn == 1)
	{
		gluLookAt((Bzoom*sin(PI / 180 * Bcamangle)), (Bzoom * sin(PI / 180 * Byangle)), (Bzoom* cos(PI / 180 * Bcamangle)), 0, 0, 0, 0, 1, 0);
	}

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
		if (turn == false)
			white_shot_check[choose] = true;
		else
			blue_shot_check[choose] = true;
		break;
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
				turn = 1;
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
				turn = 0;
			}
		}

		for (int j = 0; j < 5; ++j)
		{
			if ((pow(white_ball[i].x - blue_ball[j].x, 2) + pow(white_ball[i].y - blue_ball[j].y, 2)) < 100)
			{
					pre_force = white_ball[i].force;
					t_angle = asin((-(white_ball[i].y - blue_ball[j].y)) / 10);
					white_ball[i].sin = sin(PI - ((2*white_ball[i].angle) + t_angle));
					white_ball[i].cos = cos(PI - ((2 * white_ball[i].angle) + t_angle));
					blue_ball[j].sin = sin((-(white_ball[i].y - blue_ball[j].y)) / 10);
					blue_ball[j].cos = cos((-(white_ball[i].x - blue_ball[j].x)) / 10);
					blue_ball[j].force = pre_force * cos((PI) - (t_angle + white_ball[i].angle));
					white_ball[i].force = pre_force * sin((PI) - (t_angle + white_ball[i].angle));
					pre_force = 0;
					blue_shot_check[j] = true;
					printf("파랑돌 출발\n");
	
				//else if ((white_shot_check[i] == false) && (blue_shot_check[j] == true))
				//{
				//	t_angle = asin((-(blue_ball[j].y - white_ball[i].y)) / 10);
				//	blue_ball[j].sin = sin(PI / 2 - (blue_ball[j].angle + (2 * t_angle)));
				//	blue_ball[j].cos = cos(PI / 2 - (blue_ball[j].angle + (2 * t_angle)));
				//	white_ball[i].sin = sin((-(blue_ball[j].y - white_ball[i].y)) / 10);
				//	white_ball[i].cos = cos((-(blue_ball[j].y - white_ball[i].y)) / 10);
				//	blue_ball[j].force = pre_force * sin((PI / 2) - (t_angle + white_ball[i].angle));
				//	white_ball[i].force = pre_force * cos((PI / 2) - (t_angle + white_ball[i].angle));
				//	pre_force = 0;
				//	white_shot_check[i] = true;
				//}
				
			}

			if ((pow(blue_ball[i].x - white_ball[j].x, 2) + pow(blue_ball[i].y - white_ball[j].y, 2)) < 100)
			{
				//if ((white_shot_check[j] == true) && (blue_shot_check[i] == false))
				//{
				//	t_angle = asin((-(white_ball[j].y - blue_ball[i].y)) / 10);
				//	white_ball[j].sin = sin(PI / 2 - (white_ball[j].angle + (2 * t_angle)));
				//	white_ball[j].cos = cos(PI / 2 - (white_ball[j].angle + (2 * t_angle)));
				//	blue_ball[i].sin = sin((-(white_ball[j].y - blue_ball[i].y)) / 10);
				//	blue_ball[i].cos = cos((-(white_ball[j].y - blue_ball[i].y)) / 10);
				//	blue_ball[i].force = pre_force * cos((PI / 2) - (t_angle + white_ball[j].angle));
				//	white_ball[j].force = pre_force * sin((PI / 2) - (t_angle + white_ball[j].angle));
				//	pre_force = 0;
				//	blue_shot_check[i] = true;
				//}
				if ((white_shot_check[j] == false) && (blue_shot_check[i] == true))
				{
					t_angle = asin((-(blue_ball[i].y - white_ball[j].y)) / 10);
					blue_ball[i].sin = sin(PI / 2 - (blue_ball[i].angle + (2 * t_angle)));
					blue_ball[i].cos = cos(PI / 2 - (blue_ball[i].angle + (2 * t_angle)));
					white_ball[j].sin = sin((-(blue_ball[i].y - white_ball[j].y)) / 10);
					white_ball[j].cos = cos((-(blue_ball[i].y - white_ball[j].y)) / 10);
					blue_ball[i].force = pre_force * sin((PI / 2) - (t_angle + white_ball[j].angle));
					white_ball[j].force = pre_force * cos((PI / 2) - (t_angle + white_ball[j].angle));
					pre_force = 0;
					white_shot_check[j] = true;
				}
			}
		}
	}
	glutPostRedisplay();
	glutTimerFunc(10, Timer, 1);
}
/*void Shot()
{
	if (turn == 0)
	{
		if (white_ball[choose].force > 0)
		{
			white_ball[choose].x -= white_ball[choose].force * white_ball[choose].sin;
			white_ball[choose].y -= white_ball[choose].force * white_ball[choose].cos;
			white_ball[choose].force -= 0.01*FRICTION;
			printf("%f %f\n", white_ball[choose].x, white_ball[choose].y);
			if ((white_ball[choose].x > 100) || (white_ball[choose].x < -100) || (white_ball[choose].y > 100) || (white_ball[choose].y < -100))
			{
				falling = 1;
				white_ball[choose].z -= pow(grv, 2.0);
				grv += 0.07f;
				if (white_ball[choose].z < -500)
				{
					++Wcount;
					falling = 0;
				}
			}
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
			if ((blue_ball[choose].x > 100) || (blue_ball[choose].x < -100) || (blue_ball[choose].y > 100) || (blue_ball[choose].y < -100))
			{
				falling = 1;
				blue_ball[choose].z -= pow(grv, 2.0);
				grv += 0.07f;
				if (blue_ball[choose].z < -500)
				{
					falling = 0;
					++Bcount;
				}
			}
		}
	}
}
*/

//낙하까진 끝