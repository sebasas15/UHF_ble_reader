/*
 * default_args.h
 *
 *  Created on: 14-09-2017
 *      Author: sebasas15
 */

#ifndef READER_DEFAULT_ARGS_H_
#define READER_DEFAULT_ARGS_H_

#
#define PP_FUNC_GEN(a,b) PP_FUNC_GEN_I(a,b)
#define PP_FUNC_GEN_I(a,b) a ## b



#define PP_NARG(...) PP_NARG_FROM_1(__VA_ARGS__)

#define PP_NARG_FROM_1(...) CONCAT_NARG(RETORNO_NARG_,ISEMPTY(__VA_ARGS__),PP_NARG_(__VA_ARGS__,PP_RSEQ_N(__VA_ARGS__)))

#define CONCAT_NARG(a,b,c)  CONCAT_NARG_(a,b,c)
#define CONCAT_NARG_(a,b,c) a ## b ## (c)
#define RETORNO_NARG_0(a) a
#define RETORNO_NARG_1(a) 0
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( _1, _2, _3, _4,_5,N,...)  N
#
#define PP_COMMA(...) PP_NARG_(__VA_ARGS__,COMMA_CK(__VA_ARGS__))
#define PP_COMMA_(...) PP_NARG_(__VA_ARGS__,COMMA_CK(__VA_ARGS__))
#define PP_RSEQ_N(...) 5,4,3,2,1
#define COMMA_CK(...) 1,1,1,1,0
#
#define _TRIGGER_PARENTHESIS_(...) ,

#define ISEMPTY(...)                                                    \
_ISEMPTY(                                                               \
          /* test if there is just one argument, eventually an empty    \
             one */                                                     \
          PP_COMMA(__VA_ARGS__),                                       \
          /* test if _TRIGGER_PARENTHESIS_ together with the argument   \
             adds a comma */                                            \
          PP_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__),                 \
          /* test if the argument together with a parenthesis           \
             adds a comma */                                            \
          PP_COMMA(__VA_ARGS__ (/*empty*/)),                           \
          /* test if placing it between _TRIGGER_PARENTHESIS_ and the   \
             parenthesis adds a comma */                                \
          PP_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__ (/*empty*/))      \
          )

#define PASTE5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define _ISEMPTY(_0, _1, _2, _3) PP_COMMA(PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))
#define _IS_EMPTY_CASE_0001 ,
#
#
#
#
#
#
#
#
#
#
#
#




#endif /* READER_DEFAULT_ARGS_H_ */
