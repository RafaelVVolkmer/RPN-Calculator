
/** ===========================================================================
    @addtogroup RPNCalculator
    @addtogroup RPNCalculator_Module RPN_calculator

    @package    RPN_calculator
    @brief      This module provides functionalities for parsing, converting,
                and evaluating mathematical expressions using Reverse Polish
                Notation (RPN).

    @file       RPNCalculator.c

    @author     
    @date       16.11.2024

    @details    The RPN Calculator module allows users to input mathematical
                expressions in infix notation, which are then tokenized,
                converted to postfix (RPN) notation, and evaluated to produce
                a numerical result. It supports various mathematical functions,
                operators, and handles operator precedence and associativity.

    @note       - Ensure that the input expressions are properly formatted to
                  avoid tokenization errors.
                - The module relies on the Stack Operations module (`stack_ops`)
                  for managing operator and value stacks during conversion and
                  evaluation.

    @see        - RPNCalculator_tokenize
                - RPNCalculator_infixToPostfix
                - RPNCalculator_evaluatePostfix
                - RPNCalculator_whichOperator
                - RPNCalculator_whichFunction
                - RPNCalculator_checkPrecedence
                - RPNCalculator_isRightAssociative
 =========================================================================== **/

#ifndef RPNCALCULATOR_H_
#define RPNCALCULATOR_H_

/* ==================================== *\
 *           PUBLIC DEFINES             *
\* ==================================== */

/** ====================================
  @def      MAX_EXPRESSION_SIZE
  @package  RPN_calculator
  @brief    Defines the maximum size of 
            an expression.

  @details  Sets the maximum number of 
            characters allowed in an 
            input expression to prevent 
            buffer overflows.
 ==================================== **/
#define MAX_EXPRESSION_SIZE     (unsigned int)(1000U)

/** ====================================
  @def      MAX_NUM_TOKENS
  @package  RPN_calculator
  @brief    Defines the maximum number 
            of tokens in an expression.

  @details  Sets the maximum number of 
            tokens that can be processed 
            in an expression to prevent 
            buffer overflows.
 ==================================== **/
#define MAX_NUM_TOKENS          (unsigned int)(1000U)

/** ====================================
  @def      MAX_TOKEN_LEN
  @package  stack_ops
  @brief    Defines the maximum length 
            of each token in the operator 
            stack.
     
  @details  Sets the upper limit for 
            the length of operator tokens 
            stored in the stack to prevent 
            buffer overflows.
 ==================================== **/ 
#define MAX_TOKEN_LEN           (unsigned int)(64U)

/* ==================================== *\
 *     PUBLIC FUNCTIONS PROTOTYPES      *
\* ==================================== */

/** ============================================================================
  @fn       RPNCalculator_whichOperator
  @package  RPN_calculator
  
  @brief    Identifies the operator index from a given token.
 
  @details  Compares the input token against a predefined list of operators.
            If a match is found, returns the corresponding operator index.
            If no match is found or if the token is NULL, returns an error code.
 
  @param    token    [in]:   String representing the operator to identify.
 
  @return   Operator index on success. 
            -ENOMEM if token is NULL.
            -EINVAL if the operator is not recognized.
 =========================================================================== **/
int RPNCalculator_whichOperator(const char* token);

/** ============================================================================
  @fn       RPNCalculator_whichFunction
  @package  RPN_calculator
  
  @brief    Identifies the function index from a given token.
 
  @details  Compares the input token against a predefined list of functions.
            If a match is found, returns the corresponding function index.
            If no match is found or if the token is NULL, returns an error code.
 
  @param    token    [in]:   String representing the function to identify.
 
  @return   Function index on success. 
            -ENOMEM if token is NULL.
            -EINVAL if the function is not recognized.
 =========================================================================== **/
int RPNCalculator_whichFunction(const char* token) ;

/** ============================================================================
  @fn       RPNCalculator_checkPrecedence
  @package  RPN_calculator
  
  @brief    Determines the precedence level of a given token.
 
  @details  Evaluates whether the token is a function or an operator and assigns
            the appropriate precedence level based on predefined rules. 
            Returns an error code if the token is neither a function nor a 
            recognized operator.
 
  @param    token    [in]:   String representing the token whose precedence is to 
                             be determined.
 
  @return   Precedence level on success.
            -ENOMEM if token is NULL.
            -EINVAL if the function is not recognized.
 =========================================================================== **/
int RPNCalculator_checkPrecedence(const char* token);

/** ============================================================================
  @fn       RPNCalculator_isRightAssociative
  @package  RPN_calculator
  
  @brief    Checks if a given operator is right-associative.
 
  @details  Determines the associativity of an operator. Currently, the power 
            ('^') and factorial ('!') operators are considered right-associative. 
            All other operators are left-associative. Returns an error code if 
            the token is NULL.
 
  @param    token    [in]:   String representing the operator to check.
 
  @return   RIGHT_ASSOCIATIVE or LEFT_ASSOCIATIVE on success, 
            -ENOMEM if token is NULL.
            -EINVAL if the function is not recognized.
 =========================================================================== **/
int RPNCalculator_isRightAssociative(const char* token);

/** ============================================================================
  @fn       RPNCalculator_tokenize
  @package  RPN_calculator
  
  @brief    Tokenizes an input mathematical expression into individual tokens.
 
  @details  Processes the input expression string, identifying and separating
            numbers, functions, operators, and parentheses into distinct tokens.
            Ensures that the number of tokens does not exceed the predefined 
            maximum.
            Returns the total number of tokens on success or an error code on 
            failure.
 
  @param    expression   [in]:   String representing the mathematical expression 
                                 to tokenize.
  @param    tokens       [out]:  Array to store the extracted tokens.
 
  @return   Number of tokens on success,
            -ENOMEM if token is NULL.
            -EINVAL if the function is not recognized.
 =========================================================================== **/
int RPNCalculator_tokenize(const char* expression, char tokens[][MAX_TOKEN_LEN]);

/** ============================================================================
  @fn       RPNCalculator_infixToPostfix
  @package  RPN_calculator

  @brief    Converts an infix expression to postfix (RPN) notation.

  @details  Implements the Shunting Yard algorithm to convert an infix expression
            represented by tokens into a postfix expression, considering operator
            precedence and associativity.

  @param    tokens    [in]:  Array of strings representing the infix expression 
                             tokens.
  @param    output    [out]: Array to store the postfix expression tokens.
  @param    number    [in]:  Number of tokens in the infix expression.

  @return   Number of tokens in the postfix expression on success.
            -ENOMEM if tokens or output is NULL.
            -EINVAL if there is an error in the expression.
 =========================================================================== **/
int RPNCalculator_infixToPostfix(char tokens[][MAX_TOKEN_LEN], char output[][MAX_TOKEN_LEN], int number);

/** ============================================================================
  @fn       RPNCalculator_factorialCalculate
  @package  RPN_calculator

  @brief    Calculates the factorial of a given number.

  @details  Computes the factorial of a non-negative integer using recursion.
            Returns 1 for factorial of 0 or 1.

  @param    number    [in]:  The number to calculate the factorial of.

  @return   The factorial of the number as a double.
 =========================================================================== **/
double RPNCalculator_factorialCalculate(unsigned int number);

/** ============================================================================
  @fn       RPNCalculator_applyOperation
  @package  RPN_calculator

  @brief    Applies an arithmetic operation to two operands.

  @details  Determines the operation to perform based on the operator token and
            applies it to the provided operands. Supports addition, subtraction,
            multiplication, division, and exponentiation.

  @param    operation    [in]:  String representing the operator.
  @param    num_a        [in]:  The first operand.
  @param    num_b        [in]:  The second operand.

  @return   The result of the operation as a double.
            Returns NAN if the operation is invalid (e.g., division by zero).
 =========================================================================== **/
double RPNCalculator_applyOperation(const char* operation, double num_a, double num_b);

/** ============================================================================
  @fn       RPNCalculator_applyFunction
  @package  RPN_calculator

  @brief    Applies a mathematical function to an operand.

  @details  Determines the mathematical function to perform based on the function token
            and applies it to the provided operand. Supports various trigonometric,
            logarithmic, and other mathematical functions.

  @param    function    [in]:  String representing the function.
  @param    number      [in]:  The operand to apply the function to.

  @return   The result of the function as a double.
            Returns NAN if the function is invalid.
 =========================================================================== **/
double RPNCalculator_applyFunction(const char* function, double number);

/** ============================================================================
  @fn       RPNCalculator_evaluatePostfix
  @package  RPN_calculator

  @brief    Evaluates a postfix (RPN) expression.

  @details  Processes the postfix expression tokens, using a stack to store 
            operands. Applies operations and functions as they are encountered.

  @param    output    [in]:  Array of strings representing the tokens of the 
                             postfix expression.
  @param    number    [in]:  Number of tokens in the postfix expression.

  @return   The result of the evaluation as a double.
            Returns NAN in case of an error.
 =========================================================================== **/
double RPNCalculator_evaluatePostfix(char output[][MAX_TOKEN_LEN], int number);

#endif /* RPNCALCULATOR_H_ */

/*< end of header file >*/
