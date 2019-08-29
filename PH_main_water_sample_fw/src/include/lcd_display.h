#ifndef __MENU_DISPLAY_H__
#define __MENU_DISPLAY_H__

/*LCD Display Main Menu level 1*/

#define SCREEN_IDLE            (" * WATER-SAMPLER *  "), ("                    "), ("                    "), ("Menu            Info")
// #define SCREEN_MENU         ("     -- MENU --     "), ("  1.SET CLOCK       "), ("  2.SET BOTTLE      "), ("  3.SET TRIGGER     ")
#define SCREEN_SET_CLOCK       ("     -- MENU --     "), ("> 1.SET CLOCK       "), ("  2.MANUAL TRIGGER  "), ("  3.MOVE HOME       ")
#define SCREEN_MANUAL_TRIGGER  ("     -- MENU --     "), ("  1.SET CLOCK       "), ("> 2.MANUAL TRIGGER  "), ("  3.MOVE HOME       ")
#define SCREEN_MOVE_HOME       ("     -- MENU --     "), ("  1.SET CLOCK       "), ("  2.MANUAL TRIGGER  "), ("> 3.MOVE HOME       ")
#define SCREEN_MOVE_CURRENT    ("     -- MENU --     "), ("> 4.MOVE CURRENT    "), ("  5.VIEW LOG        "), ("  6.BACK            ")
#define SCREEN_VIEW_LOG        ("     -- MENU --     "), ("  4.MOVE CURRENT    "), ("> 5.VIEW LOG        "), ("  6.BACK            ")
#define SCREEN_BACK            ("     -- MENU --     "), ("  4.MOVE CURRENT    "), ("  5.VIEW LOG        "), ("> 6.BACK            ")


/*LCD Display Menu Sub level 2*/

#define SCREEN_SET_CLOCK_MENU        (" -Setting DateTime- "), ("                    "), ("                    "), ("Menu            Back")
#define SCREEN_MANUAL_TRIGGER_MENU   ("  -Setting Bottle-  "), ("                    "), ("    BOTTLES:        "), (".               Back")
#define SCREEN_MOVE_HOME_MENU        ("    - Move Home -   "), ("  Move to home...   "), ("       (0, 0)       "), ("Move            Back")
#define SCREEN_MOVE_CURRENT_MENU     ("  - Move Current -  "), (" Move to current... "), ("                    "), ("Move            Back")
#define SCREEN_VIEW_LOG_MENU         ("                    "), ("                    "), ("                    "), ("                Back")

#define SCREEN_SET_DATE_MENU    (" -Setting DateTime- "), ("  >                 "), ("                    "), ("Menu            Back")  
#define SCREEN_SET_TIME_MENU    (" -Setting DateTime- "), ("                    "), ("  >                 "), ("Menu            Back")

#define SCREEN_SET_DATE 		("   -Setting Date-   "), ("                    "), ("                    "), ("Save            Back")
#define SCREEN_SET_TIME 		("   -Setting Time-   "), ("                    "), ("                    "), ("Save            Back")



#endif /* __MENU_DISPLAY_H */