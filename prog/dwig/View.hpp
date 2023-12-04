#if !defined _VIEW_HPP_
#define _VIEW_HPP_
//==============================================================================
//
//  View base class
//
//==============================================================================
//
//  externals:

    #include "..\..\sys\Surface.hpp"


//==============================================================================
//
//  publics:




class
View
{

  public:    
    
    virtual void        save_background() =0;
    virtual void        redraw() =0;
    virtual void        restore_background() =0;
/*    
    static const int    x_center;
    static const int    t_origin;
    static const int    l_origin;
    static const int    r_origin;
    static const int    cell_width;
    static const int    cell_height;
    static const int    cell_dw;
*/
};

#endif // _VIEW_HPP_
