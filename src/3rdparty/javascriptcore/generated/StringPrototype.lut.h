// Automatically generated from ../runtime/StringPrototype.cpp using ../create_hash_table. DO NOT EDIT!

#include "Lookup.h"

namespace JSC {

static const struct HashTableValue stringTableValues[23] = {
   { "toString", DontEnum|Function, (intptr_t)stringProtoFuncToString, (intptr_t)0 },
   { "valueOf", DontEnum|Function, (intptr_t)stringProtoFuncToString, (intptr_t)0 },
   { "charAt", DontEnum|Function, (intptr_t)stringProtoFuncCharAt, (intptr_t)1 },
   { "charCodeAt", DontEnum|Function, (intptr_t)stringProtoFuncCharCodeAt, (intptr_t)1 },
   { "concat", DontEnum|Function, (intptr_t)stringProtoFuncConcat, (intptr_t)1 },
   { "indexOf", DontEnum|Function, (intptr_t)stringProtoFuncIndexOf, (intptr_t)1 },
   { "lastIndexOf", DontEnum|Function, (intptr_t)stringProtoFuncLastIndexOf, (intptr_t)1 },
   { "match", DontEnum|Function, (intptr_t)stringProtoFuncMatch, (intptr_t)1 },
   { "replace", DontEnum|Function, (intptr_t)stringProtoFuncReplace, (intptr_t)2 },
   { "search", DontEnum|Function, (intptr_t)stringProtoFuncSearch, (intptr_t)1 },
   { "slice", DontEnum|Function, (intptr_t)stringProtoFuncSlice, (intptr_t)2 },
   { "split", DontEnum|Function, (intptr_t)stringProtoFuncSplit, (intptr_t)2 },
   { "substr", DontEnum|Function, (intptr_t)stringProtoFuncSubstr, (intptr_t)2 },
   { "substring", DontEnum|Function, (intptr_t)stringProtoFuncSubstring, (intptr_t)2 },
   { "toLowerCase", DontEnum|Function, (intptr_t)stringProtoFuncToLowerCase, (intptr_t)0 },
   { "toUpperCase", DontEnum|Function, (intptr_t)stringProtoFuncToUpperCase, (intptr_t)0 },
   { "localeCompare", DontEnum|Function, (intptr_t)stringProtoFuncLocaleCompare, (intptr_t)1 },
   { "toLocaleLowerCase", DontEnum|Function, (intptr_t)stringProtoFuncToLowerCase, (intptr_t)0 },
   { "toLocaleUpperCase", DontEnum|Function, (intptr_t)stringProtoFuncToUpperCase, (intptr_t)0 },
   { "trim", DontEnum|Function, (intptr_t)stringProtoFuncTrim, (intptr_t)0 },
   { "trimLeft", DontEnum|Function, (intptr_t)stringProtoFuncTrimLeft, (intptr_t)0 },
   { "trimRight", DontEnum|Function, (intptr_t)stringProtoFuncTrimRight, (intptr_t)0 },
   { 0, 0, 0, 0 }
};

extern const HashTable stringTable =
    { 66, 63, stringTableValues, 0 };
} // namespace
