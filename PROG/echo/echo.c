// Echo on TX / RX link
#define GCC33
#include <avr/io.h>
#include <avr/iotn2313.h>  // definition du processeur
#include <avr/interrupt.h>

# define sbi(reg,bit) { reg = reg |   (1<<bit) ; }
# define cbi(reg,bit) { reg = reg & (~(1<<bit)); }

//# define LEDDDR  DDRD
//# define LEDPIN  PIND
//# define LEDPORT PORTD
//# define LEDBIT  3
# define LEDDDR  DDRB
# define LEDPIN  PINB
# define LEDPORT PORTB
# define LEDBIT  4
# define LEDTOGGLE {if(bit_is_set(LEDPIN,LEDBIT)) { cbi(LEDPORT,LEDBIT); } else { sbi(LEDPORT,LEDBIT); }}

# define TXDDR  DDRD
# define TXPIN  PIND
# define TXPORT PORTD
# define TXBIT  1
# define RXDDR  DDRD
# define RXPIN  PIND
# define RXPORT PORTD
# define RXBIT  0

void init(void)
{
  sbi(LEDDDR, LEDBIT);
  sbi(TXDDR,  TXBIT);
  cbi(RXDDR,  RXBIT);
}

void init_uart(void)
{
  //cbi(UCSRB,7);  // Desactive l'Interruption Receiver Enable
  // Initialisation de la transmission
  UBRRH=0x00;    // Baud Rate
  UBRRL=0x07;    // Baud Rate 57600 bps
  //sbi(UCSRB,2);  // Mode X bits
  sbi(UCSRB,3);  // Autorise transmition
  sbi(UCSRB,4);  // Autorise reception
  UCSRC=0x06;    // Mode 8 bits
  sbi(UCSRB,7);  // Autorisation de l'Interruption Receiver Enable
}

ISR(USART_RX_vect)
{
  volatile unsigned char temp = 0;
  LEDTOGGLE;
  temp = UDR;
  while(!bit_is_set(UCSRA,5));
  UDR = temp;
}

int main(void)
{
  volatile unsigned long tempo = 0;
  init();
  init_uart();
  sbi(SREG,7);
  while(1)
  {
    //if(bit_is_set(RXPIN,RXBIT))
    //  { sbi(TXPORT,TXBIT); sbi(LEDPORT,LEDBIT); }
    //else
    //  { cbi(TXPORT,TXBIT); cbi(LEDPORT,LEDBIT); }
    tempo=100000;
    while(tempo) { tempo--; }
    LEDTOGGLE;
    //if(bit_is_set(UCSRA, 7))
    //{
    //  LEDTOGGLE;
    //  temp = UDR;
    //  while(!bit_is_set(UCSRA,5));
    //  UDR = temp;
    //}
  }
}
