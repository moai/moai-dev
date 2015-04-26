/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <liboil/liboildebug.h>
#include <liboil/liboilcpu.h>
#include <liboil/liboilfault.h>
#include <liboil/liboiltest.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/**
 * SECTION:liboilclass-unstable
 * @title:OilFunctionClass
 * @short_description: Functions for manipulating function classes
 *
 * <para>
 * Functions operate on arrays of data.  The arrays can be either source
 * arrays (input only), destination arrays (output only), or in-place
 * arrays (both input and output).
 * </para>
 *
 * <para>
 * The interpretation of a parameter can usually be determined from its
 * name.  Parameters for arrays are of the form d1_1xn, where the first
 * character represents the direction (source, destination, or in-place),
 * the second represents the index for that particular direction, and
 * the characters after the underscore indicate the size of the array.
 * In this case, "1xn" represents an array that is 1 by N.  Note that
 * the index and the size can both be omitted, giving a default of 1
 * for the index and 1xn for the size.
 * </para>
 *
 * <para>
 * Parameters that represent strides are of the form "d1s".  The
 * interpretation is similar to above, except that the s indicates
 * a stride parameter.
 * </para>
 *
 * <para>
 * The exceptions to the above rule are "dest", "src", "dstr", "sstr", etc.
 * These are aliases for "d1", "s1", "d1s", and "s1s", respectively.  This
 * form is deprecated and will be removed in the 0.4 series.
 * </para>
 *
 * <para>
 * Two special parameters are "n" and "m", which determine the size of
 * the arrays in other parameters.
 * </para>
 *
 * <para>
 * Data arrays are laid out such that rows are separated by the number
 * of bytes given by the corresponding stride.  Elements in each row
 * are contiguous.  If there is no stride parameter corresponding to an
 * array, the rows of the array are contiguous.
 * </para>
 */

/**
 * SECTION:liboilimpl-unstable
 * @title:OilFunctionImpl
 * @short_description: Functions for manipulating function implementations.
 */

extern OilFunctionClass *_oil_function_class_array[];
extern OilFunctionImpl *_oil_function_impl_array[];

static int _oil_n_function_impls;
static int _oil_n_function_classes;

static void oil_init_pointers (void);
static void oil_init_structs (void);

static char * xstrdup (const char *s);

void _oil_cpu_init (void);

/**
 * SECTION:liboilinit
 * @title: Intialization
 * @short_description: Initialization functions
 */
/**
 * SECTION:liboilinit-unstable
 * @title: Intialization
 * @short_description: Initialization functions
 */
/**
 * oil_init:
 *
 * Initialize liboil.  This function must be called before any
 * other liboil function is used.  oil_init() may be called multiple
 * times.
 *
 * Since: 0.3.0
 */
static int _oil_inited = 0;
void
oil_init (void)
{

  if (_oil_inited) return;
  _oil_inited = 1;

  srand(time(NULL));

  _oil_debug_init ();
  _oil_cpu_init ();
  oil_init_pointers ();
  oil_init_structs ();

  oil_optimize_all ();

  OIL_INFO ("oil_init() finished");
}

/**
 * oil_init_no_optimize:
 *
 * Initialize liboil similar to oil_init(), but do not run the
 * profiling stage.  This function is mainly useful for internal
 * programs.
 */
void
oil_init_no_optimize (void)
{
  if (_oil_inited) return;
  _oil_inited = 1;

  srand(time(NULL));

  _oil_debug_init ();
  _oil_cpu_init ();
  oil_init_pointers ();
  oil_init_structs ();
}

/**
 * oil_optimize_all:
 *
 * Optimize all function classes.
 */
void
oil_optimize_all (void)
{
  OilFunctionClass *klass;
  int i;

  oil_fault_check_enable ();
  for (i = 0; i < _oil_n_function_classes; i++) {
    klass = oil_class_get_by_index (i);

    oil_class_optimize (klass);
  }
  OIL_INFO("%d classes, %d implementations, %d enabled",
      _oil_n_function_classes, _oil_n_function_impls, 0);
  oil_fault_check_disable ();
}

/**
 * oil_optimize:
 * @class_name: a string
 *
 * Optimize the function class that has the name specified by @class_name.
 */
void
oil_optimize (const char *class_name)
{
  OilFunctionClass *klass;

  klass = oil_class_get (class_name);
  if (klass == NULL) {
    OIL_ERROR ("could not find class %s", class_name);
    return;
  }

  oil_class_optimize (klass);
}

/**
 * oil_class_get_n_classes:
 *
 * Returns the number of function classes.
 *
 * Returns: the number of function classes
 */
int
oil_class_get_n_classes (void)
{
  return _oil_n_function_classes;
}

/**
 * oil_class_get_by_index:
 * @i: index
 *
 * Returns a pointer to the function class with index @i.
 *
 * Returns: an @OilFunctionClass
 */
OilFunctionClass *
oil_class_get_by_index (int i)
{
  if (i<0 || i>=_oil_n_function_classes) return NULL;

  return _oil_function_class_array[i];
}

/**
 * oil_impl_is_runnable:
 * @impl: an @OilFunctionImpl
 *
 * Determines whether the function implementation given by @impl
 * can be executed by the current CPU.
 *
 * Returns: 1 if the implementation can be executed, otherwise 0
 */
int
oil_impl_is_runnable (OilFunctionImpl *impl)
{
  unsigned int oil_cpu_flags = oil_cpu_get_flags();

  if ((impl->flags & OIL_CPU_FLAG_MASK) & (~oil_cpu_flags))
    return 0;
  return 1;
}

/**
 * oil_impl_is_usable:
 * @impl: an @OilFunctionImpl
 *
 * Determines whether the function implementation given by @impl
 * is useful, that is, it can be executed on the current CPU and
 * passes tests.
 *
 * Returns: 1 if the implementation can be used, otherwise 0
 */
int
oil_impl_is_usable (OilFunctionImpl *impl)
{
  unsigned int oil_cpu_flags = oil_cpu_get_flags();

  if ((impl->flags & OIL_CPU_FLAG_MASK) & (~oil_cpu_flags))
    return 0;
  if (impl->flags & OIL_IMPL_FLAG_DISABLED)
    return 0;
  return 1;
}

/**
 * oil_impl_get_by_index:
 * @i: index
 *
 * Returns a pointer to the function implementation with index @i.
 *
 * Returns: a pointer to a function implementation structure
 */
OilFunctionImpl *
oil_impl_get_by_index (int i)
{
  if (i<0 || i>=_oil_n_function_impls) return NULL;

  return _oil_function_impl_array[i];
}

/**
 * oil_class_get:
 * @class_name: the name of the function class
 *
 * Returns a pointer to the function class that has the given class
 * name.  If no such class is found, NULL is returned.
 *
 * Returns: a pointer to a function class
 */
OilFunctionClass *
oil_class_get (const char *class_name)
{
  OilFunctionClass *klass;
  int i;

  for (i = 0; i < _oil_n_function_classes; i++) {
    klass = oil_class_get_by_index (i);

    if (strcmp (klass->name, class_name) == 0)
      return klass;
  }
  return NULL;
}

/**
 * oil_class_choose_by_name:
 * @klass: a function class
 * @name: the name of an implementation
 *
 * Sets the chosen implementation for the given function class to
 * the implementation with the given name.  If no implementation
 * having the given name is found, the chosen implementation is
 * not changed.
 */
void
oil_class_choose_by_name (OilFunctionClass * klass, const char *name)
{
  OilFunctionImpl *impl;

  for(impl = klass->first_impl; impl; impl = impl->next) {
    if (impl->name && strcmp (impl->name, name) == 0) {
      klass->chosen_impl = impl;
      klass->func = impl->func;
      return;
    }
  }
}

/**
 * oil_class_optimize:
 * @klass: a function class
 *
 * Tests and profiles each implementation for the given function
 * class.  Testing compares the output of running each implementation
 * on random input against the reference implementation for the
 * same input.
 */
void
oil_class_optimize (OilFunctionClass * klass)
{
  OilFunctionImpl *impl;
  OilFunctionImpl *min_impl;
  OilTest *test;
  int ret;

  OIL_DEBUG ("optimizing class %s", klass->name);

  if (klass->reference_impl == NULL) {
    OIL_ERROR ("class %s has no reference implmentation", klass->name);
    return;
  }
  if (klass->first_impl == NULL) {
    OIL_ERROR ("class %s has no implmentations", klass->name);
    return;
  }
  
  if (klass->first_impl->next == NULL) {
    if (!oil_impl_is_runnable (klass->first_impl)) {
      OIL_ERROR ("class %s has no runable implmentations", klass->name);
      return;
    }
    OIL_DEBUG ("class %s has only one implementation %s", klass->name, klass->first_impl->name);
    klass->chosen_impl = klass->first_impl;
    klass->func = klass->first_impl->func;
    return;
  }

  test = oil_test_new (klass);
  if (test == NULL) {
    OIL_ERROR ("failed to test function class %s", klass->name);
    return;
  }

  min_impl = NULL;
  for (impl = klass->first_impl; impl; impl = impl->next) {
    OIL_LOG ("testing impl %s", impl->name);
    if (!oil_impl_is_runnable (impl))
      continue;

    ret = oil_test_check_impl (test, impl);
    if (ret) {
      impl->profile_ave = test->profile_ave;
      impl->profile_std = test->profile_std;
      OIL_LOG ("impl %s ave=%g std=%g", impl->name, impl->profile_ave,
          impl->profile_std);
      if (min_impl == NULL) {
        min_impl = impl;
      } else {
        if (impl->profile_ave < min_impl->profile_ave) {
          min_impl = impl;
        }
      }
    } else {
      OIL_WARNING("disabling implementation %s", impl->name);
      impl->profile_ave = test->profile_ave;
      impl->profile_std = test->profile_std;
      impl->flags |= OIL_IMPL_FLAG_DISABLED;
    }
  }
  if (min_impl == NULL) {
    OIL_ERROR ("failed to find optimal implementation for class %s",
        klass->name);
    return;
  }

  OIL_DEBUG("choosing implementation %s", min_impl->name);
  klass->chosen_impl = min_impl;
  klass->func = min_impl->func;

  oil_test_free (test);
}

static void
oil_init_pointers (void)
{
  int i;

  for(i=0;_oil_function_class_array[i];i++) {
    _oil_n_function_classes++;
  }

  for(i=0;_oil_function_impl_array[i];i++) {
    _oil_n_function_impls++;
  }

}

static void
oil_init_structs (void)
{
  int i;
  OilFunctionImpl *impl;

  for (i = 0; i < _oil_n_function_impls; i++) {
    impl = oil_impl_get_by_index (i);
    OIL_LOG ("registering impl %p (%s)", impl,
          (impl->name != NULL) ? impl->name : "NULL");
    if (impl->klass == NULL) {
      OIL_ERROR ("impl->klass is NULL for impl %p (%s)", impl,
          (impl->name != NULL) ? impl->name : "NULL");
      continue;
    }
    impl->next = impl->klass->first_impl;
    impl->klass->first_impl = impl;
    if (impl->flags & OIL_IMPL_FLAG_REF) {
      impl->klass->reference_impl = impl;
      impl->klass->chosen_impl = impl;
      impl->klass->func = impl->func;
    }
  }
}

/**
 * oil_class_register_impl_by_name:
 * @klass_name: the name of the class
 * @impl: an implementation
 *
 * Adds @impl to the list of implementations associated with
 * the function class given by @klass_name.
 */
void
oil_class_register_impl_by_name (const char *klass_name, OilFunctionImpl *impl)
{
  OilFunctionClass *klass;

  klass = oil_class_get (klass_name);
  if (klass == NULL) return;

  oil_class_register_impl (klass, impl);
}

/**
 * oil_class_register_impl:
 * @klass: the class
 * @impl: an implementation
 *
 * Adds @impl to the list of implementations associated with
 * the function class given by @klass.
 */
void
oil_class_register_impl (OilFunctionClass *klass, OilFunctionImpl *impl)
{
  impl->klass = klass;
  impl->next = impl->klass->first_impl;
  klass->first_impl = impl;
  if (impl->flags & OIL_IMPL_FLAG_REF) {
    impl->klass->reference_impl = impl;
    impl->klass->chosen_impl = impl;
    impl->klass->func = impl->func;
  }
}

/**
 * oil_class_register_impl_full:
 * @klass: the class
 * @func: the function
 * @name: name of the function
 * @flags: CPU flags
 *
 * Adds @func to the list of implementations associated with
 * the function class given by @klass.
 */
void
oil_class_register_impl_full (OilFunctionClass *klass,
    void (*func)(void), const char *name, unsigned int flags)
{
  OilFunctionImpl *impl;

  impl = malloc(sizeof(OilFunctionImpl));
  memset (impl, 0, sizeof(OilFunctionImpl));

  impl->func = func;
  impl->flags = flags;
  impl->name = xstrdup(name);

  oil_class_register_impl(klass,impl);
}

static char *
xstrdup (const char *s)
{
  int n = strlen(s);
  char *t;

  n = strlen(s);
  t = malloc(n + 1);
  memcpy (t, s, n);
  t[n] = 0;

  return t;
}

