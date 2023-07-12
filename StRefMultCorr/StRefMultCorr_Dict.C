// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME StRefMultCorr_Dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "BadRun.h"
#include "Param.h"
#include "StRefMultCorr.h"
#include "CentralityMaker.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_StRefMultCorr(void *p = nullptr);
   static void *newArray_StRefMultCorr(Long_t size, void *p);
   static void delete_StRefMultCorr(void *p);
   static void deleteArray_StRefMultCorr(void *p);
   static void destruct_StRefMultCorr(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::StRefMultCorr*)
   {
      ::StRefMultCorr *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::StRefMultCorr >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("StRefMultCorr", ::StRefMultCorr::Class_Version(), "StRefMultCorr.h", 43,
                  typeid(::StRefMultCorr), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::StRefMultCorr::Dictionary, isa_proxy, 4,
                  sizeof(::StRefMultCorr) );
      instance.SetNew(&new_StRefMultCorr);
      instance.SetNewArray(&newArray_StRefMultCorr);
      instance.SetDelete(&delete_StRefMultCorr);
      instance.SetDeleteArray(&deleteArray_StRefMultCorr);
      instance.SetDestructor(&destruct_StRefMultCorr);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::StRefMultCorr*)
   {
      return GenerateInitInstanceLocal((::StRefMultCorr*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::StRefMultCorr*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_CentralityMaker(void *p);
   static void deleteArray_CentralityMaker(void *p);
   static void destruct_CentralityMaker(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CentralityMaker*)
   {
      ::CentralityMaker *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CentralityMaker >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CentralityMaker", ::CentralityMaker::Class_Version(), "CentralityMaker.h", 50,
                  typeid(::CentralityMaker), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CentralityMaker::Dictionary, isa_proxy, 4,
                  sizeof(::CentralityMaker) );
      instance.SetDelete(&delete_CentralityMaker);
      instance.SetDeleteArray(&deleteArray_CentralityMaker);
      instance.SetDestructor(&destruct_CentralityMaker);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CentralityMaker*)
   {
      return GenerateInitInstanceLocal((::CentralityMaker*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CentralityMaker*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr StRefMultCorr::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *StRefMultCorr::Class_Name()
{
   return "StRefMultCorr";
}

//______________________________________________________________________________
const char *StRefMultCorr::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::StRefMultCorr*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int StRefMultCorr::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::StRefMultCorr*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *StRefMultCorr::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::StRefMultCorr*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *StRefMultCorr::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::StRefMultCorr*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CentralityMaker::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CentralityMaker::Class_Name()
{
   return "CentralityMaker";
}

//______________________________________________________________________________
const char *CentralityMaker::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CentralityMaker*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CentralityMaker::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CentralityMaker*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CentralityMaker::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CentralityMaker*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CentralityMaker::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CentralityMaker*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void StRefMultCorr::Streamer(TBuffer &R__b)
{
   // Stream an object of class StRefMultCorr.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(StRefMultCorr::Class(),this);
   } else {
      R__b.WriteClassBuffer(StRefMultCorr::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_StRefMultCorr(void *p) {
      return  p ? new(p) ::StRefMultCorr : new ::StRefMultCorr;
   }
   static void *newArray_StRefMultCorr(Long_t nElements, void *p) {
      return p ? new(p) ::StRefMultCorr[nElements] : new ::StRefMultCorr[nElements];
   }
   // Wrapper around operator delete
   static void delete_StRefMultCorr(void *p) {
      delete ((::StRefMultCorr*)p);
   }
   static void deleteArray_StRefMultCorr(void *p) {
      delete [] ((::StRefMultCorr*)p);
   }
   static void destruct_StRefMultCorr(void *p) {
      typedef ::StRefMultCorr current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::StRefMultCorr

//______________________________________________________________________________
void CentralityMaker::Streamer(TBuffer &R__b)
{
   // Stream an object of class CentralityMaker.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CentralityMaker::Class(),this);
   } else {
      R__b.WriteClassBuffer(CentralityMaker::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CentralityMaker(void *p) {
      delete ((::CentralityMaker*)p);
   }
   static void deleteArray_CentralityMaker(void *p) {
      delete [] ((::CentralityMaker*)p);
   }
   static void destruct_CentralityMaker(void *p) {
      typedef ::CentralityMaker current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CentralityMaker

namespace {
  void TriggerDictionaryInitialization_StRefMultCorr_Dict_Impl() {
    static const char* headers[] = {
"BadRun.h",
"Param.h",
"StRefMultCorr.h",
"CentralityMaker.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/ubuntu/Documents/root_v6.26.04.Linux-ubuntu20-x86_64-gcc9.4/root/include",
"/home/ubuntu/Documents/root_v6.26.04.Linux-ubuntu20-x86_64-gcc9.4/root/include/",
"/home/ubuntu/Documents/root_v6.26.04.Linux-ubuntu20-x86_64-gcc9.4/StRefMultCorr/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "StRefMultCorr_Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$StRefMultCorr.h")))  StRefMultCorr;
class __attribute__((annotate("$clingAutoload$CentralityMaker.h")))  CentralityMaker;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "StRefMultCorr_Dict dictionary payload"

#ifndef _VANILLA_ROOT_
  #define _VANILLA_ROOT_ 1
#endif
#ifndef ROOT_CINT
  #define ROOT_CINT 1
#endif
#ifndef __ROOT__
  #define __ROOT__ 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "BadRun.h"
#include "Param.h"
#include "StRefMultCorr.h"
#include "CentralityMaker.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CentralityMaker", payloadCode, "@",
"StRefMultCorr", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("StRefMultCorr_Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_StRefMultCorr_Dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_StRefMultCorr_Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_StRefMultCorr_Dict() {
  TriggerDictionaryInitialization_StRefMultCorr_Dict_Impl();
}
