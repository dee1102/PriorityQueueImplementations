

#ifndef __VEB_H_458976543568798867538649758687654752463747856374562543646__
#define __VEB_H_458976543568798867538649758687654752463747856374562543646__

#include <cmath>
#include <climits>
#include <cstdlib>
#include <cstdio>
#include <iostream>


#define DEBUG_OS std::cout
#define DEBUG_OS_ENDL std::endl

#define UNDEFINED INT_MIN


struct TvEB
{

  TvEB ( int uniSize );


  ~TvEB();


  const int universe;


  const int uniSqrt;


  const int sqrtLow;


  const int sqrtHigh;


  int min;


  int max;


  TvEB * summary;


  TvEB ** cluster;
};

bool deletevEB ( TvEB *& tree, int val );


bool find ( TvEB * tree, int val );


bool successor ( TvEB * tree, int val, int & res );



float higherSqrt ( int val );


int low ( TvEB * tree, int val );


int high ( TvEB * tree, int val );



int powTwoRoundUp ( int val );


float lowerSqrt ( int val );

bool max ( TvEB * tree, int & res );


bool insert ( TvEB *& tree, int val, int parentUniSqrt = 65536 );


int index ( TvEB * tree, int high, int low );

bool min ( TvEB * tree, int & res );


bool vEB_pred ( TvEB * tree, int val, int & res );


void vEB_print ( TvEB * tree, std::ostream & os );

#endif /* __VEB_H_458976543568798867538649758687654752463747856374562543646__ */
