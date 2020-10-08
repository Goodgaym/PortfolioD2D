#include "stdafx.h"
#include "S02_EditorScene.h"

#include "Objects/Background.h"

S02_EditorScene::S02_EditorScene(SceneValues * values)
	: Scene(values)
{
	background = new Background(Vector2(0, 0), Vector2(SCALE_X, SCALE_Y));
	
	// Collider Box ��ġ �ʱ�ȭ
	for (UINT i = 0; i < BUBBLE_LINE_SIZE_Y; i++) {
		Vector2 position = Vector2(-228.0f, 324.0f);
		if (i % 2 == 1) position.x += BUBBLE_SIZE_X / 2;
		position.y -= (i * BUBBLE_SIZE_Y);

		for (UINT j = 0; j < BUBBLE_LINE_SIZE_X; j++) {
			boxLine[i][j] = position;
			position.x += BUBBLE_SIZE_X;
		}
	}
	
	// Collider Box �ʱ�ȭ
	for (UINT i = 0; i < BUBBLE_LINE_SIZE_Y-1; i++) {
		for (UINT j = 0; j < BUBBLE_LINE_SIZE_X; j++) {
			if (i % 2 == 1 && j == BUBBLE_LINE_SIZE_X - 1);
			else
				bubbleBox[i][j] = new Rect(boxLine[i][j], Vector2(BUBBLE_SIZE_X, BUBBLE_SIZE_Y));
		}
	}
}

S02_EditorScene::~S02_EditorScene()
{
	SAFE_DELETE(background);

	for (UINT i = 0; i < BUBBLE_LINE_SIZE_Y-1; i++) {
		for (UINT j = 0; j < BUBBLE_LINE_SIZE_X; j++) {
			SAFE_DELETE(bubbleBox[i][j]);
		}
	}
}

void S02_EditorScene::Update()
{
	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;

	GENERATE_MOUSE_UNPROJECT(mouse);

	background->Update(V, P);
	
	for (UINT i = 0; i < BUBBLE_LINE_SIZE_Y-1; i++) {
		for (UINT j = 0; j < BUBBLE_LINE_SIZE_X; j++) {
			if (i % 2 == 1 && j == BUBBLE_LINE_SIZE_X - 1);
			else
			bubbleBox[i][j]->Update(V, P);
		}
	}
}

void S02_EditorScene::Render()
{
	background->Render();
	for (UINT i = 0; i < BUBBLE_LINE_SIZE_Y-1; i++) {
		for (UINT j = 0; j < BUBBLE_LINE_SIZE_X; j++) {
			if (i % 2 == 1 && j == BUBBLE_LINE_SIZE_X - 1);
			else
			bubbleBox[i][j]->Render();
		}
	}
	
	RenderImGui();
}

void S02_EditorScene::EditBubble() {
	if (ImGui::GetIO().WantCaptureMouse)
		return;

	// Ŭ���� ���� ��ġ
	if (Mouse->Down(0)) {
		for (UINT i = 0; i < BUBBLE_LINE_SIZE_Y - 1; i++) {
			for (UINT j = 0; j < BUBBLE_LINE_SIZE_X; j++) {
				if (i % 2 == 1 && j == BUBBLE_LINE_SIZE_X - 1);
				else {
					if (Collider::Aabb(bubbleBox[i][j]->World(), mouse)) {
						// ������
						switch (bubbleColor) {
						case 1:
							bubbleBox[i][j]->Color(0, 0.2, 0.9, 1);
							break;
						case 2:
							bubbleBox[i][j]->Color(0.9, 0.8, 0, 1);
							break;

						default:
							
						}
					}
				}
			}
		}
	}
}
void S02_EditorScene::RenderImGui() {

	ImGui::Combo("BubbleColor", &bubbleColor, "None\0Blue\0Red\0Green\0Purple\0Orange\0Black\0White");
	
}