extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace LuaWrapper {

  template <typename T>
  class LuaObject
  {
  public:
    static const char* luahandle() { return "undefined_object*"; }
  };

  template <typename T>
  class DataWrapper {
  public:
    static T Unpack(lua_State* L, int index)
    {
      T* c=(T*)luaL_checkudata(L, index,  LuaObject<T>::luahandle());
      if (!(*c)) {
	 lua_pushstring(L, "call with nil object");
	 lua_error(L);
       }
       return *c;
    }

    static void Pack(lua_State* L, T data)
    {
      *((T*) lua_newuserdata(L, sizeof(T)))=data;
      luaL_getmetatable(L,  LuaObject<T>::luahandle());
      lua_setmetatable(L, -2);
    }
  };


  template <>
  class DataWrapper<double> {
  public:
    static double Unpack(lua_State* L, int index)
    {
      return (double)luaL_checknumber(L,index);
    }
  
    static void Pack(lua_State* L, double data)
    {
      lua_pushnumber(L,(lua_Number)data);
    }
  };

  template <>
  class DataWrapper<int> {
  public:
    static int Unpack(lua_State* L, int index)
    {
      return (int)luaL_checkinteger(L,index);
    }
  
    static void Pack(lua_State* L, int data)
    {
      lua_pushinteger(L,data);
    }
  };

  /*
  template <typename RET,  typename P1,  RET (*F)(P1)>
  class FunctionWrapper_1_1
  {
  public:
    typedef void myobjectT;
    
    static int Wrapper  (lua_State* L)
    {
      DataWrapper<RET>::Pack (L,
			      F(
				DataWrapper<P1>::Unpack(L, 1)
				));
      return 1;
    }

    static const bool hasmeta=false;
  };


  template <typename OBJ, typename RET,  typename P1,  RET (OBJ::*F)(P1)>
  class MethodWrapper_1_1
  {
  public:
    typedef OBJ myobjectT;

    static int Wrapper  (lua_State* L)
    {
      RET (OBJ::*f)(P1)=F;
      OBJ* obj=DataWrapper<OBJ*>::Unpack(L,1);
      DataWrapper<RET>::Pack (L,
			      (obj->*f)(
					DataWrapper<P1>::Unpack(L, 2)
					));
      return 1;
    }

    static const bool hasmeta=true;
  };
  */


#include "LuaWrappers.hh"

  template <typename WRAPPER>
  class ExportToLua {
  public:
    typedef typename WRAPPER::myobjectT objectT;

    ExportToLua (lua_State* L, const char* module_name,  const char* function_name)
    {
      luaL_Reg entry[2]={ {function_name, WRAPPER::Wrapper}, {NULL,NULL}};
      if (WRAPPER::hasmeta) {
	luaL_getmetatable(L, LuaObject<objectT*>::luahandle());
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_register(L, NULL, entry);	
      } else {
	luaL_register(L, module_name, entry);
      }
    }
  };

  template <typename T>
  void DeclareToLua(lua_State* L)
  {
    luaL_Reg entry[1]={ {NULL,NULL}};
    luaL_newmetatable(L, LuaObject<T>::luahandle());
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, entry);
  }

}

// some ugly convenience defines
#define ToLuaClass(T) \
namespace LuaWrapper {  template <>  class LuaObject<Udata*> { \
public: static const char* luahandle() { return #T"*"; } }; }
