#pragma once
#define KEYMAX 256

class Keyboard {
public:
	Keyboard();
	~Keyboard();

	bool Down(int key);
	bool Up(int key);
	bool Press(int key);
	bool Toggle(int key);

private:
	bitset<KEYMAX> up;
	bitset<KEYMAX> down;
};

// bitset �Լ�
// set(i) : �ε����� ���� true, false�� ����
// reset() : �ε����� ���� false�� ����
// flip() : false <-> true �� ��ȯ
// all() : ��� ���Ұ��� true�� true ��ȯ
// none() : ��� ���Ұ��� false�� false ��ȯ
// any() : 1���� true�� true ��ȯ
// count() : true�� ������ ����