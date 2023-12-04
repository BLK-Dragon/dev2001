#if !defined _VIDEOCARD_HPP_
#define _VIDEOCARD_HPP_
//==============================================================================
//
//  VideoCard definition   
//
//==============================================================================
//
//  externals: 

    #include <vector>
    
    #include "Core.hpp"
    #include "Surface.hpp"

namespace core
{
//------------------------------------------------------------------------------
//
//  publics:




class
VideoCard  
{
  public:

    struct              mode;

                        VideoCard();
                       ~VideoCard();

    void                set_mode( int xres, int yres, int bpp, int freq=0 );
    bool                vsync_in_progress();
    void                wait_for_vsync();

    void                resize_wnd_back_buffer();
    void                flip_pages( ScreenBuffer::FlipMethod method = ScreenBuffer::vsync );

    Surface&            front_buffer();
    Surface&            back_buffer();



    struct
    mode
    {
         int            xres;
         int            yres;
         int            bpp;
         int            freq;
    };


  private:

    bool                fullscreen_mode;
    ScreenBuffer        front_buf;
    ScreenBuffer        back_buf;
    Surface             wnd_back_buf;
    std::vector<mode>   mode_info;
};

} // namespace core
#endif // _VIDEOCARD_HPP_