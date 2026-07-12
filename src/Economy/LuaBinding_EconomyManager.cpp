#include "global.h"
#include "EconomyManager.h"
#include "LuaBinding.h"
#include "BobcoinBridge.h"

/** @brief Lua bindings for EconomyManager. */
class LunaEconomyManager: public Luna<EconomyManager>
{
public:
	static int GetBalance( T* p, lua_State *L )
	{
		lua_pushnumber( L, (double)p->GetBalance() );
		return 1;
	}

	static int BuyItem( T* p, lua_State *L )
	{
		RString sItemID = SArg(1);
		long long iCost = (long long)IArg(2);
		lua_pushboolean( L, p->BuyItem(sItemID, iCost) );
		return 1;
	}

	static int HasItem( T* p, lua_State *L )
	{
		RString sItemID = SArg(1);
		lua_pushboolean( L, p->HasItem(sItemID) );
		return 1;
	}

	static int GetMarketplaceItems( T* p, lua_State *L )
	{
		std::vector<MarketplaceItem> items = p->GetMarketplaceItems();
		lua_newtable(L);

		for (size_t i = 0; i < items.size(); ++i) {
			lua_newtable(L);
			lua_pushstring(L, items[i].sID); lua_setfield(L, -2, "ID");
			lua_pushstring(L, items[i].sName); lua_setfield(L, -2, "Name");
			lua_pushnumber(L, items[i].iPrice); lua_setfield(L, -2, "Price");
			lua_rawseti(L, -2, i + 1);
		}

		return 1;
	}

	LunaEconomyManager()
	{
		ADD_METHOD( GetBalance );
		ADD_METHOD( BuyItem );
		ADD_METHOD( HasItem );
		ADD_METHOD( GetMarketplaceItems );
	}
};

LUA_REGISTER_CLASS( EconomyManager )
