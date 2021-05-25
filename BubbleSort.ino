volatile byte arr[50];
volatile byte a = 0x2F, b = 0xA2, c = 0x3A, x = 1;
volatile byte lm = 49;
volatile byte sze = 50;
void setup() {
  Serial.begin(9600);
  asm volatile(
    //https://www.electro-tech-online.com/threads/ultra-fast-pseudorandom-number-generator-for-8-bit.124249/
    "ldi r18, 0 \n"
    "lds r17, (sze) \n"
    "ldi r30, lo8(arr) \n"
    "ldi r31, hi8(arr) \n"
    "loop:call rng \n"
    "st Z+, r16 \n"
    "inc r18 \n"
    "cp r18, r17 \n"
    "brne loop \n"
    "rjmp end \n"
    "rng: lds r13, (x) \n"
    "lds r14, (a) \n"
    "lds r15, (b) \n"
    "lds r16, (c) \n"
    "inc r13 \n"
    "eor r14, r16 \n"
    "eor r14, r13 \n"
    "sts (a), r14 \n"
    "add r15,r14 \n"
    "sts (b), r15 \n"
    "lsr r15 \n"
    "eor r15, r14 \n"
    "add r16, r15 \n"
    "sts (c), r16 \n"
    "ret \n"
    "end: \n"

    " ldi r16,0xFF \n"
    " out 4,r16 \n"
  );
}

void loop() {
  if (Serial.find('r')) {
    asm volatile(
      "lds r17, (lm) \n"

      "bubble: ldi r30, lo8(arr) \n"
      "ldi r31, hi8(arr) \n"
      "lds r16, (lm) \n"
      "add r30, r16 \n"
      "adc r31, r1 \n"
      "mov r18, r17 \n"

      "i_loop:ld r13, Z \n"
      "ld  r14, -Z \n"
      "cp  r13, r14 \n"
      "brsh  L1 \n"
      "st  Z, r13 \n"
      "std Z+1, r14 \n"

      "L1: dec r18 \n"
      "brne  i_loop \n"
      "dec r17 \n"
      "brne  bubble \n"
    );
    for (int i = 0; i < 50; i++) {
      Serial.println(arr[i]);
    }

    asm volatile(

    "ldi r30, lo8(arr) \n"
    "ldi r31, hi8(arr) \n"
    "ld r14, Z \n"
    "adiw Z,49 \n"
    "ld r15, Z \n"
    "blinkLoop: out 5, r14 \n"
    "call blink \n"
    "out 5, r15 \n"
    "call blink \n"
    "rjmp blinkLoop \n"
    "blink: ldi r16, 31 \n"
    "outer: ldi r24, lo8(2021) \n"
    "ldi r25, hi8(2021) \n"
    "blink_loop: adiw r24, 1\n"
    "brne blink_loop \n"
    "dec r16 \n"
    "brne outer \n"
    "ret \n"
    );
  }
}
