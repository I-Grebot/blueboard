/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2017
 * -----------------------------------------------------------------------------
 * @file       shell_variables.c
 * @author     Bebop35 [Paul M]
 * @date       Dec 25, 2016
 * -----------------------------------------------------------------------------
 * @brief
 *   Shell variables manipulation functions
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/firm_blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "main.h"

/* Variables definition list holder */
extern const OS_SHL_VarItemTypeDef OS_SHL_varList[];
extern const size_t OS_SHL_varListLength;

/*
 * Converts a type into a printable string
 */
const char* OS_SHL_GetTypeAsString(const OS_SHL_VarTypeEnum type)
{
    switch(type)
    {
        case TYPE_BOOL:     return "bool";
        case TYPE_CHAR:     return "char";
        case TYPE_UINT8:    return "uint8";
        case TYPE_INT8:     return "int8";
        case TYPE_UINT16:   return "uint16";
        case TYPE_INT16:    return "int16";
        case TYPE_UINT32:   return "uint32";
        case TYPE_INT32:    return "int32";
        case TYPE_UINT64:   return "uint64";
        case TYPE_INT64:    return "int64";
        case TYPE_FLOAT:    return "float";
        case TYPE_DOUBLE:   return "double";
        default:            return "NC"; // Error
    }
}

/*
 * Returns the size of a type. 0 if the type is not known
 */
size_t OS_SHL_GetTypeSize(const OS_SHL_VarTypeEnum type)
{
    switch(type)
    {
        case TYPE_BOOL:     return sizeof(bool);
        case TYPE_CHAR:     return sizeof(char);
        case TYPE_UINT8:    return sizeof(uint8_t);
        case TYPE_INT8:     return sizeof(int8_t);
        case TYPE_UINT16:   return sizeof(uint16_t);
        case TYPE_INT16:    return sizeof(int16_t);
        case TYPE_UINT32:   return sizeof(uint32_t);
        case TYPE_INT32:    return sizeof(int32_t);
        case TYPE_UINT64:   return sizeof(uint64_t);
        case TYPE_INT64:    return sizeof(int64_t);
        case TYPE_FLOAT:    return sizeof(float);
        case TYPE_DOUBLE:   return sizeof(double);
        default:            return 0; // Error
    }
}

/*
 * Converts an access type into a printable string
 */
const char* OS_SHL_GetAccessAsString(const OS_SHL_VarAccessEnum acc)
{
    switch(acc)
    {
        case ACC_RD: return "RD";
        case ACC_WR: return "WR";
        case ACC_RQ: return "RQ";
        default:     return "NC"; // Error
    }
}

/*
 * Print in the buffer string the list of every variable that are accessible
 */
BaseType_t OS_SHL_GetVariablesList(char* ret, size_t retLength)
{
    const char* header = "      ID. Type     Acc.  Name                                     Unit            Value"SHELL_EOL
                         "      ------------------------------------------------------------------------------------"SHELL_EOL;

    /* Local handlers for outputting variables one at a time */
    static const OS_SHL_VarItemTypeDef* var = NULL;
    static size_t id;

    if(var == NULL)
    {
        var = OS_SHL_varList;
        id = 0;

        /* Header */
        strcpy(ret, header);
        ret += strlen(ret);
    }

    /* Display the variable */
    snprintf(ret, retLength, SHELL_VAR_PFX"%3u %-8s %-5s %-40s %-16s",
            id,
            OS_SHL_GetTypeAsString(var->type),
            OS_SHL_GetAccessAsString(var->access),
            var->name,
            var->unit);
    ret += strlen(ret);
    OS_SHL_GetVariable(var, ret, retLength);
    ret += strlen(ret);
    snprintf(ret, retLength, SHELL_EOL);

    /* There are still items to process, move to the next one */
    id++;
    if(id < OS_SHL_varListLength)
    {
        var++;
        return pdTRUE;

    /* We have finished */
    } else {
        var = NULL;
        id = 0;
        return pdFALSE;
    }

}

/*
 * Lookup a variable by its name.
 * Returns pdFALSE if not found.
 * pdTRUE is returned otherwise and var is set to the found variable structure address.
 */
BaseType_t OS_SHL_FindVariableByName(char* name, const OS_SHL_VarItemTypeDef** var)
{
    OS_SHL_VarItemTypeDef* lookup = OS_SHL_varList;
    size_t id = 0;

    do {
        /* Match! */
        if(strcasecmp(name, lookup->name) == 0) {
            *var = lookup;
            return pdTRUE;
        }

        /* Next */
        id++;
        lookup++;

    } while(id < OS_SHL_varListLength);

    /* Not found */
    return pdFALSE;
}

/*
 * Fast lookup of a variable by its index
 */
BaseType_t OS_SHL_FindVariableById(size_t id, const OS_SHL_VarItemTypeDef** var)
{
    if(id < OS_SHL_varListLength)
    {
        *var = OS_SHL_varList + id;
        return pdTRUE;
    } else {
        return pdFALSE;
    }
}

/*
 * Set a variable value based on the input string
 */
BaseType_t OS_SHL_SetVariable(OS_SHL_VarItemTypeDef const* var, char* value)
{
    size_t size;
    int32_t valueTmpInt;
    int64_t valueTmpLongInt;
    float valueTmpFloat;
    double valueTmpDouble;
    void* valueTmp;

    if((size = OS_SHL_GetTypeSize(var->type)) == 0)
        return pdFALSE; // Size Error

    if(var->access == ACC_RD)
        return pdFALSE; // Read-Only error

    // Depends on the type
    switch(var->type)
    {
        case TYPE_BOOL:
        case TYPE_CHAR:
        case TYPE_UINT8:
        case TYPE_INT8:
        case TYPE_UINT16:
        case TYPE_INT16:
        case TYPE_UINT32:
        case TYPE_INT32:
            valueTmpInt = strtol(value, NULL, 10);
            valueTmp = &valueTmpInt;
            break;

        case TYPE_INT64:
        case TYPE_UINT64:
            valueTmpLongInt = strtoll(value, NULL, 10);
            valueTmp = &valueTmpLongInt;
            break;

        case TYPE_FLOAT:
            valueTmpFloat = strtof(value, NULL);
            valueTmp = &valueTmpFloat;
            break;

        case TYPE_DOUBLE:
            valueTmpDouble = strtod(value, NULL);
            valueTmp = &valueTmpDouble;
            break;

        // Type Error
        default:
            return pdFALSE;
    }

    // Actual copy
    memcpy(var->var, valueTmp, size);

    return pdTRUE;
}


/*
 * Decode a variable path/name and write its current value
 * into the result string.
 */
BaseType_t OS_SHL_GetVariable(OS_SHL_VarItemTypeDef const* var, char* ret, size_t retLength)
{
    BaseType_t retValue = pdTRUE;
    size_t size;
    void* value;


    if((size = OS_SHL_GetTypeSize(var->type)) == 0)
        return pdFALSE; // Size Error

    if((value = malloc(size)) == NULL){} // FIXME: Apparently always returns NULL...
        //return pdFALSE; // Malloc Error

    memcpy(value, var->var, size);

    // Depends on the type
    switch(var->type)
    {
        case TYPE_BOOL:     snprintf(ret, retLength, "%u",   *((bool*) value)); break;
        case TYPE_CHAR:     snprintf(ret, retLength, "%c",   *((char*) value)); break;
        case TYPE_UINT8:    snprintf(ret, retLength, "%u",   *((uint8_t*) value)); break;
        case TYPE_INT8:     snprintf(ret, retLength, "%d",   *((int8_t*) value)); break;
        case TYPE_UINT16:   snprintf(ret, retLength, "%u",   *((uint16_t*) value)); break;
        case TYPE_INT16:    snprintf(ret, retLength, "%d",   *((int16_t*) value)); break;
        case TYPE_UINT32:   snprintf(ret, retLength, "%lu",  *((uint32_t*) value)); break;
        case TYPE_INT32:    snprintf(ret, retLength, "%ld",  *((int32_t*) value)); break;
        case TYPE_UINT64:   snprintf(ret, retLength, "%llu", *((uint64_t*) value)); break;
        case TYPE_INT64:    snprintf(ret, retLength, "%lld", *((int64_t*) value)); break;
        case TYPE_FLOAT:    snprintf(ret, retLength, "%f",   *((float*) value)); break;     // FIXME
        case TYPE_DOUBLE:   snprintf(ret, retLength, "%lf",  *((double*) value)); break;    // FIXME
        default:            retValue = pdFALSE; break; // Type Error
    }

    free(value);
    return retValue;
}
