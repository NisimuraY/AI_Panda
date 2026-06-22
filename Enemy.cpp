#include "Enemy.h"
#include "time.h"
#include "Stage.h"
#include "Player.h"

namespace
{
	const int ENEMY_SIZE = 48; //敵のサイズ 32*32
	const Point ENEMY_START_POS = { 20 * ENEMY_SIZE, 10 * ENEMY_SIZE }; //敵の初期位置
	//const Point ENEMY_START_POS = { 2 * ENEMY_SIZE, 12 * ENEMY_SIZE }; //敵の初期位置
	const DIR INIT_ENEMY_DIR = { LEFT };
	const int ENEMY_DRAW_SIZE = 32; //敵の描画サイズ
	const int animFrame[4]{ 0, 1, 2, 1 };
	const float ANIM_INTERVAL = 0.2f;
	float DegToRad = 0.017453292f;
}


Enemy::Enemy()
	: GameObject() 
{
	hImage_ = LoadGraph("Assets/panda_R.png");
	pos_ = ENEMY_START_POS; //32はブロックの位置pos_
	dir_ = INIT_ENEMY_DIR;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	//GetRand(数値)
	//3秒に1回向きをランダムに変える
	static float dir_timer = 3.0f;
	static float prog_timer = 0.5f;
	float dt = Time::DeltaTime();
	dir_timer = dir_timer - dt;
	prog_timer = prog_timer - dt;

<<<<<<< HEAD
	//if (dir_timer < 0.0f)
	//{
	//	dir_ = (DIR)(GetRand(3));
	//	dir_timer = 3.0f + dir_timer;
	//}
=======
	/*if (dir_timer < 0.0f)
	{
		dir_ = (DIR)(GetRand(3));
		dir_timer = 3.0f + dir_timer;
	}*/
>>>>>>> 56d00b49137c649fe3da54082e890e4af953e9b1

	Point newPos = pos_;
	if (prog_timer < 0.0f)
	{
<<<<<<< HEAD
=======
		
>>>>>>> 56d00b49137c649fe3da54082e890e4af953e9b1
		switch (dir_)
		{
		case UP:
			newPos.y -= ENEMY_DRAW_SIZE;
			break;
		case DOWN:
			newPos.y += ENEMY_DRAW_SIZE;
			break;
		case LEFT:
			newPos.x -= ENEMY_DRAW_SIZE;
			break;
		case RIGHT:
			newPos.x += ENEMY_DRAW_SIZE;
			break;
		default:
			break;
		}

		int mapValue = FindGameObject<Stage>()->GetMap(newPos.x/CHA_SIZE, newPos.y / CHA_SIZE);
		//移動先がステージの外に出ないようにする
		if (mapValue != 1)
		{
			pos_ = newPos;
		}
		
		else if (mapValue == 1)
		{
			switch (dir_)
			{
			case UP:
				dir_ = RIGHT;
				break;
			case DOWN:
				dir_ = LEFT;
				break;
			case LEFT:
				dir_ = UP;
				break;
			case RIGHT:
				dir_ = DOWN;
				break;
			default:
				break;
			}
		}

		prog_timer = 0.5f + prog_timer;
	}
	
	Player* p = FindGameObject<Player>();
	Point pPos = p->GetPlayerPos();
	float dx = pPos.x - pos_.x;
	float dy = pPos.y - pos_.y;

	if (NearPlayer()||FindPlayer())
	{
		DrawString(10, 10, "FIND", GetColor(0, 0, 0), TRUE);
		if (pPos.y < pos_.y)
		{
			dir_ = UP;
		}
		if (pPos.y > pos_.y)
		{
			dir_ = DOWN;
		}
		if (pPos.x < pos_.x)
		{
			dir_ = LEFT;
		}
		if (pPos.x > pos_.x)
		{
			dir_ = RIGHT;
		}
	}
}

void Enemy::Draw()
{
	static float animTimer = ANIM_INTERVAL;
	static int frame = 0;
	int nowFrame = animFrame[frame];

	Rect iRect[4] = {
		{  nowFrame * ENEMY_SIZE, 3 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 0 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 1 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE},
		{  nowFrame * ENEMY_SIZE, 2 * ENEMY_SIZE, ENEMY_SIZE, ENEMY_SIZE}
	};
	DrawBox(pos_.x, pos_.y, pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
		GetColor(255, 255, 0), FALSE,2);
	DrawRectExtendGraph(pos_.x, pos_.y,pos_.x + ENEMY_DRAW_SIZE, pos_.y + ENEMY_DRAW_SIZE,
		               iRect[dir_].x, iRect[dir_].y, iRect[dir_].w, iRect[dir_].h, hImage_, TRUE);
	if (animTimer < 0) {
		frame = (++frame) % 4;
		animTimer = ANIM_INTERVAL + animTimer;
	}
	animTimer = animTimer - Time::DeltaTime();

	EnemyView();
}

bool Enemy::NearPlayer()
{
	Player* p = FindGameObject<Player>();
	Point pPos = p->GetPlayerPos();

	float dx = pPos.x - pos_.x;
	float dy = pPos.y - pos_.y;

	float range = ENEMY_SIZE * 3;

	if ((dx * dx + dy * dy) <= (range * range))
	{
		return true;
	}

	return false;
}

bool Enemy::FindPlayer()
{
	Player* p = FindGameObject<Player>();
	Point pPos = p->GetPlayerPos();

	// 敵の向きベクトル
	Point dir = { 0, 1 }; // 下向き
	switch (dir_)
	{
	case UP:
		dir = { 0, -1 };
		break;
	case DOWN:
		dir = { 0, 1 };
		break;
	case LEFT:
		dir = { -1, 0 };
		break;
	case RIGHT:
		dir = { 1, 0 };
		break;
	default:
		break;
	}

	// 敵→プレイヤー
	float dx = pPos.x - pos_.x;
	float dy = pPos.y - pos_.y;

	// 距離
	float distance = sqrt(dx * dx + dy * dy);

	// 視界距離
	float range = 200.0f;

	if (distance > range)
		return false;

	if (distance <= 0.0f)
		return true;

	// 正規化
	dx = dx / distance;
	dy = dy / distance;

	// 内積
	float dot = dir.x * dx + dir.y * dy;

	float viewAngle = 45.0f * DegToRad;
	float cosAngle = cosf(viewAngle);

	if (dot >= cosAngle)
	{
		return true;
	}

	return false;
}

void Enemy::EnemyView()
{
	Point dir = { 0,1 };

	switch (dir_)
	{
	case UP:    dir = { 0,-1 }; break;
	case DOWN:  dir = { 0,1 };  break;
	case LEFT:  dir = { -1,0 }; break;
	case RIGHT: dir = { 1,0 };  break;
	}

	float enemyX = pos_.x + ENEMY_DRAW_SIZE / 2.0f;
	float enemyY = pos_.y + ENEMY_DRAW_SIZE / 2.0f;

	float viewAngle = 45.0f * DegToRad;
	float cosAngle = cosf(viewAngle);

	float range = 200.0f;

	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			float cellX = x * CHA_SIZE + CHA_SIZE / 2.0f;
			float cellY = y * CHA_SIZE + CHA_SIZE / 2.0f;

			float dx = cellX - enemyX;
			float dy = cellY - enemyY;

			float distance = sqrtf(dx * dx + dy * dy);

			if (distance <= 0.0001f)
				continue;

			if (distance > range)
				continue;

			dx /= distance;
			dy /= distance;

			float dot = dir.x * dx + dir.y * dy;

			if (dot >= cosAngle)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);

				DrawBox(
					x * CHA_SIZE,
					y * CHA_SIZE,
					(x + 1) * CHA_SIZE,
					(y + 1) * CHA_SIZE,
					GetColor(255, 255, 0),
					TRUE
				);

				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			/*if(cellX> enemyX - CHA_SIZE * 3&&)*/
		}
	}
}