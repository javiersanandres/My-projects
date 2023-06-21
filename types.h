#ifndef TYPES_H
#define TYPES_H

#define MQ_NAME "/miner_checker_queue" /*!<Miner-Checker queue name*/
#define SEMAPHORE_MONITOR_ALIVE "/semaphore_monitor_alive"

#define MAX_MINERS 100
#define FIRST_OBJ 0

#define BLACK   "\033[0;30m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"


typedef enum
{
  False,
  True
} Bool;


/**
  @brief Clean_up types
*/
typedef enum
{
  free_pointer,
  file_close_fd,
  file_unlink,
  shared_unlink,
  shared_munmap,
  semaphore_close,
  semaphore_unlink,
  semaphore_destroy,
  queue_close,
  queue_unlink
} Clean_types;

#endif