#include "Application.h"

int main(void)
{
    Application application;

    application.Initialize(1280, 720);
    
    application.Update();

    return 0;
}
