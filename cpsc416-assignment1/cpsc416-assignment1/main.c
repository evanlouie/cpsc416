//
//  main.c
//  cpsc416-assignment1
//
//  Created by Evan Louie on 1/15/2014.
//  Copyright (c) 2014 Evan Louie. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>


#include "state_types.c"


State current_state = START_S;
State previous_state = START_S;
State previous_print_state = START_S;
char last_char = '\0';
int sum = 0;

pthread_mutex_t lock;
int thread_count = 0;

StateTransition valid_state_transitions[] = {
    {   START_S,    UPTIME_C,   UPTIME_S    },
    {   START_S,    LOAD_C,     LOAD_S      },
    {   START_S,    EXIT_C,     EXIT_S      },
    {   START_S,    NUMBER_C,   NUMBER_S    },
    {   START_S,    OTHER_C,    OTHER_S     },
    {   UPTIME_S,   UPTIME_C,   UPTIME_S    },
    {   UPTIME_S,   UPTIME_C,   START_S     },
    {   UPTIME_S,   LOAD_C,     LOAD_S      },
    {   UPTIME_S,   EXIT_C,     EXIT_S      },
    {   UPTIME_S,   NUMBER_C,   NUMBER_S    },
    {   UPTIME_S,   OTHER_C,    OTHER_S     },
    {   LOAD_S,     UPTIME_C,   UPTIME_S    },
    {   LOAD_S,     LOAD_C,     START_S     },
    {   LOAD_S,     LOAD_C,     LOAD_S      },
    {   LOAD_S,     EXIT_C,     EXIT_S      },
    {   LOAD_S,     UPTIME_C,   UPTIME_S    },
    {   LOAD_S,     NUMBER_C,   NUMBER_S    },
    {   LOAD_S,     OTHER_C,    OTHER_S     },
    {   EXIT_S,     UPTIME_C,   UPTIME_S    },
    {   EXIT_S,     LOAD_C,     LOAD_S      },
    {   EXIT_S,     EXIT_C,     EXIT_S      },
    {   EXIT_S,     EXIT_C,     START_S     },
    {   EXIT_S,     NUMBER_C,   NUMBER_S    },
    {   EXIT_S,     OTHER_C,    OTHER_S     },
    {   OTHER_S,    UPTIME_C,   UPTIME_S    },
    {   OTHER_S,    LOAD_C,     LOAD_S      },
    {   OTHER_S,    EXIT_C,     EXIT_S      },
    {   OTHER_S,    NUMBER_C,   NUMBER_S    },
    {   OTHER_S,    OTHER_C,    OTHER_S     },
    {   NUMBER_S,   UPTIME_C,   UPTIME_S    },
    {   NUMBER_S,   LOAD_C,     LOAD_S      },
    {   NUMBER_S,   EXIT_C,     EXIT_S      },
    {   NUMBER_S,   NUMBER_C,   NUMBER_S    },
    {   NUMBER_S,   ALPHABET_C, START_S     },
    {   NUMBER_S,   OTHER_C,    OTHER_S     }
};

StateTransition *state_transition_create(State start_state, Action action, State end_state) {
    StateTransition *st = malloc(sizeof(StateTransition));
    assert(st != NULL);
    st->start_state = start_state;
    st->action = action;
    st->end_state =end_state;
    return st;
}
void state_transition_destroy(StateTransition *st) {
    assert(st != NULL);
    free(st);
}
bool valid_transition(State start_state, Action action, State end_state) {
    
    StateTransition *st = state_transition_create(start_state, action, end_state);
    for (int i = 0; i<=sizeof(valid_state_transitions); i++) {
        if (
            st->start_state == valid_state_transitions[i].start_state &&
            st->action == valid_state_transitions[i].action &&
            st->end_state == valid_state_transitions[i].end_state
            )
        {
            state_transition_destroy(st);
            return true;
        }
    }
    state_transition_destroy(st);
    return false;
}

void changeState(Action a, State s) {
    if (valid_transition(current_state, a, s)) {
        previous_state = current_state;
        current_state = s;
    }
    else printf("\nERROR: invalid state transition\n");
}

void charToState(char c) {
    switch (c) {
        case 'u':
            changeState(UPTIME_C, UPTIME_S);
            break;
        case 'l':
            changeState(LOAD_C, LOAD_S);
            break;
        case 'e':
            changeState(EXIT_C, EXIT_S);
            break;
            
        default:
            if (isalpha(c) || c==' ') {
                changeState(OTHER_C, OTHER_S);
            }
            else if (isnumber(c)) {
                if (valid_transition(current_state, NUMBER_C, NUMBER_S))
                {
                    previous_state = current_state;
                    current_state = NUMBER_S;
                    int i = c - '0';
                    sum = i;
                }
                else printf("\nERROR: invalid state transition: current-> %d\n", current_state);
            }
            
            break;
    }
}


void readChar(char c) {
    if (c=='\0' && current_state==NUMBER_S) {
        charToState(c);
        printf("NUMBER FOUND: %d\n", sum);
        previous_print_state = NUMBER_S;
        sum = 0;
    }
    else if (c=='\0' && current_state==OTHER_S && previous_print_state!=OTHER_S)  {
        previous_print_state = OTHER_S;
        printf("%d\n", (int) -1);
    }
    else {
        switch (current_state) {
            case START_S:
                charToState(c);
                break;
            case UPTIME_S:
                switch (last_char) {
                    case 'u':
                        if (c!='p') charToState(c);
                        break;
                    case 'p':
                        if (c!='t') charToState(c);
                        break;
                    case 't':
                        if (c!='i') charToState(c);
                        break;
                    case 'i':
                        if (c!='m') charToState(c);
                        break;
                    case 'm':
                        if (c!='e') charToState(c);
                        else {
                            printf("%u\n", (unsigned)time(NULL));
                            previous_print_state = UPTIME_S;
                            changeState(UPTIME_C, START_S);
                        }
                        break;
                    default:
                        charToState(c);
                        break;
                }
                break;
            case LOAD_S:
                switch (last_char) {
                    case 'l':
                        if (c!='o') charToState(c);
                        break;
                    case 'o':
                        if (c!='a') charToState(c);
                        break;
                    case 'a':
                        if (c!='d') charToState(c);
                        else {
                            printf("LOAD FOUND\n");
                            previous_print_state = LOAD_S;
                            changeState(LOAD_C, START_S);
                        }
                        break;
                    default:
                        charToState(c);
                        break;
                }
                break;
            case EXIT_S:
                switch (last_char) {
                    case 'e':
                        if (c!='x') charToState(c);
                        break;
                    case 'x':
                        if (c!='i') charToState(c);
                        break;
                    case 'i':
                        if (c!='t') charToState(c);
                        else {
                            printf("EXIT FOUND\n");
                            previous_print_state = EXIT_S;
                            changeState(EXIT_C, START_S);
                        }
                        break;
                    default:
                        charToState(c);
                        break;
                }
                break;
            case NUMBER_S:
                if (isnumber(c)) {
                    int i = c - '0';
                    sum += i;
                }
                else {
                    
                    charToState(c);
                    printf("NUMBER FOUND: %d\n", sum);
                    previous_print_state = NUMBER_S;
                    sum = 0;
                }
                break;
            case OTHER_S:
                if (c=='u' || c=='l' || c=='e' || isnumber(c)) {
                    charToState(c);
                    if (previous_print_state != OTHER_S) {
                        previous_print_state = OTHER_S;
                        printf("%d\n", (int) -1);
                    }
                    
                }
                else {
                }
                break;
            default:
                printf("ERROR: INVALID STATE\n");
                break;
        }
    }
    
    last_char = c;
}





int main(int argc, const char * argv[])
{
    current_state = START_S;
    previous_print_state = START_S;
    sum = 0;
    char *message = "uptime load exit asdf 123 13load uptexiloa123";
    for (int i = 0; i<=strlen(message); i++) {
        readChar(message[i]);
    }
    
    return 0;
}

