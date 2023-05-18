// Console input and output.
// Input is from the keyboard or serial port.
// Output is written to the screen and serial port.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"


static void consputc(int);

static int panicked = 0;

static struct {
	struct spinlock lock;
	int locking;
} cons;

static void
printint(int xx, int base, int sign)
{
	static char digits[] = "0123456789abcdef";
	char buf[16];
	int i;
	uint x;

	if(sign && (sign = xx < 0))
		x = -xx;
	else
		x = xx;

	i = 0;
	do{
		buf[i++] = digits[x % base];
	}while((x /= base) != 0);

	if(sign)
		buf[i++] = '-';

	while(--i >= 0)
		consputc(buf[i]);
}

// Print to the console. only understands %d, %x, %p, %s.
void
cprintf(char *fmt, ...)
{
	int i, c, locking;
	uint *argp;
	char *s;

	locking = cons.locking;
	if(locking)
		acquire(&cons.lock);

	if (fmt == 0)
		panic("null fmt");

	argp = (uint*)(void*)(&fmt + 1);
	for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
		if(c != '%'){
			consputc(c);
			continue;
		}
		c = fmt[++i] & 0xff;
		if(c == 0)
			break;
		switch(c){
		case 'd':
			printint(*argp++, 10, 1);
			break;
		case 'x':
		case 'p':
			printint(*argp++, 16, 0);
			break;
		case 's':
			if((s = (char*)*argp++) == 0)
				s = "(null)";
			for(; *s; s++)
				consputc(*s);
			break;
		case '%':
			consputc('%');
			break;
		default:
			// Print unknown % sequence to draw attention.
			consputc('%');
			consputc(c);
			break;
		}
	}

	if(locking)
		release(&cons.lock);
}


void
panic(char *s)
{
	int i;
	uint pcs[10];

	cli();
	cons.locking = 0;
	// use lapiccpunum so that we can call panic from mycpu()
	cprintf("lapicid %d: panic: ", lapicid());
	cprintf(s);
	cprintf("\n");
	getcallerpcs(&s, pcs);
	for(i=0; i<10; i++)
		cprintf(" %p", pcs[i]);
	panicked = 1; // freeze other CPU
	for(;;)
		;
}

#define BACKSPACE 0x100
#define CRTPORT 0x3d4
static ushort *crt = (ushort*)P2V(0xb8000);  // CGA memory

static int flagOpened = 0;
static int selected = 3;
static int buffer[9][9];
static int flagWS = 0;


void 
printMenu(int pos)
{ 
	
	int startPos = pos - 80 + 1; // -80 ide red iznad i +1 ide desno, tako dobijam gornji desni ugao
	char arr[4][255] = {{"|WHT YEL|"}, {"|RED AQU|"}, {"|PUR WHT|"}, {"|WHT BLK|"}};
	int cnt = 0;

	// pomeranje meniija u levo ukoliko nema mesta da bude desno
	if (pos%80 > 70){
		startPos -= 10;		
	}

	// punjenje buffera za zatvaranje menija
	if (flagOpened == 0){
		for (int i = 0; i < 9; i++){
			for (int j = 0; j < 9; j++){
				crt[startPos++] = buffer[i][j];		
			}		
			startPos -= 9;  // vracamo na pocetak reda
			startPos -= 80;	// dizemo kolonu na gore 				
		}
		return;	
	}


	

	// implemntacija menija
	
	for (int i = 0; i < 9; i++){	
		if (i % 2 == 1){				
			for (int j = 0; j < 9;){
				if (flagWS == 0) buffer[i][j] = crt[startPos];
				if (cnt == selected) {
					crt[startPos++] = (arr[cnt][j++]&0xff) | 0x2000;																			
				}
				else crt[startPos++] = (arr[cnt][j++]&0xff) | 0x7000;			
			}
			cnt++;	
		}else{
			for (int j = 0; j < 9; j++){
				if (flagWS == 0) buffer[i][j] = crt[startPos];
				crt[startPos++] = ('_'&0xff) | 0x7000;			
			}
		}			

		startPos -= 9;  // vracamo na pocetak reda
		startPos -= 80;	// dizemo kolonu na gore 			
	}

}





static void
cgaputc(int c)
{
	int pos;
	int upSelected = 0;
	// Cursor position: col + 80*row.
	outb(CRTPORT, 14);
	pos = inb(CRTPORT+1) << 8;
	outb(CRTPORT, 15);
	pos |= inb(CRTPORT+1);
	
	if(c == '\n'){
		pos += 80 - pos%80;
	}else if(c == BACKSPACE){
		if(pos > 0) --pos;
	}else if (flagOpened == 1 && c == 257){
		flagOpened = 0;
		flagWS = 0;
		printMenu(pos);
	}else if(c == 257 && flagOpened == 0){
		flagOpened = 1;		
		printMenu(pos);			
	}else if (c == 115 && flagOpened == 1){
		flagWS = 1;
		selected--;
		if (selected < 0) selected = 0;
		printMenu(pos);
				
	}else if (c == 119 && flagOpened == 1){
		flagWS = 1;
		selected++;
		if (selected > 3) selected = 3;
		printMenu(pos);		
	}

	 else{
		if (selected == 3) crt[pos++] = (c&0xff) | 0x0700;  // black on white
		else if (selected == 2) crt[pos++] = (c&0xff) | 0x7500; // white on purple
		else if (selected == 1) crt[pos++] = (c&0xff) | 0x3400; // aqua on red
		else if (selected == 0) crt[pos++] = (c&0xff) | 0x6700; // yellow on white
		
	}
	if(pos < 0 || pos > 25*80)
		panic("pos under/overflow");

	if((pos/80) >= 24){  // Scroll up.
		memmove(crt, crt+80, sizeof(crt[0])*23*80);
		pos -= 80;
		memset(crt+pos, 0, sizeof(crt[0])*(24*80 - pos));
	}

	outb(CRTPORT, 14);
	outb(CRTPORT+1, pos>>8);
	outb(CRTPORT, 15);
	outb(CRTPORT+1, pos);
	crt[pos] = ' ' | 0x0700;
}

void
consputc(int c)
{
	if(panicked){
		cli();
		for(;;)
			;
	}
	
	if (c == BACKSPACE && flagOpened) return;

	if(c == BACKSPACE){
		uartputc('\b'); uartputc(' '); uartputc('\b');
	} else
		uartputc(c);
	cgaputc(c);
}

#define INPUT_BUF 128
struct {
	char buf[INPUT_BUF];
	uint r;  // Read index
	uint w;  // Write index
	uint e;  // Edit index
} input;

#define C(x)  ((x)-'@')  // Control-x

void
consoleintr(int (*getc)(void))
{
	int c, doprocdump = 0;

	acquire(&cons.lock);
	while((c = getc()) >= 0){
		switch(c){
		case C('P'):  // Process listing.
			// procdump() locks cons.lock indirectly; invoke later
			doprocdump = 1;
			break;
		case C('U'):  // Kill line.
			while(input.e != input.w &&
			      input.buf[(input.e-1) % INPUT_BUF] != '\n'){
				input.e--;
				consputc(BACKSPACE);
			}
			break;
		case C('H'): case '\x7f':  // Backspace
			if(input.e != input.w){
				input.e--;
				consputc(BACKSPACE);
			}
			break;
		case 257 :
			 consputc(c);
			 break;
		case 'w':
            if(flagOpened){
                consputc(c);
                break;
            }
        case 's':
            if(flagOpened){
                consputc(c);
                break;
            }


		default:
			if (flagOpened == 1){
					break;	
			}
			if(c != 0 && input.e-input.r < INPUT_BUF){
				c = (c == '\r') ? '\n' : c;
				input.buf[input.e++ % INPUT_BUF] = c;
				consputc(c);
				if(c == '\n' || c == C('D') || input.e == input.r+INPUT_BUF){
					input.w = input.e;
					wakeup(&input.r);
				}
			}
			break;
	
		}
	}
	release(&cons.lock);
	if(doprocdump) {
		procdump();  // now call procdump() wo. cons.lock held
	}
}

int
consoleread(struct inode *ip, char *dst, int n)
{
	uint target;
	int c;

	iunlock(ip);
	target = n;
	acquire(&cons.lock);
	while(n > 0){
		while(input.r == input.w){
			if(myproc()->killed){
				release(&cons.lock);
				ilock(ip);
				return -1;
			}
			sleep(&input.r, &cons.lock);
		}
		c = input.buf[input.r++ % INPUT_BUF];
		if(c == C('D')){  // EOF
			if(n < target){
				// Save ^D for next time, to make sure
				// caller gets a 0-byte result.
				input.r--;
			}
			break;
		}
		*dst++ = c;
		--n;
		if(c == '\n')
			break;
	}
	release(&cons.lock);
	ilock(ip);

	return target - n;
}

int
consolewrite(struct inode *ip, char *buf, int n)
{
	int i;

	iunlock(ip);
	acquire(&cons.lock);
	for(i = 0; i < n; i++)
		consputc(buf[i] & 0xff);
	release(&cons.lock);
	ilock(ip);

	return n;
}

void
consoleinit(void)
{
	initlock(&cons.lock, "console");

	devsw[CONSOLE].write = consolewrite;
	devsw[CONSOLE].read = consoleread;
	cons.locking = 1;

	ioapicenable(IRQ_KBD, 0);
}
