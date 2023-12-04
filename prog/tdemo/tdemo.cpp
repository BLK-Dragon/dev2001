//==============================================================================
//
//  Tdemo app realization
//
//==============================================================================
//
//  externals: 

    #include "..\..\sys\Random.hpp"
    #include "..\..\sys\ZipFile.hpp"
    #include "..\..\sys\Tune.hpp"
    
    #include "tdemo.hpp"

    const int SlowDownRate  = 10;

//==============================================================================
//
//  publics:


core::Application*
CreateApplication()
{
    return new Tdemo; 
}


//==============================================================================

Tdemo::Tdemo()
  : //paramset_pool("tdemo"),
    //heroship_paramset("hero_ship"),
    zero_y(0),
    score_value(0),
    max_slowdown(10),
    slowdown_rate(1),
    dead_zone(500),
    flipmethod(core::ScreenBuffer::vsync),
    frames_rendered(0)
{   
    
    /*
    for( i=0; i<tdemo_data.max_asteroids; i++ )
    {                                     
        int     r   = vfx::rnd(tdemo_data.asteroid_image.size());
        Image&  img = resource_pack->image( tdemo_data.asteroid_image[r],
                                            0x00FF00FF, 60
                                          );
        
        asteroid.push_back( new Asteroid( img,
                                          Vector2D( vfx::rnd(xres),
                                                    zero_y+yres+100+
                                                    vfx::rnd(yres)
                                                  )
                                        )
                          );
        asteroid[ asteroid.size()-1 ]->set_velocity(Vector2D(0,-1-vfx::rnd(10)/
                                                                100.0) 
                                                   );
    }

    for( i=0; i<15; i++ )
    {
        core::Image&    img = resource_pack->image( ".\\image\\beam.tga",
                                                    0x00FF00FF
                                                  );
        LaserBeam*      beam = new LaserBeam( img,
                                              Vector2D( 50, zero_y-10),
                                              Vector2D( 0, 5.0 )
                                            );
        if( beam ) 
            laser_beam.push_back( beam );
        else
            throw core::Error("can't create LaserBeam");
    }

    
    hero_ship   = new HeroShip( resource_pack->image( ".\\image\\hero_ship.tga",
                                                      0x00FF00FF,
                                                      5
                                                    ),
                                Vector2D( xres/2, zero_y+100 ),
                                heroship_data
                              );

    beam_delay  = tdemo_data.beam_delay;
    beam_speed  = tdemo_data.beam_speed;
    beam_ttw    = 0;
    */
    
    
    // texts
    /*
    title  = new core::Sprite( resource_pack->image( ".\\image\\title.tga", 
                                                     0x00FF00FF
                                                   )
                             );
    title->place( Point2D(10,10) );
    
    energy = new core::Sprite( resource_pack->image( ".\\image\\energy.tga", 
                                                     0x00FF00FF
                                                   )
                             );
    energy->place( Point2D(xres-100,10) );
    
    score  = new core::Sprite( resource_pack->image( ".\\image\\score.tga", 
                                                     0x00FF00FF
                                                   )
                             );
    score->place( Point2D(10,10) );
    */
}


//------------------------------------------------------------------------------

void
Tdemo::init()
{
    zero_y      = 500000;
    score       = 0;
    
    using core::Image;
    using vfx::rgb;
    using vfx::bgr;
    int i;
    
    for( i=0; i<10; i++ )
    {    
        n_frames_timed[i]   = 0;
        timing[i]           = 0.0;
        peak_time[i]        = 0;
    }
    
    fps_dt      = 0;
    cur_fps     = 0;
    fps_t0      = ::GetTickCount();
    fps_frames  = 0;

    paramset_manager.register_paramset( "tdemo", bind_members );
    paramset_manager.register_paramset( "asteroid", bind_asteroid_members );
    BackgroundGenerator::register_paramset( &paramset_manager );
    PlayerShip::register_paramset( &paramset_manager );

    core::BinFile*  tdemo_ini       = file_system.create_file("tdemo.ini");
    core::BinFile*  background_ini  = file_system.create_file("background.ini");
    core::BinFile*  techno_font     = file_system.create_file("techno.ini");
    core::BinFile*  digital_font    = file_system.create_file("digital.ini");
    
    paramset_manager.parse_ini_file( tdemo_ini );
    paramset_manager.parse_ini_file( background_ini );
    font_manager.load_font( techno_font );
    font_manager.load_font( digital_font );

    delete tdemo_ini;
    delete background_ini;
    delete techno_font;
    delete digital_font;

    core::ParamSet  cur_paramset = paramset_manager.paramset( "tdemo", 0 );

              

    int     xpos, ypos;
    
    if( std::string(cur_paramset["fullscreen"]) == "true" )
    {
        xres = cur_paramset["xres"];
        yres = cur_paramset["yres"];
        xpos = 0;
        ypos = 0;
        freq = cur_paramset["freq"];
        videocard->set_mode( xres, yres, 16, freq );
    }
    else
    {
        xres = cur_paramset["wnd_xres"];
        yres = cur_paramset["wnd_yres"];
        xpos = cur_paramset["wnd_xpos"];
        ypos = cur_paramset["wnd_ypos"];
    }

    if( std::string(cur_paramset["vsync"]) == "true" )
        flipmethod = core::ScreenBuffer::vsync;
    else
        flipmethod = core::ScreenBuffer::no_vsync;

    
    slowdown_rate = cur_paramset["dbg.slowdown_rate"];
    
    std::clog << xres << "x" << yres << "@" << freq << "\n" << std::flush;
    std::clog <<  (const char*)cur_paramset["version"] << "\n"
              << std::flush;
                                                    
    resource_pack   = new core::ResourcePack( &file_system );
    window          = new core::Window( xpos, ypos, xres, yres );
    bk_generator    = new BackgroundGenerator( xres );

    
    // pre-cache images
    
    for( i=0; i<paramset_manager.paramset_count("asteroid"); i++ )
    {
        core::ParamSet  paramset = paramset_manager.paramset( "asteroid", i );

        resource_pack->precache_image( (const char*)paramset["image"] );
    }


    // add objects
    
    for( i=0; i<(int)cur_paramset["asteroid_count"]; i++ )
    {
        int v = vfx::rnd( paramset_manager.paramset_count("asteroid") );
        
        core::ParamSet  paramset = paramset_manager.paramset( "asteroid", v );
        std::string     img_name = paramset["image"];
        core::Image&    img      = resource_pack->image( img_name );
        double          speed    = 0.0;

        speed = (double)paramset["max_speed"] - (double)paramset["base_speed"];
        speed = (double)paramset["base_speed"] +
                speed * double(vfx::rnd(100))/100.0;

        Asteroid*       asteroid = new Asteroid( img, 
                                                 Vector2f( vfx::rnd(xres),
                                                           zero_y - 200 - 
                                                               vfx::rnd(500)
                                                     ),
                                                 Vector2f( 0, speed )
                                               );
        //asteroid->set_velocity( Vector2f(0,(double)paramset["speed"]) );                       
        asteroid->set_animation_speed( (double)paramset["animation_speed"] );
        asteroid->set_phase( vfx::rnd(asteroid->num_phases()) );
        space_object.push_back( asteroid );

        //asteroid->set_animation_speed( 0.5 );
        //asteroid->set_velocity( Vector2f(0,0.5) );
    }
    
     
    // add player ship

    Image& img  = resource_pack->image( cur_paramset["playership_image"] );
    player_ship = new PlayerShip( img,
                                  Vector2f( xres/2, zero_y+yres-100 ),
                                  Vector2f( 0, -1 )
                                );
    player_ship->set_constrain_rect( Rect2D( 0-20,0-20, xres+20,yres+20 ) );
    space_object.push_back( player_ship );


    // add missiles

    for( i=0; i<15; i++ )
    {
        core::Image&    img     = resource_pack->image(".\\image\\missile.tga");
        Missile*        missile = new Missile( img );
        
        missile->kill();
        space_object.push_back( missile );
    }

    missile1.delay  = cur_paramset["missile1.fire_delay"];
    missile1.ttw    = missile1.delay;
    missile1.speed  = cur_paramset["missile1.speed"];

    fire_point1     = Vector2i( cur_paramset["fire_point1.x"], 
                                cur_paramset["fire_point1.y"] 
                              );

    laser.max_t     = cur_paramset["laser.max_t"];
    laser.inc_dt    = cur_paramset["laser.inc_dt"];
    laser.dec_dt    = cur_paramset["laser.dec_dt"];
    laser.t         = 0.0;

    
    // add explosions
    
    for( i=0; i<15; i++ )
    {
        core::Image&    img         = resource_pack->image(".\\image\\explosion.tga");
        Explosion*      explosion   = new Explosion( img, Vector2f(0,0) );

        explosion->set_animation_speed( 0.3 );
        explosion->kill();

        space_object.push_back( explosion );
    }
    
    
    // init GUI elements

    fps_view = new gui::StaticTextView( &gui_manager, 
                                        Rect2D( Vector2i(xres/2-40,10), 80, 20 ) 
                                      );
    fps_view->set_font( &font_manager.font("Techno") );
    fps_view->set_text_color( gui::v_gradient, 0x02C5FD, 0x1150E3 );
    fps_view->set_text_style( gui::double_interlaced );


    slowdown_view = new gui::StaticTextView( &gui_manager, 
                                             Rect2D( Vector2i(10,10), 30, 10 ) 
                                           );
    slowdown_view->set_font( &font_manager.font("Digital Display") );
    slowdown_view->set_text_color( gui::v_gradient, 0xCCEE81, 0x80FF80 );
    slowdown_view->set_text_style( gui::normal );
    
    timing0_view = new gui::StaticTextView( &gui_manager, 
                                            Rect2D( Vector2i(50,10), 100, 10 ) 
                                          );
    timing0_view->set_font( &font_manager.font("Digital Display") );
    timing0_view->set_text_color( gui::v_gradient, 0x555555, 0xCCEE81 );
    timing0_view->set_text_style( gui::normal );
    
    peak1_view = new gui::StaticTextView( &gui_manager, 
                                          Rect2D( Vector2i(50,25), 100, 10 ) 
                                        );
    peak1_view->set_font( &font_manager.font("Digital Display") );
    peak1_view->set_text_color( gui::v_gradient, 0x3399FF, 0x0000FF );
    peak1_view->set_text_style( gui::normal );


    x_speed_view = new gui::ProgressBarView( &gui_manager, 
                                             Rect2D( Vector2i(145,yres-40), 80,10 ) 
                                           );
    x_speed_view->set_color( gui::h_gradient, 
                             rgb(0xB6,0x92,0xE7), rgb(0xD3,0x44,0xDF)
                           );
    x_speed_view->set_style( gui::v_interlaced );

    x_speed_value_view = new gui::StaticTextView( &gui_manager, 
                                                  Rect2D( Vector2i(10,yres-40), 
                                                          130, 10 
                                                        ) 
                                                );
    x_speed_value_view->set_font( &font_manager.font("Digital Display") );
    x_speed_value_view->set_text_color( gui::v_gradient, 
                                        rgb(0x20,0xB7,0xFF), rgb(0x00,0x76,0xAE) 
                                      );
    x_speed_value_view->set_text_style( gui::normal );


    y_speed_view = new gui::ProgressBarView( &gui_manager,
                                             Rect2D( Vector2i(145,yres-20), 80,10 )
                                           );
    y_speed_view->set_color( gui::h_gradient, 
                             rgb(0xB6,0x92,0xE7), rgb(0xD3,0x44,0xDF)
                           );
    y_speed_view->set_style( gui::v_interlaced );
    
    y_speed_value_view = new gui::StaticTextView( &gui_manager, 
                                                  Rect2D( Vector2i(10,yres-20), 
                                                          130, 10 
                                                        ) 
                                                );
    y_speed_value_view->set_font( &font_manager.font("Digital Display") );
    y_speed_value_view->set_text_color( gui::v_gradient, 
                                        rgb(0x20,0xB7,0xFF), rgb(0x00,0x76,0xAE)
                                      );
    y_speed_value_view->set_text_style( gui::normal );

    
    laser_t_view = new gui::ProgressBarView( &gui_manager,
                                             Rect2D( Vector2i(145,yres-70), 80,10 )
                                           );
    laser_t_view->set_color( gui::h_gradient, 0x00FFFF, 0x0000FF );
    laser_t_view->set_style( gui::v_interlaced );
    laser_t_text1 = new gui::StaticTextView( &gui_manager, 
                                             Rect2D( Vector2i(10,yres-70), 
                                                     130, 10 
                                                   ) 
                                           );
    laser_t_text1->set_font( &font_manager.font("Digital Display") );
    laser_t_text1->set_text_color( gui::v_gradient, 
                                   rgb(0xFF,0xFF,0xA8), rgb(0xFF,0x00,0x00)
                                 );
    laser_t_text2 = new gui::StaticTextView( &gui_manager, 
                                             Rect2D( Vector2i(145+80+5,yres-70), 
                                                     130, 10 
                                                   ) 
                                           );
    laser_t_text2->set_font( &font_manager.font("Digital Display") );
    laser_t_text2->set_text_color( gui::v_gradient, 
                                   rgb(0xFF,0x55,0x00), rgb(0xFF,0x00,0x00)
                                 );

    score_view = new gui::StaticTextView( &gui_manager, 
                                          Rect2D( Vector2i(xres-150,10), 
                                                  130, 20 
                                                ) 
                                        );
    
    score_view->set_font( &font_manager.font("Techno") );
    score_view->set_text_color( gui::v_gradient, 0xFFFFFF, 0x0000FF );
    score_view->set_text_style( gui::double_interlaced );

    float   ratio   = float(yres+1.125*dead_zone) / xres;
    float   sw      = 180;
    float   sh      = sw * ratio;
    
    scaner_view = new ScanerView( &gui_manager, 
                                  Rect2D( 0, -dead_zone, xres, yres+dead_zone ),
                                  Rect2D( Vector2i( xres-sw-5, yres-sh-5), 
                                          sw,sh
                                        )
                                );
    
    // zip testing
    /*
    core::ZipBinFile    zipfile("test.zip");

    zipfile.read_file("ini/background.ini");
    while( !zipfile.eof() )
    {
        char    str[120];
        zipfile.read_string( 120, str ); 
        std::clog << "'" << str << "'\n" << std::flush;
    }
    
    throw core::Error("EXIT");
    */
    bk_generator->activate( 0 );
    avg_fps_t0 = ::GetTickCount();
}


//------------------------------------------------------------------------------

Tdemo::~Tdemo()
{
    for( unsigned i=0; i<space_object.size(); i++ )
        delete space_object[i];


    delete fps_view;
    delete slowdown_view;
    delete timing0_view;
    delete peak1_view;
    delete x_speed_view;
    delete y_speed_view;
    delete x_speed_value_view;
    delete y_speed_value_view;
    delete laser_t_view;
    delete laser_t_text1;
    delete laser_t_text2;
    delete score_view;
    delete scaner_view;
    
    //delete title;
    //delete energy;
    /*
    for( int i=0; i<asteroid.size(); i++ )
        delete asteroid[i];
    
    for( i=0; i<laser_beam.size(); i++ )
        delete laser_beam[i];

    delete hero_ship;
    */

    delete bk_generator;
    delete resource_pack;
}


//------------------------------------------------------------------------------

core::Application::state 
Tdemo::update()
{
    int i;

    SpaceObject::set_origin( Vector2i( 0, --zero_y ) );
    
    
    for( i=0; i<space_object.size(); i++ )
    {    
        if( !space_object[i]->is_dead() )
            space_object[i]->update();


        if( space_object[i]->integrity() == 0 )
            space_object[i]->kill();
    }
    
    // check collisions

    for( i=0; i<space_object.size(); i++ )
    {   
        if( space_object[i]->is_dead() )
            continue;
        
        if( dynamic_cast<Explosion*>(space_object[i]) )
            continue; 
        
        for( int j=0; j<space_object.size(); j++ )
        {
            if( i == j )                                    continue;
            if( space_object[j]->is_dead() )                continue;
            if( dynamic_cast<Explosion*>(space_object[j]) ) continue;
            
            
            if( space_object[i]->collides_with( space_object[j] ) )
                space_object[i]->process_collision( space_object[j] );

            if( dynamic_cast<Missile*>(space_object[i])     &&
                dynamic_cast<Asteroid*>(space_object[j])    &&
                space_object[j]->integrity() == 0
              )
            {
                score += 7 + vfx::rnd(5);
            }
        }
    }
    manage_asteroids();
    manage_missiles();
    manage_player_ship();


    // other actions

    laser.t -= laser.dec_dt;
    if( laser.t < 0.0 )
        laser.t = 0.0;

    if( keyboard->pressed( core::Keyboard::esc ) )
    {
        avg_fps_t1 = ::GetTickCount();

        std::clog << "tdemo> average FPS = " 
                  << int( float(frames_rendered) / ((avg_fps_t1-avg_fps_t0)/1000.0) )
                  << "\n";
        
        
        return core::Application::finished;
    }
    
    if( keyboard->pressed( core::Keyboard::tab ) )
    {    
        peak_time[1] = 0;
    }
    
    if( keyboard->pressed( core::Keyboard::enter ) )
    {    
        std::clog << "moment FPS = " << cur_fps << "\n" << std::flush;
    }
    /*
    if( keyboard->pressed( core::Keyboard::lshift ) )
        slowdown_rate = 1;
    else
        slowdown_rate = max_slowdown;
    */
    
    return core::Application::running;
}


//------------------------------------------------------------------------------

void 
Tdemo::draw()
{
    int i;
    
    for( i=0; i<10; i++ )
    {
        if( n_frames_timed[i] == 0 )
            timing[i] = 0.0;
    }
    
//    if( keyboard->pressed( core::Keyboard::space ) )
//        return;

    //std::clog << "Tdemo::draw\n" << std::flush;
    
    core::Surface&  back_buffer    = videocard->back_buffer();
    core::Surface&  front_buffer   = videocard->front_buffer();
    core::Timer     timer;

    //Point2D     pt( window_location().left(), window_location().top()+1 );
    
    //timer.start_timing(0);
    //timer.stop_timing(0);


    // scroll buffer

    front_buffer.blit_to( back_buffer, Vector2i(0,1),
                          Rect2D( window_location().left(),
                                  window_location().top(),
                                  window_location().left() +
                                    back_buffer.width()-1,
                                  window_location().top() +
                                    back_buffer.height()-1-1
                                )
                        );
    
    
    // restore sprites background
    
    for( i=0; i<space_object.size(); i++ )
        space_object[i]->restore_background( back_buffer, Vector2i(0,1) );

    gui_manager.restore_background( back_buffer, Vector2i(0,1) );
    
    
    // draw background

    core::UByte*  line = back_buffer.memory();
    //line += ( window_location().top() + 1 ) * front_buffer.pitch();
    //line += window_location().left() * sizeof(core::UWord);

    core::UWord*  scanline  = (core::UWord*)line;

    ZeroMemory( scanline32, xres*sizeof(core::UDword) );
    timer.start_timing(1);
    for( int n=0; n<slowdown_rate; n++ )
        bk_generator->generate_scanline( scanline32, xres );
    timer.stop_timing(1);
    
    timing[1] += timer.time(1);
    
    /*
    if( ++n_frames_timed[1] == 10 )
    {
        cur_time[1]         = timing[1] / 10.0;
        timing[1]           = 0.0;
        n_frames_timed[1]   = 0;
    }
    */
    bk_generator->advance_to_next_scanline();
    
    for( i=0; i<xres; i++ )
    {
        core::UByte*  color = (core::UByte*)&scanline32[i];
                                                  /*
        scanline[i] = back_buffer.native_color( color[0], 
                                                color[1], 
                                                color[2] 
                                              );*/
        core::UByte*    bp = line+i*(back_buffer.bpp()/8);
        core::UDword*   dp = (core::UDword*)bp;
        *dp = back_buffer.native_color( color[0], 
                                        color[1], 
                                        color[2] 
                                      );
        //scanline[i] = 0xFFFFFF;
    }


    
    
    // save background
    
    for( i=0; i<space_object.size(); i++ )
        space_object[i]->save_background( back_buffer );

    gui_manager.save_background( back_buffer );
    
    
    // draw objects
    
    for( i=0; i<space_object.size(); i++ )
        space_object[i]->redraw( back_buffer );


    // draw info
    
    long    t = ::GetTickCount();

    fps_dt += t - fps_t0;
    fps_t0  = t;
    ++fps_frames;
    
    ++n_frames_timed[1];
    if( fps_dt > 1000 )
    {
        fps_dt     -= 1000;
        cur_fps     = fps_frames;
        fps_frames  = 0;
        
        cur_time[1]         = timing[1] / n_frames_timed[1];
        timing[1]           = 0.0;
        n_frames_timed[1]   = 0;

        if( cur_time[1] > peak_time[1] ) 
            peak_time[1] = cur_time[1];
    }    
    
    /*
    back_buffer.draw_text( 10+2, 10+2, 0x00FFAA11, 15, 
                           core::Msg( "[%02i] %05i   %02i FPS", 
                                      slowdown_rate, (int)cur_time[1], cur_fps 
                                    )
                         );
    */
                       /*
    graph.draw_text( font_manager.font("Digital Display"), Vector2i(10+2,10+2), 
                     "abcdefghijklmnopqrstuvwxyz", 0xFFFFFF 
                   );
    graph.draw_text( font_manager.font("Digital Display"), Vector2i(10+2,10+2),
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0xFFFFFF
                   );
                   */

    fps_view->set_text( core::Msg("%02i\tfps",cur_fps).text() );
    slowdown_view->set_text( core::Msg( "x%02i", slowdown_rate ).text() );
    timing0_view->set_text( core::Msg( "%05i", (int)cur_time[1] ).text() );
    peak1_view->set_text( core::Msg( "%05i", (int)peak_time[1] ).text() );
    
    x_speed_view->set_value( fabs(player_ship->velocity()[0]) / 3.5 );
    x_speed_value_view->set_text( core::Msg( "x-speed % 2.1f",
                                             player_ship->velocity()[0]
                                           ).text().c_str()
                                );
    y_speed_view->set_value( fabs(player_ship->velocity()[1]) / 3.5 );
    y_speed_value_view->set_text( core::Msg( "y-speed % 2.1f",
                                             player_ship->velocity()[0]
                                           ).text().c_str()
                                );

    laser_t_view->set_value( laser.t/laser.max_t );
    laser_t_text1->set_text( "LASER temp." );
    laser_t_text2->set_text( (laser.t<laser.max_t)  ? ""  : "OVERHEAT" );

    score_view->set_text( core::Msg("score %05i",score).text().c_str() );

    scaner_view->clear_all_objects();
    scaner_view->set_controlled_zone( Rect2D( 0, zero_y-dead_zone,
                                              xres, zero_y+yres+0.125*dead_zone
                                            ) 
                                    );
    scaner_view->set_visible_zone( Rect2D( 0, zero_y, xres, zero_y+yres ) );

    for( i=0; i<space_object.size(); i++  )
    {
        core::UDword    color;
        
        if     ( dynamic_cast<Asteroid*>(space_object[i]) )
            color = vfx::rgb(0xFF,0x00,0x00);
        else if( dynamic_cast<PlayerShip*>(space_object[i]) )
            color = vfx::rgb(0xFF,0xFF,0x00);
        else
            continue;

        scaner_view->add_object( Vector2i( space_object[i]->position()[0],
                                           space_object[i]->position()[1]
                                         ),
                                 color
                               ); 
    }

    gui_manager.redraw( back_buffer );

    
    //draw_string( back_buffer, Vector2i(10+2,10+2), "12" );
    
    /*
    std::clog << "scroll: " << timer.time(0) << " ns\n";
    std::clog << "background: " << timer.time(1) << " ns\n" 
              << std::flush;*/
    
    ++frames_rendered;
}


//------------------------------------------------------------------------------

core::ScreenBuffer::FlipMethod  
Tdemo::flip_method()
{
    return flipmethod; 
}


//------------------------------------------------------------------------------

void                 
Tdemo::bind_members( core::ParamSetPool* pool )
{
    pool->start_member_binding();
        pool->bind_string_member( "fullscreen" );
        pool->bind_string_member( "vsync" );
        pool->bind_int_member( "xres" );
        pool->bind_int_member( "yres" );
        pool->bind_int_member( "freq" );
        pool->bind_int_member( "wnd_xres" );
        pool->bind_int_member( "wnd_yres" );
        pool->bind_int_member( "wnd_xpos" );
        pool->bind_int_member( "wnd_ypos" );
        pool->bind_string_member( "version" );

        pool->bind_int_member("dbg.slowdown_rate",1);

        pool->bind_string_member( "playership_image" );
        
        pool->bind_int_member( "asteroid_count" );

        pool->bind_int_member( "missile1.fire_delay", 20 );
        pool->bind_int_member( "missile1.speed", 2 );

        pool->bind_int_member( "fire_point1.x" );
        pool->bind_int_member( "fire_point1.y" );

        pool->bind_double_member( "laser.max_t" );
        pool->bind_double_member( "laser.inc_dt" );
        pool->bind_double_member( "laser.dec_dt" );

        //pool->bind_ubyte_array_member( "bin" );
    pool->finish_member_binding();
}


//------------------------------------------------------------------------------

void                 
Tdemo::bind_asteroid_members( core::ParamSetPool* pool )
{
    pool->start_member_binding();
        pool->bind_string_member( "image" );
        pool->bind_double_member( "base_speed" );
        pool->bind_double_member( "max_speed" );
        pool->bind_double_member( "animation_speed" );
    pool->finish_member_binding();
}


//------------------------------------------------------------------------------

void
Tdemo::manage_asteroids()
{
    for( unsigned i=0; i<space_object.size(); i++ )
    {
        Asteroid*   asteroid;

        if( !(asteroid = dynamic_cast<Asteroid*>(space_object[i]) ) )
            continue;

        if( asteroid->integrity() == 0 )
        {
            // asteroid 'killed' -- create explosion

            for( int i=0; i<space_object.size(); i++ )
            {
                Explosion*  explosion;
                explosion = dynamic_cast<Explosion*>(space_object[i]);

                if( !explosion )                    continue;
                if( !explosion->is_dead() )         continue;
                if( !explosion->respawn_enabled() ) continue;
                
                float   x = asteroid->position()[0] + 
                            asteroid->bound_rect().width()/2 -
                            explosion->bound_rect().width()/2;
                float   y = asteroid->position()[1] +
                            asteroid->bound_rect().height()/2 -
                            explosion->bound_rect().height()/2;
                
                explosion->respawn( Vector2f(x,y), 
                                    asteroid->velocity() 
                                  );
                break;
            } 
        }
        
        if( asteroid->position()[1] > zero_y+yres+dead_zone  ||
            ( asteroid->is_dead()  &&  asteroid->respawn_enabled() )
          )
        {   
            
            asteroid->respawn( Vector2f( vfx::rnd(xres), zero_y - dead_zone ), 
                               Vector2f( 0, 1.0+2.0*(vfx::rnd(10)/10.0) )
                             ); 
            int v = vfx::rnd( paramset_manager.paramset_count("asteroid") );
            
            core::ParamSet  paramset = paramset_manager.paramset("asteroid",v);
            std::string     img_name = paramset["image"];
            core::Image&    img      = resource_pack->image( img_name );
            double          speed    = 0.0;

            speed = (double)paramset["max_speed"] - (double)paramset["base_speed"];
            speed = (double)paramset["base_speed"] + 
                    speed * double(vfx::rnd(100))/100.0;
            
            asteroid->set_velocity( Vector2f( -0.5 + double(vfx::rnd(100))/100.0,
                                              speed
                                            ) 
                                  );                       
            asteroid->set_animation_speed( (double)paramset["animation_speed"] );
            asteroid->set_phase( vfx::rnd(asteroid->num_phases()) );
        }
    }
}


//------------------------------------------------------------------------------

void
Tdemo::manage_missiles()
{
    for( unsigned i=0; i<space_object.size(); i++ )
    {
        Missile*   missile;

        if( !(missile = dynamic_cast<Missile*>(space_object[i]) ) )
            continue;

        if( missile->is_dead() )
            continue;

        if( missile->position()[1] < zero_y-dead_zone )
        {
            missile->kill();
        }
    }
}


//------------------------------------------------------------------------------

void                        
Tdemo::manage_player_ship()
{
    Vector2l    mouse_rel_pos = mouse->relative_position();

    if( mouse_rel_pos[0] < 0 )
        player_ship->accelerate( PlayerShip::left );
    else
        player_ship->decelerate( PlayerShip::left );

    if( mouse_rel_pos[0] > 0 )
        player_ship->accelerate( PlayerShip::right );
    else
        player_ship->decelerate( PlayerShip::right );

    if( mouse_rel_pos[1] < 0 )
        player_ship->accelerate( PlayerShip::up );
    else
        player_ship->decelerate( PlayerShip::up );

    if( mouse_rel_pos[1] > 0 )
        player_ship->accelerate( PlayerShip::down );
    else
        player_ship->decelerate( PlayerShip::down );
    

    if( keyboard->pressed( core::Keyboard::left ) )
        player_ship->accelerate( PlayerShip::left );
    else
        player_ship->decelerate( PlayerShip::left );

    if( keyboard->pressed( core::Keyboard::right ) )
        player_ship->accelerate( PlayerShip::right );
    else
        player_ship->decelerate( PlayerShip::right );

    if( keyboard->pressed( core::Keyboard::up ) )
        player_ship->accelerate( PlayerShip::up );
    else
        player_ship->decelerate( PlayerShip::up );

    if( keyboard->pressed( core::Keyboard::down ) )
        player_ship->accelerate( PlayerShip::down );
    else
        player_ship->decelerate( PlayerShip::down );
     

    if( mouse->pressed_button(0)                    ||  
        keyboard->pressed( core::Keyboard::lshift ) || 
        keyboard->pressed( core::Keyboard::space )
      )
    {
        if( --missile1.ttw == 0  &&  laser.t < laser.max_t )
        {
            for( int i=0; i<space_object.size(); i++ )
            {
                Missile*    missile = dynamic_cast<Missile*>(space_object[i]);

                if( !missile )                      continue;
                if( !missile->is_dead() )           continue;
                if( !missile->respawn_enabled() )   continue;

                int x = fire_point1[0] - missile->bound_rect().width() / 2;
                int y = fire_point1[1] - missile->bound_rect().height();

                x += player_ship->position()[0];
                y += player_ship->position()[1];
                
                missile->respawn( Vector2f(x,y), Vector2f(0,-missile1.speed) );
                laser.t += laser.inc_dt;

                ::PlaySound( ".\\sfx\\shot.wav", NULL, SND_FILENAME|SND_ASYNC );

                //missile1.ttw = missile1.delay;
                break;
            }

            missile1.ttw = missile1.delay * ( 0.8 + 0.4*(float(vfx::rnd(100))/100.0) );
        }

        if( missile1.ttw == 0 ) ++missile1.ttw;
    }
    
    if( keyboard->pressed( core::Keyboard::tab ) )
    {    
    }
}

