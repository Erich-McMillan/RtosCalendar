// to be implemented by device
// move following functions into tm4c lib
#include <typedef.h>

typedef enum ButtonId {
		NONE=0,
		UP=1,
		DOWN,
		LEFT,
		RIGHT,
		SELECT,
		BACK
};

void SetPendingButtonPress(uint8_t buttonid);
void WaitForInput();
uint8_t IsUpButtonPressed();
uint8_t IsDownButtonPressed();
uint8_t IsLeftButtonPressed();
uint8_t IsRightButtonPressed();
uint8_t IsSelectButtonPressed();
uint8_t IsBackButtonPressed();