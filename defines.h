// This file contains only defines
#define __CPLUSPLUS

// Task delay, theese set delays for each task
#define TASK_LOGIC_DELAY 100
#define TASK_COMMUNICATION_DELAY 100
#define TASK_SENSORS_DELAY 500
#define TASK_ALARM_DELAY 100

#define OUTPUT 1
#define HIGH 1
#define TRUE 1

#define INPUT 0
#define LOW 0
#define FALSE 0

#define UART_BUFFER_SIZE 100

// Communication protocol
#define message_end_byte	0
// Serial buffer

/* A block time of 0 means "don't block". */
#define sbDONT_BLOCK				( 0 )
// Stream buffer setup------------------------------------------------------------------------------------

/* Stream buffer length one. */
#define sbSTREAM_BUFFER_LENGTH_ONE		( ( size_t ) 1 )
/* Defines the number of tasks to create in this test and demo. */
#define sbNUMBER_OF_ECHO_CLIENTS	( 2 )
#define sbNUMBER_OF_SENDER_TASKS	( 2 )

/* Block times used when sending and receiving from the stream buffers. */
#define sbRX_TX_BLOCK_TIME			pdMS_TO_TICKS( 125UL )

/* A block time of 0 means "don't block". */
#define sbDONT_BLOCK				( 0 )
/* The number of bytes of storage in the stream buffers used in this test. */
#define sbSTREAM_BUFFER_LENGTH_BYTES	( ( size_t ) 120 )
/* Stream buffer length one. */
#define sbSTREAM_BUFFER_LENGTH_ONE		( ( size_t ) 1 )
/* The trigger level sets the number of bytes that must be present in the
stream buffer before a task that is blocked on the stream buffer is moved out of
the Blocked state so it can read the bytes. */
#define sbTRIGGER_LEVEL_1			( 1 )

// PRIORITIES
#define blinkLed_TASK_PRIORITY      ( tskIDLE_PRIORITY )
#define sensors_TASK_PRIORITY      ( tskIDLE_PRIORITY  + 1)
#define communication_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2 )
#define logic_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2 )
#define alarm_TASK_PRIORITY      ( tskIDLE_PRIORITY + 3 )

#define sbLOWER_PRIORITY			( tskIDLE_PRIORITY )
#define sbHIGHER_PRIORITY			( tskIDLE_PRIORITY + 1 )

// Set size of stack
#define sbSTACK_SIZE				( configMINIMAL_STACK_SIZE + ( configMINIMAL_STACK_SIZE >> 1 ) )
#define BLINK_LED_DELAY             ( ( portTickType ) 1000 / portTICK_RATE_MS )
