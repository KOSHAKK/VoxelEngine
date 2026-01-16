//#pragma once
//
//#include <memory>
//
//class Application
//{
//public:
//    Application();
//    
//    Application(const Application&) = delete;
//    Application(Application&&) = delete;
//    Application& operator=(const Application&) = delete;
//    Application& operator=(Application&&) = delete;
//
//
//private:
//    void init();
//    void shutdown();
//    void processEvents();
//    void render();
//
//private:
//    std::unique_ptr<Window> m_window;
//    std::unique_ptr<Game> m_game;
//
//    float deltaTime = 0.f;
//};
