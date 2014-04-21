//
//  state_types.c
//  cpsc416-assignment1
//
//  Created by Evan Louie on 1/15/2014.
//  Copyright (c) 2014 Evan Louie. All rights reserved.
//

#include <stdio.h>

typedef enum State{
    START_S,
    UPTIME_S,
    LOAD_S,
    NUMBER_S,
    EXIT_S,
    OTHER_S
} State;
typedef enum Action{
    UPTIME_C,
    LOAD_C,
    EXIT_C,
    NUMBER_C,
    OTHER_C,
    NULL_C,
    ALPHABET_C
} Action;

typedef struct StateTransition {
    State   start_state;
    Action  action;
    State   end_state;
} StateTransition;



