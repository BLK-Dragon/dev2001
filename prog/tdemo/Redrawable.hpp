#if !defined _REDRAWABLE_HPP_
#define _REDRAWABLE_HPP_
//==============================================================================
//
//  Updateable   
//
//==============================================================================
//
//  externals: 

    #include <algorithm>
    #include <vector>

    #include "..\..\sys\Surface.hpp"


//==============================================================================
//
//  publics:




class
Redrawable  
{
  public:

                                    Redrawable();
    virtual                        ~Redrawable();

    virtual void                    redraw( core::Surface& dst_surf,
                                            const Rect2D& clip_rect = Rect2D()
                                          ) =0;
    virtual void                    restore_background( core::Surface& dst_surf,
                                                        const Point2D& pos,
                                                        const Rect2D& clip_rect 
                                                            = Rect2D()
                                          ) =0;

    static void                     redraw_all( core::Surface& dst_surf,
                                                const Rect2D& clip_rect 
                                                    = Rect2D()
                                              );
    static void                     restore_all( core::Surface& dst_surf,
                                                 const Point2D& offset,
                                                 const Rect2D& clip_rect 
                                                     = Rect2D()
                                               );

  
  private:
    static std::vector<Redrawable*> obj;
};


//------------------------------------------------------------------------------

inline
Redrawable::Redrawable()
{
    obj.push_back( this );
}


//------------------------------------------------------------------------------

inline
Redrawable::~Redrawable()
{
    obj.erase( std::find( obj.begin(), obj.end(), this ) );
}


//------------------------------------------------------------------------------

inline void 
Redrawable::redraw_all( core::Surface& dst_surf, const Rect2D& clip_rect )
{
    for( int i=0; i<obj.size(); ++i )
        obj[i]->redraw( dst_surf, clip_rect );
}


//------------------------------------------------------------------------------

inline void 
Redrawable::restore_all( core::Surface& dst_surf, const Point2D& offset,
                         const Rect2D& clip_rect 
                       )
{
    for( int i=obj.size()-1; i>=0; --i )
        obj[i]->restore_background( dst_surf, offset, clip_rect );
}


#endif // _REDRAWABLE_HPP_