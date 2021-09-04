#ifndef __U_CAT_H__
#define __U_CAT_H__
/**
 * @brief Сборка всех модулей моих велосипедов
 * @note библиотека монолитна, некоторые модули зависят друг от друга
 * пытался сначала сделать независимым от стандартной библиотеки, но
 * со временем бросил это неблагодарное дело
 */



#include "u_algorithm/u_sort.h"
//---------------------------
//-- модули для приложения --
//---------------------------
#include "u_application/u_csv.h"
#include "u_application/s5_timers.h"
#include "u_application/u_dbg.h"
#include "u_application/u_fsm.h"
#include "u_application/u_fuzzy.h"
#include "u_application/u_pid.h"
#include "u_application/u_ema.h"


//---------------------------
//-- математические модули --
//---------------------------
#include "u_math/u_math.h"
#include "u_math/u_fmatrix.h"
//--------------------------------
//-- пока нахрен этот велосипед --
//--------------------------------
// #include "u_sellect/u_sellect.h"

//--------------------------

//-----------------------------------
//-- модули стандартной библиотеки --
//-----------------------------------
#include "u_stdlib/u_mallocat.h"
#include "u_stdlib/u_string.h"
//--------------------------

//-------------------------
//-- модули типов данных --
//-------------------------
#include "u_types/u_hash.h"
#include "u_types/u_vector.h"
#include "u_types/u_cstring.h"
#include "u_types/u_ring.h"
#include "u_types/u_matrix.h"
//--------------------
//-- сетевые модули --
//--------------------
#include "u_network/u_network.h"
// #include "u_network/u_server.h"

//-------------------------
//-- модули криптографии --
//-------------------------

//TODO


#endif // __U_CAT_H__
