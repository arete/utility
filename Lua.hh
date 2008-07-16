extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <vector>

namespace LuaWrapper {

  class LuaClassData
  {
  public:
    LuaClassData(const std::string& name)
    {
      handle=name;
    }

    void insertMetaTablePtr(void* ptr)
    {
      metaTablePtrs.push_back(ptr);
      std::sort(metaTablePtrs.begin(), metaTablePtrs.end());
    }


    bool isCompatible(void* ptr)
    {
      return std::binary_search(metaTablePtrs.begin(), metaTablePtrs.end(), ptr);
    }

    std::string handle;
    std::vector <void*> metaTablePtrs;
  };

  template <typename T>
  class LuaClass
  {
  public:
    LuaClass(const std::string& name)
    {
      if (!data) {
	data=new LuaClassData(name);
      }
    }
  
    static const char* luahandle()
    {
      return data->handle.c_str();
    }
  
    static T* getPtr(lua_State* L, int index)
    {
      void** p = (void**) lua_touserdata(L, index);
      if (p)
	if (lua_getmetatable(L, index)) {
	  void* meta=(void*)lua_topointer (L, -1);
	  lua_pop(L, 1);
	  if (data->isCompatible(meta))
	    return (T*)*p;
	}
      luaL_typerror(L, index ,luahandle());
      return 0;  /* avoid warnings */
    }

    static void packPtr(lua_State* L, T* obj)
    {
      T** ptr=(T**) lua_newuserdata(L, sizeof(T*));
      *ptr=obj;
      luaL_getmetatable(L,  luahandle());
      lua_setmetatable(L, -2);
    }

    static LuaClassData* data;
  };

  template <typename T>
  LuaClassData* LuaClass<T>::data=0;

  template <typename T>
  class Unpack {};

  template <>
  class Unpack<int>
  {
  public:
    static int convert(lua_State* L, int index)
    {
      return (int)luaL_checkinteger(L,index);
    }
  };

  template <>
  class Unpack<double>
  {
  public:
    static double convert(lua_State* L, int index)
    {
      return (double)luaL_checknumber(L,index);
    }
  };

  template <>
  class Unpack<bool>
  {
  public:
    static bool convert(lua_State* L, int index)
    {
      // Why the heck this function is not there ????
      // return (bool)luaL_checkboolean(L,index);
      luaL_checktype(L, index, LUA_TBOOLEAN);
      return (bool)lua_toboolean(L, index);
    }
  };

  template <>
  class Unpack<const char*>
  {
  public:
    static const char* convert(lua_State* L, int index)
    {
      return luaL_checkstring(L,index);
    }
  };

  template <>
  class Unpack<std::string>
  {
  public:
    static const std::string convert(lua_State* L, int index)
    {
      return std::string(luaL_checkstring(L,index));
    }
  };

  template <typename T>
  class Unpack<T&>
  {
  public:
    static T& convert(lua_State* L, int index)
    {
      return *LuaClass<T>::getPtr(L, index);
    } 
  };

  template <typename T>
  class Unpack<const T&>
  { 
  public:
    static const T& convert(lua_State* L, int index)
    {
      return *LuaClass<T>::getPtr(L, index);
    } 
  };

  template <typename T>
  class Unpack<T*>
  { 
  public:
    static T* convert(lua_State* L, int index)
    {
      return LuaClass<T>::getPtr(L, index);
    } 
  };

  template <typename T>
  class Pack {};

  template <>
  class Pack<int>
  {
  public:
    static void convert(lua_State* L, int value)
    {
      lua_pushinteger(L, value);
    }
  };

  template <>
  class Pack<double>
  {
  public:
    static void convert(lua_State* L, double value)
    {
      lua_pushnumber(L, value);
    }
  };

  template <>
  class Pack<bool>
  {
  public:
    static void convert(lua_State* L, bool value)
    {
      lua_pushboolean(L, value);
    }
  };

  template <>
  class Pack<const char*>
  {
  public:
    static void convert(lua_State* L, const char* value)
    {
      lua_pushstring(L, value);
    }
  };

  template <>
  class Pack<std::string>
  {
  public:
    static void convert(lua_State* L, std::string value)
    {
      lua_pushstring(L, value.c_str());
    }
  };

  template <>
  class Pack<std::string&>
  {
  public:
    static void convert(lua_State* L, std::string value)
    {
      lua_pushstring(L, value.c_str());
    }
  };

  template <>
  class Pack<const std::string&>
  {
  public:
    static void convert(lua_State* L, std::string value)
    {
      lua_pushstring(L, value.c_str());
    }
  };


  template <typename T>
  class Pack <T*>
  {
  public:
    static void convert(lua_State* L, T* obj)
    {
      LuaClass<T>::packPtr(L, obj);
    }
  };

  template <typename T>
  class Pack <T&>
  {
  public:
    static void convert(lua_State* L, T& obj)
    {
      LuaClass<T>::packPtr(L, *obj);
    }
  };

  template <typename ORIG, typename ALIAS>
  class UnpackTypedef
  {
  public:
    static ALIAS convert(lua_State* L, int index)
    {
      return (ALIAS)Unpack<ORIG>::convert(L,index);
    }
  };

  template <typename ORIG, typename ALIAS>
  class PackTypedef
  {
  public:
    static void convert(lua_State* L, ALIAS value)
    {
      Pack<ORIG>::convert(L, (ORIG)value);
    }
  };


#include "LuaWrappers.hh"

  // does not need to be expanded in LuaWrappers.hh hackery, since
  // dtors never take arguments ;)
  template <typename OBJ>
  class DtorWrapper
  {
  public:
    typedef OBJ myobjectT;

    static int Wrapper  (lua_State* L)
    {
      OBJ* obj=Unpack<OBJ*>::convert(L,1);
      delete obj;
      return 0;
    }

    static const bool hasmeta=true;
    static const bool noindex=false;
  };



  template <typename WRAPPER>
  class ExportToLua {
  public:
    typedef typename WRAPPER::myobjectT objectT;

    ExportToLua (lua_State* L, const char* module_name,  const char* function_name)
    {
      //std::cout << "register " << function_name << std::endl;
      luaL_Reg entry[2]={ {function_name, WRAPPER::Wrapper}, {NULL,NULL}};
      if (WRAPPER::hasmeta) {
	luaL_getmetatable(L, LuaClass<objectT>::luahandle());
	if (strncmp(function_name, "__", 2)!=0 && !WRAPPER::noindex) {
	  lua_getfield(L, -1, "__index");
	  luaL_register(L, 0, entry);
	  lua_pop(L,2);
	} else { // allow direct registration of ctors and __* methods
	  luaL_register(L, 0, entry);
	  lua_pop(L,1);
	}
      } else {
	luaL_register(L, module_name, entry);
      }
    }
  };

  template <typename T>
  void DeclareToLua(lua_State* L, const char* module_name=0)
  {
    luaL_newmetatable(L, LuaClass<T>::luahandle());

    if (module_name) {
      // drop a reference to the metatable into the module table
      luaL_findtable (L, LUA_GLOBALSINDEX, module_name, 1);
      lua_pushvalue(L, -2);
      lua_setfield(L, -2, LuaClass<T>::luahandle());
      lua_pop(L,1);
    }

    // create index table
    lua_newtable(L);
    lua_setfield(L, -2, "__index");

    // remember metatable raw pointer for later type checks
    LuaClass<T>::data->insertMetaTablePtr((void*)lua_topointer (L, -1));
    lua_pop(L,1);
  }

  template <typename BASE, typename DERIVED>
  void InheritMeta(lua_State* L)
  {
    luaL_getmetatable(L, LuaClass<DERIVED>::luahandle()); // -5
    lua_getfield(L,-1,"__index"); // -4
    luaL_getmetatable(L, LuaClass<BASE>::luahandle()); // -3
    lua_getfield(L,-1,"__index"); // -2

    lua_pushnil(L);  /* -1 first key */
    while (lua_next(L, -2) != 0) {
      lua_pushvalue(L, -2); // copy key and move down
      lua_insert(L, -3);
      lua_settable(L, -6); // DERIVED __index at -6
    }
    lua_pop(L, 5);
  }

  template <typename BASE, typename DERIVED>
  void AllowDowncast(lua_State* L)
  {
    luaL_getmetatable(L, LuaClass<DERIVED>::luahandle());
    LuaClass<BASE>::data->insertMetaTablePtr((void*)lua_topointer (L, -1));
    lua_pop(L,1);
  }

  template <typename C>
  void RegisterValue(lua_State* L, const char* module_name, const char* name, C value)
  {
    luaL_findtable (L, LUA_GLOBALSINDEX, module_name, 1);
    Pack<C>::convert(L, value);
    lua_setfield(L, -2, name);
    lua_pop(L,1);
  }
}


// some convenient defines:
#define ToLuaTypedef(ORIG,ALIAS) \
namespace LuaWrapper { \
  template <> class Unpack<ALIAS>:public UnpackTypedef<ORIG, ALIAS>{}; \
  template <> class Pack<ALIAS>:public PackTypedef<ORIG, ALIAS>{}; }
