#pragma once

#if COMP_MODE

#define CAN_R_ICON "r "
#define CAN_NOT_R_ICON "  "
#define CAN_W_ICON "w "
#define CAN_NOT_W_ICON "  "
#define CAN_EXEC_ICON "x "
#define CAN_NOT_EXEC_ICON "  "

#define FILE_ICON "- "
#define DIR_ICON "/ "
#define SYMLINK_TO_FILE_ICON "@ "
#define SYMLINK_TO_DIR_ICON "@ "

#else

#define CAN_R_ICON " "
#define CAN_NOT_R_ICON " "
#define CAN_W_ICON " "
#define CAN_NOT_W_ICON "  "
#define CAN_EXEC_ICON " "
#define CAN_NOT_EXEC_ICON "  "

#define FILE_ICON " "
#define DIR_ICON " "
#define SYMLINK_TO_FILE_ICON " "
#define SYMLINK_TO_DIR_ICON " "

#endif
