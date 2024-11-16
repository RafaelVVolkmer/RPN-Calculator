 /** ============================================================================
    @addtogroup StackOperation
    @addtogroup StackOperation stack_ops

    @package    stack_ops
    @brief      This module provides functionalities for stack operations,
                including managing operator and value stacks for an RPN calculator.

    @file       stackops.h

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

#ifndef STACKOPS_H_
#define STACKOPS_H_

/* ==================================== *\
 *           PUBLIC DEFINES             *
\* ==================================== */

/** ====================================
  @def      MAX_STACK_SIZE
  @package  stack_ops
  @brief    Defines the maximum number 
            of elements a stack can hold.
     
  @details  Sets the upper limit for 
            the number of elements that 
            can be stored in either the 
            operator or value stack.
 ==================================== **/ 
#define MAX_STACK_SIZE      (unsigned int)(1000U)

/** ====================================
  @def      MAX_TOKEN_LENGTH
  @package  stack_ops
  @brief    Defines the maximum length 
            of each token in the operator 
            stack.
     
  @details  Sets the upper limit for 
            the length of operator tokens 
            stored in the stack to prevent 
            buffer overflows.
 ==================================== **/ 
#define MAX_TOKEN_LENGTH    (unsigned int)(64U)

/* ==================================== *\
 *       PUBLIC TYPES DEFINITION        *
\* ==================================== */

/** ============================================================================
  @enum     stackStates
  @package  stack_ops

  @typedef  stack_states_t

  @brief    Defines the possible states of the operator stack.

  @details  Enumerates the states indicating whether the operator stack is full or empty.
============================================================================ **/
typedef enum stackStates
{
    STACK_FULL,                                     /*< Indicates that the operator stack is full >*/
    STACK_EMPTY                                     /*< Indicates that the operator stack is empty >*/
} stack_states_t;

/** ============================================================================
  @struct   stack_op_t
  @package  stack_ops

  @typedef  stack_op_t

  @brief    Represents the operator stack structure.

  @details  Contains a two-dimensional array to store operator tokens and an integer 
            to keep track of the top index of the stack.
============================================================================ **/
typedef struct 
{
    char    data[MAX_STACK_SIZE][MAX_TOKEN_LENGTH];  /*< Array to store operator tokens >*/
    int     top;                                     /*< Index of the top element in the stack >*/
} stack_op_t;

/** ============================================================================
  @struct   stack_val_t
  @package  stack_ops

  @typedef  stack_val_t

  @brief    Represents the value stack structure.

  @details  Contains an array to store numerical values and an integer to keep track 
            of the top index of the stack.
============================================================================ **/
typedef struct 
{
    double  data[MAX_STACK_SIZE];                   /*< Array to store numerical values >*/
    int     top;                                    /*< Index of the top element in the stack >*/
} stack_val_t;

/* ==================================== *\
 *     PUBLIC FUNCTIONS PROTOTYPES      *
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
            -EINVAL if the stack is full.
 =========================================================================== **/
int Stack_pushOp (stack_op_t *stack, const char* value);

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
char* Stack_popOp (stack_op_t *stack);

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
char* Stack_peekOp (stack_op_t *stack);

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
int Stack_isEmpty (stack_op_t *stack);

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
int Stack_pushVal (stack_val_t *stack_val, double value);

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
double Stack_popVal (stack_val_t *stack_val);

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
int Stack_isEmptyVal (stack_val_t *stack_val);

#endif /* STACKOPS_H_ */

/*< end of header file >*/
