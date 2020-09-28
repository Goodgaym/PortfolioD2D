#include "stdafx.h"
#include "S01_GameScene.h"
#include "Stage.h"

#include "Objects/Arrow.h"
#include "Objects/Bubble.h"
#include "Objects/Background.h"
#include "Objects/Ceiling.h"
#include "Objects/LaunchDragon.h"
#include "Objects/LaunchingPad.h"
#include "Objects/Monster.h"

S01_GameScene::S01_GameScene(SceneValues * values)
	: Scene(values)
	, fBubbleRotation(0.0f)
	, fRotationSpeed(100.0f)
	, fStageTime(0.0f)
	, fCeilingY(CEILING_Y)
	, stageState(State::READY)
	, uiStage(0)
	, uiExplodeCount(0)
	, uiCeilingLine(0)
	, uiOverLine(BALL_LINE_SIZE_Y - 1)
	, fBubbleShake(80.0f)
	, fBubbleDistance(0.0f)
	, fShakeSpeed(80.0f)
{
	SAFE_DELETE(values->MainCamera);
	values->MainCamera = new Camera();

	wstring spriteFile = L"./puzzleBubble.png";
	wstring shaderFile = L"Effect.fx";

	// Background / Launching Pad
	bubbleBag1 = new Sprite(spriteFile, shaderFile, 781, 1813, 845, 1845);
	bubbleBag2 = new Sprite(spriteFile, shaderFile, 846, 1813, 910, 1845);
	bubbleBag1->Position(-212.0f, -352.0f);
	bubbleBag2->Position(-212.0f, -352.0f);
	bubbleBag1->Scale(SCALE_X, SCALE_Y);
	bubbleBag2->Scale(SCALE_X, SCALE_Y);

	deadLine = new Sprite(spriteFile, shaderFile, 581, 2185, 709, 2201);
	deadLine->Position(0.0f, -276.0f);
	deadLine->Scale(SCALE_X, SCALE_Y);

	arrow = new Arrow(Vector2(0, -320.0f), Vector2(SCALE_X, SCALE_Y));
	background = new Background(Vector2(0, 0), Vector2(SCALE_X, SCALE_Y));
	launchDragon = new LaunchDragon(Vector2(-68.0f, -352.0f), Vector2(SCALE_X, SCALE_Y));
	launchingPad = new LaunchingPad(Vector2(0, -335.0f), Vector2(SCALE_X, SCALE_Y));
	ceiling = new Ceiling(Vector2(0.0f, 368.0f), Vector2(SCALE_X, SCALE_Y));

	readyBubble = NULL;
	waitBubble = NULL;
	shootBubble = NULL;

}

S01_GameScene::~S01_GameScene()
{
	SAFE_DELETE(arrow);
	SAFE_DELETE(background);
	SAFE_DELETE(launchDragon);
	SAFE_DELETE(launchingPad);
	SAFE_DELETE(bubbleBag1);
	SAFE_DELETE(bubbleBag2);
	SAFE_DELETE(deadLine);
	SAFE_DELETE(ceiling);

	if (waitBubble != NULL)
		SAFE_DELETE(waitBubble);
	if (readyBubble != NULL)
		SAFE_DELETE(readyBubble);
	if (shootBubble != NULL)
		SAFE_DELETE(shootBubble);

	for (UINT i = 0; i < BALL_LINE_SIZE_Y; i++) {
		for (UINT j = 0; j < BALL_LINE_SIZE_X; j++) {
			SAFE_DELETE(bubbles[i][j]);
		}
	}
}

void S01_GameScene::Update()
{
	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;

	/// ���� : Begin
	//// State::READY
	if (stageState == State::READY) {
		// Bubbles Line Position Set / Bubbles Set
		for (UINT i = 0; i < BALL_LINE_SIZE_Y; i++) {
			Vector2 position = Vector2(-228.0f, 324.0f);
			if (i % 2 == 1) position.x += BALL_SIZE_X / 2;
			position.y -= (i * BALL_SIZE_Y);

			for (UINT j = 0; j < BALL_LINE_SIZE_X; j++) {
				bubbleLine[i][j] = position;
				position.x += BALL_SIZE_X;
			}
		}
		// Stage Set
		if (uiStage >= 3) {
			ceiling->Stage(1);
			background->Stage(1);
		}
		for (UINT i = 0; i < BALL_LINE_SIZE_Y; i++) {
			for (UINT j = 0; j < BALL_LINE_SIZE_X; j++) {
				if (stage[uiStage][i][j] != 0) {
					bubbles[i][j] = new Bubble(bubbleLine[i][j], Vector2(SCALE_X, SCALE_Y), stage[uiStage][i][j]);
					bRemainsColor[bubbles[i][j]->GetBubbleColor()]++;
				}
			}
		}

		// Ready/Wating Bubble
		waitBubble = new Bubble(
			Vector2(BALL_INIT_POS_X + 100.0f, BALL_INIT_POS_Y), Vector2(SCALE_X, SCALE_Y), RandomSeed());
		waitBubble->BubbleState(BState::NEXT);

		readyBubble = new Bubble(Vector2(-0.0f, -320.0f), Vector2(SCALE_X, SCALE_Y), RandomSeed());
		readyBubble->BubbleState(BState::READY);

		// ���� �ʱ�ȭ
		launchingPad->State(SDState::IDLE);
		ceiling->Position(0.0f, 368.0f);
		ceiling->FrameNumber(0);
		fBubbleRotation = 0.0f;
		fStageTime = 0.0f;
		fCeilingY = CEILING_Y;
		uiOverLine = BALL_LINE_SIZE_Y - 1;
		uiCeilingLine = 0;

		Audio->Play("BGM");
		Audio->Play("READY");

		// State ����
		stageState = State::GO;
	}

	if (stageState == State::GO) {
		fStageTime += Time::Delta();
		if (fStageTime >= 0.016f) {
			Audio->Play("GO");
			fStageTime = 0.0f;

			// State ����
			stageState = State::GAME;
		}
	}

	//// State::GAME
	if (stageState == State::GAME) {
		/// ȸ��
		if (Key->Press('A')) {
			fBubbleRotation += fRotationSpeed * LIMIT_ANGLE / LAUNCHER_ANGLE * Time::Delta();
			launchingPad->State(SDState::SPIN);
		}
		else if (Key->Press('D')) {
			fBubbleRotation -= fRotationSpeed * LIMIT_ANGLE / LAUNCHER_ANGLE * Time::Delta();
			launchingPad->State(SDState::SPIN);
		}
		else {
			launchingPad->State(SDState::IDLE);
		}

		// �߻�� ȸ��
		launchingPad->FrameNumber((UINT)fBubbleRotation / 4);
		arrow->FrameNumber((int)(-fBubbleRotation * 0.75));

		/// ���� ����
		if (fBubbleRotation <= -LIMIT_ANGLE) fBubbleRotation = -LIMIT_ANGLE;
		if (fBubbleRotation >= LIMIT_ANGLE) fBubbleRotation = LIMIT_ANGLE;
		/// �߻�
		if (Key->Down(VK_SPACE) && shootBubble == NULL && deleteBubbles.size() == 0) {
			Audio->Play("SHOT");
			launchDragon->ShootBubble();
			// Waiting -> Ready
			waitBubble->BubbleState(BState::MOVEREADY);

			// Shoot Bubble
			readyBubble->shootBubble(fBubbleRotation + 90.0f);
			shootBubble = readyBubble;

			// Swap Bubble
			readyBubble = waitBubble;
			waitBubble = new Bubble(
				Vector2(BALL_INIT_POS_X, BALL_INIT_POS_Y), Vector2(SCALE_X, SCALE_Y), RandomSeed());
			waitBubble->BubbleState(BState::SPAWN);
		}

		/// õ�忡 �ε�������
		if (shootBubble != NULL && shootBubble->Position().y > fCeilingY) {
			Audio->Play("BUMP");
			Vector2 sPos = shootBubble->Position();
			UINT x_ = (UINT)((sPos.x + 224.0f) / 56);

			// shoot -> bubbles ��ȯ
			bubbles[0][x_] = shootBubble;
			shootBubble->Position(bubbleLine[0][x_]);
			shootBubble->BubbleState(BState::IDLE);

			// Explode üũ
			BubbleExplodeCheck(x_, 0);
			shootBubble = NULL;
		}

		/// �浹ó��, Bubbles ������Ʈ
		for (UINT i = 0; i < BALL_LINE_SIZE_Y; i++) {
			for (UINT j = 0; j < BALL_LINE_SIZE_X; j++) {
				if (bubbles[i][j] != NULL) {
					if (shootBubble != NULL && Collider::Cbb(shootBubble->getCollider()->World(), bubbles[i][j]->getCollider()->World())) {
						Audio->Play("BUMP");
						// �� ���̱�
						BubbleStitch(j, i);
					}
				}
			}
		}

		// ���� ���� üũ
		for (int i = 0; i < 8; i++) {
			BubbleAirCheck(i, 0);
		}

		// õ�� ������
		fStageTime += Time::Delta();
		CeilingDown();

		// �¸� (���� ���� 0�� �������)
		UINT ballCount = 0;
		for (UINT i = 1; i <= 8; i++) {
			ballCount += bRemainsColor[i];
		}
		if (ballCount <= 0) {
			fStageTime = 0;
			// State ����
			stageState = State::WIN;
		}
		// �й�
		for (UINT j = 0; j < BALL_LINE_SIZE_X; j++) {
			if (bubbles[uiOverLine][j] != NULL) {
				for (UINT i = 0; i < BALL_LINE_SIZE_Y; i++) {
					for (UINT j = 0; j < BALL_LINE_SIZE_X; j++) {
						if (bubbles[i][j] != NULL) {
							bubbles[i][j]->BubbleState(BState::GAMEOVER);
						}
					}
				}
				stageState = State::LOSE;
			}
		}
	}

	//// State::WIN
	if (stageState == State::WIN) {
		Audio->Play("CLEAR");
		Audio->Stop("BGM");

		// readyBubble ����
		readyBubble->BubbleState(BState::BURST);
		deleteBubbles.push_back(readyBubble);
		readyBubble = NULL;

		// waitBubble ����
		waitBubble->BubbleState(BState::BURST);
		deleteBubbles.push_back(waitBubble);
		waitBubble = NULL;

		launchDragon->ClearStage();
		launchingPad->State(SDState::CLEAR);
		stageState = State::NEXTSTAGE;
	}

	//// State::NEXTSTAGE
	if (stageState == State::NEXTSTAGE) {
		Audio->Play("GO");
		fStageTime += Time::Delta();
		if (fStageTime >= 4.0f) {
			// �������� ����
			uiStage++;
			launchDragon->ReadyStage();
			// State ����
			stageState = State::READY;
		}
	}

	//// State::LOSE
	if (stageState == State::LOSE) {
		Audio->Play("LOSE");

		fStageTime += Time::Delta();
		if (fStageTime >= 4.0f) {
			uiStage = 0;
			launchDragon->ReadyStage();
		}
	}
	/// ���� : End

	/// Delete : Begin
	BubbleExplodeDelete();
	BubbleAirDelete();
	BubbleDelete();
	MonsterDelete();

	/// Delete : End

	/// Update : Begin
	background->Update(V, P);
	ceiling->Update(V, P);

	for (Monster* monster : deleteMonster)
		monster->Update(V, P);

	for (Bubble* bubble : deleteBubbles)
		bubble->Update(V, P);

	bubbleBag1->Update(V, P);
	bubbleBag2->Update(V, P);
	deadLine->Update(V, P);
	launchingPad->Update(V, P);
	arrow->Update(V, P);
	launchDragon->Update(V, P);

	if (readyBubble != NULL)
		readyBubble->Update(V, P);
	if (waitBubble != NULL)
		waitBubble->Update(V, P);
	if (shootBubble != NULL)
		shootBubble->Update(V, P);

	for (UINT i = 1; i <= 8; i++) {
		bRemainsColor[i] = 0;
	}
	for (UINT i = 0; i < BALL_LINE_SIZE_Y; i++) {
		for (UINT j = 0; j < BALL_LINE_SIZE_X; j++) {
			if (bubbles[i][j] != NULL) {
				bubbles[i][j]->Update(V, P);
				bRemainsColor[bubbles[i][j]->GetBubbleColor()]++;
			}
		}
	}
	/// Update : End

}

void S01_GameScene::Render()
{
	ImGui::LabelText("time", "%f", Time::Get()->Running());

	background->Render();
	ceiling->Render();

	for (Bubble* bubble : deleteBubbles)
		bubble->Render();

	for (Monster* monster : deleteMonster)
		monster->Render();

	for (UINT i = 0; i < BALL_LINE_SIZE_Y; i++) {
		for (UINT j = 0; j < BALL_LINE_SIZE_X; j++) {
			if (bubbles[i][j] != NULL)
				bubbles[i][j]->Render();
		}
	}

	bubbleBag1->Render();
	deadLine->Render();
	launchingPad->Render();
	arrow->Render();
	launchDragon->Render();
	bubbleBag2->Render();

	if (readyBubble != NULL)
		readyBubble->Render();
	if (waitBubble != NULL)
		waitBubble->Render();
	if (shootBubble != NULL)
		shootBubble->Render();
}

UINT S01_GameScene::RandomSeed()
{
	UINT rnd = 0;
	do {
		rnd = rand() % 8 + 1;
	} while (0 == bRemainsColor[rnd]);
	return rnd;
}

void S01_GameScene::CeilingDown()
{
	/// Bubbles ����
	if (fStageTime >= 15) {
		for (UINT i = 0; i < BALL_LINE_SIZE_Y; i++) {
			for (UINT j = 0; j < BALL_LINE_SIZE_X; j++) {
				if (bubbles[i][j] != NULL) {
					bubbles[i][j]->Position(
						bubbles[i][j]->Position().x + fBubbleShake * Time::Delta(),
						bubbles[i][j]->Position().y
					);
				}
			}
		}
		fBubbleDistance += fBubbleShake * Time::Delta();
		if (fBubbleDistance > 3.0f) {
			fBubbleShake = -fShakeSpeed;
		}
		if (fBubbleDistance < -3.0f) {
			fBubbleShake = fShakeSpeed;
		}
	}
	if (fStageTime >= 19) {
		fShakeSpeed = 220.0f;
	}
	else if (fStageTime >= 18) {
		fShakeSpeed = 180.0f;
	}
	else if (fStageTime >= 17) {
		fShakeSpeed = 140.0f;
	}
	else if (fStageTime >= 16) {
		fShakeSpeed = 100.0f;
	}

	/// õ�� ��������
	if (fStageTime >= 20) {
		// õ�� ���� ������
		Audio->Play("PING");
		fCeilingY -= 56;
		uiOverLine -= 1;

		// õ�� �ִϸ��̼�/��ġ ����
		ceiling->Position(ceiling->Position().x, ceiling->Position().y - 28.0f);
		ceiling->FrameNumber(++uiCeilingLine);

		for (UINT i = 0; i < BALL_LINE_SIZE_Y; i++) {
			for (UINT j = 0; j < BALL_LINE_SIZE_X; j++) {
				bubbleLine[i][j].y -= 56;
				// Bubbles ������
				if (bubbles[i][j] != NULL) {
					bubbles[i][j]->Position(bubbleLine[i][j]);
				}

				// BubbleLine ������

			}
		}
		fStageTime = 0;
	}
}

void S01_GameScene::BubbleStitch(UINT x_, UINT y_)
{
	Vector2 sPos = shootBubble->Position();
	Vector2 bPos = bubbles[y_][x_]->Position();

	/// ���� ��ġ 
	if (y_ % 2 == 0) {
		// ���� ��
		if (sPos.x < bPos.x && bPos.y - sPos.y <= -28.0f) {
			x_ -= 1; y_ -= 1;
		}
		// ������ ��
		else if (sPos.x >= bPos.x && bPos.y - sPos.y <= -28.0f) {
			y_ -= 1;
		}
		// ���� �Ʒ�
		else if (sPos.x < bPos.x && bPos.y - sPos.y >= 28.0f) {
			x_ -= 1; y_ += 1;
		}
		// ������ �Ʒ�
		else if (sPos.x >= bPos.x && bPos.y - sPos.y >= 28.0f) {
			y_ += 1;
		}
		// ����
		else if (sPos.x < bPos.x) {
			x_ -= 1;
		}
		// ������
		else if (sPos.x >= bPos.x) {
			x_ += 1;
		}
	}
	else {
		// ���� ��
		if (sPos.x < bPos.x && bPos.y - sPos.y <= -28.0f) {
			y_ -= 1;
		}
		// ������ ��
		else if (sPos.x >= bPos.x && bPos.y - sPos.y <= -28.0f) {
			x_ += 1; y_ -= 1;
		}
		// ���� �Ʒ�
		else if (sPos.x < bPos.x && bPos.y - sPos.y >= 28.0f) {
			y_ += 1;
		}
		// ������ �Ʒ�
		else if (sPos.x >= bPos.x && bPos.y - sPos.y >= 28.0f) {
			x_ += 1; y_ += 1;
		}
		// ����
		else if (sPos.x < bPos.x) {
			x_ -= 1;
		}
		// ������
		else if (sPos.x >= bPos.x) {
			x_ += 1;
		}
	}

	// shoot -> bubbles ��ȯ
	this->bubbles[y_][x_] = this->shootBubble;
	this->shootBubble->Position(bubbleLine[y_][x_]);
	this->shootBubble->BubbleState(BState::IDLE);

	// BallExplode Check
	BubbleExplodeCheck(x_, y_);
	this->shootBubble = NULL;
}

void S01_GameScene::BubbleExplodeCheck(UINT x, UINT y)
{
	// x, y�� ���� ���� ���
	if (y > uiOverLine) return;
	if (x < 0 || y < 0) return;
	if (x > 7 && y % 2 == 0) return;
	if (x > 6 && y % 2 == 1) return;

	// �̹� üũ �Ǿ�������
	if (bCheckBubbles[y][x]) return;
	// ������ ������
	if (bubbles[y][x] == NULL) return;

	if (bubbles[y][x]->BubbleState() == BState::BURST) return;
	if (bubbles[y][x]->BubbleState() == BState::INAIR) return;

	UINT ballNumber = shootBubble->GetBubbleColor();

	// ���� ���� �ƴϸ�
	if (ballNumber != bubbles[y][x]->GetBubbleColor()) return;

	// üũ
	bCheckBubbles[y][x] = true;
	uiExplodeCount++;

	if (y % 2 == 0) {	// 8ĭ¥�� ��
		BubbleExplodeCheck(x - 1, y - 1);
		BubbleExplodeCheck(x, y - 1);
		BubbleExplodeCheck(x - 1, y);
		BubbleExplodeCheck(x + 1, y);
		BubbleExplodeCheck(x - 1, y + 1);
		BubbleExplodeCheck(x, y + 1);
	}
	else {	// 7ĭ¥�� ��
		BubbleExplodeCheck(x, y - 1);
		BubbleExplodeCheck(x + 1, y - 1);
		BubbleExplodeCheck(x - 1, y);
		BubbleExplodeCheck(x + 1, y);
		BubbleExplodeCheck(x, y + 1);
		BubbleExplodeCheck(x + 1, y + 1);
	}
}

void S01_GameScene::BubbleExplodeDelete()
{
	if (uiExplodeCount < 3) {
		ZeroMemory(bCheckBubbles, sizeof(bool) * BALL_LINE_SIZE_X * BALL_LINE_SIZE_Y);
		uiExplodeCount = 0;
		return;
	}

	Audio->Play("PONG");
	for (UINT i = 0; i < BALL_LINE_SIZE_Y; i++) {
		for (UINT j = 0; j < BALL_LINE_SIZE_X; j++) {
			if (bCheckBubbles[i][j]) {
				// Monster Drop
				temp1 = bubbles[i][j]->Position().x;
				Monster* monster = new Monster(bubbles[i][j]->Position(), bubbles[i][j]->Scale(), bubbles[i][j]->GetBubbleColor());
				deleteMonster.push_back(monster);

				// state -> BURST
				bubbles[i][j]->BubbleState(BState::BURST);

				// Delete -> Push
				deleteBubbles.push_back(bubbles[i][j]);
				bubbles[i][j] = NULL;

			}
		}
	}
	uiExplodeCount = 0;
}

void S01_GameScene::BubbleAirCheck(UINT x, UINT y)
{
	// �̹� üũ �Ǿ�������
	if (bCheckAirBubbles[y][x]) return;
	// ������ ������
	if (bubbles[y][x] == NULL) return;
	// x, y�� ���� ���� ���
	if (y > uiOverLine) return;
	if (x < 0 || y < 0) return;
	if (x > 7 && y % 2 == 0) return;
	if (x > 6 && y % 2 == 1) return;

	// üũ
	bCheckAirBubbles[y][x] = true;

	if (y != 0) {
		BubbleAirCheck(x - 1, y);
		BubbleAirCheck(x + 1, y);
	}

	if (y % 2 == 0) {	// 8ĭ¥�� ��
		BubbleAirCheck(x - 1, y + 1);
		BubbleAirCheck(x, y + 1);
	}
	else {	// 7ĭ¥�� ��
		BubbleAirCheck(x, y + 1);
		BubbleAirCheck(x + 1, y + 1);
	}
}

void S01_GameScene::BubbleAirDelete()
{
	if (stageState == State::GAME) {
		for (UINT i = 0; i < BALL_LINE_SIZE_Y; i++) {
			for (UINT j = 0; j < BALL_LINE_SIZE_X; j++) {
				if (!bCheckAirBubbles[i][j] && bubbles[i][j] != NULL) {
					// state -> InAir
					bubbles[i][j]->BubbleState(BState::INAIR);

					// Delete -> Push
					deleteBubbles.push_back(bubbles[i][j]);
					bubbles[i][j] = NULL;
				}
			}
		}
		ZeroMemory(bCheckAirBubbles, sizeof(bool) * BALL_LINE_SIZE_X * BALL_LINE_SIZE_Y);
	}
}

void S01_GameScene::BubbleDelete()
{
	UINT size = deleteBubbles.size();
	for (UINT i = 0; i < size; i++) {
		// deleteBubbles �߿� DEL �����ΰ� ����
		if (deleteBubbles[i]->BubbleState() == BState::DEL) {
			SAFE_DELETE(deleteBubbles[i]);
			deleteBubbles[i] = NULL;
		}
	}

	for (UINT i = 0; i < deleteBubbles.size(); i++) {
		// ����ִ� deleteBubbles �����ֱ�
		if (deleteBubbles[i] == NULL) {
			deleteBubbles.erase(deleteBubbles.begin() + i);
			i -= 1;
		}
	}
}

void S01_GameScene::MonsterDelete()
{
	UINT size = deleteMonster.size();
	for (UINT i = 0; i < size; i++) {
		// deleteMonster �߿� DEL �����ΰ� ����
		if (deleteMonster[i]->State() == MState::DEL) {
			SAFE_DELETE(deleteMonster[i]);
			deleteMonster[i] = NULL;
		}
	}

	for (UINT i = 0; i < deleteMonster.size(); i++) {
		// ����ִ� deleteMonster �����ֱ�
		if (deleteMonster[i] == NULL) {
			deleteMonster.erase(deleteMonster.begin() + i);
			i -= 1;
		}
	}
}
