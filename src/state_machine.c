/* ==================================== *\
 *             INCLUDED FILE            *
\* ==================================== */

 /*< Dependencies >*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

/* ==================================== *\
 *            PRIVATE DEFINES           *
\* ==================================== */

#define FUNCTION_SUCCESS    0U

/* ==================================== *\
 *       PRIVATE TYPES DEFINITION       *
\* ==================================== */

typedef enum currentOfMachine
{
    CALCULATOR_MACHINE_1,
    CALCULATOR_MACHINE_0,
    MAX_MACHINE_NUM,
} current_machine_t;

typedef enum statesOfMachine
{
    IDLE_STATE,
    GET_FROM_USER,
    MAKE_OPERATION,
    RETURN_FOR_USER,
    ASK_FOR_CONTINUE,
    MAX_STATES
} state_t;

typedef enum instantsOfState
{
    PREVIOUS_STATE,
    CURRENT_STATE,
    MAX_INSTANT_STATES
} state_instat_t;

typedef enum menuOptions
{
    EXIT_PROGRAM,
    CONTINUE_PROGRAM,
    MAX_OPTIONS,
} options_t;

typedef struct __attribute__((packed)) listHeader 
{
    struct listHeader *prev_node; 
    struct listHeader *next_node;
}list_header_t;

typedef struct __attribute__((packed)) machineData
{
    int first_number;
    int second_number;
        
    int result;
        
    char operation;
} machine_data_t;

typedef struct __attribute__((packed)) stateMachine
{
    state_t state[MAX_INSTANT_STATES];
    
    machine_data_t data;
    
    list_header_t list;
} state_machine_t;

typedef int (*state_machine_func_t)(state_machine_t *);

/* ==================================== *\
 *    PRIVATE FUNCTIONS PROTOTYPES      *
\* ==================================== */

 /*< Handler Machine Operation >*/
state_machine_t*   StateMachine_getMachine      (state_machine_t *head, current_machine_t index);
int                StateMachine_setState        (state_machine_t *head, state_instat_t instant, state_t state);
state_t            StateMachine_getState        (state_machine_t *head, state_instat_t instant);
int                StateMachine_processState    (state_machine_t *head, state_t state);
state_machine_t*   StateMachine_listCreator     (unsigned int n_machines);
void               StateMachine_listDestroyer   (state_machine_t *head);

 /*< Process States >*/
static int StateMachine_processUserInput        (state_machine_t *head);
static int StateMachine_processDoOperation      (state_machine_t *head);
static int StateMachine_processUserOutput       (state_machine_t *head);
static int StateMachine_processAskForContinue   (state_machine_t *head);
static int StateMachine_processIdle             (state_machine_t *head);

/* ==================================== *\
 *  PRIVATE GLOBAL VARIABLES DEFINITION *
\* ==================================== */

const state_machine_func_t process_state[MAX_STATES] =
{
    [IDLE_STATE]        = StateMachine_processIdle,
    [GET_FROM_USER]     = StateMachine_processUserInput,
    [MAKE_OPERATION]    = StateMachine_processDoOperation,
    [RETURN_FOR_USER]   = StateMachine_processUserOutput,
    [ASK_FOR_CONTINUE]  = StateMachine_processAskForContinue
};

/* ==================================== *\
 *     PRIVATE FUNCTIONS DEFINITION     *
\* ==================================== */

static int StateMachine_processIdle(state_machine_t *head)
{
    /*< Variable Declarations >*/
    int ret = FUNCTION_SUCCESS;
    
    /*< Security Checks >*/
    if(head == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }
    
    /*< Start Function Algorithm >*/
    perror("General Error");
    
    /*< Function Output >*/
end_of_function:
    return ret;
}

static int StateMachine_processUserInput(state_machine_t *head)
{
    /*< Variable Declarations >*/
    int ret = FUNCTION_SUCCESS;
    
    machine_data_t user_input;
    
    /*< Security Checks >*/
    if(head == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }
    
    /*< Assign Initial Values >*/
    memset(&user_input, 0, sizeof(machine_data_t));
    memcpy(&user_input, &head->data, sizeof(machine_data_t));
    
    /*< Start Function Algorithm >*/
    printf("Insert first number: ");
    if (scanf("%i", &user_input.first_number) != 1)
    {
        ret = -EINVAL;
        goto end_of_function;
    }
    
    printf("\n");
    
    printf("Insert second number: ");
    if (scanf("%i", &user_input.second_number) != 1)
    {
        ret = -EINVAL;
        goto end_of_function;
    }
    
    printf("\n");
    
    printf("Insert the operation: ");
    while(getchar() != '\n');
    if (scanf("%c", &user_input.operation) != 1)
    {
        ret = -EINVAL;
        goto end_of_function;
    }
    
    printf("\n");
    
    memcpy(&head->data, &user_input, sizeof(machine_data_t));
    
    /*< Function Output >*/
    memset(&user_input, 0, sizeof(machine_data_t));

end_of_function:
    return ret;
}

static int StateMachine_processDoOperation(state_machine_t *head)
{
    /*< Variable Declarations >*/
    int ret = FUNCTION_SUCCESS;
    
    machine_data_t user_input;
    
    /*< Security Checks >*/
    if(head == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }
    
    /*< Assign Initial Values >*/
    memset(&user_input, 0, sizeof(machine_data_t));
    memcpy(&user_input, &head->data, sizeof(machine_data_t));
    
    /*< Start Function Algorithm >*/
    switch(user_input.operation)
    {
        case '+':
            user_input.result = 
                    (int)(user_input.first_number + user_input.second_number);
        break;
        
        case '-':
            user_input.result = 
                    (int)(user_input.first_number - user_input.second_number);
        break;
        
        default:
             user_input.result = 0u;
        break;
    }
    
    memcpy(&head->data, &user_input, sizeof(machine_data_t));
    
    /*< Function Output >*/
    memset(&user_input, 0, sizeof(machine_data_t));
    
end_of_function:
    return ret;
}

static int StateMachine_processUserOutput(state_machine_t *head)
{
    /*< Variable Declarations >*/
    int ret = FUNCTION_SUCCESS;
    
    machine_data_t user_input;
    
    /*< Security Checks >*/
    if(head == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }
    
    /*< Assign Initial Values >*/
    memset(&user_input, 0, sizeof(machine_data_t));
    memcpy(&user_input, &head->data, sizeof(machine_data_t));
    
    /*< Start Function Algorithm >*/
    printf("result: %i", user_input.result);
    printf("\n");
    
    memcpy(&head->data, &user_input, sizeof(machine_data_t));
    
    /*< Function Output >*/
    memset(&user_input, 0, sizeof(machine_data_t));
    
end_of_function:
    return ret;
}

static int StateMachine_processAskForContinue(state_machine_t *head)
{    
    /*< Variable Declarations >*/
    int ret = FUNCTION_SUCCESS;
    
    char option;
    
    /*< Security Checks >*/
    if(head == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }
    
    /*< Start Function Algorithm >*/
    printf("\n");
    
    while(getchar() != '\n');
    
    printf("Do you want to continue? (s / n): ");
    if (scanf("%c", &option) != 1)
    {
        ret = -EINVAL;
        goto end_of_function;
    }
        
    option = toupper(option);
    
    ret = (option == 'S') ? CONTINUE_PROGRAM    :
          (option == 'N') ? EXIT_PROGRAM        : -EINVAL;
          
    /*< Function Output >*/
end_of_function:
    return ret;
}

state_machine_t* StateMachine_listCreator(unsigned int n_machines)
{
    /*< Variable Declarations >*/
    unsigned int list_iterator  = 0u;
    
    state_machine_t *head       = NULL;
    state_machine_t *prev       = NULL;
    state_machine_t *current    = NULL;
    state_machine_t *temp       = NULL;
  
    /*< Security Checks >*/
    if (n_machines <= 0u) 
    {
        perror("Number of machines in the system not supported");
        goto end_of_function;
    }
  
    /*< Start Function Algorithm >*/
    for(list_iterator = 0u; list_iterator < n_machines; list_iterator++)
    {
        current = (state_machine_t *)malloc(sizeof(state_machine_t));
        
        if(current == NULL)
        {
            perror("Failed to allocate memory for state machine");
            
            temp = head;
            
            while(!(temp == NULL))
            {
                state_machine_t *next = (state_machine_t *)(temp->list.next_node);
                
                free(temp);
                
                temp = next;
            }
            
            goto end_of_function;
        }

        current->state[PREVIOUS_STATE]  = 0u;
        current->state[CURRENT_STATE]   = 0u;

        current->list.prev_node         = (list_header_t *)prev;
        current->list.next_node         = NULL;

        if(prev != NULL)
        {
            prev->list.next_node = (list_header_t *)current;
        }
        else
        {
            head = current;
        }

        prev = current;
    }
  
    /*< Function Output >*/
end_of_function:
    return head;
}

void StateMachine_listDestroyer(state_machine_t *head)
{
    /*< Variable Declarations >*/
    state_machine_t *current    = NULL;
    state_machine_t *next       = NULL;
  
    /*< Assign Initial Values >*/
    current = head;
  
    /*< Start Function Algorithm >*/
    while(current != NULL)
    {
        next = (state_machine_t *)(current->list.next_node);
        
        free(current);
        
        current = next;
    }
}

state_machine_t* StateMachine_getMachine(state_machine_t *head, current_machine_t index)
{
    /*< Variable Declarations >*/
    state_machine_t *current            = NULL;
    current_machine_t current_index     = 0u;
  
     /*< Security Checks >*/
    if(head == NULL)
    {
        perror("Head is null");
        goto end_of_function;
    }
  
    /*< Assign Initial Values >*/
    current = head;

    /*< Start Function Algorithm >*/
    while(current != NULL && current_index < index)
    {
        current = (state_machine_t *)(current->list.next_node);
        current_index++;
    }

    if(current_index == index && current != NULL)
    {
        goto end_of_function;
    }
    else
    {
        perror("Current is NULL");
        
        current = NULL;
        
        goto end_of_function;
    }
  
    /*< Function Output >*/
end_of_function:
    return current;
}

int StateMachine_setState(state_machine_t *head, state_instat_t instant, state_t state)
{
    /*< Variable Declarations >*/
    int ret = 0u;
  
    /*< Security Checks >*/
    if(head == NULL)
    {
        ret= -(ENOMEM);
        goto end_of_function;
    }
  
    /*< Start Function Algorithm >*/
    head->state[instant] = state;
  
    /*< Function Output >*/
end_of_function:
    return ret;
}

state_t StateMachine_getState(state_machine_t *head, state_instat_t instant)
{
    /*< Variable Declarations >*/
    state_t state = 0u;
  
    /*< Security Checks >*/
    if(head == NULL)
    {
        goto end_of_function;
    }
  
    /*< Start Function Algorithm >*/
    state = head->state[instant];
  
   /*< Function Output >*/
end_of_function:
    return state;
}

int StateMachine_processState(state_machine_t *head, state_t state)
{
    /*< Variable Declarations >*/
    int ret = 0u;
  
    /*< Security Checks >*/
    if(head == NULL)
    {
        ret = -(ENOMEM);
        goto end_of_function;
    }
  
    /*< Start Function Algorithm >*/
     process_state[state](head);

    /*< Function Output >*/
end_of_function:
     return ret;
}

/* ==================================== *\
 *    MAIN FUNCTION FOR EXECUTE TESTS   *
\* ==================================== */

int main(int argc, char *argv[])
{
    /*< Variable Declarations >*/
    state_machine_t *machine_list       = NULL;
    state_machine_t *current_machine    = NULL;
    
    current_machine_t machine_index     = 0u;
    
    int process_ret                     = 0u;
    
    state_t state                       = 0u;
    
    bool break_condition = false;
    
     /*< Assign Initial Values >*/
    state           = GET_FROM_USER;
    machine_index   = CALCULATOR_MACHINE_0;
    
    machine_list    = StateMachine_listCreator(MAX_MACHINE_NUM);
    
    current_machine = StateMachine_getMachine(machine_list, machine_index);
    
    StateMachine_setState(current_machine, CURRENT_STATE, state);
    
    /*< Start Function Algorithm >*/
    while (!break_condition)
    {
        state = StateMachine_getState(current_machine, CURRENT_STATE);
        
        switch (state)
        {
            case IDLE_STATE:
                system("clear");
                
                process_ret = StateMachine_processState(current_machine, IDLE_STATE);
            break;
            
            case GET_FROM_USER:
                system("clear");
                
                process_ret = StateMachine_processState(current_machine, GET_FROM_USER);
                
                StateMachine_setState(current_machine, PREVIOUS_STATE, state);
                state = (process_ret == FUNCTION_SUCCESS) ? MAKE_OPERATION : IDLE_STATE;
                StateMachine_setState(current_machine, CURRENT_STATE, state);
                break;
                
            case MAKE_OPERATION:
                process_ret = StateMachine_processState(current_machine, MAKE_OPERATION);
                
                StateMachine_setState(current_machine, PREVIOUS_STATE, state);
                state = (process_ret == FUNCTION_SUCCESS) ? RETURN_FOR_USER : IDLE_STATE;
                StateMachine_setState(current_machine, CURRENT_STATE, state);
                break;
                
            case RETURN_FOR_USER:
                process_ret = StateMachine_processState(current_machine, RETURN_FOR_USER);
                
                StateMachine_setState(current_machine, PREVIOUS_STATE, state);
                state = (process_ret == FUNCTION_SUCCESS) ? ASK_FOR_CONTINUE : IDLE_STATE;
                StateMachine_setState(current_machine, CURRENT_STATE, state);
                break;
                
            case ASK_FOR_CONTINUE:

                process_ret = StateMachine_processAskForContinue(current_machine);
                
                StateMachine_setState(current_machine, PREVIOUS_STATE, state);
                
                switch(process_ret)
                {
                    case CONTINUE_PROGRAM:
                        break_condition = false;
                        state = GET_FROM_USER;
                    break;
                    
                    case EXIT_PROGRAM:
                        break_condition = true;
                        state = IDLE_STATE;
                    break;
                    
                    case -EINVAL:
                        break_condition = false;
                        state = ASK_FOR_CONTINUE;
                    break;
                }
                
                StateMachine_setState(current_machine, CURRENT_STATE, state);
                break;
            
            default:
                perror("State out of range");
                break_condition = true;
                break;
        }
    }
    
    /*< Function Output >*/
    StateMachine_listDestroyer(machine_list);
    
    system("clear");
    printf("Exiting program...");
    
    return 0;
}
