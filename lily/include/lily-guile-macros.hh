/*
  This file is part of LilyPond, the GNU music typesetter.

  Copyright (C) 2005--2022 Han-Wen Nienhuys <hanwen@xs4all.nl>

  LilyPond is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LilyPond is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with LilyPond.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LILY_GUILE_MACROS_HH
#define LILY_GUILE_MACROS_HH

#include "config.hh"

#include <string>

/* this lets us "overload" macros such as get_property to take
   symbols as well as strings */
inline SCM
scm_or_str2symbol (char const *c) { return scm_from_utf8_symbol (c); }

inline SCM
scm_or_str2symbol (SCM s)
{
  assert (scm_is_symbol (s));
  return s;
}

/* Using this trick we cache the value of scm_from_utf8_symbol ("fooo")
   where "fooo" is a constant char *. This is done at the cost of one
   static variable per ly_symbol2scm() use, and the cost of C++'s
   mechanism to ensure a static variable is only initialized once.
 */
#define ly_symbol2scm(x)                                                \
  (__builtin_constant_p (x)                                             \
   ? [&] {                                                              \
     static SCM cached = scm_gc_protect_object (scm_or_str2symbol (x)); \
     return cached;                                                     \
   } ()                                                                 \
   : scm_or_str2symbol (x))

/*
  Adds the NAME as a Scheme function, and a variable to store the SCM
  version of the function in the static variable NAME_proc
*/
#define DECLARE_SCHEME_CALLBACK(NAME, ARGS)     \
  static SCM NAME ARGS;                         \
  static SCM NAME ## _proc

std::string mangle_cxx_identifier (const char *);

void ly_internal_add_type_predicate (const void *pred, const char *name);

template <typename ReturnType>
inline void ly_add_type_predicate (ReturnType (*pred) (SCM), const char *name)
{
  // ReturnType might be bool.
  // ReturnType might be int for scm_is_foo().
  static_assert (static_cast<bool> (ReturnType ()) || true,
                 "predicate return type must be convertible to bool");
  return ly_internal_add_type_predicate (reinterpret_cast<const void *> (pred),
                                         name);
}

std::string internal_predicate_to_typename (const void *pred);

template <typename ReturnType>
inline std::string predicate_to_typename (ReturnType (*pred) (SCM))
{
  static_assert (static_cast<bool> (ReturnType ()) || true,
                 "predicate return type must be convertible to bool");
  return internal_predicate_to_typename (reinterpret_cast<const void *> (pred));
}

// ly_scm_func_of_arity<n>::ptr_type is a pointer to a function taking n SCM
// arguments and returning SCM.
template <unsigned>
struct ly_scm_func_of_arity
{
  // ptr_type is defined in specializations
};

template <>
struct ly_scm_func_of_arity<0>
{
  typedef SCM (*ptr_type) ();
};

template <>
struct ly_scm_func_of_arity<1>
{
  typedef SCM (*ptr_type) (SCM);
};

template <>
struct ly_scm_func_of_arity<2>
{
  typedef SCM (*ptr_type) (SCM, SCM);
};

template <>
struct ly_scm_func_of_arity<3>
{
  typedef SCM (*ptr_type) (SCM, SCM, SCM);
};

template <>
struct ly_scm_func_of_arity<4>
{
  typedef SCM (*ptr_type) (SCM, SCM, SCM, SCM);
};


/*
  Two main macros are provided in order to export C++ functions,
  making them accessible to Scheme: LY_DEFINE and MAKE_SCHEME_CALLBACK.
  LY_DEFINE is used like this:

    LY_DEFINE (ly_cxx_name,
               req, opt, var,
               (SCM arg1, SCM arg2, [...]),
               R"(
    The docstring.
               )")
    {
      [... function body, returning SCM ...]
    }

  For grob callbacks, you'd use:

    // declaration
    class The_interface
    {
      DECLARE_SCHEME_CALLBACK (cxx_name, (SCM smob, SCM other_arg1, ...);
      [...]
    };

    // definition
    MAKE_SCHEME_CALLBACK (The_interface, cxx_name, arg_count);
    SCM
    The_interface::cxx_name (SCM smob, SCM other_arg1, [...])
    {
      [function body, returning SCM ...]
    }

  The req, opt and var arguments of LY_DEFINE are argument counts, as
  specified in the Guile documentation (node "Primitive procedures").
  For MAKE_SCHEME_CALLBACK, the number of arguments is always
  constant.

  Docstrings should be written as C++11 raw strings for the sake of
  readability and editability.

  In both cases, not only is a C++ function defined (like the
  scm_something ones from Guile), but cxx_name is automatically
  translated into an appropriate Scheme name and a Scheme function is
  exported with this name.  Here are the main conversion rules to know
  about.  They mostly follow Guile's conventions.

  - For LY_DEFINE, the 'ly_' prefix becomes 'ly:',
  - The callback 'The_interface::the_callback' becomes 'ly:the-interface::the-callback',
  - All underscores become dashes,
  - Trailing '_p' is used for predicates and becomes a question mark '?',
  - Trailing '_x' indicates mutation and becomes an exclamation mark '!',
  - Conversion functions are called 'ly_a_2_b', which becomes 'ly:a->b',
  - Comparators use 'ly_xxx_less_p', which becomes 'ly:xxx<?'.
*/

#define MAKE_SCHEME_CALLBACK_WITH_OPTARGS(TYPE, FUNC, ARGCOUNT, OPTIONAL_COUNT, DOC) \
  SCM TYPE ::FUNC ## _proc;                                             \
  void                                                                  \
  TYPE ## _ ## FUNC ## _init_functions ()                               \
  {                                                                     \
    std::string id = mangle_cxx_identifier (#TYPE "::" #FUNC);          \
    /* assignment selects the SCM function even if it is overloaded */ \
    ly_scm_func_of_arity<ARGCOUNT>::ptr_type func = TYPE::FUNC; \
    TYPE ::FUNC ## _proc = scm_c_define_gsubr (id.c_str(),                      \
                                               (ARGCOUNT-OPTIONAL_COUNT), OPTIONAL_COUNT, 0,    \
                                               reinterpret_cast<scm_t_subr> (func)); \
    ly_add_function_documentation (TYPE :: FUNC ## _proc, id.c_str(), "", \
                                   DOC);                                \
    scm_c_export (id.c_str (), NULL);                                   \
  }                                                                     \
                                                                        \
  ADD_SCM_INIT_FUNC (TYPE ## _ ## FUNC ## _callback,                    \
                     TYPE ## _ ## FUNC ## _init_functions);

#define MAKE_DOCUMENTED_SCHEME_CALLBACK(TYPE, FUNC, ARGCOUNT, DOC)              \
  MAKE_SCHEME_CALLBACK_WITH_OPTARGS(TYPE, FUNC, ARGCOUNT, 0, DOC);

#define MAKE_SCHEME_CALLBACK(TYPE, FUNC, ARGCOUNT)                      \
  MAKE_SCHEME_CALLBACK_WITH_OPTARGS(TYPE,FUNC,ARGCOUNT, 0, "");

void ly_add_function_documentation (SCM proc, const char *fname, const char *varlist, const char *doc);

#define ADD_SCM_INIT_FUNC(name, func)           \
  class name ## _scm_initter                    \
  {                                             \
  public:                                       \
    name ## _scm_initter ()                     \
    {                                           \
      add_scm_init_func (func);                 \
    }                                           \
  }                                             \
    _ ## name ## _scm_initter;

/* end define */

#define LY_DEFINE_WITHOUT_DECL(INITPREFIX, FNAME, REQ, OPT, VAR,        \
                               ARGLIST, DOCSTRING)                      \
  SCM FNAME ## _proc;                                                   \
  void                                                                  \
  INITPREFIX ## init ()                                                 \
  {                                                                     \
    std::string scheme_name = mangle_cxx_identifier (#FNAME);           \
    FNAME ## _proc = scm_c_define_gsubr (scheme_name.c_str (), REQ, OPT, VAR,   \
                                         reinterpret_cast<scm_t_subr> (FNAME)); \
    ly_add_function_documentation (FNAME ## _proc, scheme_name.c_str (),        \
                                   #ARGLIST, DOCSTRING);                \
    scm_c_export (scheme_name.c_str (), NULL);                          \
  }                                                                     \
  ADD_SCM_INIT_FUNC (INITPREFIX ## init_unique_prefix, INITPREFIX ## init); \
  SCM                                                                   \
  FNAME ARGLIST

#define LY_DEFINE(FNAME, REQ, OPT, VAR, ARGLIST, DOCSTRING)             \
  SCM FNAME ARGLIST;                                                    \
  LY_DEFINE_WITHOUT_DECL (FNAME, FNAME, REQ, OPT, VAR, ARGLIST,         \
                          DOCSTRING)

#define LY_DEFINE_MEMBER_FUNCTION(CLASS, FNAME, REQ, OPT, VAR,          \
                                  ARGLIST, DOCSTRING)                   \
  LY_DEFINE_WITHOUT_DECL (CLASS ## FNAME, CLASS::FNAME, REQ, OPT,       \
                          VAR, ARGLIST, DOCSTRING)

/* LY_DEFINE_WITH_SETTER is like LY_DEFINE, but adding a setter procedure.
   Adding, for example, ly:music-set-property! as setter to ly:music-property
   means one can write
     (set! (ly:music-property m 'property) ...)
   equivalently to
     (ly:music-set-property! m 'property ...)
   For this to work, the setter must be defined before the getter, and
   within the same compilation unit (so the init function adding it is
   guaranteed to be defined first). */
#define LY_DEFINE_WITH_SETTER(FNAME, SETTERNAME,                                      \
                              REQ, OPT, VAR,                                          \
                              ARGLIST, DOCSTRING)                                     \
  SCM FNAME ARGLIST;                                                                  \
  SCM FNAME ## _proc_without_setter;                                                  \
  SCM FNAME ## _proc;                                                                 \
  void                                                                                \
  FNAME ## init ()                                                                    \
  {                                                                                   \
    std::string scheme_name = mangle_cxx_identifier (#FNAME);                         \
    FNAME ## _proc_without_setter =                                                   \
      scm_c_make_gsubr (scheme_name.c_str (), REQ, OPT, VAR,                          \
                          reinterpret_cast<scm_t_subr> (FNAME));                      \
    FNAME ## _proc =                                                                  \
      scm_make_procedure_with_setter (FNAME ## _proc_without_setter,                  \
                                      SETTERNAME ## _proc);                           \
    ly_add_function_documentation (FNAME ## _proc, scheme_name.c_str (),              \
                                   #ARGLIST, DOCSTRING);                              \
    scm_c_define (scheme_name.c_str (), FNAME ## _proc);                              \
    scm_c_export (scheme_name.c_str (), NULL);                                        \
  }                                                                                   \
  ADD_SCM_INIT_FUNC (FNAME ## init_unique_prefix, FNAME ## init);                     \
  SCM                                                                                 \
  FNAME ARGLIST


#define get_property(p,x) (p)->internal_get_property (ly_symbol2scm (x))
#define get_pure_property(p,x,y,z)                      \
  (p)->internal_get_pure_property (ly_symbol2scm (x), y, z)
#define get_maybe_pure_property(p,w,x,y,z)                      \
  (p)->internal_get_maybe_pure_property (ly_symbol2scm (w), x, y, z)
#define get_property_data(p,x) (p)->internal_get_property_data (ly_symbol2scm (x))
#define get_object(p,x) (p)->internal_get_object (ly_symbol2scm (x))
#define set_object(p, x, y) (p)->internal_set_object (ly_symbol2scm (x), y)
#define del_property(p,x) (p)->internal_del_property (ly_symbol2scm (x))

// Is the variable named x defined at p?  Does not check enclosing scopes.  An
// optional parameter receives the value if defined.  Returns bool.
#define here_defined(p, x, ...) \
  (p)->internal_here_defined (ly_symbol2scm (x), ##__VA_ARGS__)

// Where is the variable named x defined?  Checks enclosing scopes starting
// from p.  An optional parameter receives the value if defined.  Returns
// pointer.
#define where_defined(p, x, ...) \
  (p)->internal_where_defined (ly_symbol2scm (x), ##__VA_ARGS__)

#ifdef DEBUG
/*
  TODO: include modification callback support here, perhaps
  through intermediate Grob::instrumented_set_property( .. __LINE__ ).
 */
#define set_property(p, x, y) (p)->instrumented_set_property (ly_symbol2scm (x), y, __FILE__, __LINE__, __FUNCTION__)
#else
#define set_property(p, x, y) (p)->internal_set_property (ly_symbol2scm (x), y)
#endif

// Note: For Smobs, use LY_ASSERT_SMOB instead.
#define LY_ASSERT_TYPE(pred, var, number)                                       \
  {                                                                     \
    if (!pred (var)) \
      {                                                                 \
        scm_wrong_type_arg_msg(mangle_cxx_identifier (__FUNCTION__).c_str(), \
                               number, var, \
                               predicate_to_typename (pred).c_str());   \
      }                                                                 \
  }

template <class T>
std::string calc_smob_name ();

template <class T>
T *unsmob (SCM var);

// Do not call this directly.
// Use LY_ASSERT_SMOB() which supplies the function name automatically.
template <class T>
inline T *ly_assert_smob (SCM var, int number, const char *fun)
{
  if (auto *smob = unsmob<T> (var))
    return smob;

  scm_wrong_type_arg_msg (mangle_cxx_identifier (fun).c_str (),
                          number, var, calc_smob_name<T> ().c_str ());
}

#define LY_ASSERT_SMOB(klass, var, number)                              \
  ly_assert_smob<klass> (var, number, __FUNCTION__)

#endif /* LILY_GUILE_MACROS_HH */
