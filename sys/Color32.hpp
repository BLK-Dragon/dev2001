#if !defined _COLOR32_HPP_
#define _COLOR32_HPP_
//==============================================================================
//
//  Class for 32bit pixel manipulation   
//
//==============================================================================
//
//  externals: 

    #include "Type.hpp"


namespace vfx
{
//==============================================================================
//
//  publics:




class
Color32  
{
  public:

                    Color32();
                    Color32( const core::UDword& c32 );
                    Color32( core::UDword* c32 );

                    operator core::UDword() const;

    Color32&        operator=( const core::UDword& c32 );
    Color32&        operator+=( core::UDword c32 );

  
  private:

    core::UDword    color;
    core::UDword*   color_ptr;
};



//------------------------------------------------------------------------------

inline
Color32::Color32()
  : color(0x00000000),
    color_ptr(&color)
{
}


//------------------------------------------------------------------------------

inline
Color32::Color32( const core::UDword& c32 )
  : color(c32),
    color_ptr(&color)
{
}


//------------------------------------------------------------------------------

inline
Color32::Color32( core::UDword* c32 )
  : color_ptr(c32)
{
    // color unused
}


//------------------------------------------------------------------------------

inline
Color32::operator core::UDword() const
{
    return *color_ptr; 
}


//------------------------------------------------------------------------------

inline Color32& 
Color32::operator=( const core::UDword& c32 )
{
    *color_ptr = c32;

    return *this;
}


//------------------------------------------------------------------------------

inline Color32& 
Color32::operator+=( core::UDword c32 )
{
    core::UDword*   _color_ptr = color_ptr;
    
    __asm{
            mov     EAX, _color_ptr
            movd    MM1, [EAX]
            
            movd    MM0, dword ptr [EAX]  
            paddusb MM0, qword ptr c32

            movd    [EAX], MM0

            emms
        
         }

    return *this;
}


} // namspace vfx
#endif // _COLOR32_HPP_