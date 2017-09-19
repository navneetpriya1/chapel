/*
 * Copyright 2004-2017 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Generated by clang_builtin_gen.py based on clang's Builtins.def */
#define ADD_WRAPPER_PREFIX(S) chpl_clang_builtin_wrapper_##S
static inline double ADD_WRAPPER_PREFIX(cimag)(_Complex double arg0)
{
  return __builtin_cimag(arg0);
}

static inline float ADD_WRAPPER_PREFIX(cimagf)(_Complex float arg0)
{
  return __builtin_cimagf(arg0);
}

static inline long double ADD_WRAPPER_PREFIX(cimagl)(_Complex long double arg0)
{
  return __builtin_cimagl(arg0);
}

static inline double ADD_WRAPPER_PREFIX(creal)(_Complex double arg0)
{
  return __builtin_creal(arg0);
}

static inline float ADD_WRAPPER_PREFIX(crealf)(_Complex float arg0)
{
  return __builtin_crealf(arg0);
}

static inline long double ADD_WRAPPER_PREFIX(creall)(_Complex long double arg0)
{
  return __builtin_creall(arg0);
}

static inline _Complex double ADD_WRAPPER_PREFIX(conj)(_Complex double arg0)
{
  return __builtin_conj(arg0);
}

static inline _Complex float ADD_WRAPPER_PREFIX(conjf)(_Complex float arg0)
{
  return __builtin_conjf(arg0);
}

static inline _Complex long double ADD_WRAPPER_PREFIX(conjl)(_Complex long double arg0)
{
  return __builtin_conjl(arg0);
}
