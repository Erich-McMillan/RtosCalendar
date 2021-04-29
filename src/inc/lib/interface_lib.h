// to be implemented by device
// move following functions into tm4c lib
#include <typedef.h>

typedef enum _ButtonId {
		NONE=0,
		UP=1,
		DOWN,
		LEFT,
		RIGHT,
		SELECT,
		BACK
} ButtonId;

void SetPendingButtonPress(uint8_t buttonid);
void WaitForInput(void);
uint8_t IsUpButtonPressed(void);
uint8_t IsDownButtonPressed(void);
uint8_t IsLeftButtonPressed(void);
uint8_t IsRightButtonPressed(void);
uint8_t IsSelectButtonPressed(void);
uint8_t IsBackButtonPressed(void);
