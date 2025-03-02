#include "stk.h"
#include "stdio.h"

char seciliProgram[20];

// Kullanıcının seçtiği programı oku
ListWheelGet("ListWheel1", "Value", seciliProgram);

// Variable içine kaydet
VarSet("VPProgramSecimi", seciliProgram);


printf("VPProgramSecimi:%s \n",seciliProgram);





