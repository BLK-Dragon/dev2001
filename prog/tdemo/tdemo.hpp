//==============================================================================
//
//  Tdemo app decl   
//
//==============================================================================
//
//  externals: 

    #include "..\..\sys\Application.hpp"
    #include "..\..\sys\ResourcePack.hpp"
    #include "..\..\sys\ParamSet.hpp"
    
    #include "..\..\sys\FileSystem.hpp"
    #include "..\..\sys\Font.hpp"
    
    #include "..\..\sys\GUImanager.hpp"
    #include "..\..\sys\WidgetView.hpp"
    
    #include "Asteroid.hpp"
    #include "Explosion.hpp"
    #include "Missile.hpp"
    #include "PlayerShip.hpp"
    #include "BackgroundGenerator.hpp"

    #include "Scaner.hpp"


//==============================================================================
//
//  public:




class
Tdemo
  : public core::Application  
{
  public:

                                    Tdemo();
                                   ~Tdemo();

    void                            init();
    core::Application::state        update();
    void                            draw();
    core::ScreenBuffer::FlipMethod  flip_method();

  
  private:

    static void                 bind_members( core::ParamSetPool* pool );
    static void                 bind_asteroid_members( core::ParamSetPool* p );
    core::ParamSetManager       paramset_manager;
    //core::ParamSetPool*         paramset_pool;
    
    int                         xres;
    int                         yres;
    int                         freq;
    int                         zero_y;
    const int                   dead_zone;

    int                         score;
    core::FileSystem            file_system;
    core::ResourcePack*         resource_pack;
    core::RasterFontManager     font_manager;
    gui::GUImanager             gui_manager;
    gui::StaticTextView*        fps_view;
    gui::StaticTextView*        slowdown_view;
    gui::StaticTextView*        timing0_view;
    gui::StaticTextView*        peak1_view;
    gui::ProgressBarView*       x_speed_view;
    gui::StaticTextView*        x_speed_value_view;
    gui::ProgressBarView*       y_speed_view;
    gui::StaticTextView*        y_speed_value_view;
    gui::ProgressBarView*       laser_t_view;
    gui::StaticTextView*        laser_t_text1;
    gui::StaticTextView*        laser_t_text2;
    gui::StaticTextView*        score_view;
    ScanerView*                 scaner_view;
  
    core::UDword                scanline32[1024];
    BackgroundGenerator*        bk_generator;

    //std::vector<Asteroid*>      asteroid;
    //HeroShip*                   hero_ship;
    //std::vector<LaserBeam*>     laser_beam;

    std::vector<SpaceObject*>   space_object;
    PlayerShip*                 player_ship;



    struct
    {
        int     ttw;
        int     delay;
        int     speed;
    }                           missile1;



    struct
    {
        double  t;
        double  inc_dt;
        double  dec_dt;
        double  max_t; 
    }                           laser;

    Vector2i                    fire_point1;
    
    void                        manage_asteroids();
    void                        manage_missiles();
    void                        manage_player_ship();
    core::Sprite*               title;
    
    int                         score_value;
    
    double                      cur_time[10];
    double                      peak_time[10];
    double                      timing[10];
    int                         n_frames_timed[10];

    long                        fps_t0;
    long                        fps_dt;
    int                         fps_frames;
    int                         cur_fps;
    
    long                        avg_fps_t0;
    long                        avg_fps_t1;
    int                         frames_rendered;

    
    const int                   max_slowdown;
    int                         slowdown_rate;

    core::ScreenBuffer::FlipMethod    flipmethod;
};


