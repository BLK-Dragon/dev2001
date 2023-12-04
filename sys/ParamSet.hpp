#if !defined _PARAMSET_HPP_
#define _PARAMSET_HPP_
//==============================================================================
//
//  decls for working with param-sets   
//
//==============================================================================
//
//  externals:

    #include <string>
    #include <vector>

    #include "File.hpp"


namespace core
{
//==============================================================================




class
Param  
{
  public:

                    Param();
                    /*Param( const Param& p );*/
                    Param( int i );
                    Param( double d );
                    Param( UDword d );
                    Param( UByte b );
                    Param( const char* s );
                    Param( const std::vector<UByte>* _ba );
    /*
    private:
    Param& operator= ( const Param& p );
    public:                             */
    operator int();
    operator double();
    operator UDword();
    operator UByte();
    operator const char*();
    operator std::string();
    operator const std::vector<UByte>& ();
  
  
  private:

  enum
  {
      as_invalid    = 13,
      as_int,
      as_double,
      as_udword,
      as_ubyte,
      as_string,
      as_byte_array
  }                 type;



  union
  {
      int                       i;
      double                    d;
      UDword                    dw;
      UByte                     b;
      const char*               str;
      const std::vector<UByte>* ba;
  };

};


//==============================================================================

class ParamSet;
class ParamSetPool;
class IniFileParser;
  

//==============================================================================




class
ParamSet  
{
                    friend class ParamSetPool;
  
  public:
                    ParamSet();
                    ParamSet( const ParamSetPool* p, int i );

    
    Param           operator[] ( const std::string& name );


  private:
    const ParamSetPool*     pool;
    int                     index;
};


//==============================================================================




class
ParamSetPool
{
                        friend class ParamSet;
  
  public:
                        ParamSetPool( const std::string& name );

    void                clear();

    void                start_member_binding();
    void                bind_int_member( const std::string& name, int def=0 );
    void                bind_double_member( const std::string& name, 
                                            double def=0.0 
                                          );
    void                bind_udword_member( const std::string& name, 
                                            UDword def=0x00000000 
                                          );
    void                bind_ubyte_member( const std::string& name, 
                                           UByte def=0x00
                                         );
    void                bind_string_member( const std::string& name,
                                            const std::string& def=""
                                          );
    void                bind_ubyte_array_member( const std::string& name );
    void                finish_member_binding();
    
    
    ParamSet            operator[] ( const std::string& name ) const;
    ParamSet            operator[] ( int i ) const;
    int                 size() const;
    const std::string&  name() const;

    void                new_paramset();
    void                new_int( const std::string& name, int val );
    void                new_double( const std::string & name, double val );
    void                new_udword( const std::string& name, UDword val );
    void                new_ubyte( const std::string& name, UByte val );
    void                new_string( const std::string& name, const std::string& val );
    void                new_ubyte_array( const std::string& name, 
                                         const std::vector<UByte>& val 
                                       );

  
  private:

    std::string                 class_name;
    bool                        binding_enabled;
    
    std::vector<std::string>    int_name;
    std::vector<std::string>    double_name;
    std::vector<std::string>    udword_name;
    std::vector<std::string>    ubyte_name;
    std::vector<std::string>    string_name;

    std::vector<int>            int_value;
    std::vector<double>         double_value;
    std::vector<UDword>         udword_value;
    std::vector<UByte>          ubyte_value;
    std::vector<std::string>    string_value;
    
    std::vector<int>            def_int_value;
    std::vector<double>         def_double_value;
    std::vector<UDword>         def_udword_value;
    std::vector<UDword>         def_ubyte_value;
    std::vector<std::string>    def_string_value;

    
    std::vector<std::string>            ubyte_array_name;
    
    std::vector<std::vector<UByte> >    ubyte_array_value;


    int                         n_paramsets;
    int                         int_index;
    int                         double_index;
    int                         udword_index;
    int                         ubyte_index;
    int                         string_index;
    int                         ubyte_array_index;
};


//==============================================================================

typedef void (*member_binder_func)( core::ParamSetPool* paramset_pool );



class
ParamSetManager
{
  public:
                    ParamSetManager();
                   ~ParamSetManager();

    void            register_paramset( const std::string& name, 
                                       member_binder_func mb
                                     );
    void            clear_paramset( const std::string& name );
    void            parse_ini_file( BinFile* file );
    int             paramset_count( const std::string& name );
    ParamSetPool&   paramset_pool( const std::string& name );
    ParamSet        paramset( const std::string& name, int i );

  
  private:

    bool                            members_bound;
    std::vector<ParamSetPool>       _paramset_pool;
    std::vector<member_binder_func> member_binder;

    enum
    ParamType
    {
        t_unknown       = 0,
        t_int           = 1,
        t_double        = 2,
        t_udword        = 3,
        t_ubyte         = 4,
        t_string        = 5,
        t_byte_array    = 6
    };
    
    int                 int_param;
    double              double_param;
    core::UDword        udword_param;
    core::UByte         ubyte_param;
    std::string         string_param;
    std::vector<UByte>  ubyte_array_param;

    ParamType       parse_item( const char* item );
    ParamType       parse_item_as_array( const char* item );
};


//==============================================================================
//
//  Param

inline
Param::Param()
  : type(as_invalid)
{
}

/*
//------------------------------------------------------------------------------

inline
Param::Param( const Param* p )
  : type( p.type ),
    ba( p.ba )
{
}


//------------------------------------------------------------------------------

inline Param& 
Param::operator= ( const Param& p )
{
    this->type = p.type;

    return *this;     
}
*/

//------------------------------------------------------------------------------

inline
Param::Param( int i )
  : type(as_int)
{
    this->i = i;
}


//------------------------------------------------------------------------------

inline
Param::Param( double d )
  : type(as_double)
{
    this->d = d;
}


//------------------------------------------------------------------------------

inline
Param::Param( UDword dw )
  : type(as_udword)
{
    this->dw = dw; 
}


//------------------------------------------------------------------------------

inline
Param::Param( UByte b )
  : type(as_ubyte)
{
    this->b = b; 
}


//------------------------------------------------------------------------------

inline
Param::Param( const char* s )
  : type(as_string)
{
    this->str = s; 
}


//------------------------------------------------------------------------------

inline
Param::Param( const std::vector<UByte>* _ba )
  : type(as_byte_array)
{
    this->ba = _ba;
}


//------------------------------------------------------------------------------

inline
Param::operator int()
{   
    if( type != as_int )
        throw Error("non-int in Param");
    
    return i;
}


//------------------------------------------------------------------------------

inline
Param::operator double()
{
    if( type != as_double )
        throw Error("non-double in Param");
        
    return d;
}


//------------------------------------------------------------------------------

inline
Param::operator UDword()
{
    if( type != as_udword )
        throw Error("non-udword in Param");
        
    return dw;
}


//------------------------------------------------------------------------------

inline
Param::operator UByte()
{
    if( type != as_ubyte )
        throw Error("non-ubyte in Param");
        
    return b;
}


//------------------------------------------------------------------------------

inline
Param::operator const char*()
{
    if( type != as_string )
        throw Error("non-string in Param");
        
    return str;
}


//------------------------------------------------------------------------------

inline
Param::operator std::string()
{
    if( type != as_string )
        throw Error("non-string in Param");
        
    return std::string(str);
}


//------------------------------------------------------------------------------

inline
Param::operator const std::vector<UByte>&()
{
    if( type != as_byte_array )
        throw Error("non-byte_array in Param");
        
    return *ba;
}


//==============================================================================
//
//  ParamSet

inline
ParamSet::ParamSet( const ParamSetPool* p, int i )
  : pool(p),
    index(i)
{
}


//------------------------------------------------------------------------------

inline
ParamSet::ParamSet()
  : pool(0),
    index(-1)
{
}



//==============================================================================
//
//  ParamSetPool

inline
ParamSetPool::ParamSetPool( const std::string& name )
  : class_name(name),
    binding_enabled(true),
    n_paramsets(0)
{
}


//------------------------------------------------------------------------------

inline ParamSet 
ParamSetPool::operator[] ( const std::string& name ) const
{
    for( int i=0; i<string_value.size(); i++ )
    {
        //std::clog << string_name[i] << ":" << string_value[i] << "\n" 
        //          << std::flush;
        if( string_value[i] == name )
            return ParamSet( this, i/string_name.size() );
    }

    throw Error( Msg( "there's no ParamSet named \"%s\" in pool", 
                      name.c_str()
                    ) 
               );
    
    return ParamSet();
}


//------------------------------------------------------------------------------

inline ParamSet 
ParamSetPool::operator[] ( int i ) const
{
    if( i >= n_paramsets )
        throw Error( "index out of range in ParamSetPool" );
    
    return ParamSet( this, i );
}


//------------------------------------------------------------------------------

inline int
ParamSetPool::size() const
{
    return n_paramsets;
}


//------------------------------------------------------------------------------

inline const std::string&  
ParamSetPool::name() const
{
    return class_name; 
}


} // namespace core
#endif // _PARAMSET_HPP_