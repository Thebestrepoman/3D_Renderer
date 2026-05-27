#include "renderer/renderer.h"
#include "application/application.h"
#include "application/except.h"

int main() {
    try{
        renderer::Application app;
        app.Run();
    } catch(...){
        renderer::React();
    }
    return 0;
}
