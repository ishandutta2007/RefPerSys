/*
 * File: refpersys/inc/types.h
 *
 * Description:
 *      This file is part of the Reflective Persistent System. It declares the
 *      refpersys types and their associated constants.
 *
 * Author(s):
 *      Basile Starynkevitch <basile@starynkevitch.net>
 *      Niklas Rosencrantz <niklasro@gmail.com>
 *      Abhishek Chakravarti <abhishek@taranjali.org>
 *
 * Copyright:
 *      (c) 2019 The Reflective Persistent System Team
 *      <https://refpersys.gitlab.io>
 *
 * License:
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/* create header guard */
#if (!defined __REFPERSYS_TYPES_DEFINED)
#       define __REFPERSYS_TYPES_DEFINED


/* include required header files */
#include <stdint.h>
#include <stdlib.h>
#include <cstddef>
#include <cstdio>
#include <cassert>
#include "util.h"

extern "C" {
#include "mps/code/mps.h"
#include "mps/code/mpsavm.h"
#include "mps/code/mpscamc.h"
}


/* see https://en.wikipedia.org/wiki/Flexible_array_member
   wikipage. C++ don't really have them, we are mimicking them as the
   last member of some struct which is an array of dimension 0 */
#define RPS_FLEXIBLE_DIM 0



/****************************************************************************
 * Section: Object ID Serial (rps_serial63)
 ****************************************************************************/


/* declare the rps_serial63 type; this is analogous to the Bismon
 * serial64_tyBM type in the header file id_BM.h */
typedef uint64_t rps_serial63;


/* number of digits in an object ID serial when represented the form of
 * base 62 digits */
#define RPS_SERIAL63_DIGITS 11


/* object ID serials are represented compactly in base 62, because
   there are 2*26 letters a...z and A...Z and 10 digits 0...9, so
   2*26+10 is 62 */
#define RPS_SERIAL63_BASE 62


/*
 *      RPS_SERIAL63_MIN - minimum object ID serial value
 *
 *      The RPS_SERIAL63_MIN symbolic constant defines the minimum value of an
 *      object ID serial.
 *
 *      62*62, because we want the minimal serial to be _00000000100
 *      since we don't want too many 0-s, a serial like _00000000000
 *      is too confusing to read.
 *
 *      In practice, serials are random, and won't have that much 0
 *      digits anyway.
 *
 */
#define RPS_SERIAL63_MIN ((uint64_t) 3884)


/*
 *      RPS_SERIAL63_MAX - maximum object ID serial value
 *
 *      The RPS_SERIAL63_MAX symbolic constant defines the maximum value of an
 *      object ID serial.
 *
 *      The last serial would be _9ZZZZZZZZZZ, so one more is
 *      corresponding to 10 * 62 * (62* 62*62) * (62*62*62) *
 *      (62*62*62)
 */
#define RPS_SERIAL63_MAX ((uint64_t) 8392993658683402240)


/*
 *      RPS_SERIAL63_DELTA - delta of object ID serial maxima and minima
 *
 *      The RPS_SERIAL63_DELTA symbolic constant defines the difference between
 *      the the maxiumum and minimum values of an object ID serial.
 *
 *      TODO: explain why it is RPS_SERIAL63_MAX - RPS_SERIAL63_MIN
 */
#define RPS_SERIAL63_DELTA (RPS_SERIAL63_MAX - RPS_SERIAL63_MIN)


/****************************************************************************
 * Section: Object Bucket (rps_bucket)
 ****************************************************************************/


/*
 *      RPS_BUCKET_MAX - maximum object buckets
 *
 *      The RPS_BUCKET_MAX symbolic constant defines the maximum
 *      object buckets.  The first two "digits" of a serial define its
 *      bucket, so if it starts with _1a the bucket number would be
 *      1*62+11.
 */
#define RPS_BUCKET_MAX ((size_t) 620)


/****************************************************************************
 * Section: Object ID
 ****************************************************************************/


/* TODO: object IDs are currently 128 bits, but may be reduced down to
 * 96 bits or a few more */
typedef struct rps_objid_st
{
  rps_serial63 hi;
  rps_serial63 lo; // if we reduce the number of bits (e.g. to 96 or
  // 104), lo could fit in 32 or a few more bits
} rps_objid;


/****************************************************************************
 * Section: Object
 ****************************************************************************/

/* objects are a some kind of values, but they go into AMS zones,
   because they should not be moved by the MPS GC, since they contain
   a mutex. */

/* an object is represented by a memory zone of type Rps_Object_Data,
   so object references are pointers to such zones. See type
   RpsObjectRef below. */


/****************************************************************************
 * Section: Typed Types (WIP)
 ****************************************************************************/

/* corresponds to hash_tyBM */
/* TODO: need to define */
/* TODO: replace with class rps::Hash? */
typedef uint32_t rps_hash; // an hash number is a non-zero 32 bits int

/* corresponds to gctyenum_BM; enumerates garbage collected types of
 * refpersys */
/* TODO: replace with enum rps::ValType? */
enum RPS_VALTYPE_ENUM
{
#warning TODO: RPS_VALTYPE_ENUM is still incomplete
  RPS_VALTYPE_INT = -1,   /* tagged integer */
  RPS_VALTYPE_NONE = 0,   /* nil */
  RPS_VALTYPE_STRING,     /* boxed string */
  RPS_VALTYPE_DOUBLE,     /* boxed double */
  RPS_VALTYPE_SET,        /* boxed set */
  RPS_VALTYPE_TUPLE,      /* boxed tuple */
  /// RPS_VALTYPE_NODE,       /* boxed node */
  RPS_VALTYPE_CLOSURE,    /* boxed closure */
  RPS_VALTYPE_OBJECT,     /* boxed object */
  RPS_VALTYPE_UNSPECIFIED /* unspecified value */

};





namespace rps
{

// represents a 63-bit serial code
class Serial63
{
public:
  static const uint64_t MIN = RPS_SERIAL63_MIN;
  static const uint64_t MAX = RPS_SERIAL63_MAX;
  static const uint64_t DELTA = Serial63::MAX - Serial63::MIN;
  static const uint64_t MAXBUCKET = RPS_BUCKET_MAX;

  inline Serial63()
  {
    do
      {
        m_word = rps_random_uint64();
      }
    while (!this->valid());
  }

  inline ~Serial63()
  { }

  inline bool valid()
  {
    return m_word > Serial63::MIN && m_word < Serial63::MAX;
  }

  inline uint64_t bucket()
  {
    return m_word / (Serial63::DELTA / Serial63::MAXBUCKET);
  }

  int base62(char str);
  static Serial63 parse(const char *str);

private:
  uint64_t m_word;
};


// represents an object ID
class ObjectId
{
public:
  inline ObjectId()
    : m_hi()
    , m_lo()
  { }

  ~ObjectId();

private:
  Serial63 m_hi;
  Serial63 m_lo;
};


// represents a refpersys value
class Rps_Value
{
public:

  // for tagged integers
  inline Rps_Value(intptr_t i)
    : m_int((i<<1)|1)
  { }

  inline ~Rps_Value()
  { }

  // should follow the rule of five of C++11

private:
  union
  {
    void* m_ptr;
    intptr_t m_int;
  };
}; // end of Rps_Value



////////////////////////////////////////////////////////////////

} // namespace rps


#endif /* (!defined __REFPERSYS_TYPES_DEFINED) */


class Rps_Object_Data;
typedef Rps_Object_Data* RpsObjectRef;


class Rps_Value_Data                    // begin declaration of the class
{
  const RPS_VALTYPE_ENUM _gctype;
protected:
  Rps_Value_Data(RPS_VALTYPE_ENUM ty) : _gctype(ty) {};
public:
  RPS_VALTYPE_ENUM type() const
  {
    return _gctype;
  }
};


class Rps_Value_Data_Mostly_Copying : public Rps_Value_Data
{
public:
  static void init_mps(void);

protected:
#warning FIXME _arena is probably a single static member of Rps_Value_Data
  static thread_local mps_arena_t _arena;  // MPS arena
  static thread_local mps_pool_t _pool;    // MPS pool
  static thread_local mps_ap_t _allocpt;   // MPS allocation point

  // Default constructor
  inline Rps_Value_Data_Mostly_Copying(RPS_VALTYPE_ENUM type)
    : Rps_Value_Data(type)
  { }


  // The optional gap of operator new is needed for "flexible array
  // members" trick. The caller should check or ensure that both `size`
  // and `gap` are suitably aligned to alignof(void*) which is probably
  // 8 bytes.
  //
  // This code is critical for performance. We should expect zillions
  // of allocations.

  // Basile made a mistake in asking for such an 'operator new'. It is
  // subtle but against MPS invariant.

  // read carefully
#warning operator new is probably wrong, a design bug by Basile
  void* operator new(size_t size, size_t gap = 0)
  {
    mps_addr_t addr;

    assert (size % alignof(void*) == 0);
    assert (gap % alignof(void*) == 0);
    size += gap;

    do
      {
        mps_res_t res = mps_reserve(&addr, _allocpt, size);

        if (rps_unlikely(res != MPS_RES_OK))
          {
            rps_perror_mps_reserve(size);
            abort();
          }
      }
    while (!mps_commit(_allocpt, addr, size));

    return addr;
  }

private:
  static void init_arena(void);
  static void init_pool(void);
  static void init_ap(void);
}; // end of Rps_Value_Data_Mostly_Copying





class Rps_Value_Data_Scalar_Copying : public Rps_Value_Data
{
protected:
  static thread_local mps_arena_t _arena;  // MPS arena
  static thread_local mps_pool_t _pool;    // MPS pool
  static thread_local mps_ap_t _allocpt;   // MPS allocation point


  // Default constructor
  inline Rps_Value_Data_Scalar_Copying(RPS_VALTYPE_ENUM type)
    : Rps_Value_Data(type)
  { }
};  // end of Rps_Value_Data_Scalar_Copying






class Rps_Value_Data_Mark_Sweep : public Rps_Value_Data
{
protected:
  static thread_local mps_arena_t _arena;  // MPS arena
  static thread_local mps_pool_t _pool;    // MPS pool
  static thread_local mps_ap_t _allocpt;   // MPS allocation point

  // Default constructor
  inline Rps_Value_Data_Mark_Sweep(RPS_VALTYPE_ENUM type)
    : Rps_Value_Data(type)
  { }
};  // end of Rps_Value_Data_Scalar_Copying


////////////////
class Rps_Sequence_Data : public Rps_Value_Data_Mostly_Copying
{
public:

protected:
  Rps_Sequence_Data(RPS_VALTYPE_ENUM ty, size_t size, RpsObjectRef *arr)
    : Rps_Value_Data_Mostly_Copying(ty), m_size(size)
  {
    for (size_t i=0; i<size; i++) m_objects[i] = arr[i];
  };
private:
  size_t m_size;
  RpsObjectRef m_objects[RPS_FLEXIBLE_DIM];
}; // end Rps_Sequence_Data

///////
class Rps_Tuple_Data : public Rps_Sequence_Data
{
  Rps_Tuple_Data(size_t size, RpsObjectRef *arr)
    : Rps_Sequence_Data(RPS_VALTYPE_TUPLE, size, arr)
  {
  };
public:
  static Rps_Tuple_Data* make(size_t size, RpsObjectRef *arr);
};				// end Rps_Tuple_Data

//////
class Rps_Set_Data : public Rps_Sequence_Data
{
public:
  static Rps_Set_Data* make(size_t size, RpsObjectRef *arr);
};				// end Rps_Set_Data
