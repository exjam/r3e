#ifndef SAFE_DELETE_H
#define SAFE_DELETE_H

#ifndef NULL
# define NULL 0
#endif

#define SAFE_DELETE(x) if(x){ delete x; x = NULL; }
#define SAFE_DELETE_2D(x, y) if(x){ for(unsigned int _i = 0; _i < y; ++_i){ delete x[_i]; } delete [] x; x = NULL; }

#define SAFE_DELETE_ARRAY(x) if(x){ delete [] x; x = NULL; }
#define SAFE_DELETE_ARRAY_2D(x, y) if(x){ for(unsigned int _i = 0; _i < y; ++_i){ delete [] x[_i]; } delete [] x; x = NULL; }

#endif SAFE_DELETE_H
