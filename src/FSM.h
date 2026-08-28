#ifndef ACAV_FSM_H
#define ACAV_FSM_H

enum class FSM {
    NONE,
    NORMAL,
    REQUESTING_STOP,
    STOPPED,
    RESTART
};

#endif //ACAV_FSM_H