#if !defined _FRUIT_HPP_
#define _FRUIT_HPP_
//==============================================================================
//
//  base class   
//
//==============================================================================
//
//  externals:


//==============================================================================
//
//  publics:




class
Fruit
{
  public:

    virtual void        show() =0;
};

__declspec(dllimport) extern Fruit* CreateFruit();
__declspec(dllimport) extern void DestroyFruit( Fruit* );

typedef Fruit* (*Creator)();
typedef Fruit* (*Killer)(Fruit*);

#endif // _FRUIT_HPP_