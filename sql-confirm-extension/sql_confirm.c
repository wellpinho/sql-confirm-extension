
### `sql_confirm.c`
```c
#include "postgres.h"
#include "fmgr.h"
#include "tcop/utility.h"
#include "nodes/nodes.h"
#include "nodes/parsenodes.h"
#include "utils/guc.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

static ProcessUtility_hook_type prev_ProcessUtility = NULL;

static void sql_confirm_ProcessUtility(PlannedStmt *pstmt,
                                       const char *queryString,
                                       bool readOnlyTree,
                                       ProcessUtilityContext context,
                                       ParamListInfo params,
                                       QueryEnvironment *env,
                                       DestReceiver *dest,
                                       QueryCompletion *qc)
{
    Node *parsetree = pstmt->utilityStmt;
    bool is_update_or_delete = false;
    bool has_where = true;

    if (parsetree != NULL)
    {
        switch (nodeTag(parsetree))
        {
            case T_DeleteStmt:
            {
                DeleteStmt *stmt = (DeleteStmt *) parsetree;
                is_update_or_delete = true;
                has_where = (stmt->whereClause != NULL);
                break;
            }
            case T_UpdateStmt:
            {
                UpdateStmt *stmt = (UpdateStmt *) parsetree;
                is_update_or_delete = true;
                has_where = (stmt->whereClause != NULL);
                break;
            }
            default:
                break;
        }
    }

    if (is_update_or_delete && !has_where)
    {
        bool confirmed = false;
        if (queryString && strstr(queryString, "CONFIRM") != NULL)
            confirmed = true;

        if (!confirmed)
            ereport(ERROR,
                    (errcode(ERRCODE_INSUFFICIENT_PRIVILEGE),
                     errmsg("Operation blocked: %s without WHERE requires CONFIRM",
                            (nodeTag(parsetree) == T_DeleteStmt) ? "DELETE" : "UPDATE"),
                     errhint("Append /* CONFIRM */ to the statement or add a WHERE clause.")));
    }

    if (prev_ProcessUtility)
        prev_ProcessUtility(pstmt, queryString, readOnlyTree, context, params, env, dest, qc);
    else
        standard_ProcessUtility(pstmt, queryString, readOnlyTree, context, params, env, dest, qc);
}

void _PG_init(void)
{
    prev_ProcessUtility = ProcessUtility_hook;
    ProcessUtility_hook = sql_confirm_ProcessUtility;
}

void _PG_fini(void)
{
    ProcessUtility_hook = prev_ProcessUtility;
}
