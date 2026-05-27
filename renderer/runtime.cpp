#include "runtime.h"
#include "screen.h"

namespace renderer {
    RunTime::RunTime(ScreenWidth screen_width, ScreenHeight screen_height) : window_(sf::VideoMode(screen_width, screen_height), "3d_renderer"){
    }

    bool RunTime::IsOpen(){
        return window_.isOpen();
    }

    bool RunTime::PollEvent(sf::Event& event){
        return window_.pollEvent(event);
    }

    void RunTime::Close(){
        window_.close();
    }

    sf::RenderWindow* RunTime::GetWindow(){
        return &window_;
    }
    
    void RunTime::SetTitle(std::string txt){
        window_.setTitle(txt);
    }
}  // namespace renderer
