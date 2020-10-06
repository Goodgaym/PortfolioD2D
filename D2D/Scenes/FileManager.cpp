#include "stdafx.h"
#include "Filemanager.h"


UINT FileManager::stageBubbles[BUBBLE_LINE_SIZE_Y][BUBBLE_LINE_SIZE_X];

void FileManager::Save(wstring fileName)
{
	int msg = MessageBox(Hwnd, L"Save File?", L"Save Object", MB_YESNO);

	if (msg == IDYES) {
		BinaryWriter* w = new BinaryWriter();

		string str = String::ToString(fileName);
		w->Open(fileName);

		for (int i = 0; i < BUBBLE_LINE_SIZE_Y; i++) {
			for (int j = 0; j < BUBBLE_LINE_SIZE_X; j++) {
				w->UInt(stageBubbles[i][j]);
			}
		}

		w->Close();
		SAFE_DELETE(w);

		memset(stageBubbles, 0, sizeof(stageBubbles));
		wstring temp = L"\n ������ �Ϸ�Ǿ����ϴ�.";
		MessageBox(Hwnd, temp.c_str(), L"����Ϸ�", MB_OK);
	}
}

void FileManager::Load(wstring fileName)
{
	BinaryReader* r = new BinaryReader();

	if (Path::ExistFile(fileName) == true)
		r->Open(fileName);
	else
		assert(false);

	memset(stageBubbles, 0, sizeof(stageBubbles));

	for (int i = 0; i < BUBBLE_LINE_SIZE_Y; i++) {
		for (int j = 0; j < BUBBLE_LINE_SIZE_X; j++) {
			stageBubbles[i][j] = r->UInt();
		}
	}

	r->Close();
	SAFE_DELETE(r);
}

void* FileManager::GetBubbles()
{
	return stageBubbles;
}