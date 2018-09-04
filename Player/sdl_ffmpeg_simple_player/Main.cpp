#include "Player.h"

int main()
{
	InitProgram();
	event_loop();
	UinitProgram();
	printf("\n按任意键退出............\n");
	return getchar();
}
