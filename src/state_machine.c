/* ==================================== *\
 * 		     INCLUDED FILES               *
\* ==================================== */

 /*< Dependencies >*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

/* ==================================== *\
 *       PRIVATE TYPES DEFINITION       *
\* ==================================== */
typedef enum currentOfMachine
{
    ENEMY_MACHINE,
    ALLY_MACHINE,
    MAX_MACHINE_NUM,
} current_machine_t;

typedef enum statesOfMachine
{
    IDLE_STATE,
    RUNNING_STATE,
    STOP_STATE,
    MAX_STATES
} state_t;

typedef enum instantsOfState
{
    PREVIOUS_STATE,
    CURRENT_STATE,
    MAX_INSTANT_STATES
} state_instat_t;

typedef struct __attribute__((packed)) listHeader 
{
    struct listHeader *prev_node; 
    struct listHeader *next_node;
}list_header_t;

typedef struct __attribute__((packed)) stateMachine
{
    state_t state[MAX_INSTANT_STATES];
    
    list_header_t list;
} state_machine_t;

typedef int (*state_machine_func_t)(state_machine_t *);

/* ==================================== *\
 *    PRIVATE FUNCTIONS PROTOTYPES      *
\* ==================================== */

 /*< Handler Machine Operation >*/
state_machine_t*   StateMachine_getMachine     (state_machine_t *head, current_machine_t index);
int                StateMachine_setState       (state_machine_t *head, state_instat_t instant, state_t state);
state_t            StateMachine_getState       (state_machine_t *head, state_instat_t instant);
int                StateMachine_processState   (state_machine_t *head, state_t state);
state_machine_t*   StateMachine_listCreator    (unsigned int n_machines);
void               StateMachine_listDestroyer  (state_machine_t *head);

 /*< Process States >*/
static int StateMachine_processIdleState      (state_machine_t *head);
static int StateMachine_processRunningState   (state_machine_t *head);
static int StateMachine_processStopState      (state_machine_t *head);

/* ==================================== *\
 *  PRIVATE GLOBAL VARIABLES DEFINITION *
\* ==================================== */

const state_machine_func_t process_state[MAX_STATES] =
{
    [IDLE_STATE]    = StateMachine_processIdleState,
    [RUNNING_STATE] = StateMachine_processRunningState,
    [STOP_STATE]    = StateMachine_processStopState
};

/* ==================================== *\
 *     PRIVATE FUNCTIONS DEFINITION     *
\* ==================================== */

static int StateMachine_processIdleState(state_machine_t *head)
{
    printf("IDLE STATE\n");
    return 0;
}

static int StateMachine_processRunningState(state_machine_t *head)
{
    printf("RUNNING STATE\n");
    return 0;
}

static int StateMachine_processStopState(state_machine_t *head)
{
    printf("STOP STATE\n");
    return 0;
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

        current->state[PREVIOUS_STATE]  = IDLE_STATE;
        current->state[CURRENT_STATE]   = IDLE_STATE;

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
    
    state_t state                       = IDLE_STATE;
    
    bool break_condition                = false;
  
    /*< Assign Initial Values >*/
    machine_list    = StateMachine_listCreator(MAX_MACHINE_NUM);
    
    machine_index   = ALLY_MACHINE;
    current_machine = StateMachine_getMachine(machine_list, machine_index);
    
    StateMachine_setState(current_machine, CURRENT_STATE, IDLE_STATE);
    
    state = StateMachine_getState(current_machine, CURRENT_STATE);
  
    /*< Start Function Algorithm >*/
    while(!(break_condition))
    {
        
        switch(state)
        {
            case IDLE_STATE:
                process_ret = StateMachine_processState(current_machine, IDLE_STATE);
                
                StateMachine_setState(current_machine, PREVIOUS_STATE, state);
                state = (process_ret == 0) ? RUNNING_STATE : IDLE_STATE;
                StateMachine_setState(current_machine, CURRENT_STATE, state);
                
                state = StateMachine_getState(current_machine, CURRENT_STATE);
            break;
            
            case RUNNING_STATE:
                process_ret = StateMachine_processState(current_machine, RUNNING_STATE);
                
                StateMachine_setState(current_machine, PREVIOUS_STATE, state);
                state = (process_ret == 0) ? STOP_STATE : IDLE_STATE;
                StateMachine_setState(current_machine, CURRENT_STATE, state);
                
                state = StateMachine_getState(current_machine, CURRENT_STATE);
            break;
            
            case STOP_STATE:
                process_ret = StateMachine_processState(current_machine, STOP_STATE);
                
                StateMachine_setState(current_machine, PREVIOUS_STATE, state);
                state = IDLE_STATE;
                StateMachine_setState(current_machine, CURRENT_STATE, state);
                
                state = StateMachine_getState(current_machine, CURRENT_STATE);
                
                break_condition = (process_ret == 0) ? true : false;
            break;
            
            default:
                perror("State out of range");
            break;
        }
    
    }
  
    /*< Function Output >*/
    StateMachine_listDestroyer(machine_list);
    return 0;
}
