#if !defined _TUNE_HPP_
#define _TUNE_HPP_
//==============================================================================
//
//  decls for tuning   
//
//==============================================================================
//
//  externals: 

    #include <windows.h>


namespace core
{
//==============================================================================
//
//  publics:




class
Timer  
{
  public:

    void    start_timing( int n=0 );
    void    stop_timing( int n=0 );
    double  time( int n=0 );

  
  private:

    enum
    {
            n_timers = 10
    };

    FILETIME    start_time[n_timers];
    FILETIME    stop_time[n_timers];
    double      user_time[n_timers];    // in milliseconds
    FILETIME    creation;
    FILETIME    exit;
};


//------------------------------------------------------------------------------

inline void 
Timer::start_timing( int n )
{                  
    if( n >= n_timers )
        throw Error(Msg("timer #%i unavailable ( total timers: %i )"));
    
    FILETIME    kernel_time;
    
    GetThreadTimes( GetCurrentThread(),
                    &creation, &exit, &kernel_time, &start_time[n]
                  );
    /*
    char    buf[128];
    sprintf( buf, "start %i = %08X:%08X\n", n, 
             start_time[n].dwHighDateTime, start_time[n].dwLowDateTime 
           );
    std::clog << buf;*/
}


//------------------------------------------------------------------------------

inline void 
Timer::stop_timing( int n )
{
    if( n >= n_timers )
        throw Error(Msg("timer #%i unavailable ( total timers: %i )"));
    
    FILETIME    kernel_time;
    
    GetThreadTimes( GetCurrentThread(),
                    &creation, &exit, &kernel_time, &stop_time[n]
                  );
    /*
    char    buf[128];
    sprintf( buf, "stop %i  = %08X:%08X\n", n,
             stop_time[n].dwHighDateTime, stop_time[n].dwLowDateTime 
           );
    std::clog << buf;*/
    
    /*
    ULARGE_INTEGER  ul_t0;
    ULARGE_INTEGER  ul_t1;
    
    ul_t0.LowPart   = start_time[n].dwLowDateTime;
    ul_t0.HighPart  = start_time[n].dwHighDateTime;
    ul_t1.LowPart   = stop_time[n].dwLowDateTime;
    ul_t1.HighPart  = stop_time[n].dwHighDateTime;

    __int64     t0  = ul_t0.QuadPart;
    __int64     t1  = ul_t1.QuadPart;
    __int64     dt  = t1 - t0;
    */
    DWORD   dt = stop_time[n].dwLowDateTime - start_time[n].dwLowDateTime;

    user_time[n] = double( dt )/10.0;
}


//------------------------------------------------------------------------------

inline double
Timer::time( int n )
{
    if( n >= n_timers )
        throw Error(Msg("timer #%i unavailable ( total timers: %i )"));
    
    return user_time[n];
}


} // namespace core
#endif // _TUNE_HPP_