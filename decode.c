#include "postgres.h"
#include <string.h>
#include <stdio.h>
#include "fmgr.h"
#include "parser/parse_oper.h"
/*
 *Author:   songshaohua
 *Description: achieving decode functin in PostgreSQL
 *Version:     V1.0
 *
 */
//标记数据库版本信息,放置动态库被加载到不兼容的服务器
#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

/*
 *declare function name;
 */
PG_FUNCTION_INFO_V1(decode);

/*
 *define get opertype function
 */
Oid
equal_oper_func_id(Oid argType)
{
	Oid eqOper;
	//get_sort_group_operators function include  parse_oper.h header file.
	get_sort_group_operators(argType,false,true,false,NULL,&eqOper,NULL,NULL);
	return get_opcode(eqOper);
}

/*
 * decode(value,if,then,if,then,default)
 */


Datum
decode(PG_FUNCTION_ARGS)
{
	//define reference arguement count
	int countArgs;
	//define result index value
	int result;
	int i;
	
	//get reference argument count
	countArgs = PG_NARGS();
	if (countArgs % 2 == 0)
	{
		result = countArgs - 1;
		countArgs -= -1;
	}else
	{
		result = -1; // the last arguement can allow does not exists
	}
	//The following is detail step
	if (PG_ARGISNULL(0))
	{
		for (i = 1;i < countArgs; i += 2)
		{
			if (PG_ARGISNULL(i))
			{
				result = i + 1;
				break;
			}
		}
	}else
	{
		/*
		 *FmgrInfo 为在fmgr.h 中定义的一个结构体
		 *表示可以通过fmgr调用函数之前必须先查找的系统目录信息
		 *如果要多次调用同一个函数，则只需要查找一次，并将信息结构保存以供重用
		 *
		 */
		FmgrInfo *eqOper;
		Oid  collation = PG_GET_COLLATION();
		if (fcinfo->flinfo->fn_extra == NULL)
		{
			MemoryContext oldContext;	//MemoryContext 定义在fmgr.h 头文件中，内容存储到fn_extra中
			Oid 	typeid		= get_fn_expr_argtype(fcinfo->flinfo,0);
			//get operate type oid
			Oid 	eqoperid	= equal_oper_func_id(typeid);
			/*
			 * MemoryContextSwitchTo 函数定义在
			 * executor.h 头文件中
			 */
			
			oldContext = MemoryContextSwitchTo(fcinfo->flinfo->fn_mcxt);
			//eqOper 的大小为FmgrInfo中申请内存的大小
			eqOper = palloc(sizeof(FmgrInfo));
			fmgr_info(eqoperid,eqOper);
			MemoryContextSwitchTo(oldContext);
			fcinfo->flinfo->fn_extra = eqOper;
		}else
		{
			eqOper = fcinfo->flinfo->fn_extra;
		}
		
		for (i = 1;i < countArgs;i += 2)
		{
			Datum resultText;
			if (PG_ARGISNULL(i))
			{
				continue;
			}
			resultText = FunctionCall2Coll(eqOper,collation,PG_GETARG_DATUM(0),PG_GETARG_DATUM(i));
			if (DatumGetBool(resultText))
			{
				result = i + 1;
				break;
			}
		}
	}
	
	if (result < 0 || PG_ARGISNULL(result))
	{
		PG_RETURN_NULL();
	}else
	{
		PG_RETURN_DATUM(PG_GETARG_DATUM(result));
	}
	
}

