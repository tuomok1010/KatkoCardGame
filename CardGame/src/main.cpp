#include <iostream>
#include <vector>

#include "GameManager.h"

int main()
{
	GameManager GM;

	GM.ProcessMainMenu();

	GM.PlayRound();

	return 0;
}