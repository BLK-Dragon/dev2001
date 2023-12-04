#if !defined _MENU_HPP_
#define _MENU_HPP_
//==============================================================================
//
//  Menu related things   
//
//==============================================================================
//
//  externals: 

    #include "..\..\sys\Event.hpp"
    #include "View.hpp"


//==============================================================================
//
//
//  publics:




class
MenuEvent  
{
  public:

    virtual void        selection_changed( int old_item, int new_item ) {;};
};


//==============================================================================




class
Menu  
  : public EventSource<MenuEvent>
{
  public:

                    Menu( int n_items );

    void            move_down();
    void            move_up();

  
  private:

    int             num_items;
    int             cur_item;
};


//==============================================================================




class
MenuView  
  : public View,
    public MenuEvent
{
  public:

                        MenuView( const std::vector<Image&>& images );
    
    virtual void        save_background();
    virtual void        redraw();
    virtual void        restore_background();
    
    virtual void        selection_changed( int old_item, int new_item );
};



//==============================================================================
//
//  Menu
//

inline
Menu::Menu( int n_items )
  : num_items(n_items)
{
}


//------------------------------------------------------------------------------

inline void 
Menu::move_down()
{
    int     old_item = cur_item;
    
    ++cur_item;

    if( cur_item >= num_items )
        cur_item = 0;

    EventSource<MenuEvent>::fire( MenuEvent::selection_changed, old_item, cur_item );
}


//------------------------------------------------------------------------------

inline void 
Menu::move_up()
{
    int     old_item = cur_item;
    
    --cur_item;

    if( cur_item < 0 )
        cur_item = num_items-1;

    EventSource<MenuEvent>::fire( MenuEvent::selection_changed, old_item, cur_item );
}

#endif // _MENU_HPP_