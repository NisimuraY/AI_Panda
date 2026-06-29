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
	float checkTimer = 7.0f;
	float MaxCheckTimer = 7.0f;

}


Enemy::Enemy()
	: GameObject() 
{
	hImage_ = LoadGraph("Assets/panda_R.png");
	pos_ = ENEMY_START_POS; //32はブロックの位置pos_
	dir_ = INIT_ENEMY_DIR;
	state_ = EnemyState::Patrol;
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	

	switch (state_)
	{
	case EnemyState::Patrol:
		Patrol();

		if (NearPlayer() || FindPlayer())
		{
			state_ = EnemyState::Chase;
		}
		break;

	case EnemyState::Chase:
		Chase();
		if (!NearPlayer() && !FindPlayer())
		{
			state_ = EnemyState::Search;
		}
		if (CheckAttackRange())
		{
			state_ = EnemyState::Attack;
		}
		break;

	case EnemyState::Attack:
		Attack();	

		if (!CheckAttackRange()&& NearPlayer())
		{
			state_ = EnemyState::Chase;
		}
		if (!(NearPlayer()) && !(FindPlayer()))
		{
			state_ = EnemyState::Search;
		}
		break;

	case EnemyState::Search:
		Search();
		if (NearPlayer() || FindPlayer())
		{
			state_ = EnemyState::Chase;
			checkTimer = MaxCheckTimer;
		}
		if (checkTimer < 0.0f)
		{
			state_ = EnemyState::Patrol;
			checkTimer = MaxCheckTimer;
		}
		break;
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

void Enemy::Patrol()
{
	if (state_ == EnemyState::Patrol)
	{
		DrawString(10, 10, "Patrol", GetColor(0, 0, 0), TRUE);
	}

	Move();

	Point newPos = pos_;
	int mapValue = FindGameObject<Stage>()->GetMap(newPos.x / CHA_SIZE, newPos.y / CHA_SIZE);
	if (mapValue == 1)
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
}


void Enemy::Chase()
{
	if (state_ == EnemyState::Chase)
	{
		DrawString(10, 10, "Chase", GetColor(0, 0, 0), TRUE);
	}

	Move();

	Point newPos = pos_;
	Player* p = FindGameObject<Player>();
	Point pPos = p->GetPlayerPos();
	int mapValue = FindGameObject<Stage>()->GetMap(newPos.x / CHA_SIZE, newPos.y / CHA_SIZE);
	
	float dx = pPos.x - pos_.x;
	float dy = pPos.y - pos_.y;

	if (abs(dx) > abs(dy))
	{
		if (dx > 0)
			dir_ = RIGHT;
		else
			dir_ = LEFT;
	}
	else
	{
		if (dy > 0)
			dir_ = DOWN;
		else
			dir_ = UP;
	}

}

void Enemy::Attack()
{
	if (state_ == EnemyState::Attack)
	{
		DrawString(10, 10, "Attack", GetColor(0, 0, 0), TRUE);
	}
	Chase();
}

void Enemy::Search()
{
	if (state_ == EnemyState::Search)
	{
		DrawString(10, 10, "Search", GetColor(0, 0, 0), TRUE);
	}

	float dt = Time::DeltaTime();
	checkTimer = checkTimer - dt;
	DrawFormatString(100, 10, GetColor(0, 0, 0), "%.2f", checkTimer, TRUE);
	Chase();
}

void Enemy::Move()
{
	static float dir_timer = 3.0f;
	static float prog_timer = 0.5f;
	float dt = Time::DeltaTime();
	dir_timer = dir_timer - dt;
	prog_timer = prog_timer - dt;

	Point newPos = pos_;
	if (prog_timer < 0.0f)
	{

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

		int mapValue = FindGameObject<Stage>()->GetMap(newPos.x / CHA_SIZE, newPos.y / CHA_SIZE);
		DrawFormatString(100, 100, GetColor(0, 0, 0), "%d", mapValue);
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

bool Enemy::CheckAttackRange()
{
	Player* p = FindGameObject<Player>();
	Point pPos = p->GetPlayerPos();

	float dx = pPos.x - pos_.x;
	float dy = pPos.y - pos_.y;

	float range = ENEMY_SIZE;

	if ((dx * dx + dy * dy) <= (range * range))
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

