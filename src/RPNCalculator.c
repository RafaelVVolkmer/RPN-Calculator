/** ===========================================================================
    @ingroup    RPNCalculator
    @addtogroup RPNCalculator_Module RPN_calculator

    @package    RPN_calculator
    @brief      This module provides functionalities for parsing, converting,
                and evaluating mathematical expressions using Reverse Polish
                Notation (RPN).

    @file       RPNCalculator.c
    @headerfile RPNCalculator.h

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
 
/* ==================================== *\
 *             INCLUDED FILE            *
\* ==================================== */

/*< Dependencies >*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>

/*< Implements >*/
#include <stackops.h>
#include <RPNcalculator.h>

/* ==================================== *\
 *            PRIVATE DEFINES           *
\* ==================================== */

/** ====================================
  @def      FUNCTION_SUCCESS
  @package  RPN_calculator
  @brief    Indicates successful function 
            execution.
     
  @details  Represents a successful 
            operation, typically with 
            a value of 0.
 ==================================== **/ 
#define FUNCTION_SUCCESS         (unsigned int)(0u)

/** ====================================
  @def      EMPTY_TOP
  @package  RPN_calculator
  @brief    Represents an empty stack 
            top index.
     
  @details  Used to indicate that the 
            stack is currently empty, 
            typically set to -1.
 ==================================== **/ 
#define EMPTY_TOP               (int)(-1)

/* ==================================== *\
 *       PRIVATE TYPES DEFINITION       *
\* ==================================== */

/** ============================================================================
  @enum     operationPrecedences
  @package  RPN_calculator

  @typedef  ops_precedence_t

  @brief    Defines the precedence levels for operations.

  @details  Enumerates the precedence levels assigned to operators and functions,
            with `PRECEDENCE_1` being the highest and `PRECEDENCE_5` the lowest.
 =========================================================================== **/
typedef enum operationPrecedences
{
    PRECEDENCE_1, /*< Highest precedence >*/
    PRECEDENCE_2,
    PRECEDENCE_3,
    PRECEDENCE_4,
    PRECEDENCE_5, /*< Lowest precedence >*/
    LAST_PRECEDENCE
} ops_precedence_t;

/** ============================================================================
  @enum     funcIndex
  @package  RPN_calculator

  @typedef  func_index_t

  @brief    Defines indices for mathematical functions.

  @details  Enumerates the indices corresponding to supported mathematical 
            functions, used for function identification and lookup.
 =========================================================================== **/
typedef enum funcIndex
{
    FUNC_SQRT,     /*< Square root function >*/
    FUNC_LOG,      /*< Logarithm base 10 function >*/
    FUNC_LN,       /*< Natural logarithm function >*/
    FUNC_SIN,      /*< Sine function >*/
    FUNC_COS,      /*< Cosine function >*/
    FUNC_TAN,      /*< Tangent function >*/
    FUNC_COSH,     /*< Hyperbolic cosine function >*/
    FUNC_SINH,     /*< Hyperbolic sine function >*/
    FUNC_TANH,     /*< Hyperbolic tangent function >*/
    FUNC_ASIN,     /*< Inverse sine function >*/
    FUNC_ACOS,     /*< Inverse cosine function >*/
    FUNC_ATAN,     /*< Inverse tangent function >*/
    FUNC_ARCSIN,   /*< Alternate inverse sine function >*/
    FUNC_ARCCOS,   /*< Alternate inverse cosine function >*/
    FUNC_ARCTAN,   /*< Alternate inverse tangent function >*/
    FUNC_COUNT     /*< Total number of functions >*/
} func_index_t;

/** ============================================================================
  @enum     operatorIndex
  @package  RPN_calculator

  @typedef  operator_index_t

  @brief    Defines indices for operators.

  @details  Enumerates the indices corresponding to supported operators,
            used for operator identification and lookup.
 =========================================================================== **/
typedef enum operatorIndex
{
    OP_ADD,    /*< Addition operator '+' >*/
    OP_SUB,    /*< Subtraction operator '-' >*/
    OP_MUL,    /*< Multiplication operator '*' >*/
    OP_DIV,    /*< Division operator '/' >*/
    OP_POW,    /*< Exponentiation operator '^' >*/
    OP_FACT,   /*< Factorial operator '!' >*/
    OP_COUNT   /*< Total number of operators >*/
} operator_index_t;

/** ============================================================================
  @enum     rigthLeftAssociative
  @package  RPN_calculator

  @typedef  associative_t

  @brief    Defines operator associativity types.

  @details  Enumerates whether an operator is left-associative or 
            right-associative, used in parsing expressions.
 =========================================================================== **/
typedef enum rightLeftAssociative
{
    LEFT_ASSOCIATIVE,   /*< Left-associative operator >*/
    RIGHT_ASSOCIATIVE,  /*< Right-associative operator >*/
    ASSOCIATIVE_COUNT
} associative_t;

/** ============================================================================
  @enum     instantValue
  @package  RPN_calculator

  @typedef  instant_val_t

  @brief    Defines indices for character positions in tokens.

  @details  Enumerates indices used to access characters in tokens, such as 
            the firstband second characters, for parsing purposes.
 =========================================================================== **/
typedef enum instantValue
{
    FIRST_VALUE,  /*< Index of the first character >*/
    SECOND_VALUE  /*< Index of the second character >*/
} instant_val_t;

/** ============================================================================
  @enum     bracketsCount
  @package  RPN_calculator

  @typedef  brackets_t

  @brief    Defines types of brackets used in expressions.

  @details  Enumerates the different types of brackets (parentheses, brackets, 
            braces) used for grouping in expressions.
 =========================================================================== **/
typedef enum bracketsCount
{
    PARENTHESES,    /*< Parentheses '()' >*/
    BRACKETS,       /*< Square brackets '[]' >*/
    BRACES,         /*< Curly braces '{}' >*/
    BRACKETS_COUNT
} brackets_t;

/* ==================================== *\
 *  PRIVATE GLOBAL VARIABLES DEFINITION *
\* ==================================== */

/** ============================================================================
  @var      operators_str
  @package  RPN_calculator

  @brief    Array of strings representing operator symbols.

  @details  Maps operator indices defined in `operator_index_t` to their
            corresponding string representations. This array is used for
            operator identification and processing in expression parsing and
            evaluation.
 =========================================================================== **/
const char* operators_str[OP_COUNT] =
{
    [OP_ADD]  = "+",
    [OP_SUB]  = "-",
    [OP_MUL]  = "*",
    [OP_DIV]  = "/",
    [OP_POW]  = "^",
    [OP_FACT] = "!"
};

/** ============================================================================
  @var      functions_str
  @package  RPN_calculator

  @brief    Array of strings representing function names.

  @details  Maps function indices defined in `func_index_t` to their
            corresponding string representations. This array is used for
            function identification and processing in expression parsing and
            evaluation.
 =========================================================================== **/
const char* functions_str[FUNC_COUNT] =
{
    [FUNC_SQRT]   = "sqrt",
    [FUNC_LOG]    = "log",
    [FUNC_LN]     = "ln",
    [FUNC_SIN]    = "sin",
    [FUNC_COS]    = "cos",
    [FUNC_TAN]    = "tan",
    [FUNC_COSH]   = "cosh",
    [FUNC_SINH]   = "sinh",
    [FUNC_TANH]   = "tanh",
    [FUNC_ASIN]   = "asin",
    [FUNC_ACOS]   = "acos",
    [FUNC_ATAN]   = "atan",
    [FUNC_ARCSIN] = "arcsin",
    [FUNC_ARCCOS] = "arccos",
    [FUNC_ARCTAN] = "arctan"
};

/** ============================================================================
  @var      brackets_str
  @package  RPN_calculator

  @brief    Array of strings representing bracket symbols.

  @details  Maps bracket types defined in `brackets_t` to their corresponding
            opening and closing symbols. This two-dimensional array is used for
            bracket identification and processing in expression parsing and
            evaluation.

  @note     The first dimension corresponds to the type of bracket (parentheses,
            brackets, braces), and the second dimension corresponds to the
            associativity (opening or closing bracket).
 =========================================================================== **/
const char* brackets_str[BRACKETS_COUNT][ASSOCIATIVE_COUNT] =
{
    [PARENTHESES] = {"(", ")"},
    [BRACKETS]    = {"[", "]"},
    [BRACES]      = {"{", "}"}
};

/* ==================================== *\
 *     PRIVATE FUNCTIONS DEFINITION     *
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
int RPNCalculator_whichOperator(const char* token)
{
    /*< Variable Declarations >*/
    int ret         = FUNCTION_SUCCESS; /*< Return Control >*/

    size_t iterator = 0u;

    /*< Security Checks >*/
    if(token == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }

    /*< Start Function Algorithm >*/
    for(iterator = 0u; iterator < OP_COUNT; iterator++)
    {
        if (strcmp(token, operators_str[iterator]) == FUNCTION_SUCCESS)
        {
            ret = iterator;
            goto end_of_function;
        }
    }

    ret = -(EINVAL);

    /*< Function Output >*/
 end_of_function: 
    return ret;
}

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
int RPNCalculator_whichFunction(const char* token) 
{
    /*< Variable Declarations >*/
    int ret         = FUNCTION_SUCCESS; /*< Return Control >*/

    size_t iterator = 0u;

    /*< Security Checks >*/
    if(token == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }

    /*< Start Function Algorithm >*/
    for(iterator = 0u; iterator < FUNC_COUNT; iterator++)
    {
        if (strcmp(token, functions_str[iterator]) == FUNCTION_SUCCESS)
        {
            ret = iterator;
            goto end_of_function;
        }
    }

    ret = -(EINVAL);

    /*< Function Output >*/
end_of_function:
    return ret;
}

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
int RPNCalculator_checkPrecedence(const char* token) 
{
    /*< Variable Declarations >*/
    int ret             = FUNCTION_SUCCESS; /*< Return Control >*/

    int function_index  = 0u;
    int operator_index  = 0u;

    /*< Security Checks >*/
    if(token == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }

    /*< Assign Initial Values >*/
    function_index  = RPNCalculator_whichFunction(token);
    operator_index  = RPNCalculator_whichOperator(token);
    
    /*< Start Function Algorithm >*/
    if((function_index >= FUNCTION_SUCCESS) && (operator_index < FUNCTION_SUCCESS))
    {
        ret = PRECEDENCE_1;
    }

    if((function_index < FUNCTION_SUCCESS) && (operator_index >= FUNCTION_SUCCESS))
    {
        ret = (operator_index == OP_FACT) ? PRECEDENCE_2 :
              (operator_index == OP_POW)  ? PRECEDENCE_3 :
              (operator_index == OP_MUL)  ? PRECEDENCE_4 :
              (operator_index == OP_DIV)  ? PRECEDENCE_4 :
              (operator_index == OP_ADD)  ? PRECEDENCE_5 :
              (operator_index == OP_SUB)  ? PRECEDENCE_5 : -(EINVAL);
    }

    if((function_index < FUNCTION_SUCCESS) && (operator_index < FUNCTION_SUCCESS))
    {
        ret = -(EINVAL);
    }

    /*< Function Output >*/
end_of_function:
    return ret;
}

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
int RPNCalculator_isRightAssociative(const char* token) 
{
    /*< Variable Declarations >*/
    int ret = LEFT_ASSOCIATIVE; /*< Return Control >*/

    /*< Security Checks >*/
    if(token == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }

    /*< Start Function Algorithm >*/
    if ( 
            (strcmp(token, operators_str[OP_POW])   == FUNCTION_SUCCESS) 
                                || 
            (strcmp(token, operators_str[OP_FACT])  == FUNCTION_SUCCESS) 
        )
    {
        ret = RIGHT_ASSOCIATIVE;
    }

    /*< Function Output >*/
end_of_function:
    return ret;
}

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
int RPNCalculator_tokenize(const char* expression, char tokens[][MAX_TOKEN_LEN]) 
{
    /*< Variable Declarations >*/
    int ret                 = FUNCTION_SUCCESS; /*< Return Control >*/

    size_t  iterator        = 0u;
    size_t  char_index      = 0u;
    size_t  total_tokens    = 0u;

    /*< Security Checks >*/
    if((expression == NULL) || (tokens == NULL))
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }

    /*< Start Function Algorithm >*/
    while (expression[iterator] != '\0') 
    {
        /*< Ignore whitespace >*/
        if (isspace(expression[iterator])) 
        {
            iterator++;
            continue;
        }

        /*< Tokenization of numbers - integer or decimal >*/
        if ( 
                ( ( isdigit(expression[iterator]) ) || ( expression[iterator] == '.' ) ) 
                                                   && 
                                ( char_index < (MAX_TOKEN_LENGTH - 1u) )
            ) 
        {
            if(total_tokens >= MAX_NUM_TOKENS)
            {
                ret = -(EINVAL);
                goto end_of_function;
            }

            char_index = 0u;

            while ( 
                        ( ( isdigit(expression[iterator]) ) || ( expression[iterator] == '.' ) )
                                                            &&
                                        ( char_index < (MAX_TOKEN_LENGTH - 1u) )
                                                            &&
                                            ( expression[iterator] != '\0' )
                                                           
                    ) 
            {
                tokens[total_tokens][char_index++] = expression[iterator++];
            }

            tokens[total_tokens][char_index] = '\0';

            total_tokens++;

            continue;
        }

        /*< Tokenization of functions or variables - sequence of letters >*/
        if (isalpha(expression[iterator])) 
        {
            if(total_tokens >= MAX_NUM_TOKENS)
            {
                ret = -(EINVAL);
                goto end_of_function;
            }

            char_index = 0u;

            while ( 
                            ( isalpha(expression[iterator]) ) 
                                          && 
                        ( char_index < (MAX_TOKEN_LENGTH - 1u) )
                                          &&
                            ( expression[iterator] != '\0' )
                    ) 
            {
                tokens[total_tokens][char_index++] = expression[iterator++];
            }

            tokens[total_tokens][char_index] = '\0';

            total_tokens++;

            continue;
        }

        /*< Tokenization of operators and parentheses >*/
        if (strchr("+-*/^!()[]{}", expression[iterator]) != NULL) 
        {
            if(total_tokens >= MAX_NUM_TOKENS)
            {
                ret = -(EINVAL);
                goto end_of_function;
            }

            tokens[total_tokens][FIRST_VALUE]   = expression[iterator];
            tokens[total_tokens][SECOND_VALUE]  = '\0';

            total_tokens++;
            iterator++;

            continue;
        }

        /*< Unmapped character >*/
        printf("Caractere desconhecido: %c\n", expression[iterator]);

        ret = -(EINVAL);
        goto end_of_function;
    }

    ret = (int)(total_tokens);

    /*< Function Output >*/
end_of_function:
    return ret;
}

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
int RPNCalculator_infixToPostfix(char tokens[][MAX_TOKEN_LEN], char output[][MAX_TOKEN_LEN], int number) 
{
    /*< Variable Declarations >*/
    int ret                 = FUNCTION_SUCCESS; /*< Return Control >*/

    char* token             = NULL;
    char* top_token         = NULL;
    
    size_t  total_tokens    = 0u;
    size_t iterator         = 0u;

    stack_op_t op_stack     = {0u};

    /*< Security Checks >*/
    if(tokens == NULL || output == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }

    /*< Assign Initial Values >*/
    op_stack.top = EMPTY_TOP;

    /*< Start Function Algorithm >*/
    for (iterator = 0u; iterator < number; iterator++) 
    {
        token = tokens[iterator];


        if (
                            ( isdigit(token[FIRST_VALUE]) ) 
                                           || 
                ( (token[FIRST_VALUE] == '.') && (isdigit(token[SECOND_VALUE])) )
            ) 
        {
            strcpy(output[total_tokens++], token);
            continue;
        }

        /*< Token is a function >*/
        if (RPNCalculator_whichFunction(token) >= FUNCTION_SUCCESS) 
        {
            Stack_pushOp(&op_stack, token);
            continue;
        }

        /*< Token is a open bracket >*/
        if (
                ( strcmp(token, brackets_str[PARENTHESES][LEFT_ASSOCIATIVE])    == FUNCTION_SUCCESS ) 
                                                    || 
                ( strcmp(token, brackets_str[BRACKETS][LEFT_ASSOCIATIVE])       == FUNCTION_SUCCESS )
                                                    || 
                ( strcmp(token, brackets_str[BRACES][LEFT_ASSOCIATIVE])         == FUNCTION_SUCCESS )
            ) 
        {
            Stack_pushOp(&op_stack, token);
            continue;
        }

        /*< Token is a close bracket >*/
        if (
                ( strcmp(token, brackets_str[PARENTHESES][RIGHT_ASSOCIATIVE])   == FUNCTION_SUCCESS ) 
                                                    ||
                ( strcmp(token, brackets_str[BRACKETS][RIGHT_ASSOCIATIVE])      == FUNCTION_SUCCESS ) 
                                                    || 
                ( strcmp(token, brackets_str[BRACES][RIGHT_ASSOCIATIVE])        == FUNCTION_SUCCESS )
            ) 
        {
            top_token = Stack_peekOp(&op_stack);

            while (
                                            ( top_token != NULL )
                                                    &&
                        ( strcmp(top_token, brackets_str[PARENTHESES][LEFT_ASSOCIATIVE])    != FUNCTION_SUCCESS )
                                                    &&
                        ( strcmp(top_token,  brackets_str[BRACKETS][LEFT_ASSOCIATIVE])      != FUNCTION_SUCCESS )
                                                    &&
                        ( strcmp(top_token, brackets_str[BRACES][LEFT_ASSOCIATIVE])         != FUNCTION_SUCCESS )
                   ) 
            {
                strcpy(output[total_tokens++], Stack_popOp(&op_stack));
                top_token = Stack_peekOp(&op_stack);
            }

            if (top_token == NULL) 
            {
                ret = -(EINVAL);
                goto end_of_function;
            }

            Stack_popOp(&op_stack); /*< Remove the open bracket >*/

            /*< If top is a function, pop to output >*/
            top_token = Stack_peekOp(&op_stack);

            if ((top_token != NULL) && (RPNCalculator_whichFunction(top_token) >= FUNCTION_SUCCESS) ) 
            {
                strcpy(output[total_tokens++], Stack_popOp(&op_stack));
            }

            continue;
        }

        /*< Token is a operator >*/
        if (RPNCalculator_whichOperator(token) >= FUNCTION_SUCCESS) 
        {
            while (!Stack_isEmpty(&op_stack)) 
            {
                top_token = Stack_peekOp(&op_stack);

                if (
                        (RPNCalculator_whichFunction(top_token) >= FUNCTION_SUCCESS) 
                                                        ||
                        (
                            (RPNCalculator_whichOperator(top_token) >= FUNCTION_SUCCESS) 
                                                        &&
                            (
                                (RPNCalculator_checkPrecedence(top_token) < RPNCalculator_checkPrecedence(token))
                                                        ||
                                (
                                    (RPNCalculator_checkPrecedence(top_token) == RPNCalculator_checkPrecedence(token)) 
                                                        &&
                                    (!RPNCalculator_isRightAssociative(token))
                                )
                            )
                        )
                    )
                {
                    strcpy(output[total_tokens++], Stack_popOp(&op_stack));
                }
                else
                {
                    break;
                }
            }

            Stack_pushOp(&op_stack, token);
            continue;
        }

            ret = -(EINVAL);
            goto end_of_function;
    }

    /*< Pops the remaining operators >*/
    while (!Stack_isEmpty(&op_stack)) 
    {
        top_token = Stack_popOp(&op_stack);

        if (
                ( strcmp(top_token, brackets_str[PARENTHESES][LEFT_ASSOCIATIVE])    == FUNCTION_SUCCESS )
                                                    || 
                ( strcmp(top_token, brackets_str[BRACKETS][LEFT_ASSOCIATIVE])      == FUNCTION_SUCCESS )
                                                    || 
                ( strcmp(top_token, brackets_str[BRACES][LEFT_ASSOCIATIVE])         == FUNCTION_SUCCESS )
            ) 
        {
            ret = -(EINVAL);
            goto end_of_function;
        }

        strcpy(output[total_tokens++], top_token);
    }

    ret = (int)(total_tokens);

    /*< Function Output >*/
end_of_function:
    return ret;
}

/** ============================================================================
  @fn       RPNCalculator_factorialCalculate
  @package  RPN_calculator

  @brief    Calculates the factorial of a given number.

  @details  Computes the factorial of a non-negative integer using recursion.
            Returns 1 for factorial of 0 or 1.

  @param    number    [in]:  The number to calculate the factorial of.

  @return   The factorial of the number as a double.
 =========================================================================== **/
double RPNCalculator_factorialCalculate(unsigned int number) 
{
    /*< Variable Declarations >*/
    double ret = FUNCTION_SUCCESS; /*< Return Control >*/

    /*< Security Checks >*/
    if (number == 0u || number == 1u)
    {
        ret = 1.0;
        goto end_of_function;
    }

    /*< Start Function Algorithm >*/
    ret = (number * RPNCalculator_factorialCalculate(number - 1u));

    /*< Function Output >*/
end_of_function:
    return ret;
}

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
double RPNCalculator_applyOperation(const char* operation, double num_a, double num_b) 
{
    /*< Variable Declarations >*/
    double ret          = FUNCTION_SUCCESS; /*< Return Control >*/

    int operation_index = 0u;

    /*< Security Checks >*/
    if(operation == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }

    /*< Assign Initial Values >*/
    operation_index = RPNCalculator_whichOperator(operation);

    /*< Start Function Algorithm >*/
    ret = (operation_index == OP_ADD)                  ? (num_a + num_b)   :
          (operation_index == OP_SUB)                  ? (num_a - num_b)   :
          (operation_index == OP_MUL)                  ? (num_a * num_b)   :
          (operation_index == OP_DIV && num_b != 0u)   ? (num_a / num_b)   :
          (operation_index == OP_POW)                  ? pow(num_a, num_b) : -(EINVAL);

    /*< Function Output >*/
end_of_function:
    return ret;
}

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
double RPNCalculator_applyFunction(const char* function, double number) 
{
    /*< Variable Declarations >*/
    double ret      = FUNCTION_SUCCESS; /*< Return Control >*/

    int function_index    = 0u;

    /*< Security Checks >*/
    if(function == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }

    /*< Assign Initial Values >*/
    function_index = RPNCalculator_whichFunction(function);

    /*< Start Function Algorithm >*/
    ret = (function_index == FUNC_SQRT)                                     ? ret = sqrt(number)    :
          (function_index == FUNC_LOG)                                      ? ret = log10(number)   :
          (function_index == FUNC_LN)                                       ? ret = log(number)     :
          (function_index == FUNC_SIN)                                      ? ret = sin(number)     :
          (function_index == FUNC_COS)                                      ? ret = cos(number)     :
          (function_index == FUNC_TAN)                                      ? ret = tan(number)     :
          (function_index == FUNC_COSH)                                     ? ret = cosh(number)    :
          (function_index == FUNC_SINH)                                     ? ret = sinh(number)    :
          (function_index == FUNC_TANH)                                     ? ret = tanh(number)    :
          (function_index == FUNC_ASIN || function_index == FUNC_ARCSIN)    ? ret = asin(number)    :
          (function_index == FUNC_ACOS || function_index == FUNC_ARCCOS)    ? ret = acos(number)    :
          (function_index == FUNC_ATAN || function_index == FUNC_ARCTAN)    ? ret = atan(number)    : -(EINVAL);

    /*< Function Output >*/
end_of_function:
    return ret;
}

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
double RPNCalculator_evaluatePostfix(char output[][MAX_TOKEN_LEN], int number)
{
    /*< Variable Declarations >*/
    double ret              = 0.0;   /*< Return Control >*/

    size_t iterator         = 0u;

    char* token             = NULL;

    double token_number     = 0.0;
    double operand_a        = 0.0;
    double operand_b        = 0.0;
    double result_value     = 0.0;

    int status              = FUNCTION_SUCCESS;

    stack_val_t val_stack   = {0u};

    /*< Security Checks >*/
    if (output == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }

    /*< Assign Initial Values >*/
    val_stack.top = EMPTY_TOP;

    /*< Start Function Algorithm >*/
    for (iterator = 0u; iterator < (size_t)number; iterator++)
    {
        token = output[iterator];

        /*< Token is a number >*/
        if (isdigit(token[FIRST_VALUE]) || (token[FIRST_VALUE] == '.' && isdigit(token[SECOND_VALUE])))
        {
            token_number = atof(token);
            status = Stack_pushVal(&val_stack, token_number);

            if (status != FUNCTION_SUCCESS)
            {
                ret = -(EINVAL);
                goto end_of_function;
            }

            continue;
        }

        /*< Token is a operator >*/
        if (RPNCalculator_whichOperator(token) >= FUNCTION_SUCCESS)
        {
            if (strcmp(token, operators_str[OP_FACT]) == FUNCTION_SUCCESS)
            {
                /*< Token is a unitary operator - factorial >*/
                if (Stack_isEmptyVal(&val_stack) == STACK_EMPTY)
                {
                    ret = -(EINVAL);
                    goto end_of_function;
                }

                operand_a = Stack_popVal(&val_stack);

                if (operand_a < 0.0 || (operand_a - (int)(operand_a)) != 0.0)
                {
                    ret = -(EINVAL);
                    goto end_of_function;
                }

                result_value = RPNCalculator_factorialCalculate((unsigned int)operand_a);

                status = Stack_pushVal(&val_stack, result_value);
                if (status != FUNCTION_SUCCESS)
                {
                    ret = (-EINVAL);
                    goto end_of_function;
                }
            }
            else
            {
                /*< Token is a binary operator >*/
                if (val_stack.top < 1u)
                {
                    ret = -(EINVAL);
                    goto end_of_function;
                }

                operand_b = Stack_popVal(&val_stack);
                operand_a = Stack_popVal(&val_stack);

                result_value = RPNCalculator_applyOperation(token, operand_a, operand_b);
                if (result_value == -(EINVAL))
                {
                    ret = -(EINVAL);
                    goto end_of_function;
                }

                status = Stack_pushVal(&val_stack, result_value);
                if (status != FUNCTION_SUCCESS)
                {
                    ret = -(EINVAL);
                    goto end_of_function;
                }
            }
            continue;
        }

        /*< Token is a function >*/
        if (RPNCalculator_whichFunction(token) >= FUNCTION_SUCCESS)
        {
            if (Stack_isEmptyVal(&val_stack) == STACK_EMPTY)
            {
                ret = -(EINVAL);
                goto end_of_function;
            }

            operand_a = Stack_popVal(&val_stack);

            result_value = RPNCalculator_applyFunction(token, operand_a);
            if (result_value == -(EINVAL))
            {
                ret = -(EINVAL);
                goto end_of_function;
            }

            status = Stack_pushVal(&val_stack, result_value);
            if (status != FUNCTION_SUCCESS)
            {
                ret = -(EINVAL);
                goto end_of_function;
            }
            continue;
        }

        /*< Unmapped token >*/
        ret = -(EINVAL);
        goto end_of_function;
    }

    /*< Checks if there is exactly one value on the stack >*/
    if (val_stack.top != 0u)
    {
        ret = -(EINVAL);
        goto end_of_function;
    }

    ret = Stack_popVal(&val_stack);

    /*< Function Output >*/
end_of_function:
    /*< On failure, we reset the top of the stack >*/
    if (ret < 0u)
    {
        val_stack.top = EMPTY_TOP;
    }
    return ret;
}

/*< end of file >*/
