#if !defined _TYPE_HPP_
#define _TYPE_HPP_
//==============================================================================
//
//  Types   
//
//==============================================================================
//
//  externals: 

//==============================================================================
//
//  publics:

namespace core
{
//  types:

typedef     char                Byte;
typedef     short               Word;
typedef     int                 Dword;

typedef     unsigned char       UByte;
typedef     unsigned short      UWord;
typedef     unsigned int        UDword;


// tracin'  

#if defined TRACING
    #define __T(x) std::clog << (x) << std::flush
    #define __CT(x) ,x
    #define DBG(x) x
#else
    #define __T(x)
    #define __CT(x) 
    #define DBG(x)
#endif


//------------------------------------------------------------------------------

enum{
         FPU_on_chip                 = 0x00000001,
         Virtual_Mode_Extensions     = 0x00000002,
         Debugging_Extensions        = 0x00000004,
         Page_Size_Extensions        = 0x00000008,
         Time_Stamp_Counter          = 0x00000010,
         Model_Specific_Registers    = 0x00000020,
         Physical_Address_Extensions = 0x00000040,
         Machine_Check_Extension     = 0x00000080,
         CMPXCHG8_Instruction        = 0x00000100,
         APIC_on_chip                = 0x00000200,
         Mem_Type_Range_Registers    = 0x00001000,
         Page_Global_Extensions      = 0x00002000,
         Machine_Check_Arch          = 0x00004000,
         CMOV_Instructions           = 0x00008000,
         MMX_Instructions            = 0x00800000
    };
enum{
         mask_Stepping_ID            = 0x0000000F,  
         mask_Model                  = 0x000000F0,
         mask_Family                 = 0x00000F00,
         mask_CPU_Type               = 0x00003000 
    };

 
} // namespace core

#endif // _TYPE_HPP_