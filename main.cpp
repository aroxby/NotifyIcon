// Example Implementation
// In a future iteration this could become the test driver

#include <Windows.h>
#include "NotifcationIcon.h"

void pause() {
    system("pause");
}

int main(int argc, char *argv[]) {
    HICON ico = ExtractIcon(0, "Shell32.dll", 2);
    NotifcationIcon icon;
    icon.setIcon(ico);
    icon.setTooltip("Test Icon");
    icon.add();
    icon.show();
    pause();
    icon.showBalloon("So, this is working...", "For real");
    pause();
    return 0;
}