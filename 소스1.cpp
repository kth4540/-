#include <GL/freeglut.h>
#include <gl/glut.h>
#include <stdio.h>
#include <math.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);

#define FRICTION 20;
#define PI 3.1418926535897

typedef struct stone
{
	float color; //1-��� 2-�Ķ�
	float x;
	float y;
	float angle;
	float sin;
	float cos;
	float force;
}stone;

void Load(stone sel_stone);			//���� sin, cos�� ���
void shot(stone move_stone);		//�� �̵�
void check_collide(struct stone stone1, struct stone stone2);	//�浹üũ
void collide(stone col_stone1, stone col_stone2);				//�浹�� �� ��ȭ

struct stone stones[10] = { {1,  -60, 60,  -90, NULL, NULL, 0 },{ 1, 0, 60, -90, NULL, NULL, 0 },{1, 60, 60, -90, NULL, NULL ,0 },{ 1, -30, 30, -90, NULL, NULL, 0 },{1,  30, 30, -90, NULL, NULL, 0 },
{2,  -60, -60,  90, NULL, NULL, 0 },{2,  0, -60, 90, NULL, NULL, 0 },{ 2, 60, -60, 90, NULL, NULL ,0 },{2,  -30, -30, 90, NULL, NULL, 0 },{2,  30, -30, 90, NULL, NULL, 0 } };
//�� �⺻����


struct stone blue_stone[5] = { {-60, -60,  90, NULL, NULL, 0}, {0, -60, 90, NULL, NULL, 0}, {60, -60, 90, NULL, NULL ,0}, {-30, -30, 90, NULL, NULL, 0}, {30, -30, 90, NULL, NULL, 0} };
struct stone white_stone[5] = { { -60, 60,  -90, NULL, NULL, 0 },{ 0, 60, -90, NULL, NULL, 0 },{ 60, 60, -90, NULL, NULL ,0 },{ -30, 30, -90, NULL, NULL, 0 },{ 30, 30, -90, NULL, NULL, 0 } };

int n = 999; //�������

void Load(struct stone sel_stone)
{
	sel_stone.sin = sin(sel_stone.angle * (PI / 180));
	printf("���� ������ : sin %f\n", sel_stone.sin);
	sel_stone.cos = cos(sel_stone.angle * (PI / 180));
	printf("���� ������ : cos %f\n", sel_stone.cos);
	shot(sel_stone);
	printf("�߻��Լ� �����\n");
};		//���� sin, cos�� ����

void shot(struct stone move_stone)
{
	printf("�� �̵�\n");
	while (move_stone.force > 0)
	{
		move_stone.x = move_stone.x + ((((move_stone.force) / 10))*move_stone.cos);
		printf("x��ǥ �̵� : %f", move_stone.x);
		move_stone.y = move_stone.y + ((((move_stone.force) / 10))*move_stone.sin);
		printf("y��ǥ �̵� : %f", move_stone.y);
		for (int i = 0; i < 10; ++i)
		{
			if ((move_stone.x == stones[i].x) && (move_stone.y == stones[i].y))
			{
				++i;
			}										//�ڱ��ڽŰ��� �񱳴� �н�
			check_collide(move_stone, stones[i]);	//�浹üũ
		}
		move_stone.force = move_stone.force - FRICTION;
	}
};		//�� �߻�

void check_collide(struct stone stone1, struct stone stone2)
{
	if ((((stone1.x - stone2.x)*(stone1.x - stone2.x)) + ((stone1.y - stone2.y)*(stone1.y - stone2.y))) < 400)
	{
		collide(stone1, stone2);
	}
}	//�浹üũ

void collide(struct stone col_stone1, struct stone col_stone2)
{
	float pre_force = col_stone2.force;
	col_stone1.sin = (-(col_stone2.y - col_stone1.y)) / 20;
	col_stone1.cos = (-(col_stone2.x - col_stone1.x)) / 20;
	float n_angle = asin(col_stone1.sin);
	col_stone2.sin = sin((PI / 2) - (n_angle + (2 * col_stone2.angle)));
	col_stone2.cos = cos((PI / 2) - (n_angle + (2 * col_stone2.angle)));
	col_stone1.force = pre_force * cos((PI / 2) - (n_angle + col_stone2.angle));
	col_stone2.force = pre_force * sin((PI / 2) - (n_angle + col_stone2.angle));
	shot(col_stone1);
	shot(col_stone2);
};	//�浹�� ��ȭ���


void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA|GLUT_DEPTH); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ����
	glutInitWindowSize(1280, 720); // �������� ũ�� ����
	glutCreateWindow("Example2"); // ������ ���� (������ �̸�)
	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}
// ������ ��� �Լ�
GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // �������� 'black' �� ����
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
			glTranslatef(stones[i].x, -30, stones[i].y);
			glutSolidSphere(5, 20, 20);
		}
		glPopMatrix();//��� ��

		glColor3f(0, 0, 1);
		glPushMatrix();
		{
			gluLookAt(0, 100, 100, 0, 0, -300, 0, 1, 0);
			glTranslatef(stones[i+5].x, -30, stones[i+5].y);
			glutSolidSphere(5, 20, 20);
		}
		glPopMatrix();//�Ķ� ��
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

void Keyboard(unsigned char key, int x, int y)
{
	struct stone sel_stone = { 0, 0, 0, NULL, NULL, 0 };

	switch (key)
	{
	case '1':
		n = 0;
		sel_stone = stones[0];
		printf("��� 1���� ���õ�\n");
		break;
	case '2':
		n = 1;
		sel_stone = stones[1];
		printf("��� 2���� ���õ�\n");
		break;
	case '3':
		n = 2;
		sel_stone = stones[2];
		printf("��� 3���� ���õ�\n");
		break;
	case '4':
		n = 3;
		sel_stone = stones[3];
		printf("��� 4���� ���õ�\n");
		break;
	case '5':
		n = 4;
		sel_stone = stones[4];
		printf("��� 5���� ���õ�\n");
		break;
	case '6':
		n = 5;
		sel_stone = stones[5];
		printf("�Ķ� 1���� ���õ�\n");
		break;
	case '7':
		n = 6;
		sel_stone = stones[6];
		printf("�Ķ� 2���� ���õ�\n");
		break;
	case '8':
		n = 7;
		sel_stone = stones[7];
		printf("�Ķ� 3���� ���õ�\n");
		break;
	case '9':
		n = 8;
		sel_stone = stones[8];
		printf("�Ķ� 4���� ���õ�\n");
		break;
	case '0':
		n = 9;
		sel_stone = stones[9];
		printf("�Ķ� 5���� ���õ�\n");
		break;
	case '`':
		printf("�� ���� �Ϸ�\n");
		goto cmd;
	}

	cmd:
	switch (key)
	{
	case 'a':
		sel_stone.angle = sel_stone.angle + 1;
		printf("���õ� ���� ����\n");
		break;
	case 'A':
		sel_stone.angle = sel_stone.angle + 1;
		printf("���õ� ���� ����\n");
		break;
	case 'd':
		sel_stone.angle = sel_stone.angle - 1;
		printf("���õ� ���� ����\n");
		break;
	case 'D':
		sel_stone.angle = sel_stone.angle - 1;
		printf("���õ� ���� ����\n");
		break;
	case 'w':
		sel_stone.force = sel_stone.force + 10;
		printf("���õ� �� ����\n");
		break;
	case 'W':
		sel_stone.force = sel_stone.force + 10;
		printf("���õ� �� ����\n");
		break;
	case 's':
		sel_stone.force = sel_stone.force - 10;
		printf("���õ� �� ����\n");
		break;
	case 'S':
		sel_stone.force = sel_stone.force - 10;
		printf("���õ� �� ����\n");
		break;
	case 'f':
		stones[n].force = sel_stone.force;
		stones[n].angle = sel_stone.angle;
		printf("��� %d���� �߻�\n", (n + 1));
		Load(stones[n]);
		break;
	case 'i':
		for (int i = 0; i < 5; ++i)
		{
			printf("��� %d�� ���� x�� : %f\n", i, stones[i].x);
			printf("��� %d�� ���� y�� : %f\n", i, stones[i].y);
			printf("��� %d�� ���� force�� : %f\n", i, stones[i].force);
			printf("��� %d�� ���� angle�� : %f\n", i, stones[i].angle);
			printf("�Ķ� %d�� ���� x�� : %f\n", i, stones[i+5].x);
			printf("�Ķ� %d�� ���� y�� : %f\n", i, stones[i+5].y);
			printf("�Ķ� %d�� ���� force�� : %f\n", i, stones[i+5].force);
			printf("�Ķ� %d�� ���� angle�� : %f\n", i, stones[i+5].angle);
		}
		break;
	}
	glutPostRedisplay();
}//�� ��ǥ���� �Ⱥ��ұ�
