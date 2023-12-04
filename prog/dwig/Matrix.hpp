#if !defined _MATRIX_HPP_
#define _MATRIX_HPP_
//==============================================================================
//
//  matrix declaration
//
//==============================================================================
//
//  externals:
    
    #include "..\..\sys\Exception.hpp"
    #include "..\..\sys\Sprite.hpp"
    #include "..\..\sys\ResourcePack.hpp"
    
    #include "Clip.hpp"
    #include "Cursor.hpp"
    #include "SchemeElement.hpp"
    #include "View.hpp"



//==============================================================================
//
//  publics:




class 
Matrix
  : public View
{
  public:
                                Matrix();
                               ~Matrix();  


    void                        re_create( int level, int rank );
    void                        bind_to_resourcepack( core::ResourcePack* rp );
    
    void                        save_background( core::Surface& surf );
    void                        redraw( core::Surface& surf );
    void                        restore_background( core::Surface& surf );

    int                         num_lines() const;

    Cursor*                     add_cursor( const Vector2i& pos );

    
  private:
    
    void                        clear();
    Clip*                       clip_at( const Vector2i& pos );
    void                        add_jumper( const Vector2i& in_pos, 
                                            const Vector2i& out_pos 
                                          );
    void                        add_clip( const Vector2i& pos );
    void                        add_indicator( int line );
    std::vector<Clip*>          clip;
    std::vector<SchemeElement*> element;
    std::vector<Cursor*>        cursor;
    std::vector<View*>          view;

    int                         max_node_x_coord;
    int                         n_lines;
    core::Sprite*               matrix_view;
    core::ResourcePack*         resource_pack;
};


//------------------------------------------------------------------------------

inline int                         
Matrix::num_lines() const
{
    return n_lines; 
}



//==============================================================================




class
MatrixError
  : public core::Error
{
  
  public:

                    MatrixError( const core::Msg& msg="" );
};


//------------------------------------------------------------------------------

inline
MatrixError::MatrixError( const core::Msg& msg )
  : core::Error( msg, "Matrix Error" )
{
}


#endif // _MATRIX_HPP_
