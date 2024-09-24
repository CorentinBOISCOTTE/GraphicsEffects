#include "Application.h"

int main(void)
{
    Application application;

    application.Initialize(1920, 1080);
    
    application.Update();

    return 0;
}
