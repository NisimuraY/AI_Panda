#pragma once
#include ".\Library\GameObject.h"
#include "global.h"


class Enemy :
    public GameObject
{
	int hImage_;//‰æ‘œID
	Point pos_;//ˆÊ’u
	DIR dir_;//ˆÚ“®•ûŒü
public:
	Enemy();
	~Enemy();
	void Update() override;
	void Draw() override;

	void Patrol();
	void Chase();
	void Attack();
	void Search();

private:
	bool NearPlayer();
	bool FindPlayer();
	void EnemyView();

	bool CheckAttackRange();

	enum class EnemyState
	{
		Patrol,
		Chase,
		Attack,
		Search
	};

	enum class EnemyState state_;
};

