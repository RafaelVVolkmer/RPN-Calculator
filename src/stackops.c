 /** ===========================================================================
    @ingroup    StackOperation
    @addtogroup StackOperation stack_ops

    @package    stack_ops
    @brief      This module provides functionalities for stack operations,
                including managing operator and value stacks for an RPN calculator.

    @file       stackops.c
    @headerfile stackops.h

    @author     Rafael V. Volkmer (rafael.v.volkmer@gmail.com)
    @date       16.11.2024
 
    @details    The Stack Operations module manages operator and value stacks used 
                in a Reverse Polish Notation (RPN) calculator. 
                It provides functionalities to push and pop operators and numerical 
                values, check stack states, and peek at the top elements without 
                odifying the stacks.
     
    @note       - Ensure that the stacks are properly initialized before 
                  performing any push or pop operations.
                - The module is designed for single-threaded environments. 
                  For multi-threaded applications, synchronization mechanisms 
                  should be implemented to ensure thread safety.
     
    @see        - Stack_pushOp
                - Stack_popOp 
                - Stack_peekOp
                - Stack_isEmpty
                - Stack_pushVal
                - Stack_popVal
                - Stack_isEmptyVal
 =========================================================================== **/

/* ==================================== *\
 *             INCLUDED FILE            *
\* ==================================== */

/*< Dependencies >*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*< Implements >*/
#include <stackops.h>

/* ==================================== *\
 *            PRIVATE DEFINES           *
\* ==================================== */

/** ====================================
  @def      FUNCTION_SUCCESS
  @package  stack_ops
  @brief    Indicates successful function 
            execution.
     
  @details  Represents a successful 
            operation, typically with 
            a value of 0.
 ==================================== **/ 
#define FUNCTION_SUCCESS    (unsigned int)(0U)

/** ====================================
  @def      EMPTY_TOP
  @package  stack_ops
  @brief    Represents an empty stack 
            top index.
     
  @details  Used to indicate that the 
            stack is currently empty, 
            typically set to -1.
 ==================================== **/ 
#define EMPTY_TOP           (int)(-1)

/** ====================================
  @def      SIZE_OFFSET
  @package  stack_ops
  @brief    Offset value used for stack 
            operations.
     
  @details  Defines the offset value 
            of 1 used when pushing 
            elements onto the stack 
            to increment the top index.
 ==================================== **/ 
#define SIZE_OFFSET         (unsigned int)(1U)

/* ==================================== *\
 *     PRIVATE FUNCTIONS DEFINITION     *
\* ==================================== */

/** ============================================================================
  @fn       Stack_pushOp
  @package  stack_ops
  
  @brief    Pushes an operator onto the operator stack.
 
  @details  Adds a new operator to the top of the operator stack. 
            It first checks if the stack is not NULL and if there is space 
            available in the stack. If the stack is full or NULL, it returns 
            an appropriate error code.
 
  @param    stack    [in/out]:   Pointer to the operator stack structure.
  @param    value    [in]:       String representing the operator to push.
 
  @return   0 on success. 
            -ENOMEM if the stack is NULL. 
            -EINVAL if the stack is full..
 =========================================================================== **/
int Stack_pushOp(stack_op_t *stack, const char* value) 
{
    /*< Variable Declarations >*/
    int ret = FUNCTION_SUCCESS; /*< Return Control >*/

    /*< Security Checks >*/
    if(stack == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }

    if (stack->top >= (MAX_STACK_SIZE - SIZE_OFFSET)) 
    {
        ret = -(EINVAL);
        goto end_of_function;
    }

    /*< Start Function Algorithm >*/
    strcpy(stack->data[++(stack->top)], value);

    /*< Function Output >*/
end_of_function:
    return ret;
}

/** ============================================================================
  @fn       Stack_popOp
  @package  stack_ops
  
  @brief    Pops the top operator from the operator stack.
 
  @details  Removes and returns the top operator from the operator stack.
            It first checks if the stack is not NULL and not empty. 
            If the stack is empty or NULL, it returns NULL to indicate an error.
 
  @param    stack    [in/out]:   Pointer to the operator stack structure.
 
  @return   Pointer to the popped operator string on success. 
            NULL if the stack is empty or NULL.
 =========================================================================== **/
char* Stack_popOp(stack_op_t *stack) 
{
    /*< Variable Declarations >*/
    char *stack_data = NULL; /*< Return Control >*/

    /*< Security Checks >*/
    if (stack == NULL) 
    {
        stack_data = NULL;
        goto end_of_function;
    }

    if (stack->top == EMPTY_TOP) 
    {
        stack_data = NULL;
        goto end_of_function;
    }

    /*< Start Function Algorithm >*/
    stack_data = stack->data[(stack->top)--];

    /*< Function Output >*/
end_of_function:
    return stack_data;
}

/** ============================================================================
  @fn       Stack_peekOp
  @package  stack_ops
  
  @brief    Peeks at the top operator of the operator stack without removing it.
 
  @details  Retrieves the top operator from the operator stack without modifying 
            the stack. It first checks if the stack is not NULL and not empty. 
            If the stack is empty or NULL, it returns NULL to indicate an error.
 
  @param    stack    [in]:   Pointer to the operator stack structure.
 
  @return   Pointer to the top operator string on success. 
            NULL if the stack is empty or NULL.
 =========================================================================== **/
char* Stack_peekOp(stack_op_t *stack) 
{
    /*< Variable Declarations >*/
    char *stack_data = NULL; /*< Return Control >*/

    /*< Security Checks >*/
    if (stack == NULL) 
    {
        stack_data = NULL;
        goto end_of_function;
    }

    if (stack->top == EMPTY_TOP) 
    {
        stack_data = NULL;
        goto end_of_function;
    }

    /*< Start Function Algorithm >*/
    stack_data = stack->data[stack->top];

    /*< Function Output >*/
end_of_function:
    return stack_data;
}

/** ============================================================================
  @fn       Stack_isEmpty
  @package  stack_ops
  
  @brief    Checks if the operator stack is empty.
 
  @details  Determines whether the operator stack is empty or full. 
            It first checks if the stack is not NULL. If the stack is NULL, 
            it returns an error code. Otherwise, it returns STACK_EMPTY 
            if the stack is empty or STACK_FULL if it contains elements.
 
  @param    stack    [in]:   Pointer to the operator stack structure.
 
  @return   STACK_EMPTY if the stack is empty, STACK_FULL if it is not empty. 
            -ENOMEM if the stack pointer is NULL.
 =========================================================================== **/
int Stack_isEmpty(stack_op_t *stack) 
{
    /*< Variable Declarations >*/
    int ret = FUNCTION_SUCCESS; /*< Return Control >*/

    /*< Security Checks >*/
    if(stack == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }

    /*< Start Function Algorithm >*/
    ret = (stack->top == EMPTY_TOP) ? STACK_EMPTY : STACK_FULL;

    /*< Function Output >*/
end_of_function:
    return ret;
}

/** ============================================================================
  @fn       Stack_pushVal
  @package  stack_ops
  
  @brief    Pushes a numerical value onto the value stack.
 
  @details  Adds a new numerical value to the top of the value stack. 
            It first checks if the stack is not NULL and if there is space 
            available in the stack. If the stack is full or NULL, it returns 
            an appropriate error code.
 
  @param    stack_val   [in/out]:   Pointer to the value stack structure.
  @param    value       [in]:       Double value to push onto the stack.
 
  @return   0 on success. 
            -ENOMEM if the stack is NULL. 
            -EINVAL if the stack is full.
 =========================================================================== **/
int Stack_pushVal(stack_val_t *stack_val, double value) 
{
    /*< Variable Declarations >*/
    int ret = FUNCTION_SUCCESS; /*< Return Control >*/

    /*< Security Checks >*/
    if (stack_val == NULL) 
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }

    if (stack_val->top >= (MAX_STACK_SIZE - SIZE_OFFSET)) 
    {
        ret = -(EINVAL);
        goto end_of_function;
    }

    /*< Start Function Algorithm >*/
    stack_val->data[++(stack_val->top)] = value;

    /*< Function Output >*/
end_of_function:
    return ret;
}

/** ============================================================================
  @fn       Stack_popVal
  @package  stack_ops
  
  @brief    Pops the top numerical value from the value stack.
 
  @details  Removes and returns the top numerical value from the value stack.
            It first checks if the stack is not NULL and not empty. 
            If the stack is empty or NULL, it returns an error code.
 
  @param    stack_val   [in/out]:   Pointer to the value stack structure.
 
  @return   The popped double value on success. 
            -EINVAL if the stack is empty.
            -ENOMEM if the stack pointer is NULL.
 =========================================================================== **/
double Stack_popVal(stack_val_t *stack_val) 
{
    /*< Variable Declarations >*/
    double ret = FUNCTION_SUCCESS; /*< Return Control >*/

    /*< Security Checks >*/
    if (stack_val == NULL) 
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }

    if (stack_val->top == EMPTY_TOP) 
    {
        ret = -(EINVAL);
        goto end_of_function;
    }

    /*< Start Function Algorithm >*/
    ret = stack_val->data[(stack_val->top)--];

    /*< Function Output >*/
end_of_function:
    return ret;
}

/** ============================================================================
  @fn       Stack_isEmptyVal
  @package  stack_ops
  
  @brief    Checks if the value stack is empty.
 
  @details  Determines whether the value stack is empty or full. 
            It first checks if the stack is not NULL. If the stack is NULL, 
            it returns an error code. Otherwise, it returns STACK_EMPTY 
            if the stack is empty or STACK_FULL if it contains elements.
 
  @param    stack_val   [in]:   Pointer to the value stack structure.
 
  @return   STACK_EMPTY if the stack is empty, STACK_FULL if it is not empty. 
            -ENOMEM if the stack pointer is NULL.
 =========================================================================== **/
int Stack_isEmptyVal(stack_val_t *stack_val) 
{
    /*< Variable Declarations >*/
    int ret = FUNCTION_SUCCESS; /*< Return Control >*/

    /*< Security Checks >*/
    if(stack_val == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }

    /*< Start Function Algorithm >*/
    ret = (stack_val->top == EMPTY_TOP) ? STACK_EMPTY : STACK_FULL;

    /*< Function Output >*/
end_of_function:
    return ret;
}

/*< end of file >*/
