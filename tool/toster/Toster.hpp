#if !defined _TOSTER_HPP_
#define _TOSTER_HPP_

extern int
OpenToster();

extern void
RegisterApplyCallback( VOID* callback );

extern void
AddIntVariable( int* var, int min, int max, char name[] );

extern void
AddFloatVariable( float* var, float min, float max, int num_steps, char name[] );

extern void
AddColorVariable( unsigned int* color, char name[] );

extern void
AddList( int* value, char items[], char name[] );

extern void
PrintConsole( char* str );

#endif // _TOSTER_HPP_