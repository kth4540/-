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
	float color; //1-흰색 2-파랑
	float x;
	float y;
	float angle;
	float sin;
	float cos;
	float force;
}stone;

void Load(stone sel_stone);			//돌의 sin, cos값 계산
void shot(stone move_stone);		//돌 이동
void check_collide(struct stone stone1, struct stone stone2);	//충돌체크
void collide(stone col_stone1, stone col_stone2);				//충돌시 돌 변화

struct stone stones[10] = { {1,  -60, 60,  -90, NULL, NULL, 0 },{ 1, 0, 60, -90, NULL, NULL, 0 },{1, 60, 60, -90, NULL, NULL ,0 },{ 1, -30, 30, -90, NULL, NULL, 0 },{1,  30, 30, -90, NULL, NULL, 0 },
{2,  -60, -60,  90, NULL, NULL, 0 },{2,  0, -60, 90, NULL, NULL, 0 },{ 2, 60, -60, 90, NULL, NULL ,0 },{2,  -30, -30, 90, NULL, NULL, 0 },{2,  30, -30, 90, NULL, NULL, 0 } };
//돌 기본설정


struct stone blue_stone[5] = { {-60, -60,  90, NULL, NULL, 0}, {0, -60, 90, NULL, NULL, 0}, {60, -60, 90, NULL, NULL ,0}, {-30, -30, 90, NULL, NULL, 0}, {30, -30, 90, NULL, NULL, 0} };
struct stone white_stone[5] = { { -60, 60,  -90, NULL, NULL, 0 },{ 0, 60, -90, NULL, NULL, 0 },{ 60, 60, -90, NULL, NULL ,0 },{ -30, 30, -90, NULL, NULL, 0 },{ 30, 30, -90, NULL, NULL, 0 } };

int n = 999; //뭐였드라

void Load(struct stone sel_stone)
{
	sel_stone.sin = sin(sel_stone.angle * (PI / 180));
	printf("각도 설정됨 : sin %f\n", sel_stone.sin);
	sel_stone.cos = cos(sel_stone.angle * (PI / 180));
	printf("각도 설정됨 : cos %f\n", sel_stone.cos);
	shot(sel_stone);
	printf("발사함수 실행됨\n");
};		//돌의 sin, cos값 설정

void shot(struct stone move_stone)
{
	printf("돌 이동\n");
	while (move_stone.force > 0)
	{
		move_stone.x = move_stone.x + ((((move_stone.force) / 10))*move_stone.cos);
		printf("x좌표 이동 : %f", move_stone.x);
		move_stone.y = move_stone.y + ((((move_stone.force) / 10))*move_stone.sin);
		printf("y좌표 이동 : %f", move_stone.y);
		for (int i = 0; i < 10; ++i)
		{
			if ((move_stone.x == stones[i].x) && (move_stone.y == stones[i].y))
			{
				++i;
			}										//자기자신과의 비교는 패스
			check_collide(move_stone, stones[i]);	//충돌체크
		}
		move_stone.force = move_stone.force - FRICTION;
	}
};		//돌 발사

void check_collide(struct stone stone1, struct stone stone2)
{
	if ((((stone1.x - stone2.x)*(stone1.x - stone2.x)) + ((stone1.y - stone2.y)*(stone1.y - stone2.y))) < 400)
	{
		collide(stone1, stone2);
	}
}	//충돌체크

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
};	//충돌시 변화계산


void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA|GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치지정
	glutInitWindowSize(1280, 720); // 윈도우의 크기 지정
	glutCreateWindow("Example2"); // 윈도우 생성 (윈도우 이름)
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}
// 윈도우 출력 함수
GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 바탕색을 'black' 로 지정
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
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
	glPopMatrix();	// 바둑판

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
	glPopMatrix();	// 바둑판 줄

	
	for (int i = 0; i < 5; ++i)
	{
		glColor3f(1, 1, 1);
		glPushMatrix();
		{
			gluLookAt(0, 100, 100, 0, 0, -300, 0, 1, 0);
			glTranslatef(stones[i].x, -30, stones[i].y);
			glutSolidSphere(5, 20, 20);
		}
		glPopMatrix();//흰색 돌

		glColor3f(0, 0, 1);
		glPushMatrix();
		{
			gluLookAt(0, 100, 100, 0, 0, -300, 0, 1, 0);
			glTranslatef(stones[i+5].x, -30, stones[i+5].y);
			glutSolidSphere(5, 20, 20);
		}
		glPopMatrix();//파랑 돌
	}




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
	struct stone sel_stone = { 0, 0, 0, NULL, NULL, 0 };

	switch (key)
	{
	case '1':
		n = 0;
		sel_stone = stones[0];
		printf("흰색 1번돌 선택됨\n");
		break;
	case '2':
		n = 1;
		sel_stone = stones[1];
		printf("흰색 2번돌 선택됨\n");
		break;
	case '3':
		n = 2;
		sel_stone = stones[2];
		printf("흰색 3번돌 선택됨\n");
		break;
	case '4':
		n = 3;
		sel_stone = stones[3];
		printf("흰색 4번돌 선택됨\n");
		break;
	case '5':
		n = 4;
		sel_stone = stones[4];
		printf("흰색 5번돌 선택됨\n");
		break;
	case '6':
		n = 5;
		sel_stone = stones[5];
		printf("파랑 1번돌 선택됨\n");
		break;
	case '7':
		n = 6;
		sel_stone = stones[6];
		printf("파랑 2번돌 선택됨\n");
		break;
	case '8':
		n = 7;
		sel_stone = stones[7];
		printf("파랑 3번돌 선택됨\n");
		break;
	case '9':
		n = 8;
		sel_stone = stones[8];
		printf("파랑 4번돌 선택됨\n");
		break;
	case '0':
		n = 9;
		sel_stone = stones[9];
		printf("파랑 5번돌 선택됨\n");
		break;
	case '`':
		printf("돌 선택 완료\n");
		goto cmd;
	}

	cmd:
	switch (key)
	{
	case 'a':
		sel_stone.angle = sel_stone.angle + 1;
		printf("선택돌 각도 증가\n");
		break;
	case 'A':
		sel_stone.angle = sel_stone.angle + 1;
		printf("선택돌 각도 증가\n");
		break;
	case 'd':
		sel_stone.angle = sel_stone.angle - 1;
		printf("선택돌 각도 감소\n");
		break;
	case 'D':
		sel_stone.angle = sel_stone.angle - 1;
		printf("선택돌 각도 감소\n");
		break;
	case 'w':
		sel_stone.force = sel_stone.force + 10;
		printf("선택돌 힘 증가\n");
		break;
	case 'W':
		sel_stone.force = sel_stone.force + 10;
		printf("선택돌 힘 증가\n");
		break;
	case 's':
		sel_stone.force = sel_stone.force - 10;
		printf("선택돌 힘 감소\n");
		break;
	case 'S':
		sel_stone.force = sel_stone.force - 10;
		printf("선택돌 힘 감소\n");
		break;
	case 'f':
		stones[n].force = sel_stone.force;
		stones[n].angle = sel_stone.angle;
		printf("흰색 %d번돌 발사\n", (n + 1));
		Load(stones[n]);
		break;
	case 'i':
		for (int i = 0; i < 5; ++i)
		{
			printf("흰색 %d번 돌의 x값 : %f\n", i, stones[i].x);
			printf("흰색 %d번 돌의 y값 : %f\n", i, stones[i].y);
			printf("흰색 %d번 돌의 force값 : %f\n", i, stones[i].force);
			printf("흰색 %d번 돌의 angle값 : %f\n", i, stones[i].angle);
			printf("파랑 %d번 돌의 x값 : %f\n", i, stones[i+5].x);
			printf("파랑 %d번 돌의 y값 : %f\n", i, stones[i+5].y);
			printf("파랑 %d번 돌의 force값 : %f\n", i, stones[i+5].force);
			printf("파랑 %d번 돌의 angle값 : %f\n", i, stones[i+5].angle);
		}
		break;
	}
	glutPostRedisplay();
}//왜 좌표값이 안변할까
