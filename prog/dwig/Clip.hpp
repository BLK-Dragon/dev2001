#if !defined _CLIP_HPP_
#define _CLIP_HPP_
//==============================================================================
//
//
//
//==============================================================================
//
//  externals:

    #include <vector>

    #include "..\..\sys\Vector.hpp"
    


//==============================================================================
//
//  publics:

class SchemeElement;



class 
Clip
{
    
  public:
    
                            Clip( const Vector2i& p );
    
    Vector2i                position() const;
    float                   cur_intensity() const;

    void                    connect( SchemeElement* elem );

  
  private:    
    
    Vector2i                    pos;
    std::vector<SchemeElement*> element;
};


//------------------------------------------------------------------------------

inline 
Clip::Clip( const Vector2i& p )
  : pos(p)
{
}


//------------------------------------------------------------------------------

inline Vector2i                
Clip::position() const
{
    return pos; 
}


//------------------------------------------------------------------------------

inline void
Clip::connect( SchemeElement* elem )
{
    element.push_back( elem );
}


#endif // _CLIP_HPP_
