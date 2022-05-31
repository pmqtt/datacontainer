//
// Created by cefour on 16.05.22.
//

#ifndef DATACONTAINER_MACROHELPER_H
#define DATACONTAINER_MACROHELPER_H

#define _GET_NTH_ARG(_1, _2, _3, _4, _5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,N, ...) N

#define _fe_0(_call, ...)
#define _fe_1(_call, x) _call( x )
#define _fe_2(_call, x, ...) _call( x ), _fe_1(_call, __VA_ARGS__)
#define _fe_3(_call, x, ...) _call( x ), _fe_2(_call, __VA_ARGS__)
#define _fe_4(_call, x, ...) _call( x ), _fe_3(_call, __VA_ARGS__)
#define _fe_5(_call, x, ...) _call( x ), _fe_4(_call, __VA_ARGS__)
#define _fe_6(_call, x, ...) _call( x ), _fe_5(_call, __VA_ARGS__)
#define _fe_7(_call, x, ...) _call( x ), _fe_6(_call, __VA_ARGS__)
#define _fe_8(_call, x, ...) _call( x ), _fe_7(_call, __VA_ARGS__)
#define _fe_9(_call, x, ...) _call( x ), _fe_8(_call, __VA_ARGS__)
#define _fe_10(_call, x, ...) _call( x ), _fe_9(_call, __VA_ARGS__)
#define _fe_11(_call, x, ...) _call( x ), _fe_10(_call, __VA_ARGS__)
#define _fe_12(_call, x, ...) _call( x ), _fe_11(_call, __VA_ARGS__)
#define _fe_13(_call, x, ...) _call( x ), _fe_12(_call, __VA_ARGS__)
#define _fe_14(_call, x, ...) _call( x ), _fe_13(_call, __VA_ARGS__)
#define _fe_15(_call, x, ...) _call( x ), _fe_14(_call, __VA_ARGS__)
#define _fe_16(_call, x, ...) _call( x ), _fe_15(_call, __VA_ARGS__)
#define _fe_17(_call, x, ...) _call( x ), _fe_16(_call, __VA_ARGS__)
#define _fe_18(_call, x, ...) _call( x ), _fe_17(_call, __VA_ARGS__)
#define _fe_19(_call, x, ...) _call( x ), _fe_18(_call, __VA_ARGS__)

#define CALL_MACRO_X_FOR_EACH(x, ...) \
    _GET_NTH_ARG("ignored", ##__VA_ARGS__, \
    _fe_19,_fe_18,_fe_17,_fe_16,_fe_15,_fe_14,_fe_13,_fe_12,_fe_11,_fe_10,_fe_9,_fe_8,_fe_7,_fe_6 ,_fe_5,_fe_4, _fe_3, _fe_2, _fe_1, _fe_0)(x, ##__VA_ARGS__)



#define ENUM_ITEM(ITEM) ITEM
#define ENUM_ITEM_STR(ITEM) #ITEM

#define CREATE_STRINGABLE_ENUM(ENAME,...) \
    enum ENAME{ \
       CALL_MACRO_X_FOR_EACH(ENUM_ITEM,__VA_ARGS__)\
    }; \
    std::string ENAME##toString(ENAME v) {       \
        std::vector<std::string> val { CALL_MACRO_X_FOR_EACH(ENUM_ITEM_STR,__VA_ARGS__)}; \
        return val[v];\
    }

#define UNUSED(x) (void)(x)

#endif //DATACONTAINER_MACROHELPER_H
