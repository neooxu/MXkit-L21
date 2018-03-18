

#include "ATCmdParser.h"

#ifdef LF
#undef LF
#define LF  10
#else
#define LF  10
#endif

#ifdef CR
#undef CR
#define CR  13
#else
#define CR  13
#endif

bool _dbg_on = false;
static struct oob *_oobs;
static int _timeout = 0;
static uint8_t _buffer[AT_BUFFER_SIZE];
static const char *_output_delimiter;
static int _output_delim_size;

void ATCmdParser_hal_init(void);
int ATCmdParser_hal_putc(char c);
int ATCmdParser_hal_getc(void);
bool ATCmdParser_hal_readable(void);
void ATCmdParser_hal_flush(void);


static inline void debug_if(int condition, const char *format, ...) {
	if (condition) {
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
}

bool ATCmdParser_vrecv(const char *response, va_list args)
{
	char _in_prev = 0;  
	bool _aborted;   
restart:
	_aborted = false;
	// Iterate through each line in the expected response
	while (response[0]) {
		// Since response is const, we need to copy it into our buffer to
		// add the line's null terminator and clobber value-matches with asterisks.
		//
		// We just use the beginning of the buffer to avoid unnecessary allocations.
		int i = 0;
		int offset = 0;
		bool whole_line_wanted = false;

		while (response[i]) {
			if (response[i] == '%' && response[i+1] != '%' && response[i+1] != '*') {
				_buffer[offset++] = '%';
				_buffer[offset++] = '*';
				i++;
			} else {
				_buffer[offset++] = response[i++];
				// Find linebreaks, taking care not to be fooled if they're in a %[^\n] conversion specification
				if (response[i - 1] == '\n' && !(i >= 3 && response[i-3] == '[' && response[i-2] == '^')) {
					whole_line_wanted = true;
					break;
				}
			}
		}
		
		if (_buffer[offset-2] == CR) {
			_buffer[offset-2] = LF;
			offset--;
		}


		// Scanf has very poor support for catching errors
		// fortunately, we can abuse the %n specifier to determine
		// if the entire string was matched.
		_buffer[offset++] = '%';
		_buffer[offset++] = 'n';
		_buffer[offset++] = 0;

		debug_if(_dbg_on, "AT? %s\n", _buffer);
		// To workaround scanf's lack of error reporting, we actually
		// make two passes. One checks the validity with the modified
		// format string that only stores the matched characters (%n).
		// The other reads in the actual matched values.
		//
		// We keep trying the match until we succeed or some other error
		// derails us.
		int j = 0, dummy = 0;
		int dummy_pos[20];

		while (true) {
			// Receive next character
			int c = ATCmdParser_hal_getc();
			if (c < 0) {
				debug_if(_dbg_on, "AT(Timeout)\n");
				return false;
			}
			// Simplify newlines (borrowed from retarget.cpp)=>LF
			if ((c == CR && _in_prev != LF) ||
			(c == LF && _in_prev != CR)) {
				c = '\n';
			} else if ((c == CR && _in_prev == LF) ||
			(c == LF && _in_prev == CR)) {
				_in_prev = c;
				// onto next character
				continue;
				} 
			
			/* Possible not existed string may cause %n function failed:
			example: "cmd:%*s\r\n%n" not match "cmd:\r\n", %n will not 
			give a valid value, so we give some dummy chars */
			if (c=='\n'&&_in_prev==':') {
				dummy_pos[dummy++] = j;
				_buffer[offset + j++] = 'x';
			}

			_in_prev = c;

			_buffer[offset + j++] = c;
			_buffer[offset + j] = 0;
			

			struct oob *oob = _oobs;
			// Check for oob data
			for (; oob; oob = (struct oob *)oob->next) {
				if ((unsigned)j == oob->len && memcmp(
				oob->prefix, _buffer+offset, oob->len) == 0) {
					debug_if(_dbg_on, "AT! %s\n", oob->prefix);
					oob->cb();

					if (_aborted) {
						debug_if(_dbg_on, "AT(Aborted)\n");
						return false;
					}
					// oob may have corrupted non-reentrant buffer,
					// so we need to set it up again
					goto restart;
				}
			}

			// Check for match
			int count = -1;
			if (whole_line_wanted && c != '\n') {
				// Don't attempt scanning until we get delimiter if they included it in format
				// This allows recv("Foo: %s\n") to work, and not match with just the first character of a string
				// (scanf does not itself match whitespace in its format string, so \n is not significant to it)
				} else {
				sscanf(_buffer+offset, _buffer, &count);
				debug_if(_dbg_on, "%s=====%s, %d,%d\r\n", _buffer, _buffer+offset, count, j);
			}
			
			// We only succeed if all characters in the response are matched
			if (count == j) {
				/* Remove dummy chars*/
				for (int k = 0; k < dummy; k++, j-- ) {
					dummy_pos[k] -= k;
					memcpy(_buffer+offset+dummy_pos[k], _buffer+offset+dummy_pos[k]+1, j-dummy_pos[k]-1);
					debug_if(_dbg_on, "AT= %s\n", _buffer+offset);
				}				
				
				debug_if(_dbg_on, "AT= %s\n", _buffer+offset);
				// Reuse the front end of the buffer
				memcpy(_buffer, response, i);
				_buffer[i] = 0;

				// Store the found results
				vsscanf(_buffer+offset, _buffer, args);
				// Jump to next line and continue parsing
				response += i;
				break;
			}

			// Clear the buffer when we hit a newline or ran out of space
			// running out of space usually means we ran into binary data
			if (c == '\n' || j+1 >= AT_BUFFER_SIZE - offset) {
				debug_if(_dbg_on, "AT< %s", _buffer+offset);
				j = 0;
				dummy = 0;
			}
		}
	}

	return true;
}

// Command parsing with line handling
bool ATCmdParser_vsend(const char *command, va_list args)
{
	// Create and send command
	if (vsprintf(_buffer, command, args) < 0) {
		return false;
	}

	for (int i = 0; _buffer[i]; i++) {
		if (ATCmdParser_hal_putc(_buffer[i]) < 0) {
			return false;
		}
	}

	// Finish with newline
	for (size_t i = 0; _output_delimiter[i]; i++) {
		if (ATCmdParser_hal_putc(_output_delimiter[i]) < 0) {
			return false;
		}
	}

	debug_if(_dbg_on, "AT> %s\n", _buffer);
	return true;
}

bool ATCmdParser_recv(const char *response, ...)
{
	va_list args;
	va_start(args, response);
	bool res = ATCmdParser_vrecv(response, args);
	va_end(args);
	return res;
}

bool ATCmdParser_send(const char *command, ...)
{
	va_list args;
	va_start(args, command);
	bool res = ATCmdParser_vsend(command, args);
	va_end(args);
	return res;
}

// read/write handling with timeouts
int ATCmdParser_write(const char *data, int size)
{
	int i = 0;
	for ( ; i < size; i++) {
		if (ATCmdParser_hal_putc(data[i]) < 0) {
			return -1;
		}
	}
	return i;
}

int ATCmdParser_read(char *data, int size)
{
	int i = 0;
	for ( ; i < size; i++) {
		int c = ATCmdParser_hal_getc();
		if (c < 0) {
			return -1;
		}
		data[i] = c;
	}
	return i;
}

void ATCmdParser_debug(bool on)
{
	_dbg_on = on;
}

void ATCmdParser_add_oob(const char *prefix, oob_callback cb)
{
	struct oob *oob = malloc(sizeof(struct oob));
	oob->len = strlen(prefix);
	oob->prefix = prefix;
	oob->cb = cb;
	oob->next = _oobs;
	_oobs = oob;
	
}

bool ATCmdParser_process_oob(void)
{
	if (!ATCmdParser_hal_readable()) {
		return false;
	}

	int i = 0;
	while (true) {
		// Receive next character
		int c = ATCmdParser_hal_getc();
		if (c < 0) {
			return false;
		}
		_buffer[i++] = c;
		_buffer[i] = 0;

		// Check for oob data
		struct oob *oob = _oobs;
		while (oob) {
			if (i == (int)oob->len && memcmp(
			oob->prefix, _buffer, oob->len) == 0) {
				debug_if(_dbg_on, "AT! %s\r\n", oob->prefix);
				oob->cb();
				return true;
			}
			oob = oob->next;
		}
		
		// Clear the buffer when we hit a newline or ran out of space
		// running out of space usually means we ran into binary data
		if (i+1 >= AT_BUFFER_SIZE ||
		strcmp(&_buffer[i-_output_delim_size], _output_delimiter) == 0) {

			debug_if(_dbg_on, "AT< %s", _buffer);
			i = 0;
		}
	}
}

int ATCmdParser_analyse_args(char args[], char *arg_list[], int list_size)
{
	char _in_prev = 0;
	int arg_num = 1;
	size_t len = strlen(args);
	
	arg_list[0] = args;
	
	for (int i = 0; i <= len; i++) {
		debug_if(_dbg_on, "check %c, %d\r\n", args[i], len);
		if (args[i]==',') {
			debug_if(_dbg_on, "find ,\r\n");
			if (_in_prev=='\\') {
				args[i-i] = ',';
				memcpy(&args[i], &args[i+1], len-(i+1));
			}
			else {
				args[i] = 0;
				arg_list[arg_num++] = &args[i+1];
				if(arg_num>list_size) break;
			}
		}
		_in_prev = args[i];
	}
	return arg_num;
}

void ATCmdParser_set_timeout(int timeout)
{
	_timeout = timeout;
}

void ATCmdParser_init(const char *output_delimiter, int timeout, bool debug)
{
	_timeout = timeout;
	_dbg_on = debug;
	
	_output_delimiter = output_delimiter;
	_output_delim_size = strlen(output_delimiter);
	
	ATCmdParser_hal_init();
}

///////////////////////////////////////////////////////////////////////////////

#include <hal_usart_async.h>
#include <utils.h>

extern struct usart_async_descriptor USART_AT;
static struct io_descriptor *io_at;
static volatile bool tx_complete = true;


static void rx_cb_USART_AT(const struct usart_async_descriptor *const io_descr)
{
	/* Driver will store the received data to its buffer */
}

static void tx_cb_USART_AT(const struct usart_async_descriptor *const io_descr)
{
	tx_complete = true;
}

void ATCmdParser_hal_init(void)
{
	usart_async_register_callback(&USART_AT, USART_ASYNC_RXC_CB, rx_cb_USART_AT);
	usart_async_register_callback(&USART_AT, USART_ASYNC_TXC_CB, tx_cb_USART_AT);
	usart_async_get_io_descriptor(&USART_AT, &io_at);
	usart_async_enable(&USART_AT);
}

int ATCmdParser_hal_putc(char c)
{
	uint32_t current = ms_ticker_read();
	
	do {
		if(tx_complete) {
			tx_complete = false;
			return io_write(io_at, &c, 1) == 1 ? 0 : -1;				
		}
	} while((ms_ticker_read() - current) < _timeout);

	return -1;
}

int ATCmdParser_hal_getc(void)
{
	uint32_t current = ms_ticker_read();
	uint8_t ch;
	
	do {
		if (io_read(io_at, &ch, 1) == 1) return ch;
	} while((ms_ticker_read() - current) < _timeout);
	
	return -1;
}

bool ATCmdParser_hal_readable(void)
{
	struct usart_async_descriptor *descr = CONTAINER_OF(io_at, struct usart_async_descriptor, io);
	if(ringbuffer_num(&descr->rx)) return true;
	return false;
}


void ATCmdParser_hal_flush(void)
{
	uint32_t                       num;
	uint8_t                        tmp;
	struct usart_async_descriptor *descr = CONTAINER_OF(io_at, struct usart_async_descriptor, io);
	
	CRITICAL_SECTION_ENTER()
    for(num = ringbuffer_num(&descr->rx); num>0; num--) {
		ringbuffer_get(&descr->rx, &tmp);
	}
	CRITICAL_SECTION_LEAVE()
}