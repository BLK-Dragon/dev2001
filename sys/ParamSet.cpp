//==============================================================================
//
//  Realization of ParamSetPool   
//
//==============================================================================
//
//  externals: 

    #include "ParamSet.hpp"

namespace core
{
//==============================================================================
//
//  publics:


Param 
ParamSet::operator[] ( const std::string& name )
{
    if( pool==0  ||  index<0 )
        throw Error("bad paramset");
    
    for( int i=0; i<pool->int_name.size(); i++ )
    {
        if( pool->int_name[i] == name )
        {
            int index = this->index * pool->int_name.size() + i;
            
            return Param( pool->int_value[ index ] );
        } 
    }
    
    for( i=0; i<pool->double_name.size(); i++ )
    {
        if( pool->double_name[i] == name )
        {
            int index = this->index * pool->double_name.size() + i;

            return Param( pool->double_value[ index ] );
        }
    }

    for( i=0; i<pool->udword_name.size(); i++ )
    {
        if( pool->udword_name[i] == name )
        {
            int index = this->index * pool->udword_name.size() + i;

            return Param( (UDword)pool->udword_value[ index ] );
        }
    }

    for( i=0; i<pool->ubyte_name.size(); i++ )
    {
        if( pool->ubyte_name[i] == name )
        {
            int index = this->index * pool->ubyte_name.size() + i;

            return Param( (UByte)pool->ubyte_value[ index ] );
        }
    }

    for( i=0; i<pool->string_name.size(); i++ )
    {
        if( pool->string_name[i] == name )
        {
            int index = this->index * pool->string_name.size() + i;

            return Param( pool->string_value[ index ].c_str() );
        }
    }
    
    for( i=0; i<pool->ubyte_array_name.size(); i++ )
    {
        if( pool->ubyte_array_name[i] == name )
        {
            int index = this->index * pool->ubyte_array_name.size() + i;
            const std::vector<UByte>*  val = &pool->ubyte_array_value[ index ];

            return Param( val );
        }
    }
    
    throw Error( Msg( "paramset pool \"%s\" hasn't member \"%s\"",
                      pool->class_name.c_str(),
                      name.c_str()
                    ) 
               );
    
    return Param();
}


//------------------------------------------------------------------------------

void                
ParamSetPool::clear()
{
    int_value.clear();   
    double_value.clear();
    udword_value.clear();
    ubyte_value.clear(); 
    string_value.clear();
    ubyte_array_value.clear();

    //binding_enabled = true;
    n_paramsets     = 0;
}


//------------------------------------------------------------------------------

void            
ParamSetPool::start_member_binding()
{   
    if( !binding_enabled )
        Error( "binding in ParamSetPool can be performed only once" );

    string_name.push_back( "name" );
    def_string_value.push_back("");
}


//------------------------------------------------------------------------------

void            
ParamSetPool::bind_int_member( const std::string& name, int def )
{
    int_name.push_back( name );
    def_int_value.push_back( def );
}


//------------------------------------------------------------------------------

void            
ParamSetPool::bind_double_member( const std::string& name, double def )
{
    double_name.push_back( name );     
    def_double_value.push_back( def );
}


//------------------------------------------------------------------------------

void            
ParamSetPool::bind_udword_member( const std::string& name, UDword def )
{
    udword_name.push_back( name );     
    def_udword_value.push_back( def );
}


//------------------------------------------------------------------------------

void            
ParamSetPool::bind_ubyte_member( const std::string& name, UByte def )
{
    ubyte_name.push_back( name );     
    def_ubyte_value.push_back( def );
}


//------------------------------------------------------------------------------

void            
ParamSetPool::bind_string_member( const std::string& name, 
                                  const std::string& def 
                                )
{
    string_name.push_back( name );     
    def_string_value.push_back( def );
}


//------------------------------------------------------------------------------

void            
ParamSetPool::bind_ubyte_array_member( const std::string& name )
{
    ubyte_array_name.push_back( name );     
}


//------------------------------------------------------------------------------

void            
ParamSetPool::finish_member_binding()
{
    binding_enabled = false;     
}


//------------------------------------------------------------------------------

void 
ParamSetPool::new_paramset()
{
    int_index = int_value.size();
    
    for( int i=0; i<int_name.size(); i++ )
        int_value.push_back( def_int_value[i] );


    double_index = double_value.size();
    
    for( i=0; i<double_name.size(); i++ )
        double_value.push_back( def_double_value[i] );
    

    udword_index = udword_value.size();
    
    for( i=0; i<udword_name.size(); i++ )
        udword_value.push_back( def_udword_value[i] );
    

    ubyte_index = ubyte_value.size();
    
    for( i=0; i<ubyte_name.size(); i++ )
        ubyte_value.push_back( def_ubyte_value[i] );


    string_index = string_value.size();
    
    for( i=0; i<string_name.size(); i++ )
        string_value.push_back( def_string_value[i] );
    
    ubyte_array_index = ubyte_array_value.size();
    for( i=0; i<ubyte_array_name.size(); i++ )
        ubyte_array_value.push_back( std::vector<UByte>() );

    ++n_paramsets;
}


//------------------------------------------------------------------------------

void 
ParamSetPool::new_int( const std::string& name, int val )
{
    for( int i=0; i<int_name.size(); i++ )
    {
        if( int_name[i] == name )
        {    
            int_value[ int_index + i ] = val;
            break;
        }
    }
}


//------------------------------------------------------------------------------

void 
ParamSetPool::new_double(const std::string & name, double val)
{
    for( int i=0; i<double_name.size(); i++ )
    {
        if( double_name[i] == name )
        {    
            double_value[ double_index + i ] = val;
            break;
        }
    }
}


//------------------------------------------------------------------------------

void 
ParamSetPool::new_udword( const std::string& name, UDword val )
{
    for( int i=0; i<udword_name.size(); i++ )
    {
        if( udword_name[i] == name )
        {    
            udword_value[ udword_index + i ] = val;
            break;
        }
    }
}


//------------------------------------------------------------------------------

void 
ParamSetPool::new_ubyte( const std::string& name, UByte val )
{
    for( int i=0; i<ubyte_name.size(); i++ )
    {
        if( ubyte_name[i] == name )
        {    
            ubyte_value[ ubyte_index + i ] = val;
            break;
        }
    }
}


//------------------------------------------------------------------------------

void 
ParamSetPool::new_string( const std::string& name, const std::string& val )
{
    //std::clog << '\"' << val << '\"' << "\n";
    for( int i=0; i<string_name.size(); i++ )
    {
        if( string_name[i] == name )
        {    
            string_value[ string_index + i ] = val;
            break;
        }
    }
}


//------------------------------------------------------------------------------

void 
ParamSetPool::new_ubyte_array( const std::string& name, 
                               const std::vector<UByte>& val 
                             )
{
    for( int i=0; i<ubyte_array_name.size(); i++ )
    {
        if( ubyte_array_name[i] == name )
        {
            ubyte_array_value[ ubyte_array_index + i ] = val;
            break;
        }
    }
}



//==============================================================================
//
//  ParamsetManager

ParamSetManager::ParamSetManager()
  : members_bound(false)
{
}


//------------------------------------------------------------------------------

ParamSetManager::~ParamSetManager()
{
}


//------------------------------------------------------------------------------

void 
ParamSetManager::register_paramset( const std::string& name,
                                    member_binder_func mb
                                  )  
{
    _paramset_pool.push_back( ParamSetPool(name) );
    member_binder.push_back( mb );
}


//------------------------------------------------------------------------------

ParamSetManager::ParamType      
ParamSetManager::parse_item( const char* item )
{
    char*   src;
    
    // check is it string

    if( src=strchr( item, '\"' ) )
    {
        char*   end;
        char    str[120];

        ++src;
        if( end=strchr( src, '\"' ) )
        {
            strncpy( str, src, end-src );
            str[end-src] = '\0';
            string_param = std::string( str );
            return t_string;
        }
    }
                                            
    
    // check is it char
    if( item[0] == '\''  &&  item[2] == '\'' )
    {
        char    ch;
        sscanf( item, "\'%c\'", &ch ); 

        ubyte_param = ch;
        return t_ubyte;
    }


    // check is it hex-value

    if( !strncmp( item, "0x", 2 ) )
    {
        sscanf( item, "0x%X", &udword_param );
        return t_udword;
    }

    if( item[0] == '#' )
    {
        core::UDword    c;
        core::UByte*    p   = (core::UByte*)&c;

        sscanf( item+1, "%X", &c );
        udword_param = p[2] | (p[1]<<8) | (p[0]<<16);
        return t_udword;
    }


    // check is it bin-balue

    if( item[strlen(item)-1] == 'b' )
    {
        core::UDword    rank    = 0x00000001;
        const char*     digit   = item + strlen(item)-1 -1;

        udword_param = 0x00000000;
        while( digit >= item )
        {
            if( *digit == '1' )
                udword_param += rank;
            
            rank <<= 1;
            --digit;
        } 

        return t_udword;
    }

    
    // check is it float

    if( strchr( item, '.' ) )
    {
        double_param = atof(item);
        return t_double;
    }

    int_param = atoi(item);
    return t_int;
}


//------------------------------------------------------------------------------

ParamSetManager::ParamType
ParamSetManager::parse_item_as_array( const char* item )
{
    const char*         src         = item+1;
    char*               dst;
    char                buf[120]    = "";
    bool                complete    = false;

    enum
    {
        begin_quest     = 1,
        end_quest       = 2,
        value_gathering = 3
    } state = begin_quest;

    ubyte_array_param.clear();

    while( !complete )
    {
        switch( state )
        {
            case begin_quest :
                if( *src == '}' )
                {
                    return (ubyte_array_param.size() != 0)  
                           ? t_byte_array
                           : t_unknown;
                }
                if( *src == ',' )
                    continue;
                if( isalnum(*src) )
                {
                    state   = value_gathering;
                    dst     = buf;
                    *dst++  = *src;
                }
                break;
            /*
            case end_quest :
                 
                break;
            */
            case value_gathering :
                if( !isalnum( *src ) )
                {
                    *dst = '\0';
                    switch( parse_item( buf ) )
                    {
                        case t_ubyte :
                            ubyte_array_param.push_back( ubyte_param );
                            break;

                        case t_udword :
                            ubyte_array_param.push_back( udword_param );
                            break;
                    }
                    state   = begin_quest;
                    buf[0]  = '\0';
                } 
                *dst++ = *src;
                break;
        }
    
        ++src;
    }
    
    return t_unknown;
}


//------------------------------------------------------------------------------

void
ParamSetManager::parse_ini_file( BinFile* file )
{
    if( !members_bound )
    {
        for( unsigned i=0; i<_paramset_pool.size(); i++ )
            (member_binder[i])( &_paramset_pool[i] );
        
        members_bound = true;
    }
    
    
    enum
    {
        paramset_quest   = 13,
        paramset_reading
    };

    int     state   = paramset_quest;
    bool    reparse = false;

    while( !file->eof() )
    {
        int             i;
        char            line[120];
        char            paramset_name[120];
        char            param_name[120];
        char            param_value[120];
        char*           src;
        char*           dst;
        ParamSetPool*   cur_pool;


        if( !reparse )
        {
            file->read_string( 120, line );
        }
        reparse = false;

        if( strstr( line, "//" ) )
            continue;

        switch( state )
        {
            case paramset_quest :
                if( !( src=strchr( line, '[' ) ) )
                    break;

                dst = paramset_name;
                ++src;
                while( *src != ']'  &&  *src != '\0' )
                    *dst++ = *src++;
                *dst = '\0';

                cur_pool = 0;
                for( i=0; i<_paramset_pool.size(); i++ )
                {
                    if( _paramset_pool[i].name() == paramset_name )
                    {
                        cur_pool = &_paramset_pool[i];
                        break;
                    }
                }

                if( !cur_pool )
                    break;

                //std::clog << paramset_name << " found\n" << std::flush;
                cur_pool->new_paramset();
                state = paramset_reading;
                break; // case paramset_quest

            case paramset_reading :
                if( strchr( line, '[' ) )
                {
                    state   = paramset_quest;
                    reparse = true;
                    break;
                }

                if( !( src=strchr( line, '=' ) ) )
                    break;

                ++src;
                while( *src == ' ' )
                    ++src;

                dst = param_value;
                while( *src != '\n'  &&  *src != '\0' )
                    *dst++ = *src++;
                *dst = '\0';

                src = line;
                while( *src == ' ' )
                    ++src;

                dst = param_name;
                while( *src != ' '  &&  *src != '\0' )
                    *dst++ = *src++;
                *dst = '\0';

                if( param_value[0] == '{' )
                {
                    switch( parse_item_as_array( param_value ) )
                    {
                        case t_byte_array :
                            cur_pool->new_ubyte_array( param_name,
                                                       ubyte_array_param
                                                     );
                            break;
 
                    }
                    break; 
                }
                
                switch( parse_item( param_value ) )
                {
                    case t_int :
                        cur_pool->new_int( param_name, int_param );
                        break;

                    case t_double :
                        cur_pool->new_double( param_name, double_param ); 
                        break;

                    case t_udword :
                        cur_pool->new_udword( param_name, udword_param ); 
                        cur_pool->new_ubyte( param_name, (UByte)udword_param ); 
                        break;

                    case t_ubyte :
                        //cur_pool->new_udword( param_name, udword_param ); 
                        cur_pool->new_ubyte( param_name, ubyte_param ); 
                        break;

                    case t_string :
                        cur_pool->new_string( param_name, string_param ); 
                        break;

                    case t_unknown :
                        break;


                    default :
                        throw Error("unrecongnizable param type");
                }

                break; // case paramset_reading
        } // switch( state )
    }
}


//------------------------------------------------------------------------------

int             
ParamSetManager::paramset_count( const std::string& name )
{
    for( int p=0; p<_paramset_pool.size(); p++ )
    {
        if( _paramset_pool[p].name() == name )
            return _paramset_pool[p].size();
    }

    throw Error( Msg( "paramset manager hasn't paramset named '%s'",
                      name.c_str()
                    )
               );
    
    return -1;
}


//------------------------------------------------------------------------------

void            
ParamSetManager::clear_paramset( const std::string& name )
{
    for( int p=0; p<_paramset_pool.size(); p++ )
    {
        if( _paramset_pool[p].name() == name )
            _paramset_pool[p].clear();
    }
}


//------------------------------------------------------------------------------

ParamSetPool&
ParamSetManager::paramset_pool( const std::string& name )
{
    for( int p=0; p<_paramset_pool.size(); p++ )
    {
        if( _paramset_pool[p].name() == name )
            return _paramset_pool[p];
    }

    throw Error(Msg( "ParamSet manager hasn't paramset pool named '%s'",
                     name.c_str()
                     )
               );

    return ParamSetPool("");
}


//------------------------------------------------------------------------------

ParamSet
ParamSetManager::paramset( const std::string& name, int i )
{
    for( int p=0; p<_paramset_pool.size(); p++ )
    {
        if( _paramset_pool[p].name() == name )
            return _paramset_pool[p][i];
    }

    throw Error(Msg( "ParamSet manager hasn't paramset pool named '%s'",
                     name.c_str()
                     )
               );

    return ParamSet();
}


} // namespace core

