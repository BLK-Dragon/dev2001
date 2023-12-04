#if !defined _DWIGAPP_HPP_
//==============================================================================
//
//  DWIG application decl
//
//==============================================================================
//
//  externals:

    #include <vector>
    
    #include "..\..\sys\Application.hpp"
    #include "..\..\sys\ResourcePack.hpp"
    #include "..\..\sys\Sprite.hpp"

    #include "Matrix.hpp"


//==============================================================================
//
//  publics:




class
DwigApp 
  : public core::Application
{
  
  public:    
    
                        DwigApp();
                       ~DwigApp();
    
    virtual void        init();
    virtual state       update();
    virtual void        draw();

 
  private:
    core::FileSystem            file_system;
    core::ResourcePack*         resource_pack;


    // animation issues

    std::vector<Rect2D>         dirty_rect;


    // Matrix issues

    Matrix                      matrix;

    
    // state handling issues
    



    class
    StateHandler
      : public core::KbdEvent
    {
      public:

        StateHandler( DwigApp* a )
          : app(a),
            locked(false)
        {};
        
        bool                                locked_in() { return locked; };

        virtual void                        enter_state() {};
        virtual void                        leave_state() {};
        
        virtual void                        draw() =0;
        virtual core::Application::state    update() =0;

      
      protected:

        void                                lock() { locked = true; };
        void                                unlock() { locked = false; };
        DwigApp*    app;
        
      
      private:

        bool        locked;
    };

    
    // Initial State




    class InitialStateHandler
      : public StateHandler
    {
      public:

                                            InitialStateHandler(  DwigApp* a  )
                                              : StateHandler(a)
                                            {}
        
        virtual void                        enter_state();
        virtual void                        draw();
        virtual core::Application::state    update();
        
        virtual void                        key_down( core::UDword key );
         
    };
    
    
    // Main Menu handler
    



    class MainMenuHandler 
      : public StateHandler
    {    
      
      public:    

                                            MainMenuHandler( DwigApp* a )
                                              : StateHandler(a),
                                                cur_item(0),
                                                prev_item(-1),
                                                first_entrance(true),
                                                menu_item_changed(true),
                                                exiting(false)
                                            {};
                                            ~MainMenuHandler();
        
        virtual void                        enter_state();
        virtual void                        leave_state();

        virtual void                        draw();
        virtual core::Application::state    update();

        virtual void                        key_down( core::UDword key );

      
      private:

        enum
        {
            options     = 0,
            play_bout   = 1,
            quit        = 2,

            max_item    = 2
        };

        enum
        {
            item_normal     = 0,
            item_selected   = 1
        };
        std::vector<core::Sprite*>          menu_item;
        int                                 prev_item, cur_item;

        bool                                first_entrance;
        bool                                first_draw;
        bool                                menu_item_changed;
        bool                                exiting;
    };
    

    // Bout state handler




    class
    BoutHandler
      : public StateHandler
    {
      
      public:                                            
                                            
                                            BoutHandler(  DwigApp* a  )
                                              : StateHandler(a),
                                                first_draw(true)
                                            {}
        
        virtual void                        draw();
        virtual core::Application::state    update();
        
        virtual void                        enter_state();
        virtual void                        leave_state();
        
        virtual void                        key_down( core::UDword key );

      
      private:

        bool                                first_draw;
         
    };


    enum
    {
        state_MainMenu      = 0,
        state_InitialState  = 1,
        state_Bout          = 2
    };
    std::vector<StateHandler*>      state_handler;
    StateHandler*                   cur_state_handler;
    StateHandler*                   new_state_handler;


    void                change_state( int s );

};

#endif // _DWIGAPP_HPP_
