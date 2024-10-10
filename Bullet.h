#include "Enemy.h"
#include "Player.h"
struct Bullet
{
	int xpos;
	int ypos;
	char valid;
	int damage;
	int speed;
	int count;
};
void BulletMove(int range, int stageIndex, EnemyInfo* EnemyArr, Bullet* BulletArr, Bullet* BulletCmp, int cmpAmount);
void BulletCollision(int stageIndex, EnemyInfo* EnemyArr, Bullet* BulletArr, Player* player1, Bullet* BulletCmp, int cmpAmount);
void BulletCreate(int stageIndex, EnemyInfo* EnemyArr, Bullet* BulletArr, Player* player1);