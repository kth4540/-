#include <stdio.h>
#include <math.h>

#define FRICTION 20;
#define PI 3.1418926535897

typedef struct stone
{
	float x;
	float y;
	float angle;
	float sin;
	float cos;
	float force;
}stone;

void Load(stone sel_stone[])
{
	printf("input force : ");
	scanf("%f", &sel_stone->force);
	printf("input angle : ");
	scanf("%f", &sel_stone->angle);
	sel_stone->sin = sin(sel_stone->angle * (PI / 180));
	sel_stone->cos = cos(sel_stone->angle * (PI / 180));
	shot(sel_stone);
};
void shot(stone move_stone[])
{
	while (move_stone->force < 0)
	{
		move_stone->x = move_stone->x + ((((move_stone->force)/10))*move_stone->cos);
		move_stone->y = move_stone->y + ((((move_stone->force)/10))*move_stone->sin);
		move_stone->force = move_stone->force - FRICTION;
	}
};
void collide(stone col_stone1[], stone col_stone2[])
{
	float pre_force = col_stone2->force;
	col_stone1->sin = (-(col_stone2->y - col_stone1->y)) / 20;
	col_stone1->cos = (-(col_stone2->x - col_stone1->x)) / 20;
	float n_angle = asin(col_stone1->sin);
	col_stone2->sin = sin((PI / 2) - (n_angle + (2 * col_stone2->angle)));
	col_stone2->cos = cos((PI / 2) - (n_angle + (2 * col_stone2->angle)));
	col_stone1->force = pre_force * cos((PI / 2) - (n_angle + col_stone2->angle));
	col_stone2->force = pre_force * sin((PI / 2) - (n_angle + col_stone2->angle));
	shot(col_stone1);
	shot(col_stone2);
};
