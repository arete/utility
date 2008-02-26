// The aim of this file is to create code for
// FunctionWrapper_*_* and MethodWrapper_*_* classes

// Sorry for the grave hack,
// c++ templates definitly suck at variadic signatures...

// please port me to the next c++ standard !!

// the macro hackery in this file iterates from N=7 to N=0
// and includes this file once for each iteration.

// name(X) is X##N
// ai(X) is "X" if i<=N, ignored otherwise
// aic(X) is ",X" if i<=N, ignored otherwise

#ifndef N
#define N 7
#define name(X) X##7
#define a1(X) X
#define a1c(X) ,X
#define a2(X) X
#define a2c(X) ,X
#define a3(X) X
#define a3c(X) ,X
#define a4(X) X
#define a4c(X) ,X
#define a5(X) X
#define a5c(X) ,X
#define a6(X) X
#define a6c(X) ,X
#define a7(X) X
#define a7c(X) ,X
#elif N > 6
#undef N
#undef name
#undef a7
#undef a7c
#define N 6
#define a7(X)
#define a7c(X)
#define name(X) X##6
#elif N > 5
#undef N
#undef name
#undef a6
#undef a6c
#define N 5
#define a6(X)
#define a6c(X)
#define name(X) X##5
#elif N > 4
#undef N
#undef name
#undef a5
#undef a5c
#define N 4
#define a5(X)
#define a5c(X)
#define name(X) X##4
#elif N > 3
#undef N
#undef name
#undef a4
#undef a4c
#define N 3
#define a4(X)
#define a4c(X)
#define name(X) X##3
#elif N > 2
#undef N
#undef name
#undef a3
#undef a3c
#define N 2
#define a3(X)
#define a3c(X)
#define name(X) X##2
#elif N > 1
#undef N
#undef name
#undef a2
#undef a2c
#define N 1
#define a2(X)
#define a2c(X)
#define name(X) X##1
#elif N > 0
#undef N
#undef name
#undef a1
#undef a1c
#define N 0
#define a1(X)
#define a1c(X)
#define name(X) X##0
#endif




// content starts here




template <
  typename RET
  a1c(typename P1)
  a2c(typename P2)
  a3c(typename P3)
  a4c(typename P4)
  a5c(typename P5)
  a6c(typename P6)
  a7c(typename P7),
  RET (*F)(a1(P1)a2c(P2)a3c(P3)a4c(P4)a5c(P5)a6c(P6)a7c(P7))>
class name(FunctionWrapper_1_)
{
public:
  typedef void myobjectT;
  
  static int Wrapper  (lua_State* L)
  {
    DataWrapper<RET>::Pack (L,F(
				a1(DataWrapper<P1>::Unpack(L, 1))
				a2c(DataWrapper<P2>::Unpack(L, 2))
				a3c(DataWrapper<P3>::Unpack(L, 3))
				a4c(DataWrapper<P4>::Unpack(L, 4))
				a5c(DataWrapper<P5>::Unpack(L, 5))
				a6c(DataWrapper<P6>::Unpack(L, 6))
				a7c(DataWrapper<P7>::Unpack(L, 7))
			      ));
    return 1;
  }

  static const bool hasmeta=false;
};


template <
  typename OBJ, typename RET
  a1c(typename P1)
  a2c(typename P2)
  a3c(typename P3)
  a4c(typename P4)
  a5c(typename P5)
  a6c(typename P6)
  a7c(typename P7),
  RET (OBJ::*F)(a1(P1)a2c(P2)a3c(P3)a4c(P4)a5c(P5)a6c(P6)a7c(P7))>
class name(MethodWrapper_1_)
{
public:
  typedef OBJ myobjectT;

  static int Wrapper  (lua_State* L)
  {
    RET (OBJ::*f)(a1(P1)a2c(P2)a3c(P3)a4c(P4)a5c(P5)a6c(P6)a7c(P7))=F;
    OBJ* obj=DataWrapper<OBJ*>::Unpack(L,1);
    DataWrapper<RET>::Pack (L,
			    (obj->*f)(
				a1(DataWrapper<P1>::Unpack(L, 1))
				a2c(DataWrapper<P2>::Unpack(L, 2))
				a3c(DataWrapper<P3>::Unpack(L, 3))
				a4c(DataWrapper<P4>::Unpack(L, 4))
				a5c(DataWrapper<P5>::Unpack(L, 5))
				a6c(DataWrapper<P6>::Unpack(L, 6))
				a7c(DataWrapper<P7>::Unpack(L, 7))
				      ));
    return 1;
  }

  static const bool hasmeta=true;
};


// content end

#if N > 0
#include "LuaWrappers.hh"
#else
#undef N
#undef a1
#undef a1c
#undef a2
#undef a2c
#undef a3
#undef a3c
#undef a4
#undef a4c
#undef a5
#undef a5c
#undef a6
#undef a6c
#undef a7
#undef a7c
#undef name
#endif
