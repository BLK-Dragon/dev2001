#if !defined _EVENT_HPP_
#define _EVENT_HPP_
//==============================================================================
//
//  Event system
//
//==============================================================================
//
//  externals: 

    #include <vector>


//==============================================================================
//
//  publics:




template< class EventSubscriber >
class
EventSource
{
  public:

    void            subscribe( EventSubscriber* es );
    void            unsubscribe( EventSubscriber* es );

    void            fire( void (EventSubscriber::*method)() );
    void            fire( void (EventSubscriber::*method)(int), int i );
    void            fire( void (EventSubscriber::*method)(unsigned int), unsigned int i );
    void            fire( void (EventSubscriber::*method)(char), char ch );
    void            fire( void (EventSubscriber::*method)(int,int), int i1, int i2 );

  
  protected:

    std::vector<EventSubscriber*>   subscriber;
};


//------------------------------------------------------------------------------

template< class EventSubscriber >
inline void
EventSource<EventSubscriber>::subscribe( EventSubscriber* es )
{
    subscriber.push_back( es );     
}


//------------------------------------------------------------------------------

template< class EventSubscriber >
inline void
EventSource<EventSubscriber>::unsubscribe( EventSubscriber* es )
{
    for( unsigned i=0; i<subscriber.size(); i++ )
    {
        if( subscriber[i] == es )
            subscriber.erase( subscriber.begin() + i ); 
    }
}


//------------------------------------------------------------------------------

template< class EventSubscriber >
inline void            
EventSource<EventSubscriber>::fire( void (EventSubscriber::*method)() )
{
    for( unsigned i=0; i<subscriber.size(); i++ ) 
        (subscriber[i]->*method)();
}


//------------------------------------------------------------------------------

template< class EventSubscriber >
inline void            
EventSource<EventSubscriber>::fire( void (EventSubscriber::*method)(int), 
                                    int i 
                                  )
{
    for( unsigned j=0; j<subscriber.size(); j++ ) 
        (subscriber[j]->*method)( i );
}


//------------------------------------------------------------------------------

template< class EventSubscriber >
inline void            
EventSource<EventSubscriber>::fire( void (EventSubscriber::*method)(unsigned int), 
                                    unsigned int i 
                                  )
{
    for( unsigned j=0; j<subscriber.size(); j++ ) 
        (subscriber[j]->*method)( i );
}


//------------------------------------------------------------------------------

template< class EventSubscriber >
inline void            
EventSource<EventSubscriber>::fire( void (EventSubscriber::*method)(char), 
                                    char ch 
                                  )
{
    for( unsigned j=0; j<subscriber.size(); j++ ) 
        (subscriber[j]->*method)( ch );
}


//------------------------------------------------------------------------------

template< class EventSubscriber >
inline void            
EventSource<EventSubscriber>::fire( void (EventSubscriber::*method)(int,int), 
                                    int i1, int i2 
                                  )
{
    for( unsigned j=0; j<subscriber.size(); j++ ) 
        (subscriber[j]->*method)( i1, i2 );
}


#endif // _EVENT_HPP_
