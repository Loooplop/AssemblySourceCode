volatile byte buttonPressed;
volatile byte ledBlinks;

void setup() {
  Serial.begin(9600);
  buttonPressed = 0;
  ledBlinks = 10;
  asm volatile(
    " ldi r16,0xFF \n"
    " out 4,r16 \n"
    " out 5,r16 \n"
  );
}
void loop() {
  if (Serial.find('p')) {
    buttonPressed = 0x1;
  }
  asm volatile(
    "lds r16, (buttonPressed) \n"
    "lds r20, (ledBlinks) \n"
    "cp r16, __zero_reg__ \n"
    "breq  mainSequence \n"

    "call blink \n"
    "ldi r18, 0b00001011 \n"
    "out 5, r18 \n"
    "call blink \n"
    "ldi r18, 0b00001001 \n"
    "out 5, r18 \n"
    "call blink \n"
    "mainBlinkLoop: ldi r18, 0b00001001 \n"
    "out 5, r18 \n"
    "call blink \n"
    "ldi r18, 0b01001001 \n"
    "out 5, r18 \n"
    "call blink \n"
    "dec r20 \n"
    "cp r20,__zero_reg__ \n"
    "brne mainBlinkLoop \n"
    
    "ldi r18, 0b00001001 \n"
    "out 5, r18 \n"
    "call blink \n"
    "ldi r18, 0b00011001 \n"
    "out 5, r18 \n"
    "call blink \n"
    
    "lds r20, (ledBlinks) \n"
    "sts (buttonPressed), r1 \n"
    "rjmp mainSequenceP \n"
    
    "mainSequence: ldi r18, 0b00011010 \n"
    "out 5, r18 \n"
    "call blink \n"
    "mainSequenceP: ldi r18, 0b00100001 \n"
    "out 5, r18 \n"
    "call blink \n"
    "call blink \n"
    "call blink \n"
    "ldi r18, 0b00010011 \n"
    "out 5, r18 \n"
    "call blink \n"
    "ldi r18, 0b00001100 \n"
    "out 5, r18 \n"
    "call blink \n"
    "rjmp dne \n"
    
    "blink: ldi r16, 31 \n"
    "outer: ldi r24, lo8(2021) \n"
    "ldi r25, hi8(2021) \n"
    "blink_loop: adiw r24, 1\n"
    "brne blink_loop \n"
    "dec r16 \n"
    "brne outer \n"
    "ret \n"
    
    "dne: \n"
  );
  Serial.flush();
};
